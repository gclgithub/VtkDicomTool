#pragma once
#include "BasicVtkDefine.h"
#include "ViewType.h"

class vtkLineSource;

class SceneCrossLine
{
public:
	static PTR<SceneCrossLine> New();
	~SceneCrossLine();
	void resetCrossLine(double* pos);
	void UpdateCrossLine(double* pos, const ViewType& type);
	ViewType check_cross(vtkActor* actor, const ViewType& type);
private:
	SceneCrossLine();
	int index = 1;
private:
	GetMacro(std::vector<SPTR<vtkActor>>, line_actor_list)
	SetMacro(std::vector<double>,bounds)
};

