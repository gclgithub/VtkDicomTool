#include "SceneCrossLine.h"
#include "vtkPoints.h"
#include "vtkLine.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkLineSource.h"
#include "vtkDataSetMapper.h"
PTR<SceneCrossLine> SceneCrossLine::New()
{
	return PTR<SceneCrossLine>(new SceneCrossLine());
}

SceneCrossLine::~SceneCrossLine()
{

}

SceneCrossLine::SceneCrossLine()
{
	double color[3] = { 1.0,0.0,0.0 };
	int size = 1;
	double width = 5000;
	for (int i = 0; i < 3; ++i)
	{
		vtkNew<vtkLineSource> line_source;
		if (i == 0) 
		{
			line_source->SetPoint1(-1 * width, 0.0, 0.0);
			line_source->SetPoint2(width, 0.0, 0.0);
		}
		else if (i == 1) 
		{
			line_source->SetPoint1(0.0, -1 * width, 0.0);
			line_source->SetPoint2(0.0, width, 0.0);
		}
		else 
		{
			line_source->SetPoint1(0, 0, -1 * width);
			line_source->SetPoint2(0, 0, width);
		}
		line_source->Update();

		vtkNew<vtkDataSetMapper> mapper;
		mapper->SetInputConnection(line_source->GetOutputPort());
		mapper->SetResolveCoincidentTopologyToPolygonOffset();
		vtkNew<vtkActor> line_actor;
		line_actor->SetMapper(mapper);
		line_actor->GetProperty()->SetColor(color);
		line_actor->GetProperty()->SetLineWidth(size);
		line_actor->GetProperty()->SetOpacity(0.5);
		line_actor->PickableOn();
		m_line_actor_list.emplace_back(line_actor);
	}
}


void SceneCrossLine::resetCrossLine(double* pos)
{
	//direction x
	m_line_actor_list[0]->SetPosition(pos);
	//direction y
	m_line_actor_list[1]->SetPosition(pos);
	//direction z
	m_line_actor_list[2]->SetPosition(pos);

}

void SceneCrossLine::UpdateCrossLine(double* res_pos,const ViewType& type)
{
	switch (type)
	{
	case TRA_M:
	{
		auto x_actor = m_line_actor_list[0];
		{
			auto pos = x_actor->GetPosition();
			pos[2] = res_pos[2];
			x_actor->SetPosition(pos);
			//std::cout << "pos1:" << pos[0] << "," << pos[1] << "," << pos[2] << std::endl;
		}

		auto y_actor = m_line_actor_list[1];
		{
			auto pos = y_actor->GetPosition();
			pos[2] = res_pos[2];
			y_actor->SetPosition(pos);
		}
		break;
	}
	case COR_M:
	{
		auto x_actor = m_line_actor_list[0];
		{
			auto pos = x_actor->GetPosition();
			pos[1] = res_pos[1];
			x_actor->SetPosition(pos);
		}

		auto z_actor = m_line_actor_list[2];
		{
			auto pos = z_actor->GetPosition();
			pos[1] = res_pos[1];
			z_actor->SetPosition(pos);
		}
		break;
	}
	case SAG_M:
	{
		auto y_actor = m_line_actor_list[1];
		{
			auto pos = y_actor->GetPosition();
			pos[0] = res_pos[0];
			y_actor->SetPosition(pos);
		}

		auto z_actor = m_line_actor_list[2];
		{
			auto pos = z_actor->GetPosition();
			pos[0] = res_pos[0];
			z_actor->SetPosition(pos);
		}
		break;
	}
	default:
		break;
	}

	for (auto actor: m_line_actor_list)
	{
		actor->Modified();
	}
}

ViewType SceneCrossLine::check_cross(vtkActor* actor, const ViewType& type)
{
	for (int i = 0; i < m_line_actor_list.size(); i++)
	{
		if (actor != m_line_actor_list[i].Get())continue;

		if (i == 0) 
		{
			if (type == TRA_M)
				return COR_M;
			else
				return TRA_M;
		}
		else if (i == 1) 
		{
			if (type == TRA_M)
				return SAG_M;
			else
				return TRA_M;
		}
		else 
		{
			if (type == COR_M)
				return SAG_M;
			else
				return COR_M;
		}
	}
}
