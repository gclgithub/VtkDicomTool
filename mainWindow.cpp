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
}

mianWindow::~mianWindow()
{
    self = nullptr;
}

void mianWindow::initiate()
{
   //read dicom seriers
	vtkNew<vtkDICOMImageReader> reader;
	reader->SetDirectoryName("D:\\ct_data\\mri\\PA1\\ST0\\SE4");
	reader->Update();
	reader->GetImageOrientationPatient();

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
