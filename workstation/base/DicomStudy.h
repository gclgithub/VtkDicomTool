#pragma once
#include "BasicDefine.h"
/*
 * �洢����study������
 * ��Dicom��׼��,Studyָ����һϵ��series��ɵļ���,
 * һ��study�е�����series��ͬ���豸(��CT,MRI,PETCT��)���ɵ�,
 * ͬһ�����˿��ܻ�ͬʱ���ж����豸�ļ��,����ʱ��ÿ�����˾Ͷ�Ӧ���Study,
 * �������ֻ������һ�ּ��,��ֻ��һ��Study.
 */
class DicomSeries;
class DicomPatient;
class DicomStudy
{
public:
	static PTR<DicomStudy> New();
	~DicomStudy();
protected:
	DicomStudy();
private:
	GetMacro(std::string, uid)
	//SetGetMacro(WEAKPTR<DicomPatient>, patient)
	GetRefMacro(std::vector<PTR<DicomSeries>>, series)
	GetMacro(bool, parse_from_files)
};

