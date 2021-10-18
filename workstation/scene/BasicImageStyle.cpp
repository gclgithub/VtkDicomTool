
#include "BasicImageStyle.h" 

SPTR<BasicImageStyle> BasicImageStyle::New(const ViewType& type)
{
	return SPTR<BasicImageStyle>(new BasicImageStyle(type));
}

//构造函数
BasicImageStyle::BasicImageStyle(const ViewType& type) :vtkInteractorStyleTrackballCamera(), view_type_(type)
{
}

void BasicImageStyle::updataSlicer()
{
	auto c_origin = reslice_->GetResliceAxesOrigin();
	auto mat1 = reslice_->GetResliceAxes();

	vtkNew<vtkMatrix4x4> mat;
	mat->SetElement(1, 1, -1);
	mat->SetElement(2, 2, -1);
	double k[3];
	k[0] = mat->GetElement(0, 0) * c_origin[0];
	k[1] = mat->GetElement(1, 1) * c_origin[1];
	k[0] = mat->GetElement(2, 2) * c_origin[2];

	int  index = 3 - view_type_;
	int cur_index = (c_origin[index] - origin_[index]) / spacing_[index] + 1;
	int max_count = size_[3 - view_type_];
	//RENDER_DATA_MODEL->updataInfoSlicer(cur_index, max_count, view_type_);
}

//析构函数
BasicImageStyle::~BasicImageStyle()
{
}

void BasicImageStyle::setImageParameter(const int size[3], const double spacing[3], const double origin[3])
{
	size_[0] = size[0];
	size_[1] = size[1];
	size_[2] = size[2];

	spacing_[0] = spacing[0];
	spacing_[1] = spacing[1];
	spacing_[2] = spacing[2];

	origin_[0] = origin[0];
	origin_[1] = origin[1];
	origin_[2] = origin[2];
}

void BasicImageStyle::OnMouseMove()
{

}

void BasicImageStyle::OnLeftButtonDown()
{

}

void BasicImageStyle::OnLeftButtonUp()
{
}

void BasicImageStyle::OnMiddleButtonDown()
{

}

void BasicImageStyle::OnMiddleButtonUp()
{

}

void BasicImageStyle::OnRightButtonDown()
{

}

void BasicImageStyle::OnRightButtonUp()
{
}

void BasicImageStyle::OnMouseWheelForward()
{
	//if (view_type_ == VOL)
	//{
	//	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	//	return;
	//}

	if (reslice_)
	{
		double spacing = reslice_->GetOutput()->GetSpacing()[2];
		auto matrix = reslice_->GetResliceAxes();
		double point[4], center[4];
		point[0] = 0.0;
		point[1] = 0.0;
		point[2] = spacing * 1;
		point[3] = 1.0;
		matrix->MultiplyPoint(point, center);
		matrix->SetElement(0, 3, center[0]);
		matrix->SetElement(1, 3, center[1]);
		matrix->SetElement(2, 3, center[2]);
		reslice_->Update();
	}
	updataSlicer();
}

void BasicImageStyle::OnMouseWheelBackward()
{
	/*if (view_type_ == VOL_M)
	{
		vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
		return;
	}*/
	if (reslice_)
	{
		double spacing = reslice_->GetOutput()->GetSpacing()[2];
		auto matrix = reslice_->GetResliceAxes();
		double point[4], center[4];
		point[0] = 0.0;
		point[1] = 0.0;
		point[2] = -spacing * 1;
		point[3] = 1.0;

		matrix->MultiplyPoint(point, center);
		matrix->SetElement(0, 3, center[0]);
		matrix->SetElement(1, 3, center[1]);
		matrix->SetElement(2, 3, center[2]);
		reslice_->Update();
	}
	updataSlicer();
}
