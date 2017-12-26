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
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>
using namespace cv;
using namespace std;
constexpr int camera_num = 4, interval = 60;
constexpr int camera_w = 960, camera_h = 540;

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

    Ui::MainWindow *ui;
    VideoCapture cap[camera_num]; //用来读取视频结构
    QTimer timer;//视频播放的定时器
    bool isRun = false;
    bool isSaveImages = false;
    bool isSaveVideos = false;
    string str_images_save = "images/";
    string str_videos_save = "videos/";
    int count_interval = 0;
    int timer_save_videos = 5000;
    VideoWriter writer[4];
    Mat frame[camera_num];
    Mat frame_temp, combine1, combine2, combineAll;
};

#endif // MAINWINDOW_H
