#include "camera_node.h"

static int xioctl(int fd, int request, void *arg)
{
    int r;
        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);
        return r;
}


camera::camera(){

}

camera::~camera(){
}

int camera::run(bool& module_enabled,
                bool& running,
                bool& buffer_flag,
                bool& record_images_EN,
                std::string & folder_path,
                int& image_num_saved){

  //
  int cropped_width=width;
  int cropped_height=height-bot_trim-top_trim;
  cv::VideoWriter oVideoWriter;

  if(!use_opencv_cap){
    start_stream();
  }

  start= std::chrono::high_resolution_clock::now();
  image_num_saved=0;
  bool saving_started=false;
  std::stringstream file_name;
  std::ofstream timefile;

  while(module_enabled){

    if(record_images_EN){
      if(!saving_started){
        file_name.str("");
        file_name<< folder_path<<"/cam"<< cam_no<<"time.txt";
        timefile.open (file_name.str().c_str());
        saving_started=true;
        start= std::chrono::high_resolution_clock::now();
        //std::cout<<"saving cam "<< cam_no<<" " <<file_name.str().c_str()<<std::endl;
        if(images_or_video){

        }else{
          std::stringstream video_file_name;
          video_file_name<< folder_path<<"/cam"<<cam_no<<"/cam.avi";
          oVideoWriter=cv::VideoWriter(video_file_name.str().c_str(),cv::VideoWriter::fourcc('p', 'n', 'g', ' '),30, cv::Size(cropped_width,cropped_height), true);

          if (oVideoWriter.isOpened() == false){
            std::cout << "Cannot save the video to a file" << std::endl;
          }
        }
      }

      if(save_command){

        std::chrono::system_clock::time_point start_writing = std::chrono::high_resolution_clock::now();
        std::stringstream image_name;
        cv::Mat decodedMat;

        if(use_opencv_cap){

          cap >> decodedMat;
          auto end = std::chrono::high_resolution_clock::now();
          std::chrono::duration<double> elapsed_seconds = end-start;
          buffer_time=elapsed_seconds.count();

          std::stringstream line;
          line<<image_num_saved<<" "<<buffer_time<< " \n";
          timefile<<line.str().c_str();

        }else{
              if(xioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
                  perror("Could not queue buffer, VIDIOC_QBUF");
                  return -1;
              }

              fd_set fds;
              FD_ZERO(&fds);
              FD_SET(fd, &fds);
              struct timeval tv = {0};
              tv.tv_sec = 0.04;
              int r = select(fd+1, &fds, NULL, NULL, &tv);

              if(-1 == r){
                  perror("Waiting for Frame");
                  return 1;
              }

              // Dequeue the buffer
              if(xioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
                  perror("Could not dequeue the buffer, VIDIOC_DQBUF");
                  return -1;
              }

              auto end = std::chrono::high_resolution_clock::now();
              std::chrono::duration<double> elapsed_seconds = end-start;
              buffer_time=elapsed_seconds.count();

              std::stringstream line;
              line<<image_num_saved<<" "<<buffer_time<< " \n";
              timefile<<line.str().c_str();

              cv_image = cv::Mat(width, height, CV_8UC3, buffer);

              if(image_type=="color"){
                  decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
              }else if(image_type=="gray"){
                  if(cam_no==4){
                    decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
                  }else{
                    decodedMat= cv::imdecode(cv_image,cv::IMREAD_GRAYSCALE);
                  }
              }
          }
          // save to disk

          cv::Mat cropped_image;

          if(top_trim>0 || bot_trim>0){
              cropped_image=decodedMat(cv::Rect(cv::Point(0,top_trim),cv::Point(width, height-bot_trim)));
          }else{
              cropped_image=decodedMat.clone();
          }
          //cv::putText(cropped_image, std::to_string (buffer_time),cv::Point(10, cropped_height-10),  cv::FONT_HERSHEY_SCRIPT_SIMPLEX,1,cv::Scalar(118, 185, 0), 2);

          if(images_or_video){
            if(image_format=="jpg"){
                image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num_saved<<".jpg";
                cv::imwrite(image_name.str().c_str(), cropped_image, {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, JPEG_q});
            }else if(image_format=="png"){
                image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num_saved<<".png";
                //cv::imwrite(image_name.str().c_str(), cropped_image, {cv::ImwriteFlags::IMWRITE_PNG_COMPRESSION, PNG_COMPRESSION});
                cv::imwrite(image_name.str().c_str(), cropped_image, {cv::ImwritePNGFlags::IMWRITE_PNG_STRATEGY_DEFAULT});
            }else if(image_format=="BMP"){
              image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num_saved<<".BMP";
              cv::imwrite(image_name.str().c_str(), cropped_image);
            }
          }else{
            oVideoWriter.write(cropped_image);
          }



          image_num_saved++;
          save_command=false;

          std::chrono::system_clock::time_point end_writing = std::chrono::high_resolution_clock::now();
          std::chrono::duration<double> writing_seconds = end_writing-start_writing;
          writing_duration=writing_seconds.count();
          if(writing_duration>0.05){
            std::cout<<" Writing time "<<cam_no<<" "<<writing_duration<<std::endl;
          }


      }

    }else{
        if(saving_started){
          timefile.close();
          saving_started=false;
          oVideoWriter.release();
          //std::cout<<"stop saving cam "<< cam_no<<std::endl;
        }
        image_num_saved=0;
        if(buffer_flag){

          if(use_opencv_cap){
            cap >> cv_image;
            cvtColor( cv_image, cv_image, cv::COLOR_BGR2RGB );

            //std::cout<<"cv_image "<<cv_image.size() << "  "<<cv_image.channels() <<std::endl;
          }else{

              if(xioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
                  perror("Could not queue buffer, VIDIOC_QBUF");
                  return -1;
              }

              fd_set fds;
              FD_ZERO(&fds);
              FD_SET(fd, &fds);
              struct timeval tv = {0};
              tv.tv_sec = 0.04;
              int r = select(fd+1, &fds, NULL, NULL, &tv);

              if(-1 == r){
                  perror("Waiting for Frame");
                  return 1;
              }

              // Dequeue the buffer
              if(xioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
                  perror("Could not dequeue the buffer, VIDIOC_DQBUF");
                  return -1;
              }
           }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;
            buffer_time=elapsed_seconds.count();
            buffer_flag=false;
        }
      }
  }

  if(!use_opencv_cap){
    stop_stream();
  }
  running=false;
}



bool camera::calibrate(std::string cam){


}

int camera::init(int n){

  int usb_port_no=-1;

  if(use_opencv_cap){

    std::stringstream camera_name;
  	camera_name<< "/dev/video"<<n;
  	fd = open(camera_name.str().c_str(),O_RDWR); // Read Write capability
  	if(fd < 0){
  			perror("Failed to open device, OPEN");
  	}
  	// 2. Ask the device if it can capture frames

  	if(xioctl(fd, VIDIOC_QUERYCAP, &capability) < 0){
  			// something went wrong... exit
  			perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
  	}

  	__u8* busInfo=capability.bus_info;
  	const char* busInfo_=(char*) busInfo;
  	usb_port_no=int(busInfo_[strlen(busInfo_)-1])-48;
    if(usb_port_no==1)usb_port_no=1;
    else if(usb_port_no==2)usb_port_no=2;
    else if(usb_port_no==8)usb_port_no=3;
    else if(usb_port_no==5)usb_port_no=4;

  	std::cout<<n<< "  connected to  "<< capability.bus_info<< " USB Prot Number --> "<< usb_port_no<< std::endl;
    close(fd);

    cap=cv::VideoCapture(n,cv::CAP_V4L2);
    if (!cap.isOpened()) {
      std::cout << "cannot open camera"<<std::endl;
    }

    //settings
    std::stringstream camera_config_file_path;
    camera_config_file_path<<"../camera_node/bin/cam"<<usb_port_no<<".txt";
    std::ifstream camera_file (camera_config_file_path.str().c_str());

    if (camera_file.is_open()){
      std::string line;
      while ( getline (camera_file,line)){
        std::stringstream ss(line);
        std::vector<std::string> str_list;

        while (ss){
          std::string s;
          if (!getline( ss, s, '=' )) break;
          str_list.push_back( s );
        }
        //std::cout<<str_list[0]<<"  "<<str_list[1]<<std::endl;
        if (str_list[0]=="V4L2_CID_BRIGHTNESS_val")V4L2_CID_BRIGHTNESS_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_CONTRAST_val")V4L2_CID_CONTRAST_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_SHARPNESS_val")V4L2_CID_SHARPNESS_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_SATURATION_val")V4L2_CID_SATURATION_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_HUE_val")V4L2_CID_HUE_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_GAMMA_val")V4L2_CID_GAMMA_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_GAIN_val")V4L2_CID_GAIN_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_BACKLIGHT_COMPENSATION_val")V4L2_CID_BACKLIGHT_COMPENSATION_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_WHITE_BALANCE_TEMPERATURE_val")V4L2_CID_WHITE_BALANCE_TEMPERATURE_val=stoi(str_list[1]);
        else if (str_list[0]=="V4L2_CID_EXPOSURE_ABSOLUTE_val")V4L2_CID_EXPOSURE_ABSOLUTE_val=stoi(str_list[1]);
      }
       camera_file.close();
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH,width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,height);
    // cap.set(cv::CAP_PROP_BRIGHTNESS,V4L2_CID_BRIGHTNESS_val);
    // cap.set(cv::CAP_PROP_CONTRAST,V4L2_CID_CONTRAST_val);
    // cap.set(cv::CAP_PROP_SATURATION,V4L2_CID_SATURATION_val);
    // cap.set(cv::CAP_PROP_HUE,V4L2_CID_HUE_val);
    // cap.set(cv::CAP_PROP_GAIN,V4L2_CID_GAIN_val);
    // cap.set(cv::CAP_PROP_EXPOSURE,V4L2_CID_EXPOSURE_ABSOLUTE_val);
    // cap.set(cv::CAP_PROP_SHARPNESS,V4L2_CID_SHARPNESS_val);
    // cap.set(cv::CAP_PROP_GAMMA,V4L2_CID_GAMMA_val);

    cap.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FPS,120);
    //cap.set(CV_CAP_PROP_MODE,cv::CAP_MODE_RGB);

    double fps = cap.get(cv::CAP_PROP_FPS);
    std::cout <<"CAP_PROP_FPS  "<< cap.get(cv::CAP_PROP_FPS)<<std::endl;

  }else{
      // v4l2-ctl --list-devices
      std::stringstream camera_name;
      camera_name<< "/dev/video"<<n;
      fd = open(camera_name.str().c_str(),O_RDWR); // Read Write capability
      if(fd < 0){
          perror("Failed to open device, OPEN");
          return -1;
      }

      // 2. Ask the device if it can capture frames

      if(xioctl(fd, VIDIOC_QUERYCAP, &capability) < 0){
          // something went wrong... exit
          perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
          return -1;
      }

      __u8* busInfo=capability.bus_info;
      const char* busInfo_=(char*) busInfo;
      usb_port_no=int(busInfo_[strlen(busInfo_)-1])-48;
      std::cout<< " "<< capability.bus_info<< " USB Prot Number --> "<< usb_port_no<< std::endl;

      std::stringstream camera_config_file_path;
      camera_config_file_path<<"../camera_node/bin/cam"<<usb_port_no<<".txt";
      std::ifstream camera_file (camera_config_file_path.str().c_str());

      if (camera_file.is_open()){
        std::string line;
        while ( getline (camera_file,line)){
          std::stringstream ss(line);
          std::vector<std::string> str_list;

          while (ss){
            std::string s;
            if (!getline( ss, s, '=' )) break;
            str_list.push_back( s );
          }
          std::cout<<str_list[0]<<"  "<<str_list[1]<<std::endl;
          if (str_list[0]=="V4L2_CID_BRIGHTNESS_val")V4L2_CID_BRIGHTNESS_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_CONTRAST_val")V4L2_CID_CONTRAST_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_SHARPNESS_val")V4L2_CID_SHARPNESS_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_SATURATION_val")V4L2_CID_SATURATION_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_HUE_val")V4L2_CID_HUE_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_GAMMA_val")V4L2_CID_GAMMA_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_GAIN_val")V4L2_CID_GAIN_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_BACKLIGHT_COMPENSATION_val")V4L2_CID_BACKLIGHT_COMPENSATION_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_WHITE_BALANCE_TEMPERATURE_val")V4L2_CID_WHITE_BALANCE_TEMPERATURE_val=stoi(str_list[1]);
          else if (str_list[0]=="V4L2_CID_EXPOSURE_ABSOLUTE_val")V4L2_CID_EXPOSURE_ABSOLUTE_val=stoi(str_list[1]);
        }
         camera_file.close();
      }

      //std::cout<<" here "<<std::endl;

      //printf("{ Capability: driver:'%s', card:'%s',buf_info:'%s',version:%d,capabilities:0x%x}\n",\
    			capability.driver,capability.card,capability.bus_info,capability.version,capability.capabilities);
      //setting up
      //v4l2-ctl -d /dev/video0 -l


      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_BRIGHTNESS;//V4L2_CID_GAMMA;//V4L2_CID_WHITE_BALANCE_TEMPERATURE;//V4L2_CID_BRIGHTNESS;//V4L2_CID_CONTRAST;//V4L2_CID_SHARPNESS;
      control.value=V4L2_CID_BRIGHTNESS_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_BRIGHTNESS Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_CONTRAST;
      control.value=V4L2_CID_CONTRAST_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_CONTRAST Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_SHARPNESS;
      control.value=V4L2_CID_SHARPNESS_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_SHARPNESS Could not request buffer from device, VIDIOC_G_CTRL");

        }

    ///////////////////////////////////////////////////////////
      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_SATURATION;
      control.value=V4L2_CID_SATURATION_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_SATURATION Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_HUE;
      control.value=V4L2_CID_HUE_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_HUE Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_AUTO_WHITE_BALANCE;
      control.value=1;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("VIDIOC_S_CTRL Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_GAMMA;
      control.value=V4L2_CID_GAMMA_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_GAMMA Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_GAIN;
      control.value=V4L2_CID_GAIN_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_GAIN Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_BACKLIGHT_COMPENSATION;
      control.value=V4L2_CID_BACKLIGHT_COMPENSATION_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_BACKLIGHT_COMPENSATION Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;
      control.value=V4L2_CID_WHITE_BALANCE_TEMPERATURE_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_WHITE_BALANCE_TEMPERATURE Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_POWER_LINE_FREQUENCY;
      control.value=2;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_POWER_LINE_FREQUENCY Could not request buffer from device, VIDIOC_G_CTRL");

        }

      // enum  v4l2_colorfx{
      // V4L2_COLORFX_NONE = 0 , V4L2_COLORFX_BW = 1 , V4L2_COLORFX_SEPIA = 2 , V4L2_COLORFX_NEGATIVE = 3 ,
      // V4L2_COLORFX_EMBOSS = 4 , V4L2_COLORFX_SKETCH = 5 , V4L2_COLORFX_SKY_BLUE = 6 , V4L2_COLORFX_GRASS_GREEN = 7 ,
      // V4L2_COLORFX_SKIN_WHITEN = 8 , V4L2_COLORFX_VIVID = 9 , V4L2_COLORFX_AQUA = 10 , V4L2_COLORFX_ART_FREEZE = 11 ,
      // V4L2_COLORFX_SILHOUETTE = 12 , V4L2_COLORFX_SOLARIZATION = 13 , V4L2_COLORFX_ANTIQUE = 14 , V4L2_COLORFX_SET_CBCR = 15 ,
      // V4L2_COLORFX_PASTEL = 16
      // };
      //
      // for(int i=V4L2_COLORFX_NONE; i<V4L2_COLORFX_PASTEL; i++){
      //
      // }


    ///////////////////////////////////////////////////////////////////////////////

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_EXPOSURE_AUTO;
      control.value=3;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_EXPOSURE_AUTO Could not request buffer from device, VIDIOC_G_CTRL");

        }

      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_EXPOSURE_ABSOLUTE;
      control.value=V4L2_CID_EXPOSURE_ABSOLUTE_val;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_EXPOSURE_ABSOLUTE Could not request buffer from device, VIDIOC_G_CTRL");

        }


      memset(&control, 0, sizeof(control));
      control.id = V4L2_CID_EXPOSURE_AUTO_PRIORITY;
      control.value=1;
        if(xioctl(fd, VIDIOC_S_CTRL, &control) < 0){
            perror("V4L2_CID_EXPOSURE_AUTO_PRIORITY Could not request buffer from device, VIDIOC_G_CTRL");

        }
  }
  return usb_port_no;

}

int camera::set_params(int w, int h, int cam_num, int top_t, int bot_t, std::string img_format){

  width=w;
  height=h;
  top_trim=top_t;
  bot_trim=bot_t;
  cam_no=cam_num;
  image_format=img_format;

  // load parameters
  std::stringstream calib_file_name;
  calib_file_name<<"../camera_node/calibration/cam"<<cam_num <<".txt";
  std::ifstream califile (calib_file_name.str().c_str());
  std::string line;
  if (califile.is_open()){
    while(getline(califile,line)){
      std::stringstream ss(line);
      std::vector<std::string> str_list;

      while (ss){
        std::string s;
        if (!getline( ss, s, ' ' )) break;
        str_list.push_back( s );
      }

      if (str_list[0]=="fx")fx=std::stof(str_list[1]);
      else if (str_list[0]=="fy")fy=std::stof(str_list[1]);
      else if (str_list[0]=="cx")cx=std::stof(str_list[1]);
      else if (str_list[0]=="cy")cy=std::stof(str_list[1]);
      else if (str_list[0]=="d1")distcoeffs[0]=std::stof(str_list[1]);
      else if (str_list[0]=="d2")distcoeffs[1]=std::stof(str_list[1]);
      else if (str_list[0]=="d3")distcoeffs[2]=std::stof(str_list[1]);
      else if (str_list[0]=="d4")distcoeffs[3]=std::stof(str_list[1]);
      else if (str_list[0]=="d5")distcoeffs[4]=std::stof(str_list[1]);
      else if (str_list[0]=="E")re_error=std::stof(str_list[1]);
    }
    califile.close();
  }

  if(use_opencv_cap){

  }else{
      CLEAR(imageFormat);
      imageFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      imageFormat.fmt.pix.width = width;
      imageFormat.fmt.pix.height = height;
      imageFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
      imageFormat.fmt.pix.field = V4L2_FIELD_NONE;
      // tell the device you are using this format
      if(xioctl(fd, VIDIOC_S_FMT, &imageFormat) < 0){
          perror("Device could not set format, VIDIOC_S_FMT");
          return -1;
      }


      struct v4l2_streamparm *setfps;
      setfps = (struct v4l2_streamparm *)calloc(1,sizeof(struct v4l2_streamparm));
      memset(setfps,0,sizeof(struct v4l2_streamparm));
      setfps->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      setfps->parm.capture.capability=V4L2_CAP_TIMEPERFRAME;
      setfps->parm.capture.timeperframe.numerator = 1;
      setfps->parm.capture.timeperframe.denominator = 120;

      setfps->parm.output.capability=V4L2_CAP_TIMEPERFRAME;
      setfps->parm.output.timeperframe.numerator = 1;
      setfps->parm.output.timeperframe.denominator = 120;

      if(ioctl(fd, VIDIOC_S_PARM,setfps) < 0)
      {
        perror("Device could not set fps VIDIOC_S_PARM");
      }

      // 4. Request Buffers from the device
      CLEAR(requestBuffer);
      requestBuffer.count = 1; // one request buffer
      requestBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // request a buffer wich we an use for capturing frames
      requestBuffer.memory = V4L2_MEMORY_MMAP;

      if(xioctl(fd, VIDIOC_REQBUFS, &requestBuffer) < 0){
          perror("Could not request buffer from device, VIDIOC_REQBUFS");
          return -1;
      }


      // 5. Quety the buffer to get raw data ie. ask for the you requested buffer
      // and allocate memory for it

      CLEAR(queryBuffer);
      queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      queryBuffer.memory = V4L2_MEMORY_MMAP;
      queryBuffer.index = 0;
      if(xioctl(fd, VIDIOC_QUERYBUF, &queryBuffer) < 0){
          perror("Device did not return the buffer information, VIDIOC_QUERYBUF");
          return -1;
      }


      // use a pointer to point to the newly created buffer
      // mmap() will map the memory address of the device to
      // an address in memory
      CLEAR(buffer);
      buffer = (unsigned char*)mmap(NULL, queryBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED,fd, queryBuffer.m.offset);
      memset(buffer, 0, queryBuffer.length);


      // 6. Get a frame
      // Create a new buffer type so the device knows whichbuffer we are talking about
      CLEAR(bufferinfo);
      memset(&bufferinfo, 0, sizeof(bufferinfo));
      bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      bufferinfo.memory = V4L2_MEMORY_MMAP;
      bufferinfo.index = 0;
    }

}


int camera::start_stream(){
    // Activate streaming
    type = bufferinfo.type;
    if(xioctl(fd, VIDIOC_STREAMON, &type) < 0){
        perror("Could not start streaming, VIDIOC_STREAMON");
        return -1;
    }
}

int camera::stop_stream(){

  type = bufferinfo.type;
  // end streaming
  if(xioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
      perror("Could not end streaming, VIDIOC_STREAMOFF");
      return -1;
  }

  std::cout<< "Stopped Camera running "<< glfwGetTime()<<std::endl;
  return 0;
}


int camera::get_data(bool& module_enabled, bool& running, bool& buffer_flag){
  start_stream();
  start= std::chrono::high_resolution_clock::now();
  while(module_enabled){

    if(buffer_flag){

        if(xioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
            perror("Could not queue buffer, VIDIOC_QBUF");
            return -1;
        }

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        struct timeval tv = {0};
        tv.tv_sec = 0.04;
        int r = select(fd+1, &fds, NULL, NULL, &tv);

        if(-1 == r){
            perror("Waiting for Frame");
            return 1;
        }

        // Dequeue the buffer
        if(xioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
            perror("Could not dequeue the buffer, VIDIOC_DQBUF");
            return -1;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        buffer_time=elapsed_seconds.count();
        buffer_flag=false;

    }
  }

  stop_stream();
  running=false;

}

int camera::get_sync_data(bool& module_enabled, bool& running, bool& buffer_flag, bool& st_sync_flag){
  start_stream();
  start= std::chrono::high_resolution_clock::now();
  while(module_enabled){

    if(!st_sync_flag){
      start= std::chrono::high_resolution_clock::now();
      //std::cout<< "start "<<std::endl;
    }

    if(buffer_flag){

        if(xioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
            perror("Could not queue buffer, VIDIOC_QBUF");
            return -1;
        }

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        struct timeval tv = {0};
        tv.tv_sec = 0.04;
        int r = select(fd+1, &fds, NULL, NULL, &tv);

        if(-1 == r){
            perror("Waiting for Frame");
            return 1;
        }

        // Dequeue the buffer
        if(xioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
            perror("Could not dequeue the buffer, VIDIOC_DQBUF");
            return -1;
        }

        std::chrono::system_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        buffer_time=elapsed_seconds.count();
        //std::cout<<cam_no<<" "<<buffer_time<<std::endl;
        buffer_flag=false;

    }
  }

  stop_stream();
  running=false;

}


void camera::save_video_m(bool& record_images_EN, bool& buffer_flag,const std::string & folder_path, int& image_num, int& image_num2, bool&st_sync_flag){

  int cropped_width=width;
  int cropped_height=height-bot_trim-top_trim;

  std::stringstream file_name;
  file_name<< folder_path<<"/time.txt";
  std::ofstream timefile;
  timefile.open (file_name.str().c_str());


  int video_num=1;
  float buff_time=0;
  std::stringstream video_file_name;
  video_file_name<< folder_path<<"/cam"<<cam_no<<"/cam_video"<< video_num<<".avi";
  cv::VideoWriter oVideoWriter=cv::VideoWriter(video_file_name.str().c_str(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                30, cv::Size(cropped_width,cropped_height), true);

  if (oVideoWriter.isOpened() == false){
    std::cout << "Cannot save the video to a file" << std::endl;
  }

  while(record_images_EN){

    if(st_sync_flag){
        if(!buffer_flag /*writing to buffer is stopped*/ ){

          if(image_num<=image_num2){
            image_num_m++;
            image_num=image_num_m;
          }else{
            buffer_flag=true;
            continue;
          }

          //using opencv image container
          // read image from buffer
          cv::Mat cv_image = cv::Mat(width, height, CV_8UC3, buffer);

          // save to disk
          std::stringstream image_name;
          cv::Mat decodedMat;

          if(image_type=="color"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
          }else if(image_type=="gray"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_GRAYSCALE);
          }

          buff_time=buffer_time;
          // rleased buffer after image is copied
          // let camera to capture next image

          buffer_flag=true;

          cv::Mat cropped_image;

          if(top_trim>0 || bot_trim>0){
              cropped_image=decodedMat(cv::Rect(cv::Point(0,top_trim),cv::Point(width, height-bot_trim)));
          }else{
              cropped_image=cropped_image.clone();
          }
          cv::putText(cropped_image, std::to_string (buff_time),cv::Point(10, cropped_height-10),  cv::FONT_HERSHEY_DUPLEX,1.0,cv::Scalar(118, 185, 0), 2);

          // saving goes here
          oVideoWriter.write(cropped_image);

          std::stringstream line;
          line<<image_num_m<<" "<<buff_time<< " \n";
          timefile<<line.str().c_str();

          fps=image_num_m/buff_time;

          // braek the video
          if(image_num_m%frames_per_video==0){
            oVideoWriter.release();
            video_num++;
            std::stringstream video_file_name;
            video_file_name<< folder_path<<"/cam"<<cam_no<<"/cam_video"<< video_num<<".avi";
            oVideoWriter=cv::VideoWriter(video_file_name.str().c_str(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                          30, cv::Size(cropped_width,cropped_height), true);

            if (oVideoWriter.isOpened() == false){
              std::cout << "Cannot save the video to a file" << std::endl;
            }
          }

        }
      }else{
        buffer_flag=true;
      }
  }

  oVideoWriter.release();
  image_num_m=0;
  image_num=0;

  timefile.close();
}

void camera::save_video_s(bool& record_images_EN, bool& buffer_flag,const std::string & folder_path, int& image_num, int& image_num2, bool& st_sync_flag){

  int cropped_width=width;
  int cropped_height=height-bot_trim-top_trim;
  float buff_time=0;
  std::stringstream file_name;
  file_name<< folder_path<<"/time2.txt";
  std::ofstream timefile;
  timefile.open (file_name.str().c_str());

  int video_num=1;
  std::stringstream video_file_name;
  video_file_name<< folder_path<<"/cam"<<cam_no<<"/cam_video"<< video_num<<".avi";
  cv::VideoWriter oVideoWriter=cv::VideoWriter(video_file_name.str().c_str(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                               30, cv::Size(cropped_width,cropped_height), true);
  if (oVideoWriter.isOpened() == false){
    std::cout << "Cannot save the video to a file" << std::endl;
  }

  while(record_images_EN){

    if(st_sync_flag){
        if(!buffer_flag /*writing to buffer is stopped*/){

          if(image_num2<=image_num){
            image_num_s++;
            image_num2=image_num_s;
          }else{
            buffer_flag=true;
            continue;
          }

          //using opencv image container
          // read image from buffer
          cv::Mat cv_image = cv::Mat(width, height, CV_8UC3, buffer);

          // save to disk
          std::stringstream image_name;
          cv::Mat decodedMat;

          if(image_type=="color"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
          }else if(image_type=="gray"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_GRAYSCALE);
          }

          buff_time=buffer_time;

          buffer_flag=true;

          cv::Mat cropped_image;
          if(top_trim>0 || bot_trim>0){
              cropped_image=decodedMat(cv::Rect(cv::Point(0,top_trim),cv::Point(width, height-bot_trim)));
          }else{
            cropped_image=decodedMat.clone();
          }

          cv::putText(cropped_image, std::to_string (buff_time),cv::Point(10, cropped_height-10),  cv::FONT_HERSHEY_DUPLEX,1.0,cv::Scalar(118, 185, 0), 2);

          // saving goes here
          oVideoWriter.write(cropped_image);

          fps=image_num_s/buff_time;

          std::stringstream line;
          line<<image_num_s<<" "<<buff_time<< " \n";
          timefile<<line.str().c_str();

          // braek the video
          if(image_num_s%frames_per_video==0){
            oVideoWriter.release();
            video_num++;
            std::stringstream video_file_name;
            video_file_name<< folder_path<<"/cam"<<cam_no<<"/cam_video"<< video_num<<".avi";

            oVideoWriter=cv::VideoWriter(video_file_name.str().c_str(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                          30, cv::Size(cropped_width,cropped_height), true);

            if (oVideoWriter.isOpened() == false){
              std::cout << "Cannot save the video to a file" << std::endl;
            }
          }

        }
    }else{
      buffer_flag=true;
    }
  }
  oVideoWriter.release();
  image_num_s=0;
  image_num2=0;
  timefile.close();
}


void camera::save_image_m(bool& record_images_EN, bool& buffer_flag,const std::string & folder_path, int& image_num, int& image_num2, bool&st_sync_flag){

  std::stringstream file_name;
  file_name<< folder_path<<"/time.txt";
  std::ofstream timefile;
  timefile.open (file_name.str().c_str());

  auto start = std::chrono::system_clock::now();
  auto temp = std::chrono::system_clock::now();

  while(record_images_EN){
    if(st_sync_flag){
        if(!buffer_flag /*writing to buffer is stopped*/){

          // image_num_m++;
          // image_num=image_num_m;

          if(image_num<=image_num2){
            image_num_m++;
            image_num=image_num_m;
          }else{
            buffer_flag=true;
            continue;
          }

          //using opencv image container
          // read image from buffer
          cv::Mat cv_image = cv::Mat(width, height, CV_8UC3, buffer);

          // save to disk
          std::stringstream image_name;
          cv::Mat decodedMat;

          if(image_type=="color"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
          }else if(image_type=="gray"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_GRAYSCALE);
          }

          // rleased buffer after image is copied
          // let camera to capture next image

          buffer_flag=true;

          cv::Mat cropped_image;
          if(top_trim>0 || bot_trim>0){
              cropped_image=decodedMat(cv::Rect(cv::Point(0,top_trim),cv::Point(width, height-bot_trim)));
          }else{
              cropped_image=decodedMat.clone();
          }

          auto t = std::chrono::system_clock::now();
          if(image_format=="jpg"){
              image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num_m<<".jpg";
              cv::imwrite(image_name.str().c_str(), cropped_image, {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, JPEG_q});
          }else if(image_format=="png"){
              image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num_m<<".png";
              cv::imwrite(image_name.str().c_str(), cropped_image, {cv::ImwriteFlags::IMWRITE_PNG_COMPRESSION, PNG_COMPRESSION});
          }

          std::chrono::duration<double> elapsed=std::chrono::system_clock::now()-t;

          auto end = std::chrono::system_clock::now();
          std::chrono::duration<double> elapsed_seconds = end-start;

          std::chrono::duration<double> elapsed_secondstemp = end-temp;
          if(elapsed_secondstemp.count()>0.08){
            std::cout<<image_num_m<<"m "<<elapsed_secondstemp.count()<<" Saving time "<< elapsed.count()<<std::endl;
          }
          temp=end;

          std::stringstream line;
          line<<image_num_m<<" "<<elapsed_seconds.count()<< " \n";
          timefile<<line.str().c_str();

          fps=image_num/elapsed_seconds.count();

        }
    }else{
      buffer_flag=true;
    }
  }
  image_num_m=0;
  image_num=0;
  image_num2=0;
  timefile.close();
}

void camera::save_image_s(bool& record_images_EN, bool& buffer_flag,const std::string & folder_path, int& image_num, int& image_num2, bool&st_sync_flag){

  std::stringstream file_name;
  file_name<< folder_path<<"/time2.txt";
  std::ofstream timefile;
  timefile.open (file_name.str().c_str());

  auto start = std::chrono::system_clock::now();
  auto temp = std::chrono::system_clock::now();

  while(record_images_EN){
    if(st_sync_flag){
          if(!buffer_flag /*writing to buffer is stopped*/){

            //image_num_s++;

            if(image_num2<=image_num){
              image_num_s++;
              image_num2=image_num_s;
            }else{
              buffer_flag=true;
              continue;
            }

            //using opencv image container
            // read image from buffer
            cv::Mat cv_image = cv::Mat(width, height, CV_8UC3, buffer);

            // save to disk
            std::stringstream image_name;
            cv::Mat decodedMat;

            if(image_type=="color"){
                decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
            }else if(image_type=="gray"){
                decodedMat= cv::imdecode(cv_image,cv::IMREAD_GRAYSCALE);
            }

            buffer_flag=true;


            cv::Mat cropped_image;
            if(top_trim>0 || bot_trim>0){
                cropped_image=decodedMat(cv::Rect(cv::Point(0,top_trim),cv::Point(width, height-bot_trim)));
            }else{
              cropped_image=decodedMat.clone();
            }

            auto t = std::chrono::system_clock::now();
            if(image_format=="jpg"){
                image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num_s<<".jpg";
                cv::imwrite(image_name.str().c_str(), cropped_image, {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, JPEG_q});
            }else if(image_format=="png"){
                image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num_s<<".png";
                cv::imwrite(image_name.str().c_str(), cropped_image, {cv::ImwriteFlags::IMWRITE_PNG_COMPRESSION, PNG_COMPRESSION});
            }
            std::chrono::duration<double> elapsed=std::chrono::system_clock::now()-t;

            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;

            std::chrono::duration<double> elapsed_secondstemp = end-temp;
            if(elapsed_secondstemp.count()>0.08){
              std::cout<<image_num_s<< "s "<<elapsed_secondstemp.count()<< " Saving time "<< elapsed.count()<<std::endl;
            }
            temp=end;


            fps=image_num/elapsed_seconds.count();

            std::stringstream line;
            line<<image_num_s<<" "<<elapsed_seconds.count()<< " \n";
            timefile<<line.str().c_str();

          }
      }else{
        buffer_flag=true;
      }
  }
  image_num_s=0;
  image_num=0;
  image_num2=0;
  timefile.close();
}

void camera::save_image(bool& buffer_flag, const std::string & folder_path , const std::string & cam, int& image_num ){

  if(!buffer_flag /*writing to buffer is stopped*/){
      //using opencv image container
      // read image from buffer
      cv::Mat decodedMat;
      std::stringstream image_name;

      if(use_opencv_cap){
          decodedMat=cv_image.clone();
      }else{
          cv::Mat cv_image = cv::Mat(width, height, CV_8UC3, buffer);
          // save to disk


          if(image_type=="color"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
          }else if(image_type=="gray"){
              if(cam_no==4){
                decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
              }else{
                decodedMat= cv::imdecode(cv_image,cv::IMREAD_GRAYSCALE);
              }
          }
        }

      // rleased buffer after image is copied
      // let camera to capture next image
      buffer_flag=true;

      if(image_format=="jpg"){
          image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num<<".jpg";
          cv::imwrite(image_name.str().c_str(), decodedMat, {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, JPEG_q});
      }else if(image_format=="png"){
          image_name<< folder_path<<"/cam"<<cam_no<<"/img"<<image_num<<".png";
          cv::imwrite(image_name.str().c_str(), decodedMat, {cv::ImwriteFlags::IMWRITE_PNG_COMPRESSION, PNG_COMPRESSION});
      }


  }
}

void camera::capture_image(bool& buffer_flag, cv::Mat& decodedMat ){
  if(!buffer_flag /*writing to buffer is stopped*/){
    //using opencv image container
    // read image from buffer
    cv::Mat cv_image = cv::Mat(width, height, CV_8UC3, buffer);

    try{
          if(image_type=="color"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);
          }else if(image_type=="gray"){
              decodedMat= cv::imdecode(cv_image,cv::IMREAD_GRAYSCALE);
          }

          // rleased buffer after image is copied
          // let camera to capture next image
          buffer_flag=true;

    }catch (std::runtime_error& ex) {
          fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
    }

  }
}

int camera::terminate(){
  close(fd);
}
