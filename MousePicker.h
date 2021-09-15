#pragma once 
#include "vtkPointPicker.h" 
#include "BasicVtkDefine.h"
class MousePicker : public vtkPointPicker
{
public:
	static SPTR<MousePicker> New();
	//析构函数
	virtual ~MousePicker();
	void initiate();
	void setImageData(SPTR<vtkImageData> img_data) {
		img_data_ = img_data;
	}
	void setRealOrigin(double* origin) {
		real_origin_[0] = origin[0];
		real_origin_[1] = origin[1];
		real_origin_[2] = origin[2];
	};

	void getWorldPositionFromDisplay(double x, double y, double* out , SPTR<vtkImageReslice> reslicer,SPTR<vtkRenderer> renderer);
	void getDcmPositionFromDisplay(double x, double y, double* out, SPTR<vtkImageReslice> reslicer,SPTR<vtkRenderer> renderer);
	void getDcmPositionFromWorld(const double xyz[3], double* out);
protected:
	//构造函数
	MousePicker();
private:
	SPTR<vtkImageData> img_data_;
	SPTR<vtkMatrix4x4> dicom_mat_;
	double real_origin_[3];

};

