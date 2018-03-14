#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMessageBox>
#include <io.h>
#include <direct.h>
#include <cstring>
#include <unordered_map>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&save_timer, SIGNAL(timeout()), this, SLOT(onSaveTimeout()));
    connect(&show_timer, SIGNAL(timeout()), this, SLOT(show_videos()));
//    hCom = CreateFile(TEXT("COM3"),GENERIC_READ | GENERIC_WRITE,0, NULL,OPEN_EXISTING, 0, NULL);
//    DCB dcb;
//    GetCommState(hCom, &dcb);
//    dcb.BaudRate = 9600;
//    dcb.ByteSize = 8;
//    dcb.Parity = NOPARITY;
//    dcb.StopBits = ONESTOPBIT;
//    SetCommState(hCom, &dcb);
//    PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
}

MainWindow::~MainWindow()
{
//    if(hCom != (HANDLE)-1)
//        CloseHandle(hCom);
    release_writer_cap();
    delete ui;
}
void MainWindow::release_writer_cap(){
    for(int i = (int)cap.size()-1; i >= 0; i--){
        if(cap[i].isOpened()) {
            cap[i].release();
            cap.pop_back();
        }
        if(i < writer.size() && writer[i].isOpened()) {
            writer[i].release();
            writer.pop_back();
        }
    }
    camera_id_list.clear();
    camera_roi_all.clear();
    camera_roi.clear();
    frame.clear();
    window_names.clear();
    ui->configTextEdit->clear();
    destroyAllWindows();
}

void MainWindow::onSaveTimeout(){
    for(int i = 0; i < writer.size(); i++)
        if(cap[i].isOpened()){
            //cv::resize(frame[i], frame_temp, Size(camera_w, camera_h));
            if(isSaveVideos){
                string s = str_images_save+to_string(time(0))+"_"+to_string(i)+".jpg";
                imwrite(s, frame[i]);
            }
        }
//    hconcat(frame[0], frame[1], combine1);
//    hconcat(frame[2], frame[3], combine2);
//    vconcat(combine1, combine2, combineAll);
//    QSize qs = ui->show_faults_label->rect().size();
//    ui->show_faults_label->setScaledContents (true);
//    QImage image = MatToQImage(combineAll);
//    ui->show_faults_label->setPixmap(QPixmap::fromImage(image));
//    ui->show_faults_label->repaint();
}

void MainWindow::show_videos(){
    //use this determine statement to decide whether stop the machine when we click the red 'X' on the top of the opencv imshow window
    if(!cvGetWindowHandle("all")) {
        on_stopButton_clicked();
        return;
    }
    for(int i = 0; i < cap.size(); i++)
        if(cap[i].isOpened()) {
            cap[i]>>frame[i];
            cv::resize(frame[i], frame[i], Size(camera_w, camera_h));
            int row = frame[i].rows;
            int col = frame[i].cols;
            Rect rect(col*camera_roi[i][0]/100, row*camera_roi[i][1]/100, col*(camera_roi[i][2]-camera_roi[i][0])/100,
                    row*(camera_roi[i][3]-camera_roi[i][1])/100);
/*  rect 用法
 * rect(a, b, c, d) -- tl (a, b) br(a+c, b+d)
*/
            if(i == 0)
                combineAll = frame[i](rect);
            else
                hconcat(combineAll, frame[i](rect), combineAll);
            //imshow(window_names[i], frame[i](rect));
            if(isSaveVideos)
                writer[i].write(frame[i]);
        }
    imshow("all", combineAll);

}

void MainWindow::on_runButton_clicked()
{
    //WriteFile(hCom, run, dwBytesWrite, &dwBytesWrite, NULL);
    if(isRun) return;
    if(readConfig("camera.config") == false)
        return;
    isRun = true;

    cap = vector<VideoCapture>(camera_id_list.size());
    frame = vector<Mat>(camera_id_list.size());
    window_names = vector<string>(camera_id_list.size());

    if(isSaveVideos) {
        writer = vector<VideoWriter>(camera_id_list.size());
        if(_access(str_images_save.c_str(), 0) == -1)
            mkdir(str_images_save.c_str());
        if(_access(str_videos_save.c_str(), 0) == -1)
            mkdir(str_videos_save.c_str());

    }
    for(int i = 0; i < camera_id_list.size(); i++){
        cap[i].open(camera_id_list[i]);
        if(cap[i].isOpened()) {
            window_names[i] = to_string(camera_id_list[i])+"_camera";
            //namedWindow(window_names[i], WINDOW_NORMAL);
            if(isSaveVideos) {
                string s = str_videos_save + to_string(time(0))+"_"+to_string(camera_id_list[i])+".avi";
                writer[i] = VideoWriter(s, CV_FOURCC('M', 'J', 'P', 'G'), 20.0, Size(camera_w, camera_h));
            }
        }
    }
    namedWindow("all", CV_WINDOW_NORMAL);
    if(isSaveVideos)
        save_timer.start(save_videos_time);
    show_timer.start(time_interval);

}




void MainWindow::on_stopButton_clicked()
{
    if(isRun == false)
        return;
    //WriteFile(hCom, stop, dwBytesWrite, &dwBytesWrite, NULL);
    isRun = false;
    if(isSaveVideos)
        save_timer.stop();
    show_timer.stop();
    release_writer_cap();
}


void MainWindow::on_exitButton_clicked()
{
    this->close();
}



void MainWindow::getCamera_roi_all(unordered_map<string, string> mem)
{
    if(mem.find("roi_left_top_x_all") != mem.end())
        camera_roi_all.push_back(stoi(mem["roi_left_top_x_all"]));
    else
        camera_roi_all.push_back(0);
    if(mem.find("roi_left_top_y_all") != mem.end())
        camera_roi_all.push_back(stoi(mem["roi_left_top_y_all"]));
    else
        camera_roi_all.push_back(0);
    if(mem.find("roi_right_bottom_x_all") != mem.end())
        camera_roi_all.push_back(stoi(mem["roi_right_bottom_x_all"]));
    else
        camera_roi_all.push_back(100);
    if(mem.find("roi_right_bottom_y_all") != mem.end())
        camera_roi_all.push_back(stoi(mem["roi_right_bottom_y_all"]));
    else
        camera_roi_all.push_back(100);
}

void MainWindow::getCamera_roi_i(unordered_map<string, string> mem, int i)
{
    vector<int> tmp;
    if(mem.find("roi_left_top_x_"+to_string(i)) != mem.end())
        tmp.push_back(stoi(mem["roi_left_top_x_"+to_string(i)]));
    else
        tmp.push_back(camera_roi_all[0]);
    if(mem.find("roi_left_top_y_"+to_string(i)) != mem.end())
        tmp.push_back(stoi(mem["roi_left_top_y_"+to_string(i)]));
    else
        tmp.push_back(camera_roi_all[1]);
    if(mem.find("roi_right_bottom_x_"+to_string(i)) != mem.end())
        tmp.push_back(stoi(mem["roi_right_bottom_x_"+to_string(i)]));
    else
        tmp.push_back(camera_roi_all[2]);
    if(mem.find("roi_right_bottom_y_"+to_string(i)) != mem.end())
        tmp.push_back(stoi(mem["roi_right_bottom_y_"+to_string(i)]));
    else
        tmp.push_back(camera_roi_all[3]);
    camera_roi.emplace_back(tmp);
}

bool MainWindow::readConfig(string configFile) {
    fstream cfgFile(configFile);
    if(!cfgFile.is_open()) {
        QMessageBox m(QMessageBox::NoIcon, "错误", "未能载入配置文件");
        m.exec();
        return false;
    }
    release_writer_cap();
    unordered_map<string, string> mem;
    string line;
    while(!cfgFile.eof())//循环读取每一行
    {
        getline(cfgFile, line);
        if(line.empty() || line.front() == '#')
            continue;
        ui->configTextEdit->append(QString::fromStdString(line));
        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value
        string tmpKey = line.substr(0,pos);//取=号之前
        mem[tmpKey] = line.substr(pos+1);//取=号之后
    }

    if(mem.find("camera_id_list") != mem.end()) {
        int start = 0;
        string s = mem["camera_id_list"];
        int i = 0;
        while(i < s.size()) {
            while(s[i] != ',')
                i++;
            camera_id_list.push_back(stoi(s.substr(start, i-start)));
            start = ++i;
        }
        if(start < s.size())
            camera_id_list.push_back(stoi(s.substr(start)));
    }

    getCamera_roi_all(mem);

    for(int i : camera_id_list) {
        getCamera_roi_i(mem, i);
    }


    if(mem.find("isSaveVideos") != mem.end())
        if(mem["isSaveVideos"] == "true")
            isSaveVideos = true;
        else
            isSaveVideos = false;
    if(mem.find("save_videos_time") != mem.end())
        save_videos_time = stoi(mem["save_videos_time"]);

    if(mem.find("camera_w") != mem.end())
        camera_w = stoi(mem["camera_w"]);
    if(mem.find("camera_h") != mem.end())
        camera_h = stoi(mem["camera_h"]);

    if(mem.find("time_interval") != mem.end())
        time_interval = stoi(mem["time_interval"]);
    return true;
}

//bool MainWindow::readConfigFile_key_value(const char * cfgfilepath, const string & key, string & value)
//{
//    fstream cfgFile;
//    cfgFile.open(cfgfilepath);//打开文件
//    char tmp[100];
//    while(!cfgFile.eof())//循环读取每一行
//    {
//        cfgFile.getline(tmp,100);//每行读取前200个字符，200个应该足够了
//        string line(tmp);
//        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value
//        string tmpKey = line.substr(0,pos);//取=号之前
//        if(key==tmpKey)
//        {
//            value = line.substr(pos+1);//取=号之后
//            return true;
//        }
//    }
//    return false;
//}

//QImage MainWindow::MatToQImage(const cv::Mat& mat)
//{

//    if (mat.type() == CV_8UC3)
//    {
//        // Copy input Mat
//        const uchar *pSrc = (const uchar*)mat.data;
//        // Create QImage with same dimensions as input Mat
//        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
//        return image.rgbSwapped();
//    }
//    else
//    {
//        return QImage();
//    }
//}
