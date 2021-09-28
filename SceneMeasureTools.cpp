/*
 * @Author: your name
 * @Date: 2021-09-28 16:02:39
 * @LastEditTime: 2021-09-28 16:08:30
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\vtkImageTool\code\SceneMeasureTools.cpp
 */
#include "SceneMeasureTools.h" 
#include "vtkDistanceRepresentation2D.h"
#include "vtkDistanceWidget.h"
#include "vtkAngleRepresentation2D.h"
#include "vtkAngleWidget.h"
#include "vtkLeaderActor2D.h"
#include "vtkProperty2D.h""
PTR<SceneMeasureTools> SceneMeasureTools::New(ToolType tool /*= kDistance*/)
{
	return PTR<SceneMeasureTools>(new SceneMeasureTools(tool));
}

SceneMeasureTools::SceneMeasureTools(ToolType tool):type_(tool)
{
	if (tool == kDistance)
	{
		distanceWidget_ = SPTR<vtkDistanceWidget>::New();

		vtkNew<vtkDistanceRepresentation2D> representation;
		representation->SetLabelFormat("%-#6.2f mm");
		distanceWidget_->SetRepresentation(representation);
		distanceWidget_->SetPriority(0.9);
	}
	else
	{
		angleWidget_ = SPTR<vtkAngleWidget>::New();
		vtkNew<vtkAngleRepresentation2D> representation;
		representation->GetRay1()->GetProperty()->SetColor(0, 1, 0);
		representation->GetRay1()->GetProperty()->SetLineWidth(1);
		representation->GetRay2()->GetProperty()->SetColor(0, 1, 0);
		representation->GetRay1()->GetProperty()->SetLineWidth(1);
		representation->GetArc()->GetProperty()->SetColor(0, 1, 0);
		representation->GetArc()->GetProperty()->SetLineWidth(1);
		angleWidget_->SetRepresentation(representation);
		angleWidget_->SetPriority(0.9);
	}
}
SceneMeasureTools::~SceneMeasureTools()
{
}

void SceneMeasureTools::showMeasure(SPTR<vtkRenderWindowInteractor> interactor /*= nullptr*/)
{
	if (type_ == kDistance)
	{
		distanceWidget_->SetInteractor(interactor);
		distanceWidget_->ManagesCursorOn();
		distanceWidget_->On();
	}
	else 
	{
		angleWidget_->SetInteractor(interactor);
		angleWidget_->ManagesCursorOn();
		angleWidget_->On();
	}

}


