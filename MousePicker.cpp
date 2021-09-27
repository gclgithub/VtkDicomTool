
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
MousePicker::MousePicker() :vtkCellPicker()
{
	this->SetTolerance(0.005);
	//this->PickFromListOn(); add will pick bug
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

void MousePicker::getWorldPositionFromDisplay(double x, double y, double* out, PTR<SceneReslice> reslicer, SPTR<vtkRenderer> renderer)
{
	//pick
	this->Pick(x, y, 0, renderer);
	auto value = GetPickPosition();

	auto actor = reslicer->get_imgActor();
	auto pos = actor->GetPosition();
	auto orie = actor->GetOrientation();
	if (orie[0] == 90)
	{
		out[0] = value[0];
		out[1] = pos[1];
		out[2] = value[2];
	}
	else if (orie[1] == 90)
	{
		out[0] = pos[0];
		out[1] = value[1];
		out[2] = value[2];
	}
	else
	{
		out[0] = value[0];
		out[1] = value[1];
		out[2] = pos[2];
	}
}

void MousePicker::getDcmPositionFromDisplay(double x, double y, double* out, PTR<SceneReslice> reslicer, SPTR<vtkRenderer> renderer)
{
	//if (auto actor = reslicer->get_imgActor())
	//{
	//	double pos[] = { 0,0,0 };
	//	getWorldPositionFromDisplay(pos);
	//	double in[4] = { pos[0],pos[1],pos[2],1 };

	//	vtkNew<vtkMatrix4x4> invert_mat;
	//	vtkMatrix4x4::Invert(dicom_mat_, invert_mat);
	//	auto value = invert_mat->MultiplyDoublePoint(in);
	//	out[0] = value[0] + real_origin_[0];
	//	out[1] = value[1] + real_origin_[1];
	//	out[2] = value[2] + real_origin_[2];
	//}
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

void MousePicker::set_crossline(PTR<SceneCrossLine> cross_line)
{
	m_cross_line = cross_line;
	AddPickList(m_cross_line->get_line_actor_list()[0]);
	AddPickList(m_cross_line->get_line_actor_list()[1]);
	AddPickList(m_cross_line->get_line_actor_list()[2]);
}
