#pragma once
#include <string>

/*
 * �洢����Dicomͼ��������Ϣ
 * 
 * ����:
 * 
 */
class DicomImage
{
public:
	DicomImage();
	~DicomImage();
public:
	std::string ReferencedFileID;    //Dicomͼ��ľ���·��
	std::string InstanceCreationDate;//ͼƬ����ʱ��
	std::string kpv;				 //�����������е���
	std::string SliceLocation;		 //emmm
	std::string InstanceNumber;      //���ڱ��
	std::string AbsFilePath;		 //���ļ����ļ�ϵͳ�ϵľ����ļ���
	int row;
	int col;//ͼƬ�ߴ�
	std::string PixelSpacing;        //���ؿռ�
public:
	std::string getReferencedFileID();
	void setReferencedFileID(std::string value);

	std::string getInstanceCreationDate();
	void setInstanceCreationDate(std::string value);

	std::string getkpv();
	void setkpv(std::string value);

	std::string getSliceLocation();
	void setSliceLocation(std::string value);

	std::string getPixelSpacing();
	void setPixelSpacing(std::string value);

	int getRow();
	void setRow(int value);

	int getCol();
	void setCol(int value);
};

