
#include "MPRStyle.h" 
#include "vtkRenderWindowInteractor.h";
//#include "vtkPropPicker.h";
//#include "vtkCellPicker.h";
//#include "vtkActor.h";
//#include "vtkCoordinate.h";
//#include "vtkCamera.h";
#include  <QCursor>
#include <vtkRenderer.h>
#include "ViewRenderScene.h"
#include "vtkOpenGLRenderer.h"
#include "vtkCallbackCommand.h"
vtkStandardNewMacro(MPRStyle);

void MPRStyle::OnKeyDown()
{
	std::string key = this->GetInteractor()->GetKeySym();
	if (key.compare("Up") == 0)
	{
		//cout << "Up arrow key was pressed." << endl;
	}
	else if (key.compare("Down") == 0)
	{
		//cout << "Down arrow key was pressed." << endl;
	}
	// forward event
	vtkInteractorStyleImage::OnKeyDown();
}

void MPRStyle::OnMouseMove()
{
	std::cout << "OnMouseMove" << std::endl;
	if (auto render_scene = GetCurRenderScene())
	{
		if (m_last_scene != render_scene)
		{
			m_last_scene = render_scene;
			MouseFunction = POINTER;
			this->State = VTKIS_NONE;
			this->OnMiddleButtonUp();
			return;
		}
		if (render_scene->get_viewType() == VOL_M) 
		{
			vtkInteractorStyleTrackballCamera::OnMouseMove();
			return;
		}

		switch (MouseFunction)
		{
		case POINTER:
			//此时没有功能
			break;
		case DOLLY:
			this->Dolly();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		case PAN:
			this->Pan();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		case GRAYLEVEL:
			//灰阶,原左键功能
			break;
		case ROTATE:
			this->Rotate();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		case SPIN:
			this->Spin();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		default:
			break;
		}

	}
}

void MPRStyle::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->FindPokedRenderer(x, y);

}

void MPRStyle::OnLeftButtonUp()
{
	for (auto sc : m_scenes)
	{
		sc->resetCamera();
	}
	Interactor->Render();
}

void MPRStyle::OnMiddleButtonDown()
{
	//std::cout << "OnMiddleButtonDown" << std::endl;
	MouseFunction = MOUSEFUNC::PAN;
	this->GrabFocus(this->EventCallbackCommand);
	this->StartPan();
}

void MPRStyle::OnMiddleButtonUp()
{
	if (MouseFunction == MOUSEFUNC::PAN) 
	{
		this->EndPan();
		if (this->Interactor)
		{
			this->ReleaseFocus();
		}
	}
	MouseFunction = POINTER;
}

void MPRStyle::OnRightButtonDown()
{
	//std::cout << "OnRightButtonDown" << std::endl;
	MouseFunction = MOUSEFUNC::DOLLY;
	this->GrabFocus(this->EventCallbackCommand);
	this->StartDolly();
}

void MPRStyle::OnRightButtonUp()
{
	if (MouseFunction == MOUSEFUNC::DOLLY)
	{
		this->EndDolly();
		if (this->Interactor)
		{
			this->ReleaseFocus();
		}
	}
	MouseFunction = POINTER;
}

void MPRStyle::OnMouseWheelForward()
{
	if (auto render_scene = GetCurRenderScene())
	{
		render_scene->get_reslice()->moveReslice(1);
		Interactor->Render();
	}
}

void MPRStyle::OnMouseWheelBackward()
{
	if (auto render_scene = GetCurRenderScene())
	{
		render_scene->get_reslice()->moveReslice(-1);
		Interactor->Render();
	}
}
void MPRStyle::OnLeave()
{
	CurrentRenderer = nullptr;
	MouseFunction = POINTER;
	if (this->Interactor)
	{
		this->ReleaseFocus();
	}
}
//
//void MPRStyle::OnEnter()
//{
//	MouseFunction = GRAYLEVEL;
//	vtkInteractorStyleImage::OnEnter();
//}

PTR<ViewRenderScene> MPRStyle::GetCurRenderScene()
{
	if (m_scenes.size() == 0) return nullptr;
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->FindPokedRenderer(x, y);
	auto render = CurrentRenderer;
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [render](PTR<ViewRenderScene> sc) {
		return sc->get_renderer().Get() == render; });
	if (it == m_scenes.end())return nullptr;
	return *it;
}
