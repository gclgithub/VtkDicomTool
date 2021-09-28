#include "mainWindow.h"
#include "ui_mainWindow.h"
#include <QtWidgets>
#include "MultiImageView.h"
#include "BasicVtkDefine.h"
#include <QSpinBox>
#include "MultiImageView2.h"
#include "vtkDICOMImageReader.h"
mianWindow *mianWindow::self = nullptr;

mianWindow::mianWindow(QWidget *parent) : QMainWindow(parent), ui_(new Ui::MainWindow_ui)
{

    //setStyleSheet(dtcp::QHelper::getStyleQss("mianWindow"));
    ui_->setupUi(this);
    self = this;
    connect(ui_->switch_btn, &QPushButton::clicked, this, &mianWindow::OnSwitchLayout);
    connect(ui_->btn_reset, SIGNAL(clicked()), this, SLOT(OnResetScene()));
	connect(ui_->btn_angle, SIGNAL(clicked()), this, SLOT(OnActivationAngle()));
	connect(ui_->btn_ruler, SIGNAL(clicked()), this, SLOT(OnActivationRuler()));

}

mianWindow::~mianWindow()
{
    self = nullptr;
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
		m_views->initiate(reader->GetOutput());
	}
	else 
	{
		MultiImageView* views = new MultiImageView(ui_->main_widget);
		views->initiate(reader->GetOutput());
		views->showCrossViews();
	}
	//// Picker to pick pixels
	//vtkSmartPointer<vtkPointPicker> propPicker = vtkSmartPointer<vtkPointPicker>::New();
	////propPicker->PickFromListOn();
}

void mianWindow::OnClose()
{
    QCoreApplication::quit();
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
