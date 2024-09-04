#include "mavlink_node.h"

mavlink_node::mavlink_node(){
  //init();
}

mavlink_node::~mavlink_node(){

}

bool mavlink_node::init(){

  if(!initialized){
      baudrate = 57600;
      uart_name = (char*)"/dev/ttyACM0";
      port = new Serial_Port(uart_name, baudrate);
      port->start();

      if ( !port->is_running() ) // PORT_OPEN
      {
        fprintf(stderr,"ERROR: port not open\n");
        return false;
      }

      initialized=true;
      return true;

    }
}

void mavlink_node::terminate(){

  port->stop();
  delete port;
}

void mavlink_node::read_data(){

  start = std::chrono::system_clock::now();
  bool time_reset=false;
  bool success=false;
  mavlink_message_t message;
  while(mavlink_EN){

    // make saving time start from time 0.00000 seconds
    if(data_save_EN){
      if(!time_reset){
        set_home=false;
        setarmed=false;
        start = std::chrono::system_clock::now();
        time_reset=true;
      }
    } else{
      time_reset=false;
    }

    //read the message from the port
    //std::cout<< "reading.."<<std::endl;
    success = port->read_message(message);

    if(success){ //decode message
      auto end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end-start;

      switch (message.msgid){
        case MAVLINK_MSG_ID_HEARTBEAT:{
          mavlink_msg_heartbeat_decode(&message, &current_message_R.heartbeat);
          current_message_R.system_id=(int)message.sysid;
          current_message_R.companion_id=(int)message.compid;
          current_message_R.elapsed_seconds_heartbeat=elapsed_seconds;
          if(!connected){
            APsystem_id=(int)message.sysid;;
            autopilot_id=(int)message.compid;
            APheartbeat=current_message_R.heartbeat;
            if(APsystem_id>0 &&autopilot_id >0)connected=true;
          }
          //printf("MAVLINK_MSG_ID_HEARTBEAT\n");
          break;
        }

        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:{
          //printf("MAVLINK_MSG_ID_LOCAL_POSITION_NED\n");
          mavlink_msg_local_position_ned_decode(&message, &current_message_R.local_position_ned);
          current_message_R.system_id=(int)message.sysid;
          current_message_R.companion_id=(int)message.compid;
          current_message_R.elapsed_seconds_local_position_ned=elapsed_seconds;
          if(data_save_EN)save_data2(current_message_R,1);
          break;
        }
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:{
          mavlink_msg_global_position_int_decode(&message, &current_message_R.global_position_int);
          current_message_R.system_id=(int)message.sysid;
          current_message_R.companion_id=(int)message.compid;
          current_message_R.elapsed_seconds_global_position_int=elapsed_seconds;
          //printf("MAVLINK_MSG_ID_GLOBAL_POSITION_INT\n");
          //std::cout<<"lat "<<current_message_R.global_position_int.lat<<std::endl;
          if(data_save_EN)save_data2(current_message_R,2);
          break;
        }

        case MAVLINK_MSG_ID_GPS_RAW_INT:{
           mavlink_msg_gps_raw_int_decode(&message,   &current_message_R.gps_raw_int);
           current_message_R.system_id=(int)message.sysid;
           current_message_R.companion_id=(int)message.compid;
           current_message_R.elapsed_seconds_gps_raw_int=elapsed_seconds;
           //std::cout<< (int)current_message_R.gps_raw_int.lat<< " "<< (int)current_message_R.gps_raw_int.lon <<std::endl;
           if(data_save_EN)save_data2(current_message_R,3);
           break;

        }
        case MAVLINK_MSG_ID_RAW_IMU:{
          mavlink_msg_raw_imu_decode(&message, &current_message_R.raw_imu);
          current_message_R.system_id=(int)message.sysid;
          current_message_R.companion_id=(int)message.compid;
          current_message_R.elapsed_seconds_raw_imu=elapsed_seconds;
          //printf("MAVLINK_MSG_ID_RAW_IMU\n");
          //std::cout<<" raw_imu->xgyro  "<< current_message_R.raw_imu.xgyro <<std::endl;
          if(data_save_EN)save_data2(current_message_R,4);
          break;
        }

        case MAVLINK_MSG_ID_SCALED_IMU2:{ //HIGHRES_IMU
           //printf("MAVLINK_MSG_ID_HIGHRES_IMU\n");
           mavlink_msg_scaled_imu2_decode(&message, &current_message_R.scaled_imu2);
           current_message_R.system_id=(int)message.sysid;
           current_message_R.companion_id=(int)message.compid;
           current_message_R.elapsed_seconds_scaled_imu2=elapsed_seconds;
           //std::cout<< current_message_R.scaled_imu2.xacc <<std::endl;
           if(data_save_EN)save_data2(current_message_R,5);
           break;
        }

        case MAVLINK_MSG_ID_SCALED_IMU3:{ //HIGHRES_IMU
            //printf("MAVLINK_MSG_ID_HIGHRES_IMU\n");
           mavlink_msg_scaled_imu3_decode(&message, &current_message_R.scaled_imu3);
           //mavlink_msg_scaled_imu_decode(&message,  &current_message_R.scaled_imu );
           current_message_R.system_id=(int)message.sysid;
           current_message_R.companion_id=(int)message.compid;
           current_message_R.elapsed_seconds_scaled_imu3=elapsed_seconds;
           //std::cout<< current_message_R.scaled_imu2.xacc <<std::endl;
           if(data_save_EN)save_data2(current_message_R,6);
           break;
        }
        case MAVLINK_MSG_ID_ATTITUDE:{ //HIGHRES_IMU
            //printf("MAVLINK_MSG_ID_HIGHRES_IMU\n");
           mavlink_msg_attitude_decode(&message, &current_message_R.attitude);
           //mavlink_msg_scaled_imu_decode(&message,  &current_message_R.scaled_imu );
           current_message_R.system_id=(int)message.sysid;
           current_message_R.companion_id=(int)message.compid;
           current_message_R.elapsed_seconds_attitude=elapsed_seconds;
           //std::cout<< "MAVLINK_MSG_ID_ATTITUDE "<<std::endl;
           if(data_save_EN)save_data2(current_message_R,7);
           break;
        }
        // end switch
        //decode message
      }

    } //success

    // let the main thread to read the message
    if(reading_flag){
      current_message=current_message_R;
      reading_flag=false;
    }
  }
}

void mavlink_node::write_data(){
  while(mavlink_EN){
    // one time sends
    if(connected){

      if(!offboardmodEN)offboardmod_en();
      if(!data_stream_rq_sent)request_data_stream();
      if(!set_home)set_home_pose();
      if(!setarmed)set_armed();

      send_heartbeat();
      usleep(1000);
    }

  }
  connected=false;
}

int mavlink_node::set_armed(){

  bool flag=true;
  mavlink_message_t msg;

  mavlink_command_long_t com= { 0 };
  com.target_system    = APsystem_id;
  com.target_component = autopilot_id;
  com.command          = MAV_CMD_COMPONENT_ARM_DISARM ;
  com.confirmation     = true;
  com.param1           = (float)1;
  com.param2           = (float)21196;

  // Encode
  mavlink_msg_command_long_encode(system_id, companion_id, &msg, &com);
  // Send the message
  int len = port->write_message(msg);

  if(len>0){
    setarmed=true;
  }
  return len;
}

int mavlink_node::set_home_pose(){

  bool flag=true;
  mavlink_message_t msg;

  mavlink_command_long_t com= { 0 };
  com.target_system    = APsystem_id;
  com.target_component = autopilot_id;
  com.command          = MAV_CMD_DO_SET_HOME ;
  com.confirmation     = true;
  com.param1           = (float) 1; // flag >0.5 => start, <0.5 => stop

  // Encode
  mavlink_msg_command_long_encode(system_id, companion_id, &msg, &com);
  // Send the message
  int len = port->write_message(msg);

  if(len>0){
    set_home=true;
  }
  return len;
}

int mavlink_node::offboardmod_en(){
  bool flag=true;
  mavlink_command_long_t com = { 0 };
  com.target_system    = system_id;
  com.target_component = autopilot_id;
  com.command          = MAV_CMD_NAV_GUIDED_ENABLE;
  com.confirmation     = true;
  com.param1           = (float) flag; // flag >0.5 => start, <0.5 => stop
  // Encode
  mavlink_message_t msg;
  mavlink_msg_command_long_encode(system_id, companion_id, &msg, &com);

  // Send the message
  int len = port->write_message(msg);
  if (len>0)offboardmodEN=true;
  return len;
}

int mavlink_node::request_data_stream(){
  mavlink_message_t msg;
  mavlink_msg_request_data_stream_pack(system_id, companion_id, &msg, APsystem_id, autopilot_id, 	MAV_DATA_STREAM_ALL, aquision_rate, 1);
  bool len = port->write_message(msg);
  if(len>0)data_stream_rq_sent=true;
  return len;
}

int  mavlink_node::send_heartbeat(){

  mavlink_message_t msg;
  //heartbeat->type, heartbeat->autopilot, heartbeat->base_mode, heartbeat->custom_mode, heartbeat->system_status
  mavlink_msg_heartbeat_pack(system_id, companion_id, &msg, APheartbeat.type, APheartbeat.autopilot,
                             APheartbeat.base_mode, APheartbeat.custom_mode, APheartbeat.system_status);
  int len = port->write_message(msg);
  return len;
}


void mavlink_node::run(){
  if(mavlink_EN){ //mavlink_EN

    if(!running){

      running=true;
      //std::cout<< " mavlink_EN "<<std::endl;
      //reading data
      std::thread mavlink_read_thread(&mavlink_node::read_data, this);
      mavlink_read_thread.detach();

      //wring data to mavlink
      // saving data running on the main thread
      std::thread mavlink_write_thread(&mavlink_node::write_data, this);
      mavlink_write_thread.detach();

    }else{ // not running
      if(connected){
        if(!reading_flag){
            current_msg=current_message;
            //if(data_save_EN)save_data();
            reading_flag=true;

        }

      }

    }

  }else{ //mavlink_Desabled

    running=false;
    mavlink_EN=false;
    //terminate();

  }
}

int mavlink_node::save_data_init(){

  if(data_save_EN){

    std::stringstream file_name;
    file_name<< folder_path<<"/ground_truth.txt";
    gtfile.open (file_name.str().c_str());

    std::stringstream file_name2;
    file_name2<< folder_path<<"/gps.txt";
    gpsfile.open (file_name2.str().c_str());

    std::stringstream file_name3;
    file_name3<< folder_path<<"/gps_raw.txt";
    gps_raw.open (file_name3.str().c_str());

    std::stringstream file_name4;
    file_name4<< folder_path<<"/imu_raw.txt";
    imu_raw.open (file_name4.str().c_str());

    std::stringstream file_name5;
    file_name5<< folder_path<<"/imu2_scaled.txt";
    imu2_scaled.open (file_name5.str().c_str());

    std::stringstream file_name6;
    file_name6<< folder_path<<"/imu3_scaled.txt";
    imu3_scaled.open (file_name6.str().c_str());

    std::stringstream file_name7;
    file_name7<< folder_path<<"/attitude.txt";
    attitude.open (file_name7.str().c_str());

  }
}

int mavlink_node::save_data_close(){
    if(data_save_EN){
      gtfile.close();
      gpsfile.close();
      gps_raw.close();
      imu_raw.close();
      imu2_scaled.close();
      imu3_scaled.close();
      attitude.close();

    }

}

int mavlink_node::save_data2(mavlink_message msg, int msg_id){

  if(msg_id==1){
      std::stringstream line;
      //std::cout<<"Saving "<<current_msg.elapsed_seconds_global_position_int.count()<<std::endl;
      line<<msg.elapsed_seconds_local_position_ned.count()<<" "
          <<msg.local_position_ned.x<< " "
          <<msg.local_position_ned.y<< " "
          <<msg.local_position_ned.z<< " "
          <<msg.local_position_ned.vx<<" "
          <<msg.local_position_ned.vy<< " "
          <<msg.local_position_ned.vz<< " \n";
      gtfile<<line.str().c_str();

    }else if(msg_id==2){
      std::stringstream line2;
      line2<<msg.elapsed_seconds_global_position_int.count()<<" "
          <<msg.global_position_int.lat<< " "
          <<msg.global_position_int.lon<< " "
          <<msg.global_position_int.alt<< " "
          <<msg.global_position_int.vx<< " "
          <<msg.global_position_int.vy<< " "
          <<msg.global_position_int.vz<< " \n";

      gpsfile<<line2.str().c_str();
    }else if(msg_id==3){
      std::stringstream line3;
      line3<<msg.elapsed_seconds_gps_raw_int.count()<<" "
          <<msg.gps_raw_int.lat<< " "
          <<msg.gps_raw_int.lon<< " "
          <<msg.gps_raw_int.alt<< " "
          <<msg.gps_raw_int.vel<< " "
          <<msg.gps_raw_int.cog<< " "
          <<msg.gps_raw_int.eph<< " "
          <<msg.gps_raw_int.epv<< " \n";

      gps_raw<<line3.str().c_str();
    }

    else if(msg_id==4){
      std::stringstream line4;
      line4<<msg.elapsed_seconds_raw_imu.count()<<" "
          <<msg.raw_imu.xacc<< " "
          <<msg.raw_imu.yacc<< " "
          <<msg.raw_imu.zacc<< " "
          <<msg.raw_imu.xgyro<< " "
          <<msg.raw_imu.ygyro<< " "
          <<msg.raw_imu.zgyro<< " "
          <<msg.raw_imu.xmag<< " "
          <<msg.raw_imu.ymag<< " "
          <<msg.raw_imu.zmag<< " \n";

      imu_raw<<line4.str().c_str();
    }

    else if(msg_id==5){
      std::stringstream line5;
      line5<<msg.elapsed_seconds_scaled_imu2.count()<<" "
          <<msg.scaled_imu2.xacc<< " "
          <<msg.scaled_imu2.yacc<< " "
          <<msg.scaled_imu2.zacc<< " "
          <<msg.scaled_imu2.xgyro<< " "
          <<msg.scaled_imu2.ygyro<< " "
          <<msg.scaled_imu2.zgyro<< " "
          <<msg.scaled_imu2.xmag<< " "
          <<msg.scaled_imu2.ymag<< " "
          <<msg.scaled_imu2.zmag<< " \n";

      imu2_scaled<<line5.str().c_str();
    }

    else if(msg_id==6){
      std::stringstream line6;
      line6<<msg.elapsed_seconds_scaled_imu3.count()<<" "
          <<msg.scaled_imu3.xacc<< " "
          <<msg.scaled_imu3.yacc<< " "
          <<msg.scaled_imu3.zacc<< " "
          <<msg.scaled_imu3.xgyro<< " "
          <<msg.scaled_imu3.ygyro<< " "
          <<msg.scaled_imu3.zgyro<< " "
          <<msg.scaled_imu3.xmag<< " "
          <<msg.scaled_imu3.ymag<< " "
          <<msg.scaled_imu3.zmag<< " \n";

      imu3_scaled<<line6.str().c_str();
    }

    else if(msg_id==7){

      std::stringstream line7;
      line7<<msg.elapsed_seconds_attitude.count()<<" "
          <<msg.attitude.roll<< " "
          <<msg.attitude.pitch<< " "
          <<msg.attitude.yaw<< " "
          <<msg.attitude.rollspeed<< " "
          <<msg.attitude.pitchspeed<< " "
          <<msg.attitude.yawspeed<< " \n";

      attitude<<line7.str().c_str();
    }

}

int mavlink_node::save_data(){

  std::stringstream line;
  //std::cout<<"Saving "<<current_msg.elapsed_seconds_global_position_int.count()<<std::endl;
  line<<current_msg.elapsed_seconds_local_position_ned.count()<<" "
      <<current_msg.local_position_ned.x<< " "
      <<current_msg.local_position_ned.y<< " "
      <<current_msg.local_position_ned.z<< " "
      <<current_msg.local_position_ned.vx<<" "
      <<current_msg.local_position_ned.vy<< " "
      <<current_msg.local_position_ned.vz<< " \n";
  gtfile<<line.str().c_str();

  std::stringstream line2;
  line2<<current_msg.elapsed_seconds_global_position_int.count()<<" "
      <<current_msg.global_position_int.lat<< " "
      <<current_msg.global_position_int.lon<< " "
      <<current_msg.global_position_int.alt<< " "
      <<current_msg.global_position_int.vx<< " "
      <<current_msg.global_position_int.vy<< " "
      <<current_msg.global_position_int.vz<< " \n";

  gpsfile<<line2.str().c_str();

  std::stringstream line3;
  line3<<current_msg.elapsed_seconds_gps_raw_int.count()<<" "
      <<current_msg.gps_raw_int.lat<< " "
      <<current_msg.gps_raw_int.lon<< " "
      <<current_msg.gps_raw_int.alt<< " "
      <<current_msg.gps_raw_int.vel<< " "
      <<current_msg.gps_raw_int.cog<< " "
      <<current_msg.gps_raw_int.eph<< " "
      <<current_msg.gps_raw_int.epv<< " \n";

  gps_raw<<line3.str().c_str();

  std::stringstream line4;
  line4<<current_msg.elapsed_seconds_raw_imu.count()<<" "
      <<current_msg.raw_imu.xacc<< " "
      <<current_msg.raw_imu.yacc<< " "
      <<current_msg.raw_imu.zacc<< " "
      <<current_msg.raw_imu.xgyro<< " "
      <<current_msg.raw_imu.ygyro<< " "
      <<current_msg.raw_imu.zgyro<< " "
      <<current_msg.raw_imu.xmag<< " "
      <<current_msg.raw_imu.ymag<< " "
      <<current_msg.raw_imu.zmag<< " \n";

  imu_raw<<line4.str().c_str();

  std::stringstream line5;
  line5<<current_msg.elapsed_seconds_scaled_imu2.count()<<" "
      <<current_msg.scaled_imu2.xacc<< " "
      <<current_msg.scaled_imu2.yacc<< " "
      <<current_msg.scaled_imu2.zacc<< " "
      <<current_msg.scaled_imu2.xgyro<< " "
      <<current_msg.scaled_imu2.ygyro<< " "
      <<current_msg.scaled_imu2.zgyro<< " "
      <<current_msg.scaled_imu2.xmag<< " "
      <<current_msg.scaled_imu2.ymag<< " "
      <<current_msg.scaled_imu2.zmag<< " \n";

  imu2_scaled<<line5.str().c_str();

  std::stringstream line6;
  line6<<current_msg.elapsed_seconds_scaled_imu3.count()<<" "
      <<current_msg.scaled_imu3.xacc<< " "
      <<current_msg.scaled_imu3.yacc<< " "
      <<current_msg.scaled_imu3.zacc<< " "
      <<current_msg.scaled_imu3.xgyro<< " "
      <<current_msg.scaled_imu3.ygyro<< " "
      <<current_msg.scaled_imu3.zgyro<< " "
      <<current_msg.scaled_imu3.xmag<< " "
      <<current_msg.scaled_imu3.ymag<< " "
      <<current_msg.scaled_imu3.zmag<< " \n";

  imu3_scaled<<line6.str().c_str();

  std::stringstream line7;
  line7<<current_msg.elapsed_seconds_attitude.count()<<" "
      <<current_msg.attitude.roll<< " "
      <<current_msg.attitude.pitch<< " "
      <<current_msg.attitude.yaw<< " "
      <<current_msg.attitude.rollspeed<< " "
      <<current_msg.attitude.pitchspeed<< " "
      <<current_msg.attitude.yawspeed<< " \n";

  attitude<<line7.str().c_str();
}
