#pragma once  
#include "BasicVtkDefine.h"
#include <QColor>
class vtkDistanceWidget;
class vtkAngleWidget;

class SceneMeasureTools
{
public:
	enum ToolType {
		kDistance, kAngle };
	static PTR<SceneMeasureTools> New(ToolType tool = kDistance);
	~SceneMeasureTools();
	void showMeasure(SPTR<vtkRenderWindowInteractor> interactor = nullptr);
private:
	SceneMeasureTools(ToolType tool);
protected:
	SetGetMacro(QColor, color)

	SPTR<vtkDistanceWidget> distanceWidget_ = nullptr;
	SPTR<vtkAngleWidget> angleWidget_ = nullptr;

	ToolType type_ = kDistance;
};

