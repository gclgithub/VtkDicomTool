
#include "QVTKGraphView.h" 
#include <QMouseEvent>
#include <vtkCamera.h>
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
class StatusMessage
{
public:
	static std::string FormatWl(int ww, int wl)
	{
		std::stringstream tmp;
		tmp << "ww:" << ww << "/wl:" << wl;
		return tmp.str();
	}

	static std::string FormatPosition(double x, double y, double z)
	{
		std::stringstream tmp;
		tmp.precision(2);
		tmp.setf(std::ios::fixed);
		tmp << "Position:" << x << "," << y << "," << z;
		return tmp.str();
	}

	static std::string FormatSlicer(int index, int count)
	{
		std::stringstream tmp;
		tmp << "Slice:" << index << "/" << count;
		return tmp.str();
	}
};

QVTKGraphView::QVTKGraphView(QWidget* parent, const ViewType& type)
	:QVTKOpenGLNativeWidget(parent), view_type_(type)
{
	//去掉vtk体渲染的降采样效果
	interactor()->SetDesiredUpdateRate(0.1);
	interactor()->SetStillUpdateRate(0.1);

	renderer_ = SPTR<vtkOpenGLRenderer>::New();
	switch (type)
	{
	case ViewType::COR_M:
		renderer_->SetBackground(0, 0.2, 0);
		break;
	case ViewType::SAG_M:
		renderer_->SetBackground(0, 0, 0.2);
		break;
	case ViewType::TRA_M:
		renderer_->SetBackground(0.2, 0, 0);
		break;
	default:
		renderer_->SetBackground(0.2, 0, 0.2);
		break;
	}

	reslice_ = SPTR<vtkImageReslice>::New();
	renderWindow()->AddRenderer(renderer_);
}

//析构函数
QVTKGraphView::~QVTKGraphView()
{
}

void QVTKGraphView::initInfo()
{
	showViewDirection();
	showViewInfo();

	initStyles();

	interactor()->Render();
}

void QVTKGraphView::bindViewData(SPTR<vtkImageData> imgData, SPTR<vtkMatrix4x4> vtkmat)
{
	imgData->GetDimensions(img_size_);
	imgData->GetSpacing(img_spacing_);
	imgData->GetOrigin(img_origin_);
	double center[3];
	center[0] = img_origin_[0] + img_spacing_[0] * 0.5 * img_size_[0];
	center[1] = img_origin_[1] + img_spacing_[1] * 0.5 * img_size_[1];
	center[2] = img_origin_[2] + img_spacing_[2] * 0.5 * img_size_[2];

	static double axialElements[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 };

	static double cornalElements[16] = {
	1, 0, 0, 0,
	0, 0, 1, 0,
	0, -1,0, 0,
	0, 0, 0, 1 };

	static double sagittalElements[16] = {
	0, 0,-1, 0,
	1, 0, 0, 0,
	0,-1, 0, 0,
	0, 0, 0, 1 };

	auto elements = axialElements;
	if (view_type_ == SAG_M)
		elements = sagittalElements;
	else if(view_type_ == COR_M)
		elements = cornalElements;

	vtkNew<vtkMatrix4x4> resliceAxes;
	resliceAxes->DeepCopy(elements);
	resliceAxes->SetElement(0, 3, center[0]);
	resliceAxes->SetElement(1, 3, center[1]);
	resliceAxes->SetElement(2, 3, center[2]);

	reslice_->SetInputData(imgData);
	reslice_->SetOutputDimensionality(2);
	reslice_->SetResliceAxes(resliceAxes);
	reslice_->SetInterpolationModeToLinear();
	//*****************************************************************//
	vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(0, 1000);
	colorTable->SetValueRange(0.0, 1.0);
	colorTable->SetSaturationRange(0.0, 0.0);
	colorTable->SetRampToLinear();
	colorTable->Build();
	color_map_ = vtkSmartPointer<vtkImageMapToColors>::New();
	color_map_->SetLookupTable(colorTable);
	color_map_->SetInputConnection(reslice_->GetOutputPort());
	color_map_->Update();

	img_act_ = vtkImageActor::New();
	img_act_->SetInputData(color_map_->GetOutput());
	renderer_->AddActor(img_act_);
	img_act_->PickableOff();
	
	auto c_origin = reslice_->GetResliceAxesOrigin();
	int  index = 3 - view_type_;
	int cur_index = (c_origin[index] - img_origin_[index])/img_spacing_[index] + 1;
	int max_count = img_size_[3 - view_type_];
	//RENDER_DATA_MODEL->updataInfoSlicer(cur_index, max_count, view_type_);
	if (basic_style_)
	{
		basic_style_->setImageParameter(img_size_, img_spacing_, img_origin_);
	}

}

void QVTKGraphView::bindViewData3D(SPTR<vtkVolume> vol)
{
	renderer_->AddActor(vol);
}

void QVTKGraphView::showViewInfo()
{
	int font_text = 16;
	int low_height = 10;

	is_show_info_ = true;
	std::string msg = StatusMessage::FormatPosition(0, 0, 0);
	if (is_show_info_)
	{
		SPTR<vtkActor2D> actor_top_a = SPTR<vtkActor2D>::New();
		SPTR<vtkActor2D> actor_top_b = SPTR<vtkActor2D>::New();
		renderer_->AddActor2D(actor_top_a);
		renderer_->AddActor2D(actor_top_b);
		{
			vtkNew<vtkTextProperty> PositionTextProp;
			PositionTextProp->SetFontFamilyToCourier();
			PositionTextProp->SetFontSize(font_text);
			PositionTextProp->SetVerticalJustificationToBottom();
			PositionTextProp->SetJustificationToLeft();

			vtkNew<vtkTextMapper>PositionTextMapper;
			PositionTextMapper->SetInput(msg.c_str());
			PositionTextMapper->SetTextProperty(PositionTextProp);

			actor_top_a->SetMapper(PositionTextMapper);
			actor_top_a->SetPosition(10, low_height);
		}

		//position
		low_height += 20;
		msg = StatusMessage::FormatWl(128, 255);
		for (int i = 0; i < 3; ++i)
		{
			vtkNew<vtkTextProperty> sliceTextProp;
			sliceTextProp->SetFontFamilyToCourier();
			sliceTextProp->SetFontSize(font_text);
			sliceTextProp->SetVerticalJustificationToBottom();
			sliceTextProp->SetJustificationToLeft();

			vtkNew<vtkTextMapper>sliceTextMapper;
			sliceTextMapper->SetInput(msg.c_str());
			sliceTextMapper->SetTextProperty(sliceTextProp);

			actor_top_b->SetMapper(sliceTextMapper);
			actor_top_b->SetPosition(10, low_height);
		}

		actor_top_name_ = SPTR<vtkActor2D>::New();
		actor_top_licer_ = SPTR<vtkActor2D>::New();
		renderer_->AddActor2D(actor_top_name_);
		renderer_->AddActor2D(actor_top_licer_);

		int top_height = 500;
		{
			vtkNew<vtkTextProperty> sliceTextProp;
			sliceTextProp->SetFontFamilyToCourier();
			sliceTextProp->SetFontSize(font_text);
			sliceTextProp->SetVerticalJustificationToTop();
			sliceTextProp->SetJustificationToLeft();

			vtkNew<vtkTextMapper>sliceTextMapper;
			if (view_type_ == TRA_M)
				sliceTextMapper->SetInput("Transverse");
			else if (view_type_ == COR_M)
				sliceTextMapper->SetInput("Coronal");
			else
				sliceTextMapper->SetInput("Sagittal");

			sliceTextMapper->SetTextProperty(sliceTextProp);
			actor_top_name_->SetMapper(sliceTextMapper);
			actor_top_name_->SetPosition(10, top_height);
		}

		top_height -= 20;
		msg = StatusMessage::FormatSlicer(0, 0);
		{
			vtkNew<vtkTextProperty> sliceTextProp;
			sliceTextProp->SetFontFamilyToCourier();
			sliceTextProp->SetFontSize(font_text);
			sliceTextProp->SetVerticalJustificationToTop();
			sliceTextProp->SetJustificationToLeft();

			vtkNew<vtkTextMapper>sliceTextMapper;
			sliceTextMapper->SetInput(msg.c_str());
			sliceTextMapper->SetTextProperty(sliceTextProp);
			actor_top_licer_->SetMapper(sliceTextMapper);
			actor_top_licer_->SetPosition(10, top_height);
		}

	}
}

void QVTKGraphView::showViewDirection()
{
	int font_text = 20;
	QList<std::string> direction = { "R","A","L","P" };
	if (view_type_ == TRA_M)
		direction = { "R","A","L","P" };
	else if (view_type_ == COR_M)
		direction = { "R","H","L","F" };
	else if (view_type_ == SAG_M)
		direction = { "A","H","P","F" };

	for (int i = 0; i < 4; ++i)
	{
		vtkNew<vtkTextProperty> sliceTextProp;
		sliceTextProp->SetFontFamilyToCourier();
		sliceTextProp->SetFontSize(font_text);
		sliceTextProp->SetVerticalJustificationToCentered();
		sliceTextProp->SetJustificationToCentered();
		vtkNew<vtkTextMapper>sliceTextMapper;
		sliceTextMapper->SetInput(direction[i].c_str());
		sliceTextMapper->SetTextProperty(sliceTextProp);
		vtkNew<vtkActor2D> actor;
		actor->SetMapper(sliceTextMapper);
		actor->SetPosition(10, 10);
		actor_pos_list_.push_back(actor);
		renderer_->AddActor2D(actor);
	}
}

void QVTKGraphView::initStyles()
{
	basic_style_ = BasicImageStyle::New(view_type_);
	basic_style_->setImageReslice(reslice_);
	interactor()->SetInteractorStyle(basic_style_);
	basic_style_->Off();
}

void QVTKGraphView::mousePressEvent(QMouseEvent* event)
{
	if (basic_style_)
	{
		//event->get
		//mpr_style_->OnLeftButtonDown();
	}
	return QVTKOpenGLNativeWidget::mousePressEvent(event);

}

void QVTKGraphView::mouseDoubleClickEvent(QMouseEvent* event)
{
	int i = view_type_;
	return QVTKOpenGLNativeWidget::mouseDoubleClickEvent(event);

}

void QVTKGraphView::mouseMoveEvent(QMouseEvent* event)
{
	//if (mpr_style_)
	//{
	//	mpr_style_->OnMouseMove();
	//}
	return QVTKOpenGLNativeWidget::mouseMoveEvent(event);

}

void QVTKGraphView::mouseReleaseEvent(QMouseEvent* event)
{

	if (basic_style_)
	{
		basic_style_->OnLeftButtonUp();
	}
	return QVTKOpenGLNativeWidget::mouseReleaseEvent(event);

}

void QVTKGraphView::wheelEvent(QWheelEvent* event)
{
	if (basic_style_)
	{
		//auto reslice_->GetResliceAxes();
		color_map_->Update();
	}
	interactor()->Render();
	//return QVTKOpenGLNativeWidget::wheelEvent(event);
}

void QVTKGraphView::keyPressEvent(QKeyEvent* event)
{
	int i = view_type_;
	return QVTKOpenGLNativeWidget::keyPressEvent(event);

}

void QVTKGraphView::enterEvent(QEvent* e)
{
	this->setFocus();
	if (view_container_)
		view_container_->setStyleSheet("QWidget { border: 1px solid #FF8000; }");

	return QVTKOpenGLNativeWidget::enterEvent(e);

}

void QVTKGraphView::leaveEvent(QEvent* e)
{
	if (view_container_)
		view_container_->setStyleSheet("QWidget { border: 1px solid transparent; }");

	return QVTKOpenGLNativeWidget::leaveEvent(e);
}

void QVTKGraphView::resizeEvent(QResizeEvent* e)
{
	int w = width();
	int h = height();

	if (actor_top_name_) 
	{
		actor_top_name_->SetPosition(10, h - 10);
	}
	if (actor_top_licer_) 
	{
		actor_top_licer_->SetPosition(10, h - 30);

	}
	for (int i = 0; i < actor_pos_list_.size(); ++i)
	{
		auto actor = actor_pos_list_[i];
		if (i == 0)
			actor->SetPosition(30, h * 0.5);
		else if (i == 1)
			actor->SetPosition(w * 0.5, h - 30);
		else if (i == 2)
			actor->SetPosition(w - 30, h * 0.5);
		else
			actor->SetPosition(w * 0.5, 30);
	}
	if (img_act_)
	{
		auto cam = renderer_->GetActiveCamera();
		cam->ParallelProjectionOn();
		auto imgData = img_act_->GetInput();
		auto size = imgData->GetDimensions();
		auto spacing = imgData->GetSpacing();
		double rx, ry;
		rx = size[0] * spacing[0];
		ry = size[1] * spacing[1];
		double zoom_factor = rx > ry ? rx : ry;
		cam->SetParallelScale(zoom_factor * 0.5);
	}
	return QVTKOpenGLNativeWidget::resizeEvent(e);
}
