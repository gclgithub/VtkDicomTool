#pragma once 
#include "vtkInteractorStyleImage.h" 
#include "BasicVtkDefine.h"
class ViewRenderScene;
class MPRStyle : public vtkInteractorStyleImage
{
public:
	static MPRStyle* New();
	vtkTypeMacro(MPRStyle, vtkInteractorStyleImage);
	enum MOUSEFUNC
	{
		POINTER,
		DOLLY,
		PAN,
		GRAYLEVEL,
		ROTATE,
		SPIN
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
	SetGetMacro(std::vector<PTR<ViewRenderScene>>, scenes);
	PTR<ViewRenderScene> GetCurRenderScene();

private:
	VarMacro(PTR<ViewRenderScene>, last_scene);
};
