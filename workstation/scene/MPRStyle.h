#pragma once 
#include "vtkInteractorStyleImage.h" 
#include "BasicVtkDefine.h"
class ViewRenderScene;
class MousePicker;
class SceneMeasureTools;
class MPRStyle : public vtkInteractorStyleImage
{
public:
	static MPRStyle* New();
	vtkTypeMacro(MPRStyle, vtkInteractorStyleImage);
	void initiate();
	enum MOUSEFUNC
	{
		POINTER,
		DOLLY,
		PAN,
		GRAYLEVEL,
		CrossLine,
		ROTATE,
		SPIN,
		DISTANCE,
		ANGLE,
		Needle
	}MouseFunction;
protected:
	virtual void OnKeyDown()override;
	virtual void OnMouseWheelForward() override;
	virtual void OnMouseWheelBackward() override;
	virtual void OnMiddleButtonDown() override;
	virtual void OnMiddleButtonUp() override;

	virtual void OnMouseMove() override;
	virtual void OnLeftButtonDown() override;
	virtual void OnLeftButtonUp() override;
	virtual void OnRightButtonDown() override;
	virtual void OnRightButtonUp() override;

	virtual void OnLeave() override;
	//virtual void OnEnter() override;

private:
	PTR<ViewRenderScene> GetCurRenderScene();
	void moveReslicer(PTR<ViewRenderScene> ren_scene,int slicer);
	void moveCrossLine(double* pos);
private:
	SetGetMacro(std::vector<PTR<ViewRenderScene>>, scenes);
	VarMacro(PTR<ViewRenderScene>, last_scene);
	SetGetMacro(SPTR<MousePicker>, picker)

	vtkActor* m_cur_actor = nullptr;

	SetMacro(PTR<SceneMeasureTools>, tool);
};
