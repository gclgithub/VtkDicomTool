#pragma once 
#include <QWidget>
#include "BasicImageStyle.h"
#include "BasicVtkDefine.h"
#include "QVTKOpenGLNativeWidget.h"
class QVTKGraphView : public QVTKOpenGLNativeWidget
{
public:

	QVTKGraphView(QWidget* parent, const ViewType& type);
	virtual ~QVTKGraphView();
	void initInfo();

	void set_view_container(QWidget* container) 
	{
		view_container_ = container;
	}
	void bindViewData(SPTR<vtkImageData> imgData, SPTR<vtkMatrix4x4> vtkmat);
	void bindViewData3D(SPTR<vtkVolume> vol);


	void showViewInfo();
	void showViewDirection();
	void initStyles();
protected:
	// overloaded mouse press handler
	void mousePressEvent(QMouseEvent* event) override;
	// overloaded mouse double-click handler
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	// overloaded mouse move handler
	void mouseMoveEvent(QMouseEvent* event) override;
	// overloaded mouse release handler
	void mouseReleaseEvent(QMouseEvent* event) override;
	//overloaded mouse wheel handler
	
	void wheelEvent(QWheelEvent* event) override;
	// overloaded key press handler
	void keyPressEvent(QKeyEvent* event) override;
	// overloaded enter handler
	void enterEvent(QEvent*) override;
	// overloaded leave handler
	void leaveEvent(QEvent*) override;

	void resizeEvent(QResizeEvent* e) override;
private:
	SPTR<vtkOpenGLRenderer> renderer_ = nullptr;
	SPTR<vtkImageReslice> reslice_ = nullptr;
	SPTR <vtkImageActor> img_act_ = nullptr;
	SPTR<vtkImageMapToColors> color_map_ = nullptr;


	SPTR<vtkActor2D> actor_top_name_ = nullptr;
	SPTR<vtkActor2D> actor_top_licer_ = nullptr;
	QList<SPTR<vtkActor2D>> actor_pos_list_;
private:
	ViewType view_type_;
	bool is_show_info_ = false;
	QWidget* view_container_;
	BasicImageStyle* basic_style_ = nullptr;


	int img_size_[3];
	double img_spacing_[3];
	double img_origin_[3];

};

