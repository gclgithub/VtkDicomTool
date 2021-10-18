#pragma once 
#include "BasicVtkDefine.h"
#include "ViewType.h"
#include "vtkButtonWidget.h"
#include "SceneReslice.h"
class Navi3DCameraParams;
class SceneCrossLine;
class ViewRenderScene
{
public:
	static PTR<ViewRenderScene> New(SPTR<vtkRenderWindow> rewin,const ViewType& viewType);
	virtual ~ViewRenderScene();
	void resize(const int& width, const int& height);
	void initiate();
	void resetScene();
	//left top info = true、left bot、direction
	void initiate(const bool& show_left_top /*= true*/, const bool& show_left_bot /*= true*/,bool show_direction= true);
	void set_callback(vtkCommand* callback);
	void set_button_widget_state();
	void updateInfoWL(const double& wl, const double& ww);
	void updateInfoSlicer();
	void updateInfoPosition(const double& x, const double& y, const double& z);

	void load_data(SPTR<vtkImageData> imgData, SPTR<vtkMatrix4x4> vtkmat);
protected:
	ViewRenderScene(SPTR<vtkRenderWindow> rewin, const ViewType& viewType);
	//update crossline
	void update_crossline();
private:
	GetMacro(SPTR<vtkOpenGLRenderer>, renderer)
	GetMacro(PTR<SceneReslice>, reslice)

	VarMacro(std::string, view_type_msg)
	VarMacro(SPTR<vtkActor2D>, view_type_actor)
	VarMacro(SPTR<vtkActor2D>, slicer_actor)
	VarMacro(QVector<SPTR<vtkActor2D>>, direction_actor_list)
	VarMacro(SPTR<vtkActor2D>, position_actor)
	VarMacro(SPTR<vtkActor2D>, wlww_actor)

	SetMacro(SPTR<vtkRenderWindowInteractor>, interactor)
	GetMacro(SPTR<vtkButtonWidget>, button_widget)
	SetMacro(bool, full_screen)
	GetMacro(ViewType, viewType)

	//crossline
	SetGetMacro(PTR<SceneCrossLine>, cross_line)
	VarMacro(bool, first_add_line)
private:
	void initSceneDirection();
	void initSceneLeftTopInfo();
	void initSceneLeftBotInfo();
	void initSceneButton();
	//重新设置重建平面对象 ，二维视口
	void initReslicer(SPTR<vtkImageData> imgData, SPTR<vtkMatrix4x4> vtkmat, double* pos);
public:
	void setViewport(const double& leftX, const double& topY, const double& rightX, const double& bottomY);
	SPTR<vtkCamera> getCamera();
	//重置摄像机视角
	void resetCamera();
	void addActor(SPTR <vtkProp> prop);
	void removeActor(SPTR <vtkProp> prop);
	void initVolume(SPTR<vtkImageData> imgData);
private:
	Navi3DCameraParams* m_camParam = nullptr;
	void _initCamrea();

	double m_cam_pos[3] = { 0,0,0 };
	int m_width;
	int m_height;
	QList<std::string> m_direction;
};

//摄像机参数记录

class Navi3DCameraParams
{
public:
	Navi3DCameraParams() {
		m_bRecorded = false;


		isParallel = true;

		px = 0;
		py = 0;
		pz = 0;

		fx = 0;
		fy = 0;
		fz = 0;
		upx = 0;
		upy = 0;
		upz = 0;
		clippingNear = 0.0001;
		clippingFar = 10000;

		viewAngle = 50;
		parallelScale = 1;

	};
	bool  m_bRecorded = false;
public:

	bool  isParallel = true;
	//坐标
	double px = 0;
	double py = 0;
	double pz = 0;
	//焦点
	double fx = 0;
	double fy = 0;
	double fz = 0;
	//viewUp 
	double upx = 0;
	double upy = 0;
	double upz = 0;
	//远近裁剪
	double clippingNear = 0.0001;
	double clippingFar = 10000;

	double viewAngle = 50;
	double parallelScale = 1;

	void record(vtkCamera* cam);
	void apply(vtkCamera * cam);
	bool hasRecorded() { return m_bRecorded; };

	void clearRecored() { m_bRecorded = false; };

	~Navi3DCameraParams() {};
};
