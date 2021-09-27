#pragma once 
#include <QWidget>
#include "BasicVtkDefine.h"
#include "MPRStyle.h"
#include "MousePicker.h"
#include "QVTKOpenGLNativeWidget.h"
#include "ViewRenderScene.h"
class SceneCrossLine;
class fullScreenCallback;
class MultiImageView2 : public QWidget
{
	Q_OBJECT
public:
	MultiImageView2(QWidget* parent);
	virtual ~MultiImageView2();
	void initiate(SPTR<vtkImageData> img_data);

	////显示某个视口，单个独占，撑满整个窗口空间
	//void showSingleView(const ViewType& type);
	void SwitchToTCS();
	//显示交叉四视口模式
	void SwitchToTCSV();
	void fullScreenSwitch(const ViewType& type,bool is_full = false);
protected:
	void resizeEvent(QResizeEvent* event);
private:
	GetMacro(Layout, cur_layout)
	VarMacro(PTR<SceneCrossLine>, cross_line)

	QMap<ViewType, PTR<ViewRenderScene>> m_scene_map;
	QVTKOpenGLNativeWidget* qvtk_widget_ = nullptr;
	SPTR<MousePicker> m_mouse_picker;
	MPRStyle* m_mprStyle = nullptr;
	SPTR<vtkImageData> img_data_ = nullptr;
	fullScreenCallback* m_full_screen_callback;
};

class fullScreenCallback : public vtkCommand
{
public:
	static fullScreenCallback* New(MultiImageView2* view)
	{
		return new fullScreenCallback(view);
	}
	virtual void Execute(vtkObject* caller, unsigned long, void*);
	void set_scenes(QMap<ViewType, PTR<ViewRenderScene>> scenes) 
	{
		m_scene_map = scenes;
	}
private:
	fullScreenCallback(MultiImageView2* view) { m_view = view; };
	QMap<ViewType, PTR<ViewRenderScene>> m_scene_map;
	MultiImageView2* m_view = nullptr;
};