#pragma once 
#include "BasicVtkDefine.h"
#include "ViewType.h"
#include "vtkInteractorStyleTrackballCamera.h"

class BasicImageStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static SPTR<BasicImageStyle> New(const ViewType& type);
	virtual ~BasicImageStyle();
	void setImageReslice(SPTR<vtkImageReslice> reslice)
	{
		reslice_ = reslice;
	}
	void setImageParameter(const int size[3], const double spacing[3], const double origin[3]);
protected:
	BasicImageStyle(const ViewType& type);


	void updataSlicer();

public:
	void OnMouseMove() override;
	void OnLeftButtonDown() override;
	void OnLeftButtonUp() override;
	void OnMiddleButtonDown() override;
	void OnMiddleButtonUp() override;
	void OnRightButtonDown() override;
	void OnRightButtonUp() override;
	void OnMouseWheelForward() override;
	void OnMouseWheelBackward() override;
private:
	ViewType view_type_;
	bool left_press_ = false;

	SPTR<vtkImageReslice> reslice_ = nullptr;
	int size_[3];
	double spacing_[3];
	double origin_[3];
};

