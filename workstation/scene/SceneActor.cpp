#include "SceneActor.h"
#include "vtkObjectFactory.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkTexture.h"
vtkStandardNewMacro(SceneActor);

vtkCxxSetObjectMacro(SceneActor, Camera, vtkCamera);

SceneActor::~SceneActor()
{
	if (this->Camera)
	{
		this->Camera->UnRegister(this);
	}
}


int SceneActor::RenderOpaqueGeometry(vtkViewport* vp)
{
	if (!this->Mapper)
	{
		return 0;
	}

	if (!this->Property)
	{
		// force creation of a property
		this->GetProperty();
	}

	if (this->GetIsOpaque())
	{
		vtkRenderer* ren = static_cast<vtkRenderer*>(vp);
		this->Render(ren);
		return 1;
	}
	return 0;
}

int SceneActor::RenderTranslucentPolygonalGeometry(vtkViewport* vp)
{
	if (!this->Mapper)
	{
		return 0;
	}

	if (!this->Property)
	{
		// force creation of a property
		this->GetProperty();
	}

	if (!this->GetIsOpaque())
	{
		vtkRenderer* ren = static_cast<vtkRenderer*>(vp);
		this->Render(ren);
		return 1;
	}
	return 0;
}

SceneActor::SceneActor()
{
	this->Camera = nullptr;
}

void SceneActor::Render(vtkRenderer *ren)
{
	this->Property->Render(this, ren);
	if (this->BackfaceProperty)
	{
		this->BackfaceProperty->BackfaceRender(this, ren);
	}

	/* render the texture */
	if (this->Texture)
	{
		this->Texture->Render(ren);
	}
}

vtkTypeBool SceneActor::HasTranslucentPolygonalGeometry()
{
	if (!this->Mapper)
	{
		return 0;
	}
	// make sure we have a property
	if (!this->Property)
	{
		// force creation of a property
		this->GetProperty();
	}

	// is this actor opaque ?
	return !this->GetIsOpaque();
}

void SceneActor::ReleaseGraphicsResources(vtkWindow* w)
{
	return this->Superclass::ReleaseGraphicsResources(w);
}

void SceneActor::ShallowCopy(vtkProp* prop)
{
	SceneActor* f = SceneActor::SafeDownCast(prop);
	if (f != nullptr)
	{
		this->SetCamera(f->GetCamera());
	}
	this->vtkActor::ShallowCopy(prop);
}
