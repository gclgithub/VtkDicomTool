#include "DicomPatient.h"


/*
 * Ĭ�Ϲ��캯��
 */
DicomPatient::DicomPatient()
{
	indexOfCurrentStudy = 0; 
	indexOfCurrentSeries = 0;
	indexOfCurrentImage = 0;
}
/*
 * �������캯��
 * ����Patient����������,���Ƕ���Studylist�������ĺͼ�������Ķ����ǵ�ַ����
 * Ҳ����ǳ����,������������߲���Ҫ����д����,������������ν��,ֻҪ������ֵ�ǶԵ�
 * �Ϳ�����
 * Ϊ�˷�ֹ����Ҫ���鷳,δ��Ӧ�ý������Է�װ��ֹһ�������ⲿ�Ķ�ָ����޸�
 */
DicomPatient::DicomPatient(DicomPatient * old)
{
	this->PatientName = old->PatientName;
	this->PatientID =old->PatientID;
	this->indexOfCurrentStudy = old->indexOfCurrentStudy; 
	this->indexOfCurrentSeries = old->indexOfCurrentSeries;
	this->indexOfCurrentImage = old->indexOfCurrentImage; 

	for(int i=0;i<old->StudyList.size();i++)
	{
		this->StudyList.push_back(old->StudyList[i]);
	}
}
/*
 * ����
 */
DicomPatient::~DicomPatient()
{
}
/*
 * ��λ,�����е�������λ
 */
void DicomPatient::reset()
{
	indexOfCurrentStudy = 0;
	indexOfCurrentSeries = 0;
	indexOfCurrentImage = 0;
}
/*
 * ��ȡ��ǰStudy
 */
DicomStudy * DicomPatient::getCurrentDicomStudy()
{
	return this->StudyList[indexOfCurrentStudy];
}
/*
 * ͨ��studyid���õ�ǰ��study
 */
void DicomPatient::setCurrentDicomStudyById(std::string &studyid)
{
	int index = 0;
	for(index=0;index<this->StudyList.size(); index++)
	{
		if (StudyList[index]->StudyId == studyid)
		{
			indexOfCurrentStudy = index;
			return;
		}
	}
	indexOfCurrentStudy = 0;
}
/*
 * ��ȡ��ǰSeries
 */
DicomSeries * DicomPatient::getCurrentDicomSeries()
{
	return this->StudyList[indexOfCurrentStudy]->SeriesList[indexOfCurrentSeries];
}
/*
 * ͨ��seriesnum���õ�ǰ��Series
 */
void DicomPatient::setCurrentDicomSeriesById(std::string & seriesnum)
{
	//�������е�series,��������ʱ��ֱ�Ӱ�study��series����ȫ���л�
	for(int i=0;i<this->StudyList.size();i++)
	{
		for(int j=0;j<this->StudyList[i]->SeriesList.size();j++)
		{
			if(this->StudyList[i]->SeriesList[j]->SeriseNumber==seriesnum)
			{
				indexOfCurrentSeries = j;
				indexOfCurrentStudy = i;
				indexOfCurrentImage = 0;
				return;
			}
		}
	}
}
/*
 * ��ȡ��ǰImage
 */
DicomImage * DicomPatient::getCurrentDicomImage()
{
	return this->StudyList[indexOfCurrentStudy]->SeriesList[indexOfCurrentSeries]->ImageList[indexOfCurrentImage];
}
/*
 * ��ȡ��һ��Image,���ƶ�ָ��,Խ���Զ��ص���һ��
 */
DicomImage * DicomPatient::getNextDicomImage()
{
	indexOfCurrentImage = 
		(indexOfCurrentImage == StudyList[indexOfCurrentStudy]->
								SeriesList[indexOfCurrentSeries]->
								ImageList.size() - 1 ?
								0 : indexOfCurrentImage + 1);
	return getCurrentDicomImage();
}
/*
 * ͨ��RFID�ڵ�ǰ���˵���Ϣ�м�����һ��ͼƬ
 */
DicomImage * DicomPatient::getDicomImageByRfid(std::string & rfid)
{
	//����������˵�ȫ��Image
	//һ�������ɹ�,�����л�Study,Series,Image��Current����,�����ص�ǰͼƬ
	for(int i=0;i<this->StudyList.size();i++)
	{
		for(int j=0;j<this->StudyList[i]->SeriesList.size();j++)
		{
			for(int k=0;k<this->StudyList[i]->SeriesList[j]->ImageList.size();k++)
			{
				if(this->StudyList[i]->SeriesList[j]->ImageList[k]->ReferencedFileID== rfid)
				{
					indexOfCurrentStudy = i;
					indexOfCurrentSeries = j;
					indexOfCurrentImage = k;
					return  getCurrentDicomImage();
				}
			}
		}
	}
}
/*
 * ȡ��ǰstudy,��ǰseries�ĵ�index��ͼƬ,���Խ��,����ԭ��������ָ�����һ��ͼƬ
 * index��0��ʼ
 */
 DicomImage * DicomPatient::getDicomImageByIndex(int index)
 {
	 if (index>this->StudyList[indexOfCurrentStudy]->SeriesList[indexOfCurrentSeries]->ImageList.size())
	 {
		 //Խ��,���ص�ǰͼƬ
		 return getCurrentDicomImage();
	 }else
	 {
		 indexOfCurrentImage = index;
		 return this->StudyList[indexOfCurrentStudy]->SeriesList[indexOfCurrentSeries]->ImageList[indexOfCurrentImage];
	 }
 }
