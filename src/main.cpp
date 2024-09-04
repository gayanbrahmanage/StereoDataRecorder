

// https://www.cnblogs.com/kevin-heyongyuan/articles/11070935.html


#include <iostream>
#include <stdio.h>
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
#include <fstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <inttypes.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <chrono>
#include <ctime>

//opencv include
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <common/mavlink.h>

using std::string;
using namespace std;

#include <common/mavlink.h>
//#include "autopilot_interface.h"
#include "serial_port.h"
#include "udp_port.h"


using namespace std;

#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_POSITION     0b0000110111111000
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_VELOCITY     0b0000110111000111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_ACCELERATION 0b0000110000111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_FORCE        0b0000111000111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_ANGLE    0b0000100111111111
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_RATE     0b0000010111111111

#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_TAKEOFF      0x1000
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_LAND         0x2000
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_LOITER       0x3000
#define MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_IDLE         0x4000

void mavlink(){

	mavlink_status_t status;
	mavlink_message_t msg;
	int chan = MAVLINK_COMM_0;
	// quit handler
	Generic_Port *port_quit;

	int system_id=0;
	int autopilot_id=0;
	int companion_id=0;

	/*
	As shown in the output of your ls command, /dev/ttyUSB0 and all similart tty device files all belong to dialout group.
	crw-rw-rw- 1 root dialout 188, 0 apr 2 09:23 /dev/ttyUSB0
	Thus you need to add yourself to that group via usemod command:
	sudo usermod -a -G dialout $USER

	The /dev directory is recreated at every boot, so your chmod vanishes.
	The better way is to determine the group ownership of the device withls -l, then add yourself to that group with sudo adduser $USER group
	Here's my expanded explanation:
	Open a "terminal window" by pressing Ctrl-Alt-T.
	Left Click to select this window.

	Explore. Type:
	ls -l /dev/ttyACM0
	man ls
	Fix. Type;

	sudo adduser $USER $(stat --format="%G" /dev/ttyACM0 )

	When prompted, type your login password.
	This will set things up so that your next (and all subsequent) login will have group access to /dev/ttyACM0.

	*/

  std::cout << "MAVLINK started..." << std::endl;
  int baudrate = 115200; //57600
  char *uart_name = (char*)"/dev/ttyACM0";

  Generic_Port *port;
  port = new Serial_Port(uart_name, baudrate);

  /*
   * Start the port and autopilot_interface
   * This is where the port is opened, and read and write threads are started.
   */


  port->start();

	int result;
	// --------------------------------------------------------------------------
	//   CHECK PORT
	// --------------------------------------------------------------------------
	if ( !port->is_running() ) // PORT_OPEN
	{
		fprintf(stderr,"ERROR: port not open\n");
		throw 1;
	}
	// --------------------------------------------------------------------------
	//   READ THREAD
	// --------------------------------------------------------------------------
	// Global Position
	mavlink_heartbeat_t heartbeat;
	mavlink_global_position_int_t global_position_int;
	mavlink_local_position_ned_t local_position_ned;
	mavlink_control_system_state_t control_system_state;
	mavlink_raw_imu_t raw_imu;

	int len1=-1;
	int len =-1;

	printf("START READ THREAD \n");
	while(1){

			mavlink_set_position_target_local_ned_t sp;
			sp.type_mask = MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_VELOCITY & MAVLINK_MSG_SET_POSITION_TARGET_LOCAL_NED_YAW_RATE;
			sp.coordinate_frame = MAV_FRAME_LOCAL_NED;
			sp.vx       = 0.0;
			sp.vy       = 0.0;
			sp.vz       = 0.0;
			sp.yaw_rate = 0.0;

			sp.target_system    = system_id;
			sp.target_component = autopilot_id;

			mavlink_message_t message_send;
			mavlink_msg_set_position_target_local_ned_encode(system_id, companion_id, &message_send, &sp);
			int len = port->write_message(message_send);

			//usleep(1000);   // Stream at 4Hz
			//std::cout<< len<<std::endl;
			//write_setpoint();

			mavlink_message_t message;
			bool success = port->read_message(message);

			//usleep(1000);
			system_id=(int)message.sysid;
			companion_id=(int)message.compid;
			//std::cout<<"message.sysid "<< (int)message.sysid<<"  message.compid "<<(int)message.compid<<std::endl;

			mavlink_message_t message_send2;
			mavlink_msg_heartbeat_pack(255, 125, &message_send2, 12, 3, 0,0,0);
			len = port->write_message(message_send2);
			//usleep(1000);

			if(success){
				//std::cout<< "success "<< success<< " message.sysid "<<message.sysid<<std::endl;
				switch (message.msgid){
					case MAVLINK_MSG_ID_HEARTBEAT:{
						mavlink_msg_heartbeat_decode(&message, &heartbeat);
						//printf("MAVLINK_MSG_ID_HEARTBEAT\n");
						//std::cout<<"heartbeat.type "<< heartbeat.type<<std::endl;
						break;
					}
					case MAVLINK_MSG_ID_SYS_STATUS:{
						//printf("MAVLINK_MSG_ID_SYS_STATUS\n");
						break;
					}

					case MAVLINK_MSG_ID_LOCAL_POSITION_NED:{
						printf("MAVLINK_MSG_ID_LOCAL_POSITION_NED\n");
						mavlink_msg_local_position_ned_decode(&message, &local_position_ned);

						std::cout<<"x "<<local_position_ned.x<< "y "<<local_position_ned.y<<"z "<<local_position_ned.z<<std::endl;
						break;
					}

					case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:{
						mavlink_msg_global_position_int_decode(&message, &global_position_int);
						//printf("MAVLINK_MSG_ID_GLOBAL_POSITION_INT\n");
						//std::cout<<"lat "<<global_position_int.lat<<std::endl;
						break;
					}

					case MAVLINK_MSG_ID_RAW_IMU:{
						mavlink_msg_raw_imu_decode(&message, &raw_imu);
						//printf("MAVLINK_MSG_ID_RAW_IMU\n");
						std::cout<<" raw_imu->xgyro  "<< raw_imu.xgyro <<std::endl;
						break;
					}
					case MAVLINK_MSG_ID_CONTROL_SYSTEM_STATE:{
						//mavlink_msg_raw_imu_decode(&message, &raw_imu);
						//printf("MAVLINK_MSG_ID_ODOMETRY\n");
						//std::cout<<" raw_imu->xgyro  "<< raw_imu.xgyro <<std::endl;
						break;
					}
					case MAVLINK_MSG_ID_GPS_STATUS:{
						//mavlink_msg_raw_imu_decode(&message, &raw_imu);
						printf("MAVLINK_MSG_ID_GPS_STATUS\n");
						//std::cout<<" raw_imu->xgyro  "<< raw_imu.xgyro <<std::endl;
						break;
					}



				}

				if(len1<0){
					// Prepare command for off-board mode
					std::cout<<" Prepare command for off-board mode "<<std::endl;
					bool flag=true;
					mavlink_command_long_t com = { 0 };
					com.target_system    = system_id;
					com.target_component = autopilot_id;
					com.command          = MAV_CMD_NAV_GUIDED_ENABLE;
					com.confirmation     = true;
					com.param1           = (float) flag; // flag >0.5 => start, <0.5 => stop
					// Encode
					mavlink_message_t message1;
					mavlink_msg_command_long_encode(system_id, companion_id, &message1, &com);

					// Send the message
					len1 = port->write_message(message1);
					usleep(100);

					// Prepare command for off-board mode
					mavlink_command_long_t com2 = { 0 };
					com2.target_system    = system_id;
					com2.target_component = autopilot_id;
					com2.command          = MAV_CMD_DO_SET_HOME ;
					com2.confirmation     = true;
					com2.param1           = (float) flag; // flag >0.5 => start, <0.5 => stop

					// Encode

					mavlink_msg_command_long_encode(system_id, companion_id, &message1, &com2);
					// Send the message
					int len = port->write_message(message1);
					usleep(100);

					uint8_t req_stream_id=6;

					//mavlink_msg_request_data_stream_pack(system_id, companion_id, &message1,system_id,autopilot_id, req_stream_id, 4, 1);
					mavlink_msg_request_data_stream_pack(255, 125, &message1, system_id, autopilot_id, MAV_DATA_STREAM_ALL, 30, 1);
					len1 = port->write_message(message1);
					usleep(100);
				}
			}

 	}

	// --------------------------------------------------------------------------
	//   CHECK FOR MESSAGES
	// --------------------------------------------------------------------------


	printf("CHECK FOR MESSAGES\n");


  port->stop();

  delete port;

}

void calibrate(std::string cam){
  std::cout << "Calibrating.." << std::endl;

  cv::Mat cameraMatrix,distCoeffs,R,T;
  // Defining the dimensions of checkerboard
  int CHECKERBOARD[2]{8,6};
  // Creating vector to store vectors of 3D points for each checkerboard image
  std::vector<std::vector<cv::Point3f> > objpoints;
  // Creating vector to store vectors of 2D points for each checkerboard image
  std::vector<std::vector<cv::Point2f> > imgpoints;
  // Defining the world coordinates for 3D points
  std::vector<cv::Point3f> objp;

  float cube_size=75.0f;

  for(int i{0}; i<CHECKERBOARD[1]; i++){
    for(int j{0}; j<CHECKERBOARD[0]; j++){
      objp.push_back(cv::Point3f(j*cube_size,i*cube_size,0));
    }
  }

  // Extracting path of individual image stored in a given directory
  std::vector<cv::String> images;
  // Path of the folder containing checkerboard images
  std::stringstream p;
  p<<"../../ImageDATA/stereo/calibration_data/"<<cam<<"/*.jpg";
  std::string path = p.str();

  cv::glob(path, images);
  cv::Mat frame, gray;
  // vector to store the pixel coordinates of detected checker board corners
  std::vector<cv::Point2f> corner_pts;
  bool success;
  // Looping over all the images in the directory
  for(int i{0}; i<images.size(); i++){

    frame = cv::imread(images[i]);
    cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);
    // Finding checker board corners
    // If desired number of corners are found in the image then success = true
    success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),
    corner_pts, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);

    /*
     * If desired number of corner are detected,
     * we refine the pixel coordinates and display
     * them on the images of checker board
    */
    if(success){
      cv::TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);
      // refining pixel coordinates for given 2d points.
      cv::cornerSubPix(gray,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);
      // Displaying the detected corner points on the checker board
      cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);

      objpoints.push_back(objp);
      imgpoints.push_back(corner_pts);
			cv::imwrite("../../out/out"+cam+"_img_"+std::to_string(i)+".jpg",frame );
			cv::imshow("Image",frame);
			cv::waitKey(10);
    }



  }
  /*
   * Performing camera calibration by
   * passing the value of known 3D points (objpoints)
   * and corresponding pixel coordinates of the
   * detected corners (imgpoints)
  */
  double error=cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T);
  std:;cout<< " Error "<<error<<std::endl;
  std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
  std::cout << "distCoeffs : " << distCoeffs << std::endl;
  //std::cout << "Rotation vector : " << R << std::endl;
  //std::cout << "Translation vector : " << T << std::endl;
}

int show_camera(){
  // 1.  Open the device
  int fd; // A file descriptor to the video device


  fd = open("/dev/video0",O_RDWR); // Read Write capability
  if(fd < 0){
      perror("Failed to open device, OPEN");
      return 1;
  }

  // 2. Ask the device if it can capture frames
  v4l2_capability capability;
  if(ioctl(fd, VIDIOC_QUERYCAP, &capability) < 0){
      // something went wrong... exit
      perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
      return 1;
  }
  std::cout<< " "<< capability.bus_info<<std::endl;

  // 3. Set Image format
  v4l2_format imageFormat;
  imageFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  imageFormat.fmt.pix.width = 1280;
  imageFormat.fmt.pix.height = 720;
  imageFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  imageFormat.fmt.pix.field = V4L2_FIELD_NONE;
  // tell the device you are using this format
  if(ioctl(fd, VIDIOC_S_FMT, &imageFormat) < 0){
      perror("Device could not set format, VIDIOC_S_FMT");
      return 1;
  }


  // 4. Request Buffers from the device
  v4l2_requestbuffers requestBuffer = {0};
  requestBuffer.count = 1; // one request buffer
  requestBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // request a buffer wich we an use for capturing frames
  requestBuffer.memory = V4L2_MEMORY_MMAP;

  if(ioctl(fd, VIDIOC_REQBUFS, &requestBuffer) < 0){
      perror("Could not request buffer from device, VIDIOC_REQBUFS");
      return 1;
  }


  // 5. Quety the buffer to get raw data ie. ask for the you requested buffer
  // and allocate memory for it
  v4l2_buffer queryBuffer = {0};
  queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  queryBuffer.memory = V4L2_MEMORY_MMAP;
  queryBuffer.index = 0;
  if(ioctl(fd, VIDIOC_QUERYBUF, &queryBuffer) < 0){
      perror("Device did not return the buffer information, VIDIOC_QUERYBUF");
      return 1;
  }
  // use a pointer to point to the newly created buffer
  // mmap() will map the memory address of the device to
  // an address in memory
  unsigned char* buffer = (unsigned char*)mmap(NULL, queryBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED,fd, queryBuffer.m.offset);
  memset(buffer, 0, queryBuffer.length);


  // 6. Get a frame
  // Create a new buffer type so the device knows whichbuffer we are talking about
  v4l2_buffer bufferinfo;
  memset(&bufferinfo, 0, sizeof(bufferinfo));
  bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  bufferinfo.memory = V4L2_MEMORY_MMAP;
  bufferinfo.index = 0;

  // Activate streaming
  int type = bufferinfo.type;
  if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
      perror("Could not start streaming, VIDIOC_STREAMON");
      return 1;
  }

/***************************** Begin looping here *********************/
while(1){
  // Queue the buffer
  if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
      perror("Could not queue buffer, VIDIOC_QBUF");
      return 1;
  }

  // Dequeue the buffer
  if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
      perror("Could not dequeue the buffer, VIDIOC_DQBUF");
      return 1;
  }

  //unsigned char* temp=(unsigned char* )malloc(1280*720*3);

  //std::cout<<"queryBuffer.length "<<queryBuffer.length<<std::endl;

  unsigned char* temp=(unsigned char*)calloc(queryBuffer.length, sizeof(unsigned char));
  memcpy(temp, buffer, queryBuffer.length);
  //
  // printf ("\n== %s ==", temp);

  cv::Mat cvmat = cv::Mat(1280, 720, CV_8UC3, (void*)temp);
  cv::Mat decodedMat= cv::imdecode(cvmat,cv::IMREAD_COLOR);
  ///std::cout<< cvmat.at<uchar>(10,10)<<std::endl;
  //


  cv::imshow("Display window", decodedMat);
  cv::waitKey(10);

}

/******************************** end looping here **********************/

  // end streaming
  if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
      perror("Could not end streaming, VIDIOC_STREAMOFF");
      return 1;
  }

  close(fd);
}

static int xioctl(int fd, int request, void *arg)
{
    int r;
        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);
        return r;
}

void show_camera_opencv(int cam_no){

	int width=1280;
	int height=720;
	double fps_set=30;

	std::stringstream camera_name;
	camera_name<< "/dev/video"<<cam_no;

	struct v4l2_capability capability={};
	int fd = open(camera_name.str().c_str(),O_RDWR); // Read Write capability
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
	int usb_port_no=int(busInfo_[strlen(busInfo_)-1])-48;
	std::cout<< " "<< capability.bus_info<< " USB Prot Number --> "<< usb_port_no<< std::endl;

	close(fd);

	cv::VideoCapture cap(cam_no,cv::CAP_V4L);
	if (!cap.isOpened()) {
		std::cout << "cannot open camera"<<std::endl;
	}else{
		std::cout << cap.getBackendName()<<std::endl;
	}
	cap.set(cv::CAP_PROP_FRAME_WIDTH,width);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT,height);
	cap.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
	cap.set(cv::CAP_PROP_FPS,100);
	double fps = cap.get(cv::CAP_PROP_FPS);
	std::cout <<"fps "<< fps<< " " <<cap.get(cv::CAP_PROP_FRAME_WIDTH)<<"x"<<cap.get(cv::CAP_PROP_FRAME_HEIGHT)<<" "<<cap.get(cv::CAP_PROP_FOURCC) <<std::endl;

	//cap.set(CAP_PROP_MODE,CAP_MODE_RGB);

	cv::Mat image;
	std::chrono::system_clock::time_point start= std::chrono::high_resolution_clock::now();
	int image_no=0;
	while (true) {
		cap >> image;
		image_no++;
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		float buffer_time=elapsed_seconds.count();
		//std::cout<<"fps "<<image_no/buffer_time<<std::endl;

		cv::imshow("Display window", image);
	  cv::waitKey(1);
	}

}

int main(int argc, char * argv[]) {

    //calibrate("cam1");
    //calibrate("cam2");
		//show_camera();
    //mavlink();
		int cam_no=atoi(argv[1]);
		show_camera_opencv(cam_no);

    return 0;
}
