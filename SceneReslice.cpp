#include "SceneReslice.h"
#include "vtkImageProperty.h"

PTR<SceneReslice> SceneReslice::New(SPTR<vtkImageData> data, SPTR<vtkMatrix4x4> mtx)
{
	return PTR<SceneReslice>(new SceneReslice(data, mtx));
}

SceneReslice::~SceneReslice()
{

}

void SceneReslice::moveReslice(int value)
{
	if (m_reslice)
	{
		int is_up = 1;
		auto orie = m_imgActor->GetOrientation();
		if (orie[0] == 90 || orie[1] == 90) 
			is_up = -1;

		double spacing = m_reslice->GetOutput()->GetSpacing()[2];
		double point[4], center[4];
		point[0] = 0.0;
		point[1] = 0.0;
		point[2] = is_up * spacing * value;
		point[3] = 1.0;

		auto matrix = m_imgActor->GetMatrix();
		matrix->MultiplyPoint(point, center);
		m_imgActor->SetPosition(center);
		doReslice();
	}
}

void SceneReslice::doReslice()
{
	SPTR<vtkMatrix4x4> matrix = m_imgActor->GetMatrix();
	vtkNew<vtkMatrix4x4> matrix2;
	vtkMatrix4x4::Invert(m_roatMatx, matrix2);
	vtkMatrix4x4::Multiply4x4(matrix2, matrix, matrix2);
	m_reslice->SetResliceAxes(matrix2);
	m_color_map->Update();
}

void SceneReslice::doReslice(SPTR<vtkMatrix4x4> matrix)
{
	vtkNew<vtkMatrix4x4> matrix2;
	vtkMatrix4x4::Invert(m_roatMatx, matrix2);
	vtkMatrix4x4::Multiply4x4(matrix2, matrix, matrix2);
	m_reslice->SetResliceAxes(matrix2);
	m_color_map->Update();
}

void SceneReslice::get_window_level(double& ww, double& wl, double& defaultWW, double& defaultWL)
{
	ww = m_imgActor->GetProperty()->GetColorWindow();
	wl = m_imgActor->GetProperty()->GetColorLevel();
	defaultWW = m_wwDefault;
	defaultWL = m_wlDefault;
}

void SceneReslice::get_slicer_num(int& max_slicer, int& cur_slicer)
{
	auto pos = m_imgActor->GetPosition();
	auto orie = m_imgActor->GetOrientation();
	if (orie[0] == 90)
	{
		max_slicer = m_img_size[1];
		int delta = pos[1] / m_img_spacing[1];
		cur_slicer = delta + 1;
	}
	else if (orie[1] == 90)
	{
		max_slicer = m_img_size[0];
		int delta = pos[0] / m_img_spacing[0];
		cur_slicer = delta + 1;
	}
	else
	{
		max_slicer = m_img_size[2];
		int delta = pos[2] / m_img_spacing[2];
		cur_slicer = max_slicer - delta;
	}
}

void SceneReslice::resetReslicer()
{
	if (m_reslice) 
	{
		m_imgActor->SetPosition(m_center[0], m_center[1], m_center[2]);
		doReslice();
	}
}

SceneReslice::SceneReslice(SPTR<vtkImageData> data, SPTR<vtkMatrix4x4> mtx):m_roatMatx(mtx)
{
	data->GetDimensions(m_img_size);
	data->GetSpacing(m_img_spacing);
	data->GetOrigin(m_img_origin);
	m_center[0] = m_img_origin[0] + int(m_img_size[0] * 0.5 - 0.5) * m_img_spacing[0];
	m_center[1] = m_img_origin[1] + int(m_img_size[1] * 0.5 - 0.5) * m_img_spacing[1];
	m_center[2] = m_img_origin[2] + int(m_img_size[2] * 0.5 - 0.5) * m_img_spacing[2];

	static double axialElements[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 };

	vtkNew<vtkMatrix4x4> resliceAxes;
	resliceAxes->DeepCopy(axialElements);
	resliceAxes->SetElement(0, 3, m_center[0]);
	resliceAxes->SetElement(1, 3, m_center[1]);
	resliceAxes->SetElement(2, 3, m_center[2]);

	m_reslice = SPTR<vtkImageReslice>::New();
	m_reslice->SetInputData(data);
	m_reslice->SetOutputDimensionality(2);
	m_reslice->SetResliceAxes(resliceAxes);
	m_reslice->SetInterpolationModeToLinear();
	if (true) 
	{
		vtkNew<vtkLookupTable> colorTable;
		double* min_max = data->GetScalarRange();
		colorTable->SetRange(min_max[0], min_max[1]);
		colorTable->SetValueRange(0.0, 1.0);
		colorTable->SetSaturationRange(0.0, 0.0);
		colorTable->SetRampToLinear();
		colorTable->Build();

		m_color_map = SPTR<vtkImageMapToColors>::New();
		m_color_map->SetLookupTable(colorTable);
		m_color_map->SetInputConnection(m_reslice->GetOutputPort());
		m_color_map->Update();

		m_imgActor = SPTR<vtkImageActor>::New();
		m_imgActor->SetInputData(m_color_map->GetOutput());
		m_imgActor->SetPosition(m_center[0], m_center[1], m_center[2]);

		m_wwDefault = m_imgActor->GetProperty()->GetColorWindow();
		m_wlDefault = m_imgActor->GetProperty()->GetColorLevel();
	} 
	else 
	{
		//New
		static double cornalElements[16] = {
		1, 0, 0, 0,
		0, 0, 1, 0,
		0, -1,0, 0,
		0, 0, 0, 1 };

		static double sagittalElements[16] = {
		0, 0,-1, 0,
		1, 0, 0, 0,
		0,-1, 0, 0,
		0, 0, 0, 1 };

		auto elements = axialElements;
		//if (m_viewType == SAG_M)
		//	elements = sagittalElements;
		//else if (m_viewType == COR_M)
		//	elements = cornalElements;

		vtkNew<vtkMatrix4x4> resliceAxes;
		resliceAxes->DeepCopy(elements);
		resliceAxes->SetElement(0, 3, m_center[0]);
		resliceAxes->SetElement(1, 3, m_center[1]);
		resliceAxes->SetElement(2, 3, m_center[2]);


		m_reslice = SPTR<vtkImageReslice>::New();
		m_reslice->SetInputData(data);
		m_reslice->SetOutputDimensionality(2);
		m_reslice->SetResliceAxes(resliceAxes);
		m_reslice->SetInterpolationModeToLinear();

		//*****************************************************************//
		vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
		colorTable->SetRange(0, 1000);
		colorTable->SetValueRange(0.0, 1.0);
		colorTable->SetSaturationRange(0.0, 0.0);
		colorTable->SetRampToLinear();
		colorTable->Build();
		m_color_map = SPTR<vtkImageMapToColors>::New();
		m_color_map->SetLookupTable(colorTable);
		m_color_map->SetInputConnection(m_reslice->GetOutputPort());
		m_color_map->Update();

		m_imgActor = SPTR<vtkImageActor>::New();
		m_imgActor->SetInputData(m_color_map->GetOutput());
	}
	
}
