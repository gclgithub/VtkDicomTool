#pragma once
#include <QMainWindow>
#include "workstation/scene/BasicVtkDefine.h"
class MultiImageView2;
class DicomStudy;
class viewTreeModel;
class vtkImageData;
namespace Ui {
    class MainWindow_ui;
}

class mianWindow : public QMainWindow
{
    Q_OBJECT
public:
    mianWindow(QWidget *parent = nullptr);
    ~mianWindow();
    static mianWindow*Instance() { return self; }
    void initiate();

public slots:
    void OnOpenSeriesFolder();
    void OnSwitchLayout();
	void OnResetScene();
	void OnActivationAngle();
	void OnActivationRuler();
	void OnClose();

    void OnlineTreeViewDoubleClick(const QModelIndex& index);

private:
    void viewTreeRefresh();
private:
    static mianWindow*self;
    QTimer *main_timer;
    QScopedPointer<Ui::MainWindow_ui> ui_;

    MultiImageView2* m_views = nullptr;

    bool is_active_tool = false;

    std::map<std::string, PTR<DicomStudy>> study_list_;
    viewTreeModel* m_dicomdirtreemodel;

    SPTR<vtkImageData> data_;
};
