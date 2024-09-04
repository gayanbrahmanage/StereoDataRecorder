#ifndef viz_parameters_H
#define viz_parameters_H

class viz_parameters{
public:
  //OpenGL
  float bg_color_R=0.1f;
  float bg_color_G=0.1f;
  float bg_color_B=0.1f;



  int pose_buffer_size=100;

  //imgui
  float menu1_windwo_height=110;
  float mavlink_window_width=300;
  float mavlink_bottom_window_height=100;


  // camera module Discrete 1280x720
  int camera_view_width=1280;
  int camera_view_height=880;
  int status_bar_width=50;

  float menu_windwo_width=300;
  float menu_windwo_height=768;

  float menu2_windwo_width=300;
  float menu2_windwo_height=768;

  int width=camera_view_width+menu_windwo_width+status_bar_width+10;
  int height=camera_view_height+menu1_windwo_height+10;

};
#endif
