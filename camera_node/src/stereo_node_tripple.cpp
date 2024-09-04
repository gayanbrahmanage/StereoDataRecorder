#include "camera_node.h"

stereo_node_tripple::stereo_node_tripple(){

}

stereo_node_tripple::~stereo_node_tripple(){

}

int stereo_node_tripple::init(){

  if(!initialized){
      camera* camera1=new camera;
      int cam_no=camera1->init(0);
      cam1=camera1;
      cam1->set_params(width, height,cam_no, top_trim, bot_trim, image_format);


      camera* camera2=new camera;
      int cam_no2=camera2->init(2);
      cam2=camera2;
      cam2->set_params(width, height,cam_no2, top_trim, bot_trim, image_format);


      camera* camera3=new camera;
      int cam_no3=camera3->init(4);
      cam3=camera3;
      cam3->set_params(width, height,cam_no3, top_trim, bot_trim, image_format);


      camera* camera4=new camera;
      int cam_no4=camera4->init(6);
      cam4=camera4;
      cam4->set_params(width, height,cam_no4, top_trim, bot_trim, image_format);


      t = clock(); //start time
      initialized=true;

    }
}


void stereo_node_tripple::save_image(){

  cam1->save_image(buffer1_flag, folder_path , "cam1", image_by_image_num2 );
  cam2->save_image(buffer2_flag, folder_path , "cam2", image_by_image_num2 );
  cam3->save_image(buffer3_flag, folder_path , "cam3", image_by_image_num2 );
  cam4->save_image(buffer4_flag, folder_path , "cam4", image_by_image_num2 );
  image_by_image_num2++;

}

void stereo_node_tripple::sync_thread(){
    while(module_enabled){
      if(!cam1->save_command && !cam2->save_command && !cam3->save_command && !cam4->save_command){
        //
        int delay=70-(int)cam4->writing_duration;
        if(delay>0)std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        cam1->save_command=true;
        cam2->save_command=true;
        cam3->save_command=true;
        cam4->save_command=true;
      }
  }
}

void stereo_node_tripple::run(){

  if(module_enabled){
    if(!running){

      // create reading  threds here
      running=true;
      std::thread cam1_thread(&camera::run, cam1,
      std::ref(module_enabled),
      std::ref(running),
      std::ref(buffer1_flag),
      std::ref(record_images_EN),
      std::ref(folder_path),
      std::ref(image_num1));
      cam1_thread.detach();

      std::thread cam2_thread(&camera::run, cam2,
      std::ref(module_enabled),
      std::ref(running),
      std::ref(buffer2_flag),
      std::ref(record_images_EN),
      std::ref(folder_path),
      std::ref(image_num2));
      cam2_thread.detach();

      std::thread cam3_thread(&camera::run, cam3,
      std::ref(module_enabled),
      std::ref(running),
      std::ref(buffer3_flag),
      std::ref(record_images_EN),
      std::ref(folder_path),
      std::ref(image_num3));
      cam3_thread.detach();

      std::thread cam4_thread(&camera::run, cam4,
      std::ref(module_enabled),
      std::ref(running),
      std::ref(buffer4_flag),
      std::ref(record_images_EN),
      std::ref(folder_path),
      std::ref(image_num4));
      cam4_thread.detach();


      std::thread sync(&stereo_node_tripple::sync_thread,this);
      sync.detach();

    }else{
      // read data if needed
      if(record_images_EN){

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        float time=elapsed_seconds.count();
        saving_fps=image_num1/time;
        saving_fps2=image_num2/time;
        saving_fps3=image_num3/time;
        saving_fps4=image_num4/time;




        //std::cout<<"recording..."<<std::endl;
      }else{
        start= std::chrono::high_resolution_clock::now();
        if(!buffer1_flag && !buffer2_flag && !buffer3_flag && !buffer4_flag){
          ready=false;
          if(cam1->use_opencv_cap){
            cv_image1=cam1->cv_image.clone();
            cv_image2=cam2->cv_image.clone();
            cv_image3=cam3->cv_image.clone();
            cv_image4=cam4->cv_image.clone();
          }else{
            freeStr(&image_data1);
            image_data1=load_image_from_memory(cam1->buffer, width * height * channels*sizeof(unsigned char), width, height, channels, channels);
            freeStr(&image_data2);
            image_data2=load_image_from_memory(cam2->buffer, width * height * channels*sizeof(unsigned char), width, height, channels, channels);
            freeStr(&image_data3);
            image_data3=load_image_from_memory(cam3->buffer, width * height * channels*sizeof(unsigned char), width, height, channels, channels);
            freeStr(&image_data4);
            image_data4=load_image_from_memory(cam4->buffer, width * height * channels*sizeof(unsigned char), width, height, channels, channels);
          }
          buffer4_flag=true;
          buffer3_flag=true;
          buffer2_flag=true;
          buffer1_flag=true;
          ready=true;

        }

      }

    }

  }else{ //module disabled
    running=false;


  }

}
