#include "camera_node.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
camera_node::camera_node(){


}

camera_node::~camera_node(){

}

int camera_node::init(){
  if(!initialized){
    int cam_no=cam->init(0);
    cam->set_params(width, height,1, top_trim, bot_trim, image_format);
    t = clock(); //start time
    initialized=true;
    std::cout<<"Camera No "<<cam_no<<std::endl;
  }
}


void camera_node::run(){

  clock_t dt=clock() - t; //time spent;
  t = clock(); //new time
  saving_fps=CLOCKS_PER_SEC/((float)dt);

  if(module_enabled){

    if(!running){
      running=true;
      //std::cout<< "Startd Camera running "<< glfwGetTime()<<std::endl;

      std::thread cam_thread(&camera::get_data, cam,
        std::ref(module_enabled),
        std::ref(running),
        std::ref(buffer_flag));

      cam_thread.detach();

    }else{
      //reading data
      if(!buffer_flag){
        //wrinting image
        if(record_images_EN){
          //using opencv image container
          // read image from buffer
          cv_image = cv::Mat(width, height, CV_8UC3, cam->buffer);
          // rleased buffer after image is copied
          // let camera to capture next image
          buffer_flag=true;
          ready=true;

          if(ready){

            cv::Mat decodedMat= cv::imdecode(cv_image,cv::IMREAD_COLOR);

            std::stringstream image_name;
            //image_name<< folder_path<<"/img"<<image_num<<".png";
            //cv::imwrite(image_name.str().c_str(), decodedMat);

            image_name<< folder_path<<"/img"<<image_num<<".jpg";
            cv::imwrite(image_name.str().c_str(), decodedMat, {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, 70});


            // using stbi image
            // using stbi makes the saving images slow
            // opencv is faster
            // write_image(image_name.str().c_str(), width, height, channels, image_data, width * channels);
            image_num++;
          }
        } else{
          //using stbi image
          freeStr(&image_data);
          image_data=load_image_from_memory(cam->buffer, width * height * channels*sizeof(unsigned char), width, height, channels, channels);
          buffer_flag=true;
          ready=true;
        }

      }

    }

  }else{
    if(running){
      running=false;
      buffer_flag=true;
      ready=false;
      //cam->stop_stream();
    }
  }


}
