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
#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    //QImage  MatToQImage(const cv::Mat& mat);
    void on_runButton_clicked();
    //void onTimeout();
    void on_stopButton_clicked();
    void on_exitButton_clicked();
    void onSaveTimeout();
    void show_videos();
private:
    void read_config();
    bool readConfig(string configFile);
    //bool readConfigFile(const char * cfgfilepath, const string & key, string & value);
    void release_writer_cap();

    char run[8] = {(char)1, (char)5, (char)0, (char)0, (char)0xff, (char)0, (char)0x8c, (char)0x3a};
    char stop[8] = {(char)1, (char)5, (char)0, (char)0, (char)0, (char)0, (char)0xcd, (char)0xca};
    DWORD dwBytesWrite = 8;
    Ui::MainWindow *ui;
    vector<VideoCapture> cap; //用来读取视频结构
    vector<VideoWriter> writer;//用来保存视频结构
    vector<string> window_names;
    vector<Mat> frame;
    QTimer show_timer;//视频播放的定时器
    QTimer save_timer;
    vector<int> camera_id_list;
    vector<vector<int>> camera_roi;

    vector<int> camera_roi_all;

    int time_interval = 50;
    int camera_w = 960;
    int camera_h = 540;
    bool isRun = false;
    bool isSaveVideos = false;
    string str_images_save = "images/";
    string str_videos_save = "videos/";
    //HANDLE hCom;
    int save_videos_time = 2000;
    Mat frame_tmp, combine1, combine2, combineAll;
    void getCamera_roi_all(unordered_map<string, string> mem);
    void getCamera_roi_i(unordered_map<string, string> mem, int i);
};

#endif // MAINWINDOW_H
