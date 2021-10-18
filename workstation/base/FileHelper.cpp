#include <sys/stat.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


#include "DicomHelper.h"
#include "FileHelper.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem> 

#define WHITESPACE_CHARS " \t\n\r"

#undef streq
#ifdef _UNICODE
#define  streq(x,y) (wcscmp(x,y) == 0)
#else
#define streq(x,y) (strcmp(x,y)==0)
#endif

#include "QString"
#include "QDir"


std::string GetWorkingDir() {
	char path[MAX_PATH] = "";
	GetCurrentDirectoryA(MAX_PATH, path);
	PathAddBackslashA(path);
	return path;
}

std::string GetAppDataPath(const std::string& customed_path) {
	std::string data_path = std::string(getenv("LOCALAPPDATA")) + "\\prostate\\";
	DWORD ftyp = GetFileAttributesA(data_path.c_str());
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return data_path;
	else return GetWorkingDir();
}


FileHelper::FileHelper()
{
}

FileHelper::~FileHelper()
{

}

PTR<FileHelper> FileHelper::New()
{
	return PTR_NEW(FileHelper);
}

void win_path_to_unix_ath(char* path)
{
	char* c;
	for (c = path; *c; c++)
		if (*c == '\\')
			*c = '/';
}

//void  FileHelper::ScanDir(std::string dir, std::vector<std::string>& filenames)
//{
//	filenames.clear();
//	win_path_to_unix_ath((char*)dir.c_str());
//	if (dir[dir.size() - 1] != '/')
//		dir += ("/");
//#ifdef WIN32
//	//typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > std::string;
//	std::wstring windir(dir.begin(), dir.end()), file;
//
//	WIN32_FIND_DATA find;
//	HANDLE h;
//	DWORD attr;
//	bool add;
//
//	h = FindFirstFile((windir + _T("*")).c_str(), &find);
//	if (h != INVALID_HANDLE_VALUE)
//	{
//		do
//		{
//			if (!streq(find.cFileName, _T(".")) && !streq(find.cFileName, _T("..")))
//			{
//				file = windir + find.cFileName;
//				attr = GetFileAttributes(file.c_str());
//				if (attr & FILE_ATTRIBUTE_DIRECTORY)
//				{
//					std::vector<std::string> temp;
//					//std::string fn = find.cFileName;
//					ScanDir(std::string(file.begin(), file.end()), temp);
//
//					for (int k = 0; k < temp.size(); k++)
//						filenames.push_back(temp[k]);
//				}
//				else
//				{
//					//std::string fn = find.cFileName;
//					filenames.push_back(std::string(file.begin(), file.end()));
//				}
//			}
//		} while (FindNextFile(h, &find));
//
//		FindClose(h);
//	}
//#endif
//}

static int ListDirectoryContents(const char* sDir, std::vector < std::string>& file_path_list)
{
	WIN32_FIND_DATAA fdFile;
	HANDLE hFind = NULL;
	char sPath[2048];
	//Specify a file mask. *.* = We want everything!
	sprintf(sPath, "%s\\*.*", sDir);
	if ((hFind = FindFirstFileA(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	do
	{
		if (strcmp(fdFile.cFileName, ".") != 0
			&& strcmp(fdFile.cFileName, "..") != 0)
		{
			sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

			if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				ListDirectoryContents(sPath, file_path_list); //Recursion, I love it!
			}
			else {
				file_path_list.push_back(sPath);
			}
		}
	} while (FindNextFileA(hFind, &fdFile));

	FindClose(hFind); //Always, Always, clean things up!
	return 0;
}

int FileHelper::ScanFileList(const std::string& directory_name, std::vector<std::string>& file_list)
{
	file_list.clear();
	auto result = ListDirectoryContents(directory_name.c_str(), file_list);
	return 0;
}

int FileHelper::ReadFileBinaryData(const std::string& file_name, std::string& res)
{
	std::string buffer;
	std::ifstream fin(file_name.c_str(), std::ios::binary);
	if (!fin) return 0;
	fin.seekg(0, fin.end);
	int length = fin.tellg();
	if (length == 0) {
		res = "";
		return 0;
	}
	fin.seekg(0, fin.beg);
	buffer.resize(length);
	fin.read(&buffer[0], length);
	res = std::move(buffer);
	return 0;
}

bool FileHelper::ReadDicom(std::vector<std::string>& filenames)
{
	study_list_.clear();
	for (auto& filename : filenames) 
	{
		//检查是否dcm文件
		//TODO:
		auto buffer = PTR_NEW(std::vector<char>);
		buffer->clear();
		try
		{
			std::ifstream fin{ filename.c_str(), std::ios::binary };
			fin.seekg(0, fin.end);
			int length = fin.tellg();
			//std::cout << filename << ":" << length << std::endl;
			if (length == 0)	continue;
			fin.seekg(0, fin.beg);
			buffer->resize(length);
			fin.read(buffer->data(), length);
			fin.close();


			DcmInputBufferStream dataBuf;
			dataBuf.setBuffer(buffer->data(), buffer->size());
			dataBuf.setEos();

			DcmFileFormat  fileformat;
			if (fileformat.read(dataBuf).bad())	continue;
			auto dataset = fileformat.getDataset();
			auto study_uid = Dicom::GetTagString(dataset, DCM_StudyInstanceUID);
			if (study_uid.empty()) continue;
			auto study = check_study(study_uid);

			auto series_uid = Dicom::GetTagString(dataset, DCM_SeriesInstanceUID);
			if (series_uid.empty()) continue;
			auto series = check_series(study, series_uid, dataset);
			auto& ss = series->get_file_list();
			ss.emplace_back(filename);
		}
		catch (const std::exception&)
		{
			continue;
		}

	}
	return true;
}

bool FileHelper::get_dicom_study(std::map<std::string, PTR<DicomStudy>>& study_list)
{
	study_list.clear();
	study_list.insert(study_list_.begin(), study_list_.end());
	if (!copyFileToDir()) 
	{
		return false;
	}
	return true;
}

PTR<DicomStudy> FileHelper::check_study(const std::string& study_id)
{
	auto iter = study_list_.find(study_id);
	if (iter == study_list_.end())
	{
		auto study = DicomStudy::New();
		study_list_[study_id] = study;
	}
	return study_list_[study_id];
}

PTR<DicomSeries> FileHelper::check_series(PTR<DicomStudy> study, const std::string& series_uid, DcmDataset* dataset)
{
	auto& series_list = study->get_series();
	auto it = std::find_if(series_list.begin(), series_list.end(), [series_uid](PTR<DicomSeries> tmp) {
		return tmp->get_series_guid() == series_uid; });
	if (it == series_list.end())
	{
		auto series = DicomSeries::New();
		series->set_series_guid(series_uid);
		series->set_study_uid(study->get_uid());
		auto study_id = Dicom::GetTagString(dataset, DCM_PatientID);
		auto modality = Dicom::GetTagString(dataset, DCM_Modality);
		auto patient_pid = Dicom::GetTagString(dataset, DCM_PatientID);
		auto patient_name = Dicom::GetTagString(dataset, DCM_PatientName);
		auto sop_class_uid = Dicom::GetTagString(dataset, DCM_SOPClassUID);
		auto series_des = Dicom::GetTagString(dataset, DCM_SeriesDescription);
		
		series->set_study_id(study_id);
		series->set_modality(modality);
		series->set_patient_pid(patient_pid);
		series->set_patient_name(patient_name);
		series->set_sop_class_uid(sop_class_uid);
		series->set_series_description(series_des);

		auto sop_instance_uid = Dicom::GetTagString(dataset, DCM_SOPInstanceUID);
		auto& sop_list = series->get_sop_instance_uid_list();
		sop_list.emplace_back(sop_instance_uid);
		
		series_list.emplace_back(series);
		return series;
	}
	auto sop_instance_uid = Dicom::GetTagString(dataset, DCM_SOPInstanceUID);
	auto& sop_list = (*it)->get_sop_instance_uid_list();
	sop_list.emplace_back(sop_instance_uid);
	return *it;
}

bool FileHelper::copyFileToDir()
{
	if (study_list_.size() == 0)return false;

	QString dir_path = QString(GetAppDataPath("").c_str());
	std::cout << "data path:" << GetAppDataPath("").c_str() << std::endl;
	QDir dir(dir_path);
	if (dir.exists())
	{
		dir.removeRecursively();
	}
	dir.mkpath("./");

	//copy file
	for (auto& study: study_list_)
	{
		auto sk = QString(study.first.c_str());
		if (!dir.exists(sk)) dir.mkdir(sk);
		auto& ss = study.second->get_series();
		for (auto& s : ss)
		{
			auto sid = QString(s->get_series_guid().c_str());
			QString s_path = sk + "/" + sid;
			if (!dir.exists(s_path)) dir.mkdir(s_path);
			QString out_path = dir.path() + "/" + s_path;
			s->set_file_path(out_path.toStdString());

			std::vector<std::string> new_file_list;
			auto& file_list = s->get_file_list();
			for each (auto file in file_list)
			{
				auto fileinfo = QFileInfo(file.c_str());
				auto name = fileinfo.fileName();
				QString out_file = out_path + "/" + name;
				new_file_list.emplace_back(out_file.toStdString());
				std::string des_path = out_file.toStdString();
				QString src_file = file.c_str();
				src_file.replace("\\", "/");
				QFile qf(src_file);
				if (!qf.copy(out_file)) 
				{
					return false;
				}
			}
			file_list.assign(new_file_list.begin(), new_file_list.end());
		}

	}
	return true;
}

