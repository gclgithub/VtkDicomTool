#pragma once
#include <string>
#include <vector>
#include "DicomStudy.h"

/*
 * 该类存储和病人相关的一些信息
 * 在Dicom标准中,一个数据库中存放着多个病人的信息
 * 每个病人可能有1到多个Study,每个Study可能有1到多个Series,每个Series可能有多张Image,
 * 同一个病人的所有下级信息的病人姓名和病人ID是相同的
 * 注意,这也许不符合主观印象,但是关于病人的个人信息并不都存放在病人这个层级上
 * 我们考虑到,一个人的一生会做很多次检查,每次检查都有可能使用1到多种医学影像手段,所以其他的信息
 * 比如年龄,检查时间,甚至性别,都是不固定的,这些信息要随着每次检查而变动,
 * 而在Dicom中,我们认为每个病人有唯一的PatientID作为识别,只要ID相同,所有进入PACS系统的医学影像都将
 * 进入同一个目录被管理
 * 这里我们不考虑病人改名的情况
 */
class DicomPatient
{
public:
	static PTR<DicomPatient> New();
	~DicomPatient();
public:
	DicomPatient();

};

