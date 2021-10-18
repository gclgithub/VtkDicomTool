#include "DicomReadToVtk.h"
#include "DicomHelper.h"
#include "vtkImageFlip.h"
#include "workstation/io/DataPool.h"
namespace Dicom
{
	typedef  signed short       PixelType;
	typedef std::vector<std::string>    FileNamesContainer;
	typedef itk::Image< PixelType, 3 >      ImageType;
	typedef itk::ImageSeriesReader< ImageType >     ReaderType;
	typedef itk::GDCMImageIO       ImageIOType;
	typedef itk::GDCMSeriesFileNames     NamesGeneratorType;
	typedef itk::ImageToVTKImageFilter< ImageType> itkTovtkFilterType;

	itkTovtkFilterType::Pointer m_itkTovtkImageFilter;
	ReaderType::Pointer m_reader;

	SPTR<vtkImageData> flipData(SPTR<vtkImageData> data, int aix)
	{
		vtkNew<vtkImageFlip> flip;
		flip->SetInputData(data);
		flip->SetFilteredAxes(aix);
		//flip->SetFilteredAxis(aix);
		flip->Update();
		auto n = flip->GetOutput();
		return n;
	}

	SPTR<vtkImageData> dcmToVtk(const std::string& dir)
	{
		m_reader = ReaderType::New();
		ImageIOType::Pointer dicomIO = ImageIOType::New();
		m_reader->SetImageIO(dicomIO);
		NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
		nameGenerator->SetInputDirectory(dir);
		FileNamesContainer fileNames = nameGenerator->GetInputFileNames();
		m_reader->SetFileNames(fileNames);
		m_reader->Update();

		std::string tagkey = "0028|1050";
		std::string win_center;
		if (dicomIO->GetValueFromTag(tagkey, win_center))
		{
			Data_Pool.set_window_center(std::stof(win_center));
		}

		std::string tagkey2 = "0028|1051";
		std::string win_width;
		if (dicomIO->GetValueFromTag(tagkey2, win_width))
		{
			Data_Pool.set_window_width(std::stof(win_width));
		}
		//ITK TO VTK 
		m_itkTovtkImageFilter = itkTovtkFilterType::New();
		auto oData = m_reader->GetOutput();
		m_itkTovtkImageFilter->SetInput(oData);
		m_itkTovtkImageFilter->Update();

		return flipData(m_itkTovtkImageFilter->GetOutput(), 1);
	}

	void dcmToVtk2(const std::string& dir, SPTR<vtkImageData> data)
	{

	}

}
