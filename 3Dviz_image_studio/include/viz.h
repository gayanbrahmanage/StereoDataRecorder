#ifndef viz_H
#define viz_H

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



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader_m.h>
#include <camera.h>
#include "viz_parameters.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <axes.h>
#include <pose.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//modules
#include "camera_node.h"
#include "viz_utils.h"
#include "viz_utils2.h"
#include "mavlink_node.h"
#include <irrKlang.h> // sound


class viz{
public:
  // sound
  irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

  GLuint image_texture=0;
  GLuint image_texture2=0;
  GLuint image_texture3=0;
  GLuint image_texture4=0;

  cv::Mat cv_image1;
  cv::Mat cv_image2;
  cv::Mat cv_image3;
  cv::Mat cv_image4;

  std::string folder_path;
  ImVec4 color_multipler=ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


  //modules
  camera_node* cam_node=new camera_node;
  stereo_node* stcam_node=new stereo_node;
  stereo_node_tripple* stcam_node_tripple=new stereo_node_tripple;
  mavlink_node* mvlink_node=new mavlink_node;

  glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
  axes ax;
  pose _pose;
  //shader
  unsigned int shaderProgram;
  unsigned int fragmentShader;
  unsigned int vertexShader;

  Shader ourShader;
  // FLAGS
  bool MOUSE_BUTTON_MIDDLE_PRESSED=false;

  viz_parameters* viz_param= new viz_parameters;
  GLFWwindow* window;

  viz();
  ~viz();
  void init();
  void draw_axes(float, float);
  void run();
  void update_view(double radius, double anglex, double angley);

  // Modules Function on VIZ class
  void module_mavlink();
  void module_camera(bool, bool, bool, unsigned char*, int , int, int);
  void module_camera(bool, bool, bool, cv::Mat);
  void module_stereo_camera(bool , bool , bool , unsigned char* , unsigned char*, int , int , int, int, int, cv::Mat, cv::Mat );
  void module_stereo_camera_tripple(bool, bool , bool , unsigned char* , unsigned char*, unsigned char*,unsigned char*,cv::Mat,cv::Mat,cv::Mat,cv::Mat, int , int , int, int, int);
  void module_3Dsensor( std::vector<Eigen::Matrix4d,Eigen::aligned_allocator<Eigen::Matrix4d>>);



};

#endif
