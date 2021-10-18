/*
 * @Author: your name
 * @Date: 2021-10-18 15:49:32
 * @LastEditTime: 2021-10-18 15:51:15
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\vtkImageTool\code\workstation\io\DataPool.h
 */
#pragma once 
#include <memory>
#include "workstation/scene/BasicVtkDefine.h"
#include "vtkRenderWindowInteractor.h"
#include "workstation/scene/SceneMeasureTools.h"
class DataPool
{
public:
	static DataPool& GetInstance();
	~DataPool();
protected:
	DataPool();
	//roi
	SetGetMacro(int, cur_roi_num)
	//SetGetMacro(WEAKPTR<std::vector<PTR<WRoi>>>, roi_list)
	SetGetMacro(WEAKPTR<SceneMeasureTools>, measure_tool)
	SetGetMacro(QString, img_id)
	SetGetMacro(SPTR<vtkRenderWindowInteractor>, interactor)

	SetGetMacro(double, window_center)
	SetGetMacro(double, window_width)
public:
	SPTR<vtkImageData> get_img_data(const std::string& series_id, const std::string& path);


	QString get_seriesdescription()
	{
		return seriesdescription_;
	}
	void set_seriesdescription(const QString& seriesdescription);
	//PTR<WRoi> get_cur_roi();
private:
	static std::auto_ptr<DataPool> instance;
	QString img_id_;
	QString seriesdescription_;
	std::map<std::string, SPTR<vtkImageData>> img_map_;
};
#define Data_Pool DataPool::GetInstance()