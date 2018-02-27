#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include <QtWidgets/QMainWindow>
#include <string>
#include <windows.h>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <tchar.h>

using namespace cv;
using namespace std;
constexpr int camera_num = 4, interval = 60;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage  MatToQImage(const cv::Mat& mat);
private slots:
    void on_run_clicked();
    void onTimeout();
    void on_stop_clicked();

    void on_exit_clicked();

    void on_write_clicked();

private:
    char run[8] = {(char)1, (char)5, (char)0, (char)0, (char)0xff, (char)0, (char)0x8c, (char)0x3a};
    char stop[8] = {(char)1, (char)5, (char)0, (char)0, (char)0, (char)0, (char)0xcd, (char)0xca};
    DWORD dwBytesWrite = 8;
    Ui::MainWindow *ui;
    VideoCapture cap[camera_num]; //用来读取视频结构
    QTimer timer;//视频播放的定时器
    int camera_w = 1920, camera_h = 1080;
    bool isRun = false;
    bool isSaveImages = false;
    bool isSaveVideos = false;
    string str_images_save = "images/";
    string str_videos_save = "videos/";
    HANDLE hCom;
    int count_interval = 0;
    int timer_save_videos = 5000;
    VideoWriter writer[4];
    Mat frame[camera_num];
    Mat frame_temp, combine1, combine2, combineAll;
};

#endif // MAINWINDOW_H
