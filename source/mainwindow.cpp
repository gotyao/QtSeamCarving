#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//0----->width
//|
//|
//v
//height

void MainWindow::on_pushButton_clicked(){
    filename = QFileDialog::getOpenFileName(this,
                                            "open a file",
                                            "C:",
                                            "image(*.PNG *.png *.jpg) ;; all (*.*)");
    ui->lineEdit->setText(filename);
}

void MainWindow::on_pushButton_2_clicked(){
    clock_t start, finish;
    start = clock();
    QImage qTemp;
    qTemp.load(filename);
    int Wnew = ui->lineEdit_2->text().toInt();
    int Hnew = ui->lineEdit_3->text().toInt();

    vector<unsigned> qimage;
    width = qTemp.width();
    height = qTemp.height();

    QRgb qTempCol;
    for (int i = 0; i < qTemp.height(); i++){
        for (int j = 0; j < qTemp.width(); j++){
            qTempCol = qTemp.pixel(j, i);
            qimage.push_back(qTempCol);
        }
    }

    SeamCarver seam(qimage, width, height);
    clock_t test;
    for(int i = 0; i < width - Wnew; i++){
        seam.removeVerticalSeam();
    };
    for(int i = 0; i < height - Hnew; i++){
        seam.removeHorizontalSeam();
        test = clock();
        qDebug() << test - start;
    }
    qDebug() << "ok";

    QImage out(Wnew, Hnew, QImage::Format::Format_ARGB32);
    for (int i = 0; i < Hnew; i++){
        for (int j = 0; j < Wnew; j++){
            QRgb temp = qimage[i*Wnew + j];
            out.setPixel(j, i, temp);
        }
    }

    finish = clock();
    ui->statusbar->showMessage("裁剪用时：" + QString::number(finish-start) + "ms");

    QString pfix = filename.last(4);
    filename.erase(filename.end()-4, filename.end());
    QPixmap pix = QPixmap::fromImage(out);
    pix.save(filename + "_cut" + pfix);
}
