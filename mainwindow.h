#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include <QtWidgets/QMainWindow>
#include <QPaintEvent>
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>
using namespace cv;

constexpr int camera_num = 4;
constexpr int camera_w = 800, camera_h = 450;

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
    void on_pushButton_clicked();
    void onTimeout();
    void on_pushButton_2_clicked();

    void on_exit_clicked();

    void on_pushButton_3_clicked();

private:

    Ui::MainWindow *ui;
    VideoCapture cap[camera_num]; //用来读取视频结构
    QTimer timer;//视频播放的定时器
    bool isRun = true;
    Mat frame[camera_num];
    Mat frame_temp, combine1, combine2, combineAll;
};

#endif // MAINWINDOW_H
