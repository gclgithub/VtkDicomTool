
#include "MousePicker.h" 
#include "vtkImageData.h"
#include "vtkMatrix4x4.h"
#include "vtkRenderer.h"
#include "vtkImageReslice.h"
SPTR<MousePicker> MousePicker::New()
{
	return SPTR<MousePicker>(new MousePicker);
}

//构造函数
MousePicker::MousePicker() :vtkPointPicker()
{
}

//析构函数
MousePicker::~MousePicker()
{
}

void MousePicker::initiate()
{
	if (img_data_== nullptr)return;
	int size[3];
	double spacing[3];
	img_data_->GetDimensions(size);
	img_data_->GetSpacing(spacing);
	dicom_mat_ = SPTR<vtkMatrix4x4>::New();

	dicom_mat_->SetElement(1, 1, -1);
	dicom_mat_->SetElement(1, 3, size[1] * spacing[1]);

	dicom_mat_->SetElement(2, 2, -1);
	dicom_mat_->SetElement(2, 3, size[2] * spacing[2]);
}

void MousePicker::getWorldPositionFromDisplay(double x, double y, double* out, SPTR<vtkImageReslice> reslicer, SPTR<vtkRenderer> renderer)
{

	double centers[3];
	reslicer->GetResliceAxesOrigin(centers);
	std::cout << "centers:" << centers[0] << "," << centers[1] << "," << centers[2] << std::endl;

	this->Pick(x, y, 0, renderer);
	auto value = GetPickPosition();
	out[0] = value[0];
	out[1] = value[1];
	out[2] = centers[2];
}

void MousePicker::getDcmPositionFromDisplay(double x, double y, double* out, SPTR<vtkImageReslice> reslicer, SPTR<vtkRenderer> renderer)
{
	double centers[3];
	reslicer->GetResliceAxesOrigin(centers);
	std::cout << "centers:" << centers[0] << "," << centers[1] << "," << centers[2] << std::endl;

	double world_pos[3];
	this->Pick(x, y, 0, renderer);
	this->GetPickPosition(world_pos);
	double in[4] = { world_pos[0],world_pos[1],centers[2],1 };

	vtkNew<vtkMatrix4x4> invert_mat;
	vtkMatrix4x4::Invert(dicom_mat_, invert_mat);
	auto value = invert_mat->MultiplyDoublePoint(in);
	out[0] = value[0] + real_origin_[0];
	out[1] = value[1] + real_origin_[1];
	out[2] = value[2] + real_origin_[2];
}

void MousePicker::getDcmPositionFromWorld(const double xyz[3], double* out)
{
	double in[4] = { xyz[0],xyz[1],xyz[2],1 };
	vtkNew<vtkMatrix4x4> invert_mat;
	vtkMatrix4x4::Invert(dicom_mat_, invert_mat);
	auto value = invert_mat->MultiplyDoublePoint(in);
	out[0] = value[0] + real_origin_[0];
	out[1] = value[1] + real_origin_[1];
	out[2] = value[2] + real_origin_[2];
}
