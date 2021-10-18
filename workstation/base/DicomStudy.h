#pragma once
#include "BasicDefine.h"
/*
 * 存储关于study的数据
 * 在Dicom标准中,Study指的是一系列series组成的集合,
 * 一个study中的所有series是同种设备(如CT,MRI,PETCT等)生成的,
 * 同一个病人可能会同时进行多种设备的检查,这种时候每个病人就对应多个Study,
 * 如果病人只进行了一种检查,就只有一个Study.
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

