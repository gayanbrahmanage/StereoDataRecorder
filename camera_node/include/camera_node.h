#ifndef camera_node_H
#define camera_node_H

#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <thread>
#include "viz_utils.h"
#include <chrono>
#include <ctime>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <trigger.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

class camera{
public:

  int cam_no=-1;
  clock_t t;
  int fd=-1; // A file descriptor to the video device
  struct v4l2_capability capability={};
  struct v4l2_format imageFormat={};
  struct v4l2_requestbuffers requestBuffer = {};
  struct v4l2_buffer queryBuffer = {};
  struct v4l2_buffer bufferinfo={};
  struct v4l2_control control;
  struct v4l2_buffer buf = {0};


  int type=-1;
  unsigned char* buffer;
  float buffer_time;
  int width=1280;
  int height=800;
  int channels=3;
  int top_trim=0;
  int bot_trim=0;
  int image_num_s=0;
  int image_num_m=0;
  int fps;
  std::chrono::system_clock::time_point start;
  std::string image_format;
  std::string image_type;
  int JPEG_q=70;
  int PNG_COMPRESSION=1;
  int frames_per_video=5000;
  bool images_or_video=true;

  double fx, fy,cx,cy;
  std::vector<double> distcoeffs{0,0,0,0,0};
  double re_error=0;

  bool save_command=true;

  int V4L2_CID_BRIGHTNESS_val=0;
  int V4L2_CID_CONTRAST_val=32;
  int V4L2_CID_SHARPNESS_val=3;
  int V4L2_CID_SATURATION_val=64;
  int V4L2_CID_HUE_val=0;
  bool V4L2_CID_AUTO_WHITE_BALANCE_val=true;
  int V4L2_CID_GAMMA_val=100;
  int V4L2_CID_GAIN_val=0;
  int V4L2_CID_BACKLIGHT_COMPENSATION_val=1;
  int V4L2_CID_WHITE_BALANCE_TEMPERATURE_val=4600;
  int V4L2_CID_EXPOSURE_ABSOLUTE_val=100;

  bool use_opencv_cap=true;
  cv::VideoCapture cap;
  cv::Mat cv_image;
  float writing_duration;


  camera();
  ~camera();
  int init(int);
  int set_params(int, int, int, int, int, std::string);
  int start_stream();
  int stop_stream();
  int get_data(bool&, bool&, bool&);
  int run(bool&, bool&, bool&,bool&,std::string &,int&);


  int get_sync_data(bool&, bool&, bool&, bool&);
  void save_image_s(bool&, bool&, const std::string & , int&, int&, bool&);
  void save_image_m(bool&, bool&, const std::string & , int&, int&, bool&);
  void save_video_s(bool&, bool&, const std::string & , int&, int&, bool&);
  void save_video_m(bool&, bool&, const std::string & , int&, int&, bool&);
  void save_image(bool&, const std::string & ,const std::string &, int&);
  void capture_image(bool&, cv::Mat& );
  bool calibrate(std::string );
  int terminate();


};

class camera_node{
public:
  bool add_module=false;
  bool initialized=false;
  clock_t t;
  camera* cam=new camera;
  int image_num=0;
  std::string folder_path;
  int saving_fps=0;
  //Module FLAGS
  bool module_enabled = false;
  bool buffer_flag=true;
  bool writing_data=true;
  bool reading_data=false;
  bool running=false;
  bool ready=false;
  bool camera_show_EN=false;
  bool record_images_EN=false;
  bool images_or_video=true;


  unsigned char* image_data; // if stbi image is used
  cv::Mat cv_image; // if opencv is used

  //int width=1920; //v4l2-ctl -d /dev/video0 --list-formats-ext
  //int height=1080;

  int width=1280; //Monochrome
  int height=800;

  float image_file_size=450; //KB
  int top_trim=150;
  int bot_trim=190;
  std::string image_format="jpg";
  int channels=3;

  camera_node();
  ~camera_node();
  int init();
  void run();

};

class stereo_node{

public:
  std::chrono::system_clock::time_point start;
  bool add_module=false;
  bool initialized=false;
  clock_t t;
  camera* cam1=new camera;
  camera* cam2=new camera;


  int image_num=0;
  int image_num2=0;
  int image_by_image_num=0;
  int image_by_image_num2=0;
  std::string folder_path;
  int saving_fps=0;
  int saving_fps2=0;
  double fx, fy,cx,cy,b;

  bool module_enabled = false;
  bool camera_show_EN=false;
  bool st_sync_flag=false;
  bool record_images_EN=false;
  bool record_image_by_image_EN=false;
  bool recording=false;
  bool running=false;
  bool buffer1_flag=true;
  bool buffer2_flag=true;
  bool ready1=false;
  bool ready2=false;
  bool ready=false;
  bool left_right_flipped=false;
  bool show_trimmed=false;

  unsigned char* image_data1; // if stbi image is used
  unsigned char* image_data2; // if stbi image is used
  cv::Mat cv_image1, cv_image1_original; // if opencv is used
  cv::Mat cv_image2, cv_image2_original; // if opencv is used

  // int width=640;//1920; //v4l2-ctl -d /dev/video0 --list-formats-ext
  // int height=480;//1080;
  //int width=1920; //v4l2-ctl -d /dev/video0 --list-formats-ext
  //int height=1080;

  int width=1280; //monochrome
  int height=800;

  int top_trim=200;
  int bot_trim=200;
  float image_file_size=450; //KB
  int channels=3;
  std::string image_format="jpg";
  bool images_or_video=true;
  int JPEG_q=100;
  int PNG_COMPRESSION=0;
  bool image_type_gray=true;
  bool image_type_color=false;
  bool image_format_jpg=false;
  bool image_format_png=true;
  bool right_click_recording_EN=true;
  bool right_click_pressed=false;
  bool calibration_mod_on=false;
  bool valid_for_calibration=false;
  bool automatic_saving_for_calibartion=false;
  bool camera_type_Mono_color=true;



  stereo_node();
  ~stereo_node();
  int init();
  void find_checkboard_patterns(cv::Mat&,cv::Mat&, bool&);
  void save_valid_images();
  int calibrate();
  void save_image();
  void run();


};

class stereo_node_tripple{

public:


  std::chrono::system_clock::time_point start;
  bool add_module=false;
  bool initialized=false;
  clock_t t;
  camera* cam1=new camera;
  camera* cam2=new camera;
  camera* cam3=new camera;
  camera* cam4=new camera;


  int image_num1=0;
  int image_num2=0;
  int image_num3=0;
  int image_num4=0;

  int image_by_image_num=0;
  int image_by_image_num2=0;
  int image_by_image_num3=0;
  int image_by_image_num4=0;

  std::string folder_path;
  int saving_fps=0;
  int saving_fps2=0;
  int saving_fps3=0;
  int saving_fps4=0;

  double fx, fy,cx,cy,b;

  bool module_enabled = false;
  bool camera_show_EN=false;
  bool st_sync_flag=false;
  bool record_images_EN=false;
  bool record_image_by_image_EN=false;
  bool recording=false;
  bool running=false;
  bool buffer1_flag=true;
  bool buffer2_flag=true;
  bool buffer3_flag=true;
  bool buffer4_flag=true;
  bool ready1=false;
  bool ready2=false;
  bool ready=false;
  bool show_trimmed=false;

  unsigned char* image_data1; // if stbi image is used
  unsigned char* image_data2; // if stbi image is used
  unsigned char* image_data3; // if stbi image is used
  unsigned char* image_data4; // if stbi image is used
  cv::Mat cv_image1, cv_image1_original; // if opencv is used
  cv::Mat cv_image2, cv_image2_original; // if opencv is used
  cv::Mat cv_image3, cv_image3_original; // if opencv is used
  cv::Mat cv_image4, cv_image4_original; // if opencv is used

  // int width=640;//1920; //v4l2-ctl -d /dev/video0 --list-formats-ext
  // int height=480;//1080;
  //int width=1920; //v4l2-ctl -d /dev/video0 --list-formats-ext
  //int height=1080;

  int width=1280; //monochrome
  int height=800;

  int top_trim=0;
  int bot_trim=100;
  float image_file_size=450; //KB
  int channels=3;
  std::string image_format="jpg";
  bool images_or_video=true;
  int JPEG_q=100;
  int PNG_COMPRESSION=0;
  bool image_type_gray=true;
  bool image_type_color=false;
  bool image_format_jpg=false;
  bool image_format_png=false;
  bool image_format_bmp=true;
  bool right_click_recording_EN=true;
  bool right_click_pressed=false;
  bool camera_type_Mono_color=true;



  stereo_node_tripple();
  ~stereo_node_tripple();
  int init();
  void save_image();
  void run();
  void sync_thread();


};

#endif
