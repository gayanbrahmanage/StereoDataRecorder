#ifndef mavlink_node_H
#define mavlink_node_H

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
#include <chrono>
#include <ctime>

#include "serial_port.h"
#include "udp_port.h"
#include <common/mavlink.h>

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

struct mavlink_message{

  int system_id;
  int companion_id;
  mavlink_heartbeat_t heartbeat;
  mavlink_global_position_int_t global_position_int;
  mavlink_local_position_ned_t local_position_ned;
  mavlink_gps_raw_int_t gps_raw_int;
  mavlink_raw_imu_t raw_imu;
  mavlink_scaled_imu2_t 	scaled_imu2;
  mavlink_scaled_imu3_t scaled_imu3;
  mavlink_attitude_t attitude;


  std::chrono::duration<double> elapsed_seconds_heartbeat;
  std::chrono::duration<double> elapsed_seconds_global_position_int;
  std::chrono::duration<double> elapsed_seconds_local_position_ned;
  std::chrono::duration<double> elapsed_seconds_gps_raw_int;
  std::chrono::duration<double> elapsed_seconds_raw_imu;
  std::chrono::duration<double> elapsed_seconds_scaled_imu2;
  std::chrono::duration<double> elapsed_seconds_scaled_imu3;
  std::chrono::duration<double> elapsed_seconds_attitude;

};


class mavlink_node{
public:

  bool add_module=false;
  bool initialized=false;
  //threading FLAGS
  bool reading_flag=true;
  bool writing_flag=false;
  std::ofstream gtfile;
  std::ofstream gpsfile;
  std::ofstream gps_raw;
  std::ofstream imu_raw;
  std::ofstream imu2_scaled;
  std::ofstream imu3_scaled;
  std::ofstream attitude;

  bool mavlink_EN=false;
  bool running=false;
  bool connected=false;
  bool offboardmodEN=false;
  bool data_stream_rq_sent=false;
  bool set_home=false;
  bool setarmed=false;
  bool data_save_EN=false;

  std::string folder_path;

  mavlink_status_t status;
  mavlink_message_t msg;
  int chan = MAVLINK_COMM_0;
  // quit handler
  Generic_Port *port_quit;

  int APsystem_id=0;
  int autopilot_id=0;
  mavlink_heartbeat_t APheartbeat;

  int system_id=255;
  int companion_id=125;



  int baudrate = 57600;
  char *uart_name;
  Generic_Port *port;

  mavlink_message current_message_R; // recieving message
  mavlink_message current_message_S; // sending message
  mavlink_message current_message;
  mavlink_message current_msg;
  int aquision_rate=50;

  std::chrono::system_clock::time_point start;

  mavlink_node();
  ~mavlink_node();
  void run();
  bool init();
  void terminate();
  void read_data();
  void write_data();

  int send_heartbeat();
  int offboardmod_en();
  int request_data_stream();
  int set_home_pose();
  int set_armed();
  int save_data_init();
  int save_data();
  int save_data2(mavlink_message, int);
  int save_data_close();

};

#endif
