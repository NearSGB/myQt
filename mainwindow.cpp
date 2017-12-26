#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMessageBox>
#include <io.h>
#include <direct.h>
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
    if(isSaveVideos)
        count_interval++;
    for(int i = 0; i < camera_num; i++)
        if(cap[i].isOpened()){
            cap[i]>>frame_temp;
            if(isSaveImages){
                string s = str_images_save+to_string(time(0))+"_"+to_string(i)+".jpg";
                imwrite(s, frame_temp);
            }
            if(isSaveVideos){
                if(count_interval == timer_save_videos/interval){
                    string s = str_videos_save+to_string(time(0))+"_"+to_string(i)+".jpg";
                    imwrite(s, frame_temp);
                }
            }
            cv::resize(frame_temp, frame[i], Size(camera_w, camera_h));
        }
    if(count_interval == timer_save_videos/interval)
        count_interval = 0;
    isSaveImages = false;
    hconcat(frame[0], frame[1], combine1);
    hconcat(frame[2], frame[3], combine2);
    vconcat(combine1, combine2, combineAll);


    QSize qs = ui->label_video->rect().size();
    ui->label_video->setScaledContents(true);
    QImage image = MatToQImage(combineAll);
    ui->label_video->setPixmap(QPixmap::fromImage(image));
    ui->label_video->repaint();
}


void MainWindow::on_run_clicked()
{
    if(isRun) return;
    isRun = true;
    ui->text_isRun->setTextColor(QColor("green"));
    ui->text_isRun->setPlainText(QString("  工作"));

    for(int i = 0; i < camera_num; i++){
        cap[i].open(i);
        frame[i] = Mat(Size(camera_w, camera_h), CV_8UC3, Scalar(0,0,0));
    }

    timer.start(interval);

}


void MainWindow::on_stop_clicked()
{
    isRun = false;
    isSaveVideos = false;
    isSaveImages = false;
    timer.stop();
    for(int i = 0; i < camera_num; i++){
        if(cap[i].isOpened())
            cap[i].release();
//        if(writer[i].isOpened())
//            writer[i].release();
    }
    ui->text_isRun->setTextColor(QColor("red"));
    ui->text_isRun->setPlainText(QString("  停止"));
    ui->text_isSave->setPlainText(QString("未在保存"));
}


void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_write_clicked()
{
    if(!isRun)return;
    QString s1("确认需要保存图片吗？默认图片保存路径为./images以及./videos");
    QMessageBox message(QMessageBox::NoIcon, "保存确认", s1, QMessageBox::Yes|QMessageBox::No, NULL);
    if(message.exec() == QMessageBox::Yes){
        QString s = ui->text_timer_save_videos->toPlainText();
        int t = s.toInt();
        if(t < 1 || t > 100000){
            QMessageBox m(QMessageBox::NoIcon, "错误", "定时器单位为秒而您设置的结果为"+s+"秒不符合常理");
            m.exec();
            return;
        }
        timer_save_videos = 1000*t;
        if(_access(str_images_save.c_str(), 0) == -1)
            mkdir(str_images_save.c_str());
        if(_access(str_videos_save.c_str(), 0) == -1)
            mkdir(str_videos_save.c_str());
        isSaveImages = true;
        isSaveVideos = true;
        ui->text_isSave->setPlainText(QString("正在保存"));
    }

//    for(int i = 0; i < camera_num; i++){
//        if(cap[i].isOpened()){
//            string s = "E:/QtCode/videos/"+to_string(time(0))+"_"+to_string(i)+".avi";
//            if(!writer[i].isOpened())
//                writer[i] = VideoWriter(s, CV_FOURCC('M', 'J', 'P', 'G'), 24.0, Size(camera_w, camera_h));

//        }
//    }

}
QImage MainWindow::MatToQImage(const cv::Mat& mat)
{

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
