
#include "MPRStyle.h" 
#include "vtkRenderWindowInteractor.h";
#include "MousePicker.h" 
#include  <QCursor>
#include <vtkRenderer.h>
#include "ViewRenderScene.h"
#include "vtkOpenGLRenderer.h"
#include "vtkCallbackCommand.h"
#include "vtkPropPicker.h"
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
		double world_pos[3] = { 0,0,0 };
		auto res = render_scene->get_reslice();
		m_picker->getWorldPositionFromDisplay(pos[0], pos[1], world_pos, res, render_scene->get_renderer());
		render_scene->updataInfoPosition(world_pos[0], world_pos[1], world_pos[2]);
		//std::cout << "pos:" << pos[0] << "," << pos[1] << ",world_pos:" << world_pos[0] << "," << world_pos[1] << "," << world_pos[2] << std::endl;

		switch (MouseFunction)
		{
		case POINTER:
		{
			//no function
			break;
		}
		case DOLLY:
		{
			this->Dolly();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		}
		case PAN:
		{
			this->Pan();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		}
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
		case CrossLine:
		{
			moveCrossLine(world_pos);
			break;
		}
		case ROTATE:
		{
			this->Rotate();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		}
		case SPIN:
		{
			this->Spin();
			this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
			break;
		}
		case DISTANCE:
		{
			break;
		}		
		case ANGLE:
		{
			break;
		}
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
	m_picker->Pick(x, y, 0, CurrentRenderer);
	if (m_cur_actor = m_picker->GetActor())
	{
		MouseFunction = CrossLine;
		std::cout << "MouseFunction:" << MouseFunction << std::endl;
	}
	if (MouseFunction == POINTER) 
	{
		MouseFunction = GRAYLEVEL;
	}
}

void MPRStyle::OnLeftButtonUp()
{
	MouseFunction = POINTER;
	//for (auto sc : m_scenes)
	//{
	//	sc->resetScene();
	//}
	m_cur_actor = nullptr;
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
		if (render_scene->get_viewType() == VOL_M)
		{
			vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
			return;
		}
		moveReslicer(render_scene, 1);

	}
}

void MPRStyle::OnMouseWheelBackward()
{
	if (auto render_scene = GetCurRenderScene())
	{
		if (render_scene->get_viewType() == VOL_M) 
		{
			vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
			return;
		}
		moveReslicer(render_scene, -1);
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

void MPRStyle::moveReslicer(PTR<ViewRenderScene> ren_scene, int slicer)
{
	auto res = ren_scene->get_reslice();
	res->moveReslice(slicer);

	int* pos = this->GetInteractor()->GetEventPosition();
	double world_pos[3] = { 0,0,0 };
	m_picker->getWorldPositionFromDisplay(pos[0], pos[1], world_pos, res, ren_scene->get_renderer());
	ren_scene->updataInfoPosition(world_pos[0], world_pos[1], world_pos[2]);

	for (auto sc : m_scenes)
	{
		sc->updataInfoSlicer();
	}
	Interactor->Render();
}

void MPRStyle::moveCrossLine(double* pos)
{
	if (m_cur_actor)
	{
		auto render_scene = GetCurRenderScene();
		auto res = render_scene->get_reslice();
		auto spacing = res->get_spacing();
		//std::cout << "spacing:" << spacing[0] << "," << spacing[1] << "," << spacing[2] << std::endl;

		int* last_pos = this->GetInteractor()->GetLastEventPosition();
		double last_world_pos[3] = { 0,0,0 };
		m_picker->getWorldPositionFromDisplay(last_pos[0], last_pos[1], last_world_pos, res, render_scene->get_renderer());
		
		double delta[3] = { 0,0,0 };
		vtkMath::Subtract(pos, last_world_pos, delta);
		//std::cout << "delta:" << delta[0] << "," << delta[1] << "," << delta[2] << std::endl;

		auto tmp = render_scene->get_cross_line()->check_cross(m_cur_actor, render_scene->get_viewType());
		int slicer_delta = 0;
		if (tmp == TRA_M)
		{
			slicer_delta = int(delta[2] / spacing[2] + 0.5);
		}
		else if (tmp == COR_M)
		{
			slicer_delta = -int(delta[1] / spacing[1] + 0.5);
		}
		else
		{
			slicer_delta = int(delta[0] / spacing[0] + 0.5);
		}

		auto it = std::find_if(m_scenes.begin(), m_scenes.end(), [tmp](PTR<ViewRenderScene> sc) {
			return sc->get_viewType() == tmp; });
		if (it != m_scenes.end())
		{
			moveReslicer(*it, slicer_delta);
		}
	}
}
