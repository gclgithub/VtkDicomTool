#include "mainWindow.h"
#include "ui_mainWindow.h"
#include <QtWidgets>
#include "workstation/scene/BasicVtkDefine.h"
#include <QSpinBox>
#include "workstation/scene/MultiImageView2.h"
#include "vtkDICOMImageReader.h"
#include "workstation/base/FileHelper.h"
#include "viewTreeModel.h"
#include "workstation/io/DataPool.h"

mianWindow *mianWindow::self = nullptr;

mianWindow::mianWindow(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow_ui)
{
    //setStyleSheet(dtcp::QHelper::getStyleQss("mianWindow"));
    ui_->setupUi(this);
    self = this;
	connect(ui_->btn_open_dir, &QPushButton::clicked, this, &mianWindow::OnOpenSeriesFolder);
    connect(ui_->btn_switch, &QPushButton::clicked, this, &mianWindow::OnSwitchLayout);
    connect(ui_->btn_reset, SIGNAL(clicked()), this, SLOT(OnResetScene()));
	connect(ui_->btn_angle, SIGNAL(clicked()), this, SLOT(OnActivationAngle()));
	connect(ui_->btn_ruler, SIGNAL(clicked()), this, SLOT(OnActivationRuler()));
	m_dicomdirtreemodel = nullptr;

	connect(ui_->treeView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(OnlineTreeViewDoubleClick(const QModelIndex)));
}

mianWindow::~mianWindow()
{
    self = nullptr;
	if (m_dicomdirtreemodel != nullptr)
	{
		delete m_dicomdirtreemodel;
		m_dicomdirtreemodel = nullptr;
	}
}

void mianWindow::initiate()
{
	//std::string path = "C:\\Users\gcl\\Desktop\\Desktop\\T1WI";
	std::string path = "D:\\ct_data\\mri\\PA1\\ST0\\SE4";
   //read dicom seriers
	vtkNew<vtkDICOMImageReader> reader;
	reader->SetDirectoryName(path.c_str());
	reader->Update();
	reader->GetImageOrientationPatient();

	auto img = reader->GetOutput();
	short* data = (short*)img->GetScalarPointer();

	//FILE* pf = fopen("D:\\se.bin", "wb");
	//fwrite(data, sizeof(short), 512 * 512 * 88, pf);
	//fclose(pf);

	bool show_one_window = true;
	if (show_one_window) 
	{
		m_views = new MultiImageView2(ui_->main_widget);
		m_views->initiate();
		//m_views->initiate(reader->GetOutput());

	}
	else 
	{
		//MultiImageView* views = new MultiImageView(ui_->main_widget);
		//views->initiate(reader->GetOutput());
		//views->showCrossViews();
	}
	//// Picker to pick pixels
	//vtkSmartPointer<vtkPointPicker> propPicker = vtkSmartPointer<vtkPointPicker>::New();
	////propPicker->PickFromListOn();
}

void mianWindow::OnClose()
{
    QCoreApplication::quit();
}


void mianWindow::OnOpenSeriesFolder()
{
	//获取Series文件夹的绝对路径名
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("打开Series目录"), "D:/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty() == true) return;
	if (dir.size() < 4) 
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件夹路径错误"));
		return;
	}

	std::vector<std::string> file_list;

	auto filer = FileHelper::New();
	filer->ScanFileList(dir.toStdString(), file_list);

	if (file_list.size() > 400)
	{
		if (QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("文件数量太多，请确认是否继续，继续可能长时间等待"), 
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes)
		{
			return;
		}
	}
	filer->ReadDicom(file_list);
	if (!filer->get_dicom_study(study_list_)) 
	{
		QMessageBox::about(NULL, QString::fromLocal8Bit("错误"), "数据读取失败");
		return;
	}
	viewTreeRefresh();
}

void mianWindow::OnSwitchLayout()
{
	if (m_views->get_cur_layout() == LAYOUT_TCSV)
		m_views->SwitchToTCS();
	else if(m_views->get_cur_layout() == LAYOUT_TCS)
		m_views->SwitchToTCSV();
}

void mianWindow::OnResetScene()
{
	m_views->ResetScene();
}

void mianWindow::OnActivationAngle()
{
	if (!is_active_tool) 
	{
		m_views->ActivationTool(1);
		is_active_tool = true;
	}
	else
	{
		m_views->ActivationTool(0);
		is_active_tool = false;
	}
}

void mianWindow::OnActivationRuler()
{
	if (!is_active_tool)
	{
		m_views->ActivationTool(2);
		is_active_tool = true;
	}
	else
	{
		m_views->ActivationTool(0);
		is_active_tool = false;
	}
}


void mianWindow::viewTreeRefresh()
{
	if (study_list_.size() == 0)return;

	QStringList headers;
	headers.append(QStringLiteral("ID"));
	headers.append(QStringLiteral("详细信息"));

	if (m_dicomdirtreemodel != nullptr)
	{
		delete m_dicomdirtreemodel;
		m_dicomdirtreemodel = nullptr;
	}
	m_dicomdirtreemodel = new viewTreeModel(headers, study_list_);
	ui_->treeView->setModel(m_dicomdirtreemodel);
	ui_->treeView->expandAll();
	ui_->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	for (int column = 0; column < m_dicomdirtreemodel->columnCount(); ++column)
		ui_->treeView->resizeColumnToContents(column);
}

void mianWindow::OnlineTreeViewDoubleClick(const QModelIndex& index)
{
	viewTreeModel* m = (viewTreeModel*)index.model();
	auto item = m->getItem(index);
	auto series_id =item->get_series_id();

	std::string img_path = "";
	for (auto tmp : study_list_)
	{
		auto study = tmp.second;
		auto ss = study->get_series();
		auto item = std::find_if(ss.begin(), ss.end(), [series_id](PTR<DicomSeries> series) {
			return series->get_series_guid() == series_id; });
		if (item != ss.end())
		{
			img_path = (*item)->get_file_path();
			break;
		}
	}
	if (img_path.empty())return;
	std::cout << img_path << std::endl;

	data_ = Data_Pool.get_img_data(series_id, img_path);
	m_views->set_img_data(data_);
}
