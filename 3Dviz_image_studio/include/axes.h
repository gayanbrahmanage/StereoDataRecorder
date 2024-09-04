#ifndef axes_H
#define axes_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class axes{
public:
  unsigned int VBO, VAO, EBO;

  axes();
  ~axes();
  void init_axes(float size);
  void draw_axes();

};
#endif
