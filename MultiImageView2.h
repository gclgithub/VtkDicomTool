#pragma once 
#include <QWidget>
#include "BasicVtkDefine.h"
#include "MPRStyle.h"
#include "MousePicker.h"
#include "QVTKOpenGLNativeWidget.h"
#include "ViewRenderScene.h"
class MultiImageView2 : public QWidget
{
	Q_OBJECT
public:
	MultiImageView2(QWidget* parent);
	virtual ~MultiImageView2();
	void initiate(SPTR<vtkImageData> img_data);

	//显示某个视口，单个独占，撑满整个窗口空间
	void showSingleView(const ViewType& type);
	//显示交叉四视口模式
	void showCrossViews();

	QWidget* create_view_container(QWidget* view);

	void show_view(const double& leftX, const double& topY, const double& rightX, const double& bottomY, const ViewType& type);
protected:
	void resizeEvent(QResizeEvent* event);
private:
	VarMacro(PTR<ViewRenderScene>, scene_tra)
	VarMacro(PTR<ViewRenderScene>, scene_cor)
	VarMacro(PTR<ViewRenderScene>, scene_sag)
	VarMacro(PTR<ViewRenderScene>, scene_vom)

	//QBoxLayout* multi_widget_layout_;
	//QWidget* view_container_t_;
	//QWidget* view_container_c_;
	//QWidget* view_container_s_;
	//QWidget* view_container_v_;

	//QWidget* view_container_any_;
	//QWidget* view_container_;

	QVTKOpenGLNativeWidget* qvtk_widget_ = nullptr;
	SPTR<MousePicker> mouse_picker_;
	MPRStyle* m_mprStyle = nullptr;
	SPTR<vtkImageData> img_data_ = nullptr;
	Layout m_cur_layout = LAYOUT_NONE;
};

