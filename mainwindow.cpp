#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    isRun = true;
    ui->textEdit->setTextColor(QColor("green"));
    ui->textEdit->setPlainText("run");
    const int camera_num = 4;
    const int camera_w = 800, camera_h = 450;
    VideoCapture cap[camera_num];
    Mat frame[camera_num], frame_temp;
    Mat combine1, combine2, combineAll;
    string windowname = "video";
    namedWindow(windowname, WINDOW_NORMAL);
    moveWindow(windowname,300, 0);
    for(int i = 0; i < camera_num; i++){
        cap[i].open(i);
        frame[i] = Mat(Size(camera_w, camera_h), CV_8UC3, Scalar(0,0,0));
    }
    while(isRun)
    {
        for(int i = 0; i < camera_num; i++)
            if(cap[i].isOpened()){
                cap[i]>>frame_temp;
                cv::resize(frame_temp, frame[i], Size(camera_w, camera_h));
            }

        hconcat(frame[0], frame[1], combine1);
        hconcat(frame[2], frame[3], combine2);
        vconcat(combine1, combine2, combineAll);
        imshow(windowname, combineAll);
        waitKey(500);
    }
    for(int i = 0; i < camera_num; i++)
        cap[i].release();
    destroyAllWindows();
}


void MainWindow::on_pushButton_2_clicked()
{
    isRun = false;
    ui->textEdit->setTextColor(QColor("red"));
    ui->textEdit->setPlainText("stop");
}


void MainWindow::on_exit_clicked()
{
    isRun = false;
    this->close();
}

void MainWindow::on_pushButton_3_clicked()
{

}
