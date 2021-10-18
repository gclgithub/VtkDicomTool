
#include "MultiImageView.h" 
#include "QVTKGraphView.h"
#include <vtk-9.0/vtkRenderWindow.h>
#include <vtk-9.0/vtkMatrix4x4.h>;
QWidget* MultiImageView::create_view_container(QWidget* view)
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

MultiImageView::MultiImageView(QWidget* main_widget) :main_widget_(main_widget)
{
	//multi_widget_layout_ = nullptr;
	view_container_t_ = nullptr;
	view_container_c_ = nullptr;
	view_container_s_ = nullptr;
	view_container_v_ = nullptr;
}

//析构函数
MultiImageView::~MultiImageView()
{
}

void MultiImageView::initiate(SPTR<vtkImageData> img_data)
{
	img_data_ = img_data;
}

void MultiImageView::showSingleView(const ViewType& type)
{
	auto view = new QVTKGraphView(nullptr, type);
	view_container_any_ = create_view_container(view);

	// create Layouts
	QBoxLayout* multi_widget_layout_ = new QHBoxLayout();
	multi_widget_layout_->setContentsMargins(0, 0, 0, 0);
	auto layout = new QVBoxLayout();
	layout->addWidget(view_container_any_, 1);
	multi_widget_layout_->addLayout(layout, 1);
	main_widget_->setLayout(multi_widget_layout_);
}

void MultiImageView::showCrossViews()
{
	vtkNew<vtkMatrix4x4> mat;
	{
		auto view = new QVTKGraphView(nullptr, TRA_M);
		view_container_t_ = create_view_container(view);
		view_container_t_->setObjectName("view_container_t");
		view->initInfo();
		view->bindViewData(img_data_, mat);
	}
	{
		auto view = new QVTKGraphView(nullptr, COR_M);
		view_container_c_ = create_view_container(view);
		view_container_c_->setObjectName("view_container_c");
		view->initInfo();
		view->bindViewData(img_data_, mat);

	}
	{
		auto view = new QVTKGraphView(nullptr, SAG_M);
		view_container_s_ = create_view_container(view);
		view->initInfo();
		view->bindViewData(img_data_, mat);

	}
	{
		auto view = new QVTKGraphView(nullptr, TRA_M);
		view_container_v_ = create_view_container(view);
		view_container_s_->setObjectName("view_container_s");

		//SPTR <vtkSmartVolumeMapper> mapper = SPTR<vtkSmartVolumeMapper>::New();
		//view->bindViewData3D();

	}

	QGridLayout* gridlayout = new QGridLayout;
	gridlayout->addWidget(view_container_c_, 0, 0);
	gridlayout->addWidget(view_container_s_, 0, 1);
	gridlayout->addWidget(view_container_t_, 1, 0);
	gridlayout->addWidget(view_container_v_, 1, 1);
	main_widget_->setLayout(gridlayout);
}
