
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


void MPRStyle::initiate()
{

}

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
	//std::cout << "OnMouseMove" << std::endl;
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
		int* pos = this->GetInteractor()->GetEventPosition();

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
			//灰阶,原左键功能
		case GRAYLEVEL:
		{
			double ww, wl,dww,dwl;
			render_scene->get_reslice()->get_window_level(ww, wl, dwl, dww);

			int* last_pos = this->GetInteractor()->GetLastEventPosition();
			QPoint delta = QPoint(pos[0], pos[1]) - QPoint(last_pos[0], last_pos[1]);
			wl = -delta.y() * 0.1 + wl;
			ww = delta.x() * 0.1 + ww;
			wl = min(255.0, max(wl, 0.0));
			ww = min(255.0, max(ww, 0.0));
			for (auto sc : m_scenes)
			{
				if (sc->get_viewType() != VOL_M) 
					sc->updataInfoWL(wl, ww);
			}
			break;
		}
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
	Interactor->Render();
}

void MPRStyle::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->FindPokedRenderer(x, y);

	if (MouseFunction == POINTER) 
	{
		MouseFunction = GRAYLEVEL;
	}
}

void MPRStyle::OnLeftButtonUp()
{
	MouseFunction = POINTER;
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


	//check button_widget
	for (auto sc : m_scenes) 
	{
		if (auto btn_widget = sc->get_button_widget()) 
		{
			sc->get_button_widget()->ProcessEventsOff();
			btn_widget->Off();
		}
	}
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [render](PTR<ViewRenderScene> sc) {
		return sc->get_renderer().Get() == render; });
	if (it == m_scenes.end())return nullptr;
	if (auto btn_widget = (*it)->get_button_widget())
	{
		btn_widget->ProcessEventsOn();
		btn_widget->On();
	}
	return *it;
}
