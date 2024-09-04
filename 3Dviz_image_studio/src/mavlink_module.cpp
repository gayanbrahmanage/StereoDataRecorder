#include "viz.h"

void viz::module_mavlink(){
  ImGui::SetNextWindowPos(ImVec2(viz_param->width-viz_param->mavlink_window_width, 20));
  ImGui::SetNextWindowSize(ImVec2(viz_param->mavlink_window_width, viz_param->menu1_windwo_height));
  ImGui::Begin("MavLink Com Protocol", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoResize);
  ImGui::Text("Dekstop System ID: %d | Comp ID %d ", mvlink_node->system_id, mvlink_node-> companion_id);

  if(mvlink_node->connected){
    ImGui::Text("Connected to System: %d AP ID %d ", mvlink_node->APsystem_id,mvlink_node-> autopilot_id);

    if(mvlink_node->offboardmodEN){
      ImGui::Text("Off Board Enabled ");
    }

    if(mvlink_node->data_stream_rq_sent){
      ImGui::Text("Data Requested");
    }

    if(mvlink_node->set_home){
      ImGui::Text("Home Pose Set");
    }
  }

  ImGui::End();

// bottom window
//ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width, viz_param->height-viz_param->mavlink_bottom_window_height));
ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width, viz_param->height-viz_param->mavlink_bottom_window_height));
ImGui::SetNextWindowSize(ImVec2(viz_param->width-viz_param->menu2_windwo_width, viz_param->mavlink_bottom_window_height));
ImGui::Begin("Position Information", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoResize);

if(mvlink_node->connected){

    //std::cout << "hee "<<std::endl;
    ImGui::Text("Local Position: x:%f y:%f z:%f", (float)mvlink_node->current_msg.local_position_ned.x,
                                                  (float)mvlink_node->current_msg.local_position_ned.y,
                                                  (float)mvlink_node->current_msg.local_position_ned.z);


    ImGui::Text("Global Position: Lat:%d Lon:%d Alt:%d",(int)mvlink_node->current_msg.global_position_int.lat,
                                                  (int)mvlink_node->current_msg.global_position_int.lon,
                                                  (int)mvlink_node->current_msg.global_position_int.alt);


    ImGui::Text("RAW IMU: x_dot:%d y_dot:%d z_dot:%d",  (int)mvlink_node->current_msg.raw_imu.xgyro,
                                                  (int)mvlink_node->current_msg.raw_imu.ygyro,
                                                  (int)mvlink_node->current_msg.raw_imu.zgyro);

    ImGui::Text("SCLD IMU2: x_dot:%d y_dot:%d z_dot:%d",  (int)mvlink_node->current_msg.scaled_imu2.xgyro,
                                                  (int)mvlink_node->current_msg.scaled_imu2.ygyro,
                                                  (int)mvlink_node->current_msg.scaled_imu2.zgyro);

    ImGui::Text("SCLD IMU3: x_dot:%d y_dot:%d z_dot:%d",  (int)mvlink_node->current_msg.scaled_imu3.xgyro,
                                                  (int)mvlink_node->current_msg.scaled_imu3.ygyro,
                                                  (int)mvlink_node->current_msg.scaled_imu3.zgyro);
}

ImGui::End();

}
