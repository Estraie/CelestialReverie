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
}

//void MainWindow::my_timeout(){}

void MainWindow::toggle_simulation(){
    if(timer->isActive()){
        timer->stop();
    }else{
        timer->start(20);
    }
}

void MainWindow::update_simulation(){
    ui->celestial_widget->update_sim();
    ui->horizontal_slider->setMaximum(
        ui->horizontal_slider->value() > ui->celestial_widget->get_current_time() ?
        ui->horizontal_slider->value() : ui->celestial_widget->get_current_time()
    );
    ui->horizontal_slider->setValue(ui->celestial_widget->get_current_time());
}

MainWindow::~MainWindow()
{
    delete ui;
}

