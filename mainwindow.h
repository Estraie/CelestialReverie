#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rendering/celestialGLWidget.h>
#include <qtimer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void toggle_simulation();
    void time_change();
    void slider_value_change();

    ~MainWindow();

signals:
    void my_timeout();
private slots:
    void update_simulation();
private:
    Ui::MainWindow *ui;
    QTimer* timer;
};
#endif // MAINWINDOW_H
