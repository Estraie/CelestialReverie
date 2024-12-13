#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>
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

    ui->celestial_widget->timer = new QTimer(ui->celestial_widget);
    connect(ui->celestial_widget->timer, SIGNAL(timeout()), this, SLOT(update_simulation()));
    ui->celestial_widget->timer->start(20);
    connect(ui->toggle_button, &QPushButton::clicked, ui->celestial_widget, &celestial_gl_widget::toggle_simulation);
    connect(ui->horizontal_slider, &QSlider::sliderReleased, this, &MainWindow::time_change);
    connect(ui->horizontal_slider, &QSlider::valueChanged, this, &MainWindow::slider_value_change);
    connect(ui->actionSave, &QAction::triggered, ui->celestial_widget, &celestial_gl_widget::save_csv);
    connect(ui->actionLoad, &QAction::triggered, ui->celestial_widget, &celestial_gl_widget::load_csv);
    connect(ui->actionBrute_Force, &QAction::triggered, ui->celestial_widget, &celestial_gl_widget::set_pure_newtonian);
    connect(ui->actionBarnes_Hut, &QAction::triggered, ui->celestial_widget, &celestial_gl_widget::set_barnes_hut);
}

void MainWindow::time_change(){
    if(ui->celestial_widget->timer->isActive()){
        ui->celestial_widget->timer->stop();
    }
    ui->celestial_widget->set_time(0.02 * ui->horizontal_slider->value());
}

void MainWindow::slider_value_change(){

}

//void MainWindow::toggle_simulation(){
//    if(timer->isActive()){
//        timer->stop();
//    }else{
//        timer->start(20);
//    }
//}

//void MainWindow::dump_frame() {
//    if(ui->celestial_widget->timer->isActive()){
//        ui->celestial_widget->timer->stop();
//    }

//    QString filename = QFileDialog::getSaveFileName(
//        this,
//        "Save current frame",
//        ".",
//        "Comma Separated Values(*.csv)"
//    );
//    if (!filename.isEmpty()) {
//        QFile file(filename);

//        if (file.exists()) {
//            int ret = QMessageBox::warning(
//                nullptr,
//                "File exists",
//                "File exists, do cover?",
//                QMessageBox::Yes | QMessageBox::No
//            );

//            if (ret == QMessageBox::No) {
//                qDebug() << "Cancelled covering.";
//                return;
//            }
//        }

//        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//            QTextStream out(&file);
//            out << "Sample content.\n";
//            file.close();
//            QMessageBox::information(nullptr, "Information", "Successfully saved!");
//        } else {
//            QMessageBox::critical(nullptr, "Error", "Cannot write target file!");
//        }
//    } else {
//        qDebug() << "Cancelled saving.";
//    }
//}

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

