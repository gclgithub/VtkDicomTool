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

	//֧�ֱ�׼����Ⱦ����
	virtual int RenderOpaqueGeometry(vtkViewport* viewport);//��͸����; 
	virtual int RenderTranslucentPolygonalGeometry(vtkViewport* viewport);//͸���ļ�����;
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