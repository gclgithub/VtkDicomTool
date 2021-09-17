#pragma once
#include "BasicVtkDefine.h"
class SceneReslice 
{
public:
	static PTR<SceneReslice> New(SPTR<vtkImageData> data, SPTR<vtkMatrix4x4> mtx);
	~SceneReslice();
	void moveReslice(int value);
	void doReslice();
	void doReslice(SPTR<vtkMatrix4x4> matrix);
	double* get_center() { return m_center; }
	void get_window_level(double& ww, double& wl, double& defaultWW, double& defaultWL);
private:
	SceneReslice(SPTR<vtkImageData> data, SPTR<vtkMatrix4x4> mtx);
private:
	GetMacro(SPTR<vtkImageReslice>,reslice)
	GetMacro(SPTR<vtkImageActor>, imgActor)
	GetMacro(SPTR<vtkImageMapToColors>, color_map)
	GetMacro(SPTR<vtkMatrix4x4>, roatMatx)

	int m_img_size[3];
	double m_img_spacing[3];
	double m_img_origin[3];
	double m_center[3];

	//window level
	double m_ww;
	double m_wl;
	double m_wwDefault;
	double m_wlDefault;
};

