#pragma once
#include "BasicDefine.h"
#include "DicomStudy.h"
#include "DicomSeries.h"

class DcmDataset;
class FileHelper
{
public:
	static PTR<FileHelper> New();
	~FileHelper();
	FileHelper();
public:
	//void  ScanDir(std::string dir, std::vector<std::string>& filenames);
	static int ScanFileList(const std::string& directory_name, std::vector<std::string>& file_list);
	static int ReadFileBinaryData(const std::string& file_name, std::string& res);
	bool ReadDicom(std::vector<std::string>& filenames);
	bool get_dicom_study(std::map<std::string, PTR<DicomStudy>>& study_list);
private:
	PTR<DicomStudy> check_study(const std::string& study_id);
	PTR<DicomSeries> check_series(PTR<DicomStudy> study, const std::string& series_uid, DcmDataset* dataset);
	std::map<std::string, PTR<DicomStudy>>  study_list_;

	bool copyFileToDir();

};