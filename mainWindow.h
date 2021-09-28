#pragma once
#include <QMainWindow>
class MultiImageView2;
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
    void OnClose();
    void OnSwitchLayout();
	void OnResetScene();
	void OnActivationAngle();
	void OnActivationRuler();

private:
    static mianWindow*self;
    QTimer *main_timer;
    QScopedPointer<Ui::MainWindow_ui> ui_;

    MultiImageView2* m_views = nullptr;

    bool is_active_tool = false;
};
