#pragma once
#include "BasicDefine.h"
#include "DicomImage.h"
/*
 * ���ڴ洢series�йص���Ϣ
 * ��Dicom��׼��,seriesָ����������������ͼƬ���ɵ�һ������
 * 
 * ����:
 */
class DicomSeries
{
public:
	static PTR<DicomSeries> New();
	~DicomSeries();
protected:
	DicomSeries();
	SetGetMacro(std::string, series_guid)
	SetGetMacro(std::string, modality)
	SetGetMacro(std::string, sop_class_uid)
	SetGetMacro(std::string, operator_name)
	SetGetMacro(std::string, series_description)
	SetGetMacro(std::string, series_number)
	SetGetMacro(std::string, series_date)
	SetGetMacro(std::string, series_time)
	//temporary
	SetGetMacro(std::string, study_uid)
	SetGetMacro(std::string, study_date)
	SetGetMacro(std::string, study_time)
	SetGetMacro(std::string, referring_physician_name)
	SetGetMacro(std::string, study_id)
	SetGetMacro(std::string, accession_number)

	SetGetMacro(std::string, patient_id)
	SetGetMacro(std::string, patient_name)

	SetGetMacro(std::string, patient_pid)
	SetGetMacro(std::string, patient_guid)
	GetRefMacro(std::vector<std::string>, sop_instance_uid_list)
	GetRefMacro(std::vector<std::string>, file_list)
	GetRefMacro(std::vector<PTR<std::vector<char>>>, binary_list)
	SetGetMacro(std::string, file_path)

};

