#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QFontDatabase>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int fontId = QFontDatabase::addApplicationFont(":/en-us.ttf");
    if (fontId == -1) {
        qDebug() << "Failed to load font.";
        return;
    }

    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);

    QFont customFont(fontFamily, 8);
    QApplication::setFont(customFont);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_simulation()));
    timer->start(20);
    connect(ui->toggle_button, &QPushButton::clicked, this, &MainWindow::toggle_simulation);
    connect(ui->horizontal_slider, &QSlider::sliderReleased, this, &MainWindow::time_change);
    connect(ui->horizontal_slider, &QSlider::valueChanged, this, &MainWindow::slider_value_change);
}

void MainWindow::time_change(){
    if(timer->isActive()){
        timer->stop();
    }
    ui->celestial_widget->set_time(0.02 * ui->horizontal_slider->value());
}

void MainWindow::slider_value_change(){

}

void MainWindow::toggle_simulation(){
    if(timer->isActive()){
        timer->stop();
    }else{
        timer->start(20);
    }
}

void MainWindow::update_simulation(){
    ui->celestial_widget->update_sim();
    int mx = ui->horizontal_slider->maximum() > ui->celestial_widget->get_current_time() * 50 ?
                ui->horizontal_slider->maximum() : ui->celestial_widget->get_current_time() * 50;
    int nv = ui->celestial_widget->get_current_time() * 50;
    ui->horizontal_slider->setMaximum(
        mx
    );
    ui->horizontal_slider->setValue(nv);
//    std::cout << "Max: " << mx << "Current Value: " << nv << "\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}

