
#include "MultiImageView2.h" 
#include "QVTKGraphView.h"
#include "ViewType.h"
#include <vtkMatrix4x4.h>
#include <QBoxLayout>
#include "QtWidgets/qwidget.h"
#include "QResizeEvent"
#include "vtkTexturedButtonRepresentation2D.h"
MultiImageView2::MultiImageView2(QWidget* parent):QWidget(parent)
{
	qvtk_widget_ = new QVTKOpenGLNativeWidget;
	qvtk_widget_->interactor()->SetDesiredUpdateRate(0.1);
	qvtk_widget_->interactor()->SetStillUpdateRate(0.1);
	
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	parent->setLayout(layout);
	parent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(qvtk_widget_);

	m_mprStyle = MPRStyle::New();
	//m_mprStyle->setRenderArea(parent->pos(), parent->size());
	qvtk_widget_->interactor()->SetInteractorStyle(m_mprStyle);


	mouse_picker_ = MousePicker::New();
	m_full_screen_callback = fullScreenCallback::New(this);

}

//析构函数
MultiImageView2::~MultiImageView2()
{
	delete qvtk_widget_;
	m_full_screen_callback->Delete();
}

void MultiImageView2::initiate(SPTR<vtkImageData> img_data)
{
	img_data_ = img_data;
	vtkNew<vtkMatrix4x4> mat;
	double* m_posCenter = new double[3]{ 0,0,0 };
	for (int i = 0; i < 4; i++) 
	{
		auto scene = ViewRenderScene::New(qvtk_widget_->renderWindow(), ViewType(i));
		scene->resetReslicer(img_data_, mat, m_posCenter);
		scene->set_full_screen(true);
		if (i == ViewType::VOL_M)
			scene->initiate();
		else
			scene->initiate(true, true, true);
		scene->set_callback(m_full_screen_callback);
		m_scene_map[ViewType(i)] = scene;
	}
	SwitchToTCSV();
	m_full_screen_callback->set_scenes(m_scene_map);
}

void MultiImageView2::resizeEvent(QResizeEvent* event)
{
	this->QWidget::resizeEvent(event);
	QSize size = qvtk_widget_->size();
	int width = size.width();
	int height = size.height();
	if (m_cur_layout == LAYOUT_TCSV) 
	{
		for (auto sc: m_scene_map)
		{
			sc->resize(width * 0.5, height * 0.5);
		}
	}
	else if (m_cur_layout == LAYOUT_TCS) 
	{
		m_scene_map[TRA_M]->resize(width * 0.6, height * 1.0);
		m_scene_map[COR_M]->resize(width * 0.4, height * 0.5);
		m_scene_map[SAG_M]->resize(width * 0.4, height * 0.5);
	}
	qvtk_widget_->interactor()->Render();
	//qvtk_widget_->renderWindow()->Render();
}

void MultiImageView2::SwitchToTCS()
{
	if (m_scene_map.size() != 4)return;
	m_scene_map[TRA_M]->setViewport(0.0, 0.0, 0.6, 1.0);
	m_scene_map[COR_M]->setViewport(0.6, 0.0, 1.0, 0.5);
	m_scene_map[SAG_M]->setViewport(0.6, 0.5, 1.0, 1.0);
	m_scene_map[VOL_M]->setViewport(0.0, 0.0, 0.0, 0.0);

	m_scene_map[TRA_M]->get_renderer()->DrawOn();
	m_scene_map[COR_M]->get_renderer()->DrawOn();
	m_scene_map[SAG_M]->get_renderer()->DrawOn();
	m_scene_map[VOL_M]->get_renderer()->DrawOff();

	std::vector<PTR<ViewRenderScene>> render_scenes;
	render_scenes.emplace_back(m_scene_map[TRA_M]);
	render_scenes.emplace_back(m_scene_map[COR_M]);
	render_scenes.emplace_back(m_scene_map[SAG_M]);
	m_mprStyle->set_scenes(render_scenes);
	m_cur_layout = LAYOUT_TCS;
	this->resizeEvent(nullptr);

}

void MultiImageView2::SwitchToTCSV()
{
	if (m_scene_map.size() != 4)return;
	std::vector<PTR<ViewRenderScene>> render_scenes;
	m_scene_map[TRA_M]->setViewport(0.0, 0.0, 0.5, 0.5);
	m_scene_map[COR_M]->setViewport(0.0, 0.5, 0.5, 1.0);
	m_scene_map[SAG_M]->setViewport(0.5, 0.5, 1.0, 1.0);
	m_scene_map[VOL_M]->setViewport(0.5, 0.0, 1.0, 0.5);

	m_scene_map[TRA_M]->get_renderer()->DrawOn();
	m_scene_map[COR_M]->get_renderer()->DrawOn();
	m_scene_map[SAG_M]->get_renderer()->DrawOn();
	m_scene_map[VOL_M]->get_renderer()->DrawOn();

	render_scenes.emplace_back(m_scene_map[TRA_M]);
	render_scenes.emplace_back(m_scene_map[COR_M]);
	render_scenes.emplace_back(m_scene_map[SAG_M]);
	render_scenes.emplace_back(m_scene_map[VOL_M]);
	m_mprStyle->set_scenes(render_scenes);
	m_cur_layout = LAYOUT_TCSV;
	this->resizeEvent(nullptr);
}

void MultiImageView2::fullScreenSwitch(const ViewType& type, bool is_full /*= false*/)
{
	std::cout << "ViewType:" << type << ",is full:" << is_full << std::endl;
	QSize size = qvtk_widget_->size();
	int width = size.width();
	int height = size.height();

	if (is_full)
	{
		QMap<ViewType, PTR<ViewRenderScene>>::Iterator it = m_scene_map.begin();
		while (it != m_scene_map.end())
		{
			if (it.key() == type)
			{
				m_scene_map[type]->setViewport(0.0, 0.0, 1.0, 1.0);
				m_scene_map[type]->resize(width, height);
			}
			else
			{
				m_scene_map[it.key()]->setViewport(0.0, 0.0, 0.0, 0.0);
				m_scene_map[it.key()]->get_renderer()->DrawOff();
				//m_scene_map[it.key()]->get_button_widget()->Off();
			}
			it++;
		}
	}
	else
	{
		if (m_cur_layout == LAYOUT_TCSV)
			SwitchToTCSV();
		else if (m_cur_layout == LAYOUT_TCS)
			SwitchToTCS();
	}
}

void fullScreenCallback::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkButtonWidget* buttonWidget = reinterpret_cast<vtkButtonWidget*>(caller);
	auto* rep = reinterpret_cast<vtkTexturedButtonRepresentation2D*> (buttonWidget->GetRepresentation());
	bool state = rep->GetState();
	//this->Actor->GetProperty()->SetColor(reinterpret_cast<vtkActor *>(rep->GetButtonProp(state))->GetProperty()->GetColor());
	auto re = buttonWidget->GetCurrentRenderer();
	QMap<ViewType, PTR<ViewRenderScene>>::Iterator it = m_scene_map.begin();
	while (it != m_scene_map.end())
	{
		auto sc = it.value();
		if (sc->get_renderer().Get() == re)
		{
			m_view->fullScreenSwitch(it.key(), state);
			return;
		}
		it++;
	}
}
