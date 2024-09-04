#ifndef pose_H
#define pose_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "viz_parameters.h"
#include <Eigen/Dense>
#include <vector>

class pose{
public:
  float scale=0.01f;
  viz_parameters* viz_param= new viz_parameters;
  unsigned int VBO, VAO, EBO;
  int vertices_size;
  int indices_size;
  std::string draw_type;

  pose();
  ~pose();
  void init_pose(float size, std::string );
  void draw_pose(std::vector<Eigen::Matrix4d,Eigen::aligned_allocator<Eigen::Matrix4d>> pose);

};
#endif
