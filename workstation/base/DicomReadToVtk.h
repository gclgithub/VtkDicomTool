#pragma once
#include "workstation/scene/BasicVtkDefine.h"

//要用itk必须要添加下这两行
#pragma comment(lib,"rpcrt4.lib")
#pragma comment(lib,"WS2_32.lib")
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkShiftScaleImageFilter.h"

namespace Dicom 
{


	SPTR<vtkImageData> flipData(SPTR<vtkImageData> data, int aix);

	SPTR<vtkImageData> dcmToVtk(const std::string& dir);
	void dcmToVtk2(const std::string& dir, SPTR<vtkImageData> data);
}


