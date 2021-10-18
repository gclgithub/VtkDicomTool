#pragma once
#include "BasicVtkDefine.h"
#include "vtkActor.h"
class SceneActor :public vtkActor
{
public:
	vtkTypeMacro(SceneActor, vtkActor);
	static SceneActor* New();
	virtual void SetCamera(vtkCamera*);
	vtkGetObjectMacro(Camera, vtkCamera);
protected:
	SceneActor();
	~SceneActor() override;

	//支持标准的渲染方法
	virtual int RenderOpaqueGeometry(vtkViewport* viewport);//不透明的; 
	virtual int RenderTranslucentPolygonalGeometry(vtkViewport* viewport);//透明的几何体;
	virtual void Render(vtkRenderer*);
	vtkTypeBool HasTranslucentPolygonalGeometry() override;
	void ReleaseGraphicsResources(vtkWindow*);
	void ShallowCopy(vtkProp* prop) override;
private:
	vtkCamera* Camera;

	SceneActor(const SceneActor&) = delete;
	void operator=(const SceneActor&) = delete;
	void Render(vtkRenderer*, vtkMapper*) override {}

	//0->X,1->Y,2->Z
	SetGetMacro(int,direction)
};