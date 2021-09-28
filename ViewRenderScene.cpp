
#include "ViewRenderScene.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkTexturedButtonRepresentation2D.h"
#include "vtkPNGReader.h"
#include "vtkCommand.h"
#include "vtkImageProperty.h"
#include "SceneCrossLine.h"
class StatusMessage
{
public:
	static std::string FormatWl(int ww, int wl)
	{
		std::stringstream tmp;
		tmp << "ww:" << ww << "/wl:" << wl;
		return tmp.str();
	}

	static std::string FormatPosition(double x, double y, double z)
	{
		std::stringstream tmp;
		tmp.precision(2);
		tmp.setf(std::ios::fixed);
		tmp << "Position:" << x << "," << y << "," << z;
		return tmp.str();
	}

	static std::string FormatSlicer(int index, int count)
	{
		std::stringstream tmp;
		tmp << "Slice:" << index << "/" << count;
		return tmp.str();
	}
};

ViewRenderScene::ViewRenderScene(SPTR<vtkRenderWindow> rewin, const ViewType& viewType): m_viewType(viewType)
{
	m_renderer = SPTR<vtkOpenGLRenderer>::New();
	switch (viewType)
	{
	case ViewType::COR_M:
		m_view_type_msg = "Coronal";
		m_direction = { "R","H","L","F" };
		m_renderer->SetBackground(0, 0.2, 0);
		break;
	case ViewType::SAG_M:
		m_view_type_msg = "Sagittal";
		m_direction = { "A","H","P","F" };
		m_renderer->SetBackground(0, 0, 0.2);
		break;
	case ViewType::TRA_M:
		m_view_type_msg = "Transverse";
		m_direction = { "R","A","L","P" };
		m_renderer->SetBackground(0.2, 0, 0);
		break;
	default:
		m_renderer->SetBackground(0.2, 0, 0.2);
		break;
	}
	rewin->AddRenderer(m_renderer);
	m_interactor = rewin->GetInteractor();
}

ViewRenderScene::~ViewRenderScene()
{
	if (m_camParam != nullptr) delete m_camParam;
}

PTR<ViewRenderScene> ViewRenderScene::New(SPTR<vtkRenderWindow> rewin, const ViewType& viewType)
{
	return PTR<ViewRenderScene>(new ViewRenderScene(rewin,viewType));
}

void ViewRenderScene::setViewport(const double& leftX, const double& topY, const double& rightX, const double& bottomY)
{
	m_renderer->SetViewport(leftX, topY, rightX, bottomY);
}

void  ViewRenderScene::_initCamrea()
{
	auto cam = getCamera();
	cam->SetClippingRange(0.001, 10000);
	//记录各个camera初始参数
	m_camParam = new Navi3DCameraParams();
	m_camParam->record(cam);
}

void ViewRenderScene::initiate()
{
	if (m_full_screen)initSceneButton();
}

void ViewRenderScene::initiate(const bool& show_left_top, const bool& show_left_bot, bool show_direction)
{
	//init actor
	if (show_direction)initSceneDirection();
	if (show_left_top)initSceneLeftTopInfo();
	if (show_left_bot)initSceneLeftBotInfo();
	if (m_full_screen)initSceneButton();

	//init data
	double ww, wl, dww, dwl;
	m_reslice->get_window_level(ww, wl, dwl, dww);
	updataInfoWL(wl, ww);
	updataInfoPosition(0, 0, 0);

	updataInfoSlicer();
}

void ViewRenderScene::set_callback(vtkCommand* callback)
{
	if (m_full_screen) 
	{
		m_button_widget->AddObserver(vtkCommand::StateChangedEvent, callback);
	}
}

void ViewRenderScene::set_button_widget_state()
{
	//Switch layout change button state
	if (m_full_screen && m_button_widget) 
	{
		auto* rep = reinterpret_cast<vtkTexturedButtonRepresentation2D*> (m_button_widget->GetRepresentation());
		rep->SetState(false);
	}
}

void ViewRenderScene::updataInfoWL(const double& wl, const double& ww)
{
	m_reslice->get_imgActor()->GetProperty()->SetColorLevel(wl);
	m_reslice->get_imgActor()->GetProperty()->SetColorWindow(ww);

	if (m_wlww_actor) 
	{
		std::string msg = StatusMessage::FormatWl(wl, ww);
		auto* mapper = reinterpret_cast<vtkTextMapper*> (m_wlww_actor->GetMapper());
		mapper->SetInput(msg.c_str());
	}
}

void ViewRenderScene::updataInfoSlicer()
{
	if (m_slicer_actor && m_reslice)
	{
		// change info
		int cur_index, slice_count;
		m_reslice->get_slicer_num(slice_count, cur_index);
		std::string msg = StatusMessage::FormatSlicer(cur_index, slice_count);
		auto* mapper = reinterpret_cast<vtkTextMapper*> (m_slicer_actor->GetMapper());
		mapper->SetInput(msg.c_str());

		//change crossline position
		if (m_cross_line->get_line_actor_list().size() != 3)return;
		auto* res_pos = m_reslice->get_imgActor()->GetPosition();
		m_cross_line->UpdateCrossLine(res_pos, m_viewType);
	}
}

void ViewRenderScene::updataInfoPosition(const double& x, const double& y, const double& z)
{
	if (m_position_actor) 
	{
		std::string msg = StatusMessage::FormatPosition(x, y,z);
		auto* mapper = reinterpret_cast<vtkTextMapper*> (m_position_actor->GetMapper());
		mapper->SetInput(msg.c_str());
	}
}

void ViewRenderScene::add_crossline(PTR<SceneCrossLine> cross_line)
{
	m_cross_line = cross_line;
	//show crossline
	switch (m_viewType)
	{
	case TRA_M:
	{
		m_renderer->AddActor(cross_line->get_line_actor_list()[0]);
		m_renderer->AddActor(cross_line->get_line_actor_list()[1]);
		break;
	}
	case COR_M:
	{
		m_renderer->AddActor(cross_line->get_line_actor_list()[0]);
		m_renderer->AddActor(cross_line->get_line_actor_list()[2]);
		break;
	}
	case SAG_M:
	{
		m_renderer->AddActor(cross_line->get_line_actor_list()[1]);
		m_renderer->AddActor(cross_line->get_line_actor_list()[2]);
		break;
	}
	default:
		break;
	}

	if (m_viewType == TRA_M) 
	{
		//set position
		auto imgData = m_reslice->get_imgActor()->GetInput();
		auto size = imgData->GetDimensions();
		auto spacing = imgData->GetSpacing();
		auto origin = imgData->GetOrigin();

		std::vector<double> bounds = {
		origin[0] - 30,origin[0] + size[0] * spacing[0] + 30,
		origin[1] - 30,origin[1] + size[1] * spacing[1] + 30,
		origin[2] - 30,origin[2] + size[2] * spacing[2] + 30, };

		cross_line->set_bounds(bounds);
		cross_line->resetCrossLine(m_reslice->get_center());
	}
}

void ViewRenderScene::initSceneLeftTopInfo()
{
	int font_text = 16;
	{
		vtkNew<vtkTextProperty> textProp;
		textProp->SetFontFamilyToCourier();
		textProp->SetFontSize(font_text);
		textProp->SetVerticalJustificationToTop();
		textProp->SetJustificationToLeft();
		vtkNew<vtkTextMapper> mapper;
		mapper->SetTextProperty(textProp);
		mapper->SetInput(m_view_type_msg.c_str());

		m_view_type_actor = SPTR<vtkActor2D>::New();
		m_view_type_actor->SetMapper(mapper);
		m_renderer->AddActor2D(m_view_type_actor);
	}

	{
		vtkNew<vtkTextProperty> textProp;
		textProp->SetFontFamilyToCourier();
		textProp->SetFontSize(font_text);
		textProp->SetVerticalJustificationToTop();
		textProp->SetJustificationToLeft();

		vtkNew<vtkTextMapper>sliceTextMapper;
		sliceTextMapper->SetInput("");
		sliceTextMapper->SetTextProperty(textProp);
		m_slicer_actor = SPTR<vtkActor2D>::New();
		m_slicer_actor->SetMapper(sliceTextMapper);
		m_renderer->AddActor2D(m_slicer_actor);
	}
}

void ViewRenderScene::initSceneLeftBotInfo()
{
	//position
	int font_text = 16;
	{
		vtkNew<vtkTextProperty> PositionTextProp;
		PositionTextProp->SetFontFamilyToCourier();
		PositionTextProp->SetFontSize(font_text);
		PositionTextProp->SetVerticalJustificationToBottom();
		PositionTextProp->SetJustificationToLeft();

		vtkNew<vtkTextMapper>PositionTextMapper;
		PositionTextMapper->SetInput("");
		PositionTextMapper->SetTextProperty(PositionTextProp);
		m_position_actor = SPTR<vtkActor2D>::New();
		m_position_actor->SetMapper(PositionTextMapper);
		m_renderer->AddActor2D(m_position_actor);

	}
	//wl/ww
	{
		vtkNew<vtkTextProperty> textProp;
		textProp->SetFontFamilyToCourier();
		textProp->SetFontSize(font_text);
		textProp->SetVerticalJustificationToBottom();
		textProp->SetJustificationToLeft();
		vtkNew<vtkTextMapper>mapper;
		mapper->SetInput("");
		mapper->SetTextProperty(textProp);
		m_wlww_actor = SPTR<vtkActor2D>::New();
		m_wlww_actor->SetMapper(mapper);
		m_renderer->AddActor2D(m_wlww_actor);
	}
}

void ViewRenderScene::initSceneButton()
{
	SPTR<vtkImageData> image1;
	SPTR<vtkImageData> image2;
	vtkNew<vtkPNGReader> pngReader1;
	pngReader1->SetFileName("./images/bigger.png");
	pngReader1->Update();
	vtkNew<vtkPNGReader> pngReader2;
	pngReader2->SetFileName("./images/add_pressed.png");
	pngReader2->Update();
	image1 = pngReader1->GetOutput();
	image2 = pngReader2->GetOutput();

	// Create the widget and its representation
	vtkNew<vtkTexturedButtonRepresentation2D> buttonRepresentation;
	buttonRepresentation->SetNumberOfStates(2);
	buttonRepresentation->SetButtonTexture(0, image1);
	buttonRepresentation->SetButtonTexture(1, image2);
	buttonRepresentation->SetPlaceFactor(1);

	m_button_widget = SPTR<vtkButtonWidget>::New();
	m_button_widget->SetInteractor(m_interactor);
	m_button_widget->SetCurrentRenderer(m_renderer);
	m_button_widget->SetRepresentation(buttonRepresentation);
	m_button_widget->ProcessEventsOn();
	m_button_widget->On();
}

void ViewRenderScene::initSceneDirection()
{
	m_direction_actor_list.clear();
	int font_text = 16;
	for (int i = 0; i < 4; ++i)
	{
		vtkNew<vtkTextProperty> textProp;
		textProp->SetFontFamilyToCourier();
		textProp->SetFontSize(font_text);
		textProp->SetVerticalJustificationToCentered();
		textProp->SetJustificationToCentered();
		vtkNew<vtkTextMapper> mapper;
		mapper->SetInput(m_direction[i].c_str());
		mapper->SetTextProperty(textProp);
		auto actor2d = SPTR<vtkActor2D>::New();
		actor2d->SetMapper(mapper);
		m_renderer->AddActor2D(actor2d);
		m_direction_actor_list.push_back(actor2d);
	}
}

SPTR<vtkCamera> ViewRenderScene::getCamera()
{
	return m_renderer->GetActiveCamera();
}

void ViewRenderScene::resetCamera()
{
	auto ps = getCamera()->GetFocalPoint();
	//std::cout << "GetFocalPoint:" << ps[0] << "," << ps[1] << "," << ps[2] << std::endl;
	if (m_camParam != nullptr && m_camParam->hasRecorded())
	{
		m_camParam->apply(getCamera());
		return;
	}
	else
	{
		auto* center = m_reslice->get_center();
		auto imgData = m_reslice->get_imgActor()->GetInput();
		auto size = imgData->GetDimensions();
		auto spacing = imgData->GetSpacing();
		double rx, ry;
		rx = size[0] * spacing[0];
		ry = size[1] * spacing[1];
		auto zoom_factor = rx > ry ? rx : ry;
		getCamera()->SetParallelScale(zoom_factor*0.5);
		getCamera()->ParallelProjectionOn();
		getCamera()->SetFocalPoint(center);
		getCamera()->SetPosition(m_cam_pos);
	}
}

void ViewRenderScene::addActor(SPTR<vtkProp> prop)
{
	if (!m_renderer->HasViewProp(prop))
	{
		m_renderer->AddActor(prop);
	}
}

void ViewRenderScene::removeActor(SPTR<vtkProp> prop)
{
	if (m_renderer->HasViewProp(prop))
	{
		m_renderer->RemoveActor(prop);
	}
}

void ViewRenderScene::setCrossTargets(ViewRenderScene* view1, ViewRenderScene* view2)
{
	//if (m_viewType == VOL_M)
	//{
	//	//三维视口无此功能
	//	return;
	//}
	//m_crossLine->setCross(view1->getCrossLine(), view2->getCrossLine());
}

void ViewRenderScene::initReslicer(SPTR<vtkImageData> imgData, SPTR<vtkMatrix4x4> vtkmat, double* pos)
{
	m_reslice = SceneReslice::New(imgData, vtkmat);
	if (true) 
	{
		resetCamera();
		auto cam = getCamera();
		auto* center = m_reslice->get_center();
		switch (m_viewType)
		{
		case ViewType::COR_M:
			m_reslice->get_imgActor()->RotateX(90);
			m_cam_pos[0] = center[0];
			m_cam_pos[1] = center[1] + 3000;
			m_cam_pos[2] = center[2];
			cam->SetPosition(m_cam_pos[0], m_cam_pos[1], m_cam_pos[2]);
			cam->SetFocalPoint(center);
			cam->ParallelProjectionOn();
			cam->SetViewUp(0, 0, -1);
			break;
		case ViewType::SAG_M:
			m_reslice->get_imgActor()->RotateY(90);
			m_cam_pos[0] = center[0] - 3000;
			m_cam_pos[1] = center[1];
			m_cam_pos[2] = center[2];
			cam->SetPosition(m_cam_pos[0], m_cam_pos[1], m_cam_pos[2]);
			cam->SetFocalPoint(center);
			cam->ParallelProjectionOn();
			cam->SetViewUp(0, 0, -1);
			break;
		case ViewType::TRA_M:
			m_cam_pos[0] = center[0];
			m_cam_pos[1] = center[1];
			m_cam_pos[2] = center[2] + 3000;
			cam->SetPosition(m_cam_pos[0], m_cam_pos[1], m_cam_pos[2]);
			cam->SetFocalPoint(center);
			cam->ParallelProjectionOn();
			//cam->SetViewUp(0, -1, 0);//结石位显示
			cam->SetViewUp(0, 1, 0);
			break;
		default:
			break;
		}
		m_renderer->AddActor(m_reslice->get_imgActor());
		m_reslice->get_imgActor()->PickableOff();
		m_reslice->doReslice();
		_initCamrea();
	}
	else 
	{
		//NEW
		m_renderer->AddActor(m_reslice->get_imgActor());
		m_reslice->get_imgActor()->PickableOff();

		auto cam = getCamera();
		resetCamera();
	}
}

void ViewRenderScene::resetScene()
{
	//reset slicer
	if (m_viewType != VOL_M) 
	{
		m_reslice->resetReslicer();
		updataInfoSlicer();

		//reset wl/ww
		double ww, wl, dww, dwl;
		m_reslice->get_window_level(ww, wl, dww, dwl);
		updataInfoWL(dwl, dww);

		//reset camera
		resetCamera();
	}
}

void ViewRenderScene::resize(const int& width, const int& height)
{
	m_width = width; m_height = height;
	//left top
	int top_height = height - 10;
	if (m_view_type_actor)
	{
		m_view_type_actor->SetPosition(10, top_height);
		top_height -= 20;
	}
	if (m_slicer_actor)
	{
		m_slicer_actor->SetPosition(10, top_height);
		top_height -= 20;
	}

	//left bot
	int low_height = 10;
	if (m_position_actor)
	{
		m_position_actor->SetPosition(10, low_height);
		low_height += 20;
	}
	if (m_wlww_actor)
	{
		m_wlww_actor->SetPosition(10, low_height);
		low_height += 20;

	}

	//direction
	for (int i = 0; i < m_direction_actor_list.size(); ++i)
	{
		auto actor = m_direction_actor_list[i];
		if (i == 0)
			actor->SetPosition(30, height * 0.5);
		else if (i == 1)
			actor->SetPosition(width * 0.5, height - 30);
		else if (i == 2)
			actor->SetPosition(width - 30, height * 0.5);
		else
			actor->SetPosition(width * 0.5, 30);
	}

	if (m_full_screen) 
	{
		auto* rep = reinterpret_cast<vtkTexturedButtonRepresentation2D*> (m_button_widget->GetRepresentation());
		double bds[6];
		bds[0] = m_width - 30;
		bds[1] = m_width - 10;
		bds[2] = 10;
		bds[3] = 30;
		bds[4] = bds[5] = 0.0;
		rep->PlaceWidget(bds);
		rep->BuildRepresentation();
		m_button_widget->ProcessEventsOff();
		m_button_widget->Off();

		m_button_widget->Render();
	}
	//std::cout << "width:" << m_width << ",height:" << m_height << std::endl;
}

////////////////摄像机参数记录/////////////
void Navi3DCameraParams::record(vtkCamera* cam)
{
	double* pos = cam->GetPosition();
	px = pos[0];
	py = pos[1];
	pz = pos[2];

	double* foc = cam->GetFocalPoint();
	fx = foc[0];
	fy = foc[1];
	fz = foc[2];

	double* up = cam->GetViewUp();
	upx = up[0];
	upy = up[1];
	upz = up[2];


	clippingNear = cam->GetClippingRange()[0];
	clippingFar = cam->GetClippingRange()[1];

	isParallel = cam->GetParallelProjection();
	if (isParallel)
	{
		parallelScale = cam->GetParallelScale();
	}
	else
	{
		viewAngle = cam->GetViewAngle();
	}
	m_bRecorded = true;
}

void Navi3DCameraParams::apply(vtkCamera* cam)
{
	cam->SetParallelProjection(isParallel);
	cam->SetPosition(px, py, pz);
	cam->SetFocalPoint(fx, fy, fz);
	cam->SetViewUp(upx, upy, upz);
	cam->SetClippingRange(clippingNear, clippingFar);
	if (isParallel)
	{
		cam->SetParallelScale(parallelScale);
	}
	else
	{
		cam->SetViewAngle(viewAngle);
	}
}


