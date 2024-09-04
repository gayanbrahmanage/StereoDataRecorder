#ifndef viz_utils_H
#define viz_utils_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "turbojpeg.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void freeStr(unsigned char **str);
void freeGLuint(unsigned int **GLi);
void stb_image_free(unsigned char* image_data);
void write_image(char const *filename, int w, int h, int comp, unsigned char *data);
void write_JPEG_file(char const *filename, int width, int height, int channels, unsigned char* image_data);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, double resize_factor);
void stb_image_to_texture(unsigned char* stb_image_in, int width, int height, GLuint* out_texture, int channesl);
void cv_Mat_to_texture(cv::Mat image, int width, int height, GLuint* image_texture, int channels);
void cvMAT_to_texture(cv::Mat image, GLuint* out_texture);
void image_resize(unsigned char*, int, int, int, unsigned char*, int, int, int);
unsigned char * load_image_from_memory(unsigned char* buffer, int len , int w, int h, int channels_in_file, int desired_channels);
#endif
