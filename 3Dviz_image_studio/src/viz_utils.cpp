
#include "viz_utils.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
//https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/
//https://developpaper.com/introduction-of-simple-and-easy-to-use-image-decoding-library-stb_image/

#include "stb_image.h"
#include "stb_image_resize.h"
#include "stb_image_write.h"

void write_JPEG_file(char const *filename, int width, int height, int channels, unsigned char* image_data){

  unsigned char* compressedImage = NULL; //!< Memory is allocated by tjCompress2 if _jpegSize == 0
  long unsigned int jpegSize = 0;
  const int JPEG_QUALITY = 100;

  tjhandle jpegCompressor = tjInitCompress();

  tjCompress2(jpegCompressor, image_data, width, 0, height, TJPF_RGB,
          &compressedImage, &jpegSize, TJSAMP_444, JPEG_QUALITY,
          TJFLAG_FASTDCT);

  FILE *file = fopen(filename, "wb");
  if (fwrite(compressedImage,sizeof(unsigned char), jpegSize, file) < 1) {
    //cerr << "Could not write JPEG file: " << strerror(errno);
    //return -1;
  }
  fclose(file);

  tjDestroy(jpegCompressor);
  //to free the memory allocated by TurboJPEG (either by tjAlloc(),
  //or by the Compress/Decompress) after you are done working on it:
  tjFree(compressedImage);

  std::cout<<"jpegSize " << jpegSize<<std::endl;

}


void stb_image_free(unsigned char* image_data){
  stbi_image_free(image_data);
}

unsigned char * load_image_from_memory(unsigned char* buffer, int len , int w, int h, int channels_in_file, int desired_channels){
  std::cout<<"cv_image "<<std::endl;
   unsigned char* image_data=stbi_load_from_memory(buffer,w * h * 3, &w, &h, &channels_in_file, 3);

   return image_data;
}

void write_image(char const *filename, int width, int height, int channels, unsigned char* image_data){


  stbi_write_jpg(filename, width, height, 0, image_data, 70);

}

void cvMAT_to_texture(cv::Mat image, GLuint* image_texture){
  // Create a OpenGL texture identifier
  //GLuint image_texture;

  glDeleteTextures(1, image_texture);

  glGenTextures(1, image_texture);
  glBindTexture(GL_TEXTURE_2D, *image_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // Upload pixels into texture
  #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  #endif

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);

}
//image into a OpenGL texture with common settings
void cv_Mat_to_texture(cv::Mat image, int width, int height, GLuint* image_texture, int channels){
  // Create a OpenGL texture identifier
  //GLuint image_texture;

  glDeleteTextures(1, image_texture);

  glGenTextures(1, image_texture);
  glBindTexture(GL_TEXTURE_2D, *image_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // Upload pixels into texture
  #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  #endif

  if (channels==4){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
  }else if (channels==3){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
  }


}

void stb_image_to_texture(unsigned char* stb_image_in, int width, int height, GLuint* image_texture, int channels){
  // Create a OpenGL texture identifier
  //GLuint image_texture;

  glDeleteTextures(1, image_texture);

  glGenTextures(1, image_texture);
  glBindTexture(GL_TEXTURE_2D, *image_texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // Upload pixels into texture
  #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  #endif

  if (channels==4){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stb_image_in);
  }else if (channels==3){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, stb_image_in);
  }


}

// image resize
void image_resize(unsigned char* image_data_in, int w_in, int h_in, int ch_in,
                  unsigned char* image_data_out, int w_out, int h_out, int alpha){


  if(alpha==0){
    stbir_resize(image_data_in, w_in, h_in, 0, image_data_out, w_out, h_out, 0, STBIR_TYPE_UINT8, ch_in, STBIR_ALPHA_CHANNEL_NONE, 0,
                          STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
                          STBIR_FILTER_BOX, STBIR_FILTER_BOX,
                          STBIR_COLORSPACE_SRGB, nullptr);
  }else{
    stbir_resize(image_data_in, w_in, h_in, 0, image_data_out, w_out, h_out, 0, STBIR_TYPE_UINT8, ch_in, alpha, 0,
                          STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
                          STBIR_FILTER_BOX, STBIR_FILTER_BOX,
                          STBIR_COLORSPACE_SRGB, nullptr);
  }

  //stbi_image_free(image_data_in);



}


//Simple helper function to load an image
bool LoadTextureFromFile(const char* filename, GLuint* image_texture, int* out_width, int* out_height){
    // Load from file
    int image_width= 0;
    int image_height= 0;
    int channels=0;
    unsigned char* image_data= stbi_load(filename, &image_width, &image_height, &channels, 4);

    if (image_data == NULL)
        return false;

    stb_image_to_texture(image_data, image_width, image_height, image_texture, 4);

    *out_width = image_width;
    *out_height = image_height;

    stbi_image_free(image_data);

    return true;
}

bool LoadTextureFromFile(const char* filename, GLuint* image_texture, int* out_width, int* out_height, double resize_factor){
    // Load from file
    int image_width= 0;
    int image_height= 0;
    int channels=0;
    unsigned char* image_data= stbi_load(filename, &image_width, &image_height, &channels, 4);

    if (image_data == NULL)
        return false;

    int new_width=image_width*resize_factor;
    int new_height=image_height*resize_factor;
    // resize
    unsigned char* resized_image_data=(unsigned char*)calloc(new_width*new_height*channels, sizeof(unsigned char));
    image_resize(image_data, image_width, image_height, channels, resized_image_data, new_width, new_height, 1);

    stb_image_to_texture(resized_image_data, new_width, new_height, image_texture, 4);

    *out_width = new_width;
    *out_height = new_height;

    stbi_image_free(image_data);
    stbi_image_free(resized_image_data);

    return true;
}

void freeStr(unsigned char **str){
    free( *str );
    *str = NULL;
}

void freeGLuint(unsigned int **GLi){
  free( *GLi );
  *GLi = NULL;
}
