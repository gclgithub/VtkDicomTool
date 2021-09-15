#pragma once 
#include <QObject>
#include <QtWidgets>
#include "BasicVtkDefine.h"
#include "ViewType.h"
class MultiImageView :
	public QObject
{	//注意：必须将 QObject 放在继承基类的第一位，否则Q_OBJECT会导致错误
	Q_OBJECT
public:
	MultiImageView(QWidget* main_widget);
	virtual ~MultiImageView();
	void initiate(SPTR<vtkImageData> img_data);

	//显示某个视口，单个独占，撑满整个窗口空间
	void showSingleView(const ViewType& type);
	//显示交叉四视口模式
	void showCrossViews();

	QWidget* create_view_container(QWidget* view);
private:
	QWidget* main_widget_;

	//QBoxLayout* multi_widget_layout_;
	QWidget* view_container_t_;
	QWidget* view_container_c_;
	QWidget* view_container_s_;
	QWidget* view_container_v_;

	QWidget* view_container_any_;
	QWidget* view_container_;



	SPTR<vtkImageData> img_data_ = nullptr;
};

