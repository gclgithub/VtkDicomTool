#pragma once
#include <QMainWindow>
class MultiImageView;
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
private:
    static mianWindow*self;
    QTimer *main_timer;
    QScopedPointer<Ui::MainWindow_ui> ui_;

    MultiImageView* views_ = nullptr;
};
