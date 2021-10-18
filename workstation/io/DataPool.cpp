/*
 * @Author: your name
 * @Date: 2021-10-18 15:49:45
 * @LastEditTime: 2021-10-18 15:51:33
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\vtkImageTool\code\workstation\io\DataPool.cpp
 */

#include "DataPool.h" 
#include "workstation/base/DicomReadToVtk.h"

std::auto_ptr<DataPool> DataPool::instance;

DataPool::DataPool()
{
	//m_roi_list = std::make_shared<std::vector<PTR<WRoi>>>();
}

DataPool& DataPool::GetInstance()
{
	if (NULL == instance.get()) {
		instance.reset(new DataPool());
	}
	return *(instance.get());
}

DataPool::~DataPool()
{
	//if (m_roi_list->size() != 0) 
	//{
	//	m_roi_list->clear();
	//}
	std::cout << "~DataPool" << std::endl;

}

SPTR<vtkImageData> DataPool::get_img_data(const std::string& series_id, const std::string& path)
{
	if (img_map_.size() > 50)img_map_.clear();
	
	auto iter = img_map_.find(series_id);
	if (iter != img_map_.end())
	{
		return iter->second;
	}
	else 
	{

		//read dcm to vtk
		auto data = Dicom::dcmToVtk(path);
		img_map_[series_id] = data;
		return data;
	}
}

//PTR<WRoi> DataPool::get_cur_roi()
//{
//	if (m_cur_roi_num < 0)return nullptr;
//	auto num = m_cur_roi_num;
//	auto it = std::find_if(m_roi_list.lock()->begin(), m_roi_list.lock()->end(), [num](PTR<WRoi> roi) {
//		return roi->get_number() == num; });
//	if (it == m_roi_list.lock()->end())
//		return nullptr;
//	else
//		return (*it);
//}

void DataPool::set_seriesdescription(const QString& seriesdescription)
{
	if (seriesdescription.isEmpty())
		seriesdescription_ = "";
	else
		seriesdescription_ = seriesdescription;

}
