
#include "MultiImageView2.h" 
#include "QVTKGraphView.h"
#include "ViewType.h"
#include <vtkMatrix4x4.h>
#include <QBoxLayout>
#include "QtWidgets/qwidget.h"
#include "QResizeEvent"
QWidget* MultiImageView2::create_view_container(QWidget* view)
{
	// create Widget Container
	QWidget* container = new QWidget();
	//container->setContentsMargins(0, 0, 0, 0);
	// create Widget Layout
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	// set Layout to Widget Container
	container->setLayout(layout);
	// set SizePolicy
	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(view);
	view->setParent(container);
	if (auto scene_view = dynamic_cast<QVTKGraphView*>(view))
		scene_view->set_view_container(container);
	return container;
}

MultiImageView2::MultiImageView2(QWidget* parent):QWidget(parent)
{
	//multi_widget_layout_ = nullptr;
	//view_container_t_ = nullptr;
	//view_container_c_ = nullptr;
	//view_container_v_ = nullptr;

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
}

//析构函数
MultiImageView2::~MultiImageView2()
{
	delete qvtk_widget_;
}

void MultiImageView2::initiate(SPTR<vtkImageData> img_data)
{
	img_data_ = img_data;
}

void MultiImageView2::showSingleView(const ViewType& type)
{
	//auto view = new QVTKGraphView(nullptr, type);
	//view_container_any_ = create_view_container(view);

	//// create Layouts
	//QBoxLayout* multi_widget_layout_ = new QHBoxLayout();
	//multi_widget_layout_->setContentsMargins(0, 0, 0, 0);
	//auto layout = new QVBoxLayout();
	//layout->addWidget(view_container_any_, 1);
	//multi_widget_layout_->addLayout(layout, 1);
	//main_widget_->setLayout(multi_widget_layout_);
}


void MultiImageView2::show_view(const double& leftX, const double& topY, const double& rightX, const double& bottomY, const ViewType& type)
{


	//WRenderViewCell* viewTra = m_mprStyle->getRenderView(type);

	//vtkNew<vtkMatrix4x4> mat;
	//double* m_posCenter = new double[3]{ 0,0,0 };

	//viewTra->setRenderWindow(qvtk_widget_->renderWindow());
	//viewTra->setViewport(leftX, topY, rightX, bottomY);
	//QRect rect = main_widget_->geometry();
	////将当前容器区域转到qt全局坐标
	//rect.setTopLeft(main_widget_->mapToGlobal(rect.topLeft()));
	//rect.setBottomRight(main_widget_->mapToGlobal(rect.bottomRight()));
	////结合容器将vtk视口区域转到qt全局坐标
	//rect.setLeft(rect.left() + rect.width() * leftX);
	//rect.setBottom(rect.top() + rect.height() * (1 - topY));
	//rect.setRight(rect.left() + rect.width() * rightX);
	//rect.setTop(rect.top() + rect.height() * (1 - bottomY));

	//m_mprStyle->setViewArea(rect, type);

	//viewTra->resetReslicer(img_data_, mat, m_posCenter);

}

void MultiImageView2::resizeEvent(QResizeEvent* event)
{
	QSize size = qvtk_widget_->size();
	int width = size.width();
	int height = size.height();
	if (m_cur_layout == LAYOUT_TCSV) 
	{
		m_scene_tra->resize(width * 0.5, height * 0.5);
		m_scene_cor->resize(width * 0.5, height * 0.5);
		m_scene_sag->resize(width * 0.5, height * 0.5);
		m_scene_vom->resize(width * 0.5, height * 0.5);
	}
}

void MultiImageView2::showCrossViews()
{
	std::vector<PTR<ViewRenderScene>> render_scenes;
	vtkNew<vtkMatrix4x4> mat;
	double* m_posCenter = new double[3]{ 0,0,0 };

	m_scene_tra = ViewRenderScene::New(qvtk_widget_->renderWindow(), TRA_M);
	m_scene_tra->setViewport(0.0, 0.0, 0.5, 0.5);
	m_scene_tra->resetReslicer(img_data_, mat, m_posCenter);
	m_scene_tra->set_full_screen(true);
	m_scene_tra->initiate(true, true,true);

	m_scene_cor = ViewRenderScene::New(qvtk_widget_->renderWindow(), COR_M);
	m_scene_cor->setViewport(0.0, 0.5, 0.5, 1.0);
	m_scene_cor->resetReslicer(img_data_, mat, m_posCenter);
	m_scene_cor->set_full_screen(true);
	m_scene_cor->initiate(true, true, true);


	m_scene_sag = ViewRenderScene::New(qvtk_widget_->renderWindow(), SAG_M);
	m_scene_sag->setViewport(0.5, 0.5, 1.0, 1.0);
	m_scene_sag->resetReslicer(img_data_, mat, m_posCenter);
	m_scene_sag->set_full_screen(true);
	m_scene_sag->initiate(true, true, true);

	m_scene_vom = ViewRenderScene::New(qvtk_widget_->renderWindow(), VOL_M);
	m_scene_vom->setViewport(0.5, 0.0, 1.0, 0.5);
	m_scene_vom->resetReslicer(img_data_, mat, m_posCenter);
	m_scene_vom->set_full_screen(true);
	m_scene_vom->initiate();

	qvtk_widget_->interactor()->Render();
	m_cur_layout = LAYOUT_TCSV;

	render_scenes.emplace_back(m_scene_tra);
	render_scenes.emplace_back(m_scene_cor);
	render_scenes.emplace_back(m_scene_sag);
	render_scenes.emplace_back(m_scene_vom);
	m_mprStyle->set_scenes(render_scenes);
	//show_view(0.0, 0.0, 0.5, 0.5, TRA_M);
	//show_view(0.0, 0.5, 0.5, 1.0, SAG_M);
	//show_view(0.5, 0.5, 1.0, 1.0, COR_M);
	//show_view(0.5, 0.0, 1.0, 0.5, TRA);




	//vtkNew<vtkMatrix4x4> mat;
	//{
	//	auto view = new QVTKGraphView(nullptr, TRA);
	//	view_container_t_ = create_view_container(view);
	//	view_container_t_->setObjectName("view_container_t");
	//	view->initInfo();
	//	view->bindViewData(img_data_, mat);
	//}
	//{
	//	auto view = new QVTKGraphView(nullptr, COR);
	//	view_container_c_ = create_view_container(view);
	//	view_container_c_->setObjectName("view_container_c");
	//	view->initInfo();
	//	view->bindViewData(img_data_, mat);

	//}
	//{
	//	auto view = new QVTKGraphView(nullptr, SAG);
	//	view_container_s_ = create_view_container(view);
	//	view->initInfo();
	//	view->bindViewData(img_data_, mat);

	//}
	//{
	//	auto view = new QVTKGraphView(nullptr, TRA);
	//	view_container_v_ = create_view_container(view);
	//	view_container_s_->setObjectName("view_container_s");

	//	//SPTR <vtkSmartVolumeMapper> mapper = SPTR<vtkSmartVolumeMapper>::New();
	//	//view->bindViewData3D();

	//}

	//QGridLayout* gridlayout = new QGridLayout;
	//gridlayout->addWidget(view_container_c_, 0, 0);
	//gridlayout->addWidget(view_container_s_, 0, 1);
	//gridlayout->addWidget(view_container_t_, 1, 0);
	//gridlayout->addWidget(view_container_v_, 1, 1);
	//main_widget_->setLayout(gridlayout);
}
