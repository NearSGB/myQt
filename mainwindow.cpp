#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < camera_num; i++)
        cap[i].release();
    delete ui;
}

void MainWindow::onTimeout(){
    for(int i = 0; i < camera_num; i++)
        if(cap[i].isOpened()){
            cap[i]>>frame_temp;
            cv::resize(frame_temp, frame[i], Size(camera_w, camera_h));
        }

    hconcat(frame[0], frame[1], combine1);
    hconcat(frame[2], frame[3], combine2);
    vconcat(combine1, combine2, combineAll);


    QSize qs = ui->label_video->rect().size();
    ui->label_video->setScaledContents(true);
    QImage image = MatToQImage(combineAll);
    ui->label_video->setPixmap(QPixmap::fromImage(image));
    ui->label_video->repaint();
}


void MainWindow::on_pushButton_clicked()
{
    isRun = true;
    ui->textEdit->setTextColor(QColor("green"));
    ui->textEdit->setPlainText("  run");

    for(int i = 0; i < camera_num; i++){
        cap[i].open(i);
        frame[i] = Mat(Size(camera_w, camera_h), CV_8UC3, Scalar(0,0,0));
    }

    timer.start(333);

}


void MainWindow::on_pushButton_2_clicked()
{
    isRun = false;
    timer.stop();
    ui->textEdit->setTextColor(QColor("red"));
    ui->textEdit->setPlainText("  stop");
}


void MainWindow::on_exit_clicked()
{
    isRun = false;
    this->close();
}

void MainWindow::on_pushButton_3_clicked()
{

}
QImage MainWindow::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
//    if (mat.type() == CV_8UC1)
//    {
//        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
//        // Set the color table (used to translate colour indexes to qRgb values)
//        image.setColorCount(256);
//        for (int i = 0; i < 256; i++)
//        {
//            image.setColor(i, qRgb(i, i, i));
//        }
//        // Copy input Mat
//        uchar *pSrc = mat.data;
//        for (int row = 0; row < mat.rows; row++)
//        {
//            uchar *pDest = image.scanLine(row);
//            memcpy(pDest, pSrc, mat.cols);
//            pSrc += mat.step;
//        }
//        return image;
//    }
//    // 8-bits unsigned, NO. OF CHANNELS = 3
//    else
    if (mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else
    {
        return QImage();
    }
}
