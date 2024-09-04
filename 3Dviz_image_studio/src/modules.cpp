#include "viz.h"

void viz::module_stereo_camera_tripple(bool ready, bool module_enabled, bool module_camera_show_EN,
                                        unsigned char* stb_image_in,unsigned char* stb_image_in2,unsigned char* stb_image_in3,unsigned char* stb_image_in4,
                                        cv::Mat image1,cv::Mat image2,cv::Mat image3,cv::Mat image4,
                                        int width, int height, int channels, int top_trim, int bot_trim){

                                          //trying to add lines on top of my image here:


  // camera view
  int camera_view_width=viz_param->camera_view_width*0.4;
  int camera_view_height=viz_param->camera_view_height*0.4;

  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  // Camera 01
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(camera_view_width+10, camera_view_height+10));
  ImGui::Begin("Camera 1", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  if(module_enabled && ready && module_camera_show_EN && !stcam_node_tripple->record_images_EN){
    int view_w1=camera_view_width;
    int view_h1=view_w1 * height/width;

    int view_h2=camera_view_height;
    int view_w2=width*view_h2/height;

    int view_h=240;
    int view_w=320;

    if(view_w1<=camera_view_width && view_h1<=camera_view_height){
      view_h=view_h1;
      view_w=view_w1;
    }

    if(view_w2<=camera_view_width && view_h2<=camera_view_height){
      view_h=view_h2;
      view_w=view_w2;
    }

    if(stcam_node_tripple->cam1->use_opencv_cap){
        cv::resize(image1, image1, cv::Size(view_w, view_h), cv::INTER_LINEAR);
        cv_Mat_to_texture(image1, image1.cols, image1.rows, &image_texture, image1.channels());
        //ImGui::Image( reinterpret_cast<void*>( static_cast<intptr_t>( image_texture ) ), ImVec2( image1.cols, image1.rows ) );
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture, ImVec2(image1.cols,image1.rows));
        //ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);
    }else{
        unsigned char* temp=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));;
        image_resize(stb_image_in, width, height, channels, temp, view_w, view_h, 0);
        unsigned char* temp2=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));
        memcpy(temp2, temp, view_w * view_h * channels* sizeof(unsigned char));
        stb_image_to_texture(temp2, view_w, view_h, &image_texture, 3);
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);
        float top_trim_=top_trim*(view_h*1.0f/height);
        float bot_trim_=bot_trim*(view_h*1.0f/height);

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        drawList->AddText(ImVec2(viz_param->menu2_windwo_width+30, 20+viz_param->menu1_windwo_height), IM_COL32(255, 255, 255, 255), "Camera 1");

        if(stcam_node_tripple->show_trimmed){
            drawList->AddRectFilled(ImVec2(viz_param->menu2_windwo_width, 20+viz_param->menu1_windwo_height),
                              ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+top_trim_),IM_COL32(128, 128, 128, 150), 1.0f);

            drawList->AddRectFilled(ImVec2(viz_param->menu2_windwo_width, 20+viz_param->menu1_windwo_height+camera_view_height+10-bot_trim_),
                              ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10), IM_COL32(128, 128, 128, 150), 1.0f);
        }
        freeStr(&temp);
        freeStr(&temp2);
    }

  }else{
        int image_width = 0;
        int image_height = 0;

        bool ret = LoadTextureFromFile("../3Dviz_image_studio/images/camera.png", &image_texture, &image_width, &image_height, 0.5);
        IM_ASSERT(ret);

        ImGui::SetCursorPos(ImVec2((camera_view_width-image_width)*0.5f, (camera_view_height-image_height)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture, ImVec2(image_width,image_height));
  }

  ImGui::End();
  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  //Camera 02
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(camera_view_width+10, camera_view_height+10));
  ImGui::Begin("Camera 2", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  if(module_enabled && ready && module_camera_show_EN && !stcam_node_tripple->record_images_EN){
    int view_w1=camera_view_width;
    int view_h1=view_w1 * height/width;

    int view_h2=camera_view_height;
    int view_w2=width*view_h2/height;

    int view_h=240;
    int view_w=320;

    if(view_w1<=camera_view_width && view_h1<=camera_view_height){
      view_h=view_h1;
      view_w=view_w1;
    }

    if(view_w2<=camera_view_width && view_h2<=camera_view_height){
      view_h=view_h2;
      view_w=view_w2;
    }

    if(stcam_node_tripple->cam1->use_opencv_cap){
        cv::resize(image2, image2, cv::Size(view_w, view_h), cv::INTER_LINEAR);
        cv_Mat_to_texture(image2, image2.cols, image2.rows, &image_texture2, image2.channels());
        //ImGui::Image( reinterpret_cast<void*>( static_cast<intptr_t>( image_texture ) ), ImVec2( image1.cols, image1.rows ) );
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture2, ImVec2(image2.cols,image2.rows));
        //ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);
    }else{
        unsigned char* temp=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));;
        image_resize(stb_image_in2, width, height, channels, temp, view_w, view_h, 0);
        unsigned char* temp2=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));
        memcpy(temp2, temp, view_w * view_h * channels* sizeof(unsigned char));
        stb_image_to_texture(temp2, view_w, view_h, &image_texture2, 3);
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture2, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);

        float top_trim_=top_trim*(view_h*1.0f/height);
        float bot_trim_=bot_trim*(view_h*1.0f/height);

        ImDrawList* drawList2 = ImGui::GetWindowDrawList();

        drawList2->AddText(ImVec2(viz_param->menu2_windwo_width+camera_view_width+30, 20+viz_param->menu1_windwo_height), IM_COL32(255, 255, 255, 255), "Camera 2");

        if(stcam_node_tripple->show_trimmed){
            drawList2->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height),
                          ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+top_trim_),IM_COL32(128, 128, 128, 150), 1.0f);

            drawList2->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10-bot_trim_),
                      ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10), IM_COL32(128, 128, 128, 150), 1.0f);
        }

        freeStr(&temp);
        freeStr(&temp2);
  }

  }else{
        int image_width = 0;
        int image_height = 0;

        bool ret = LoadTextureFromFile("../3Dviz_image_studio/images/camera.png", &image_texture2, &image_width, &image_height, 0.5);
        IM_ASSERT(ret);

        ImGui::SetCursorPos(ImVec2((camera_view_width-image_width)*0.5f, (camera_view_height-image_height)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture2, ImVec2(image_width,image_height));
  }

  ImGui::End();

  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  //Camera 03
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width, viz_param->menu1_windwo_height+camera_view_height+40));
  ImGui::SetNextWindowSize(ImVec2(camera_view_width, camera_view_height));
  ImGui::Begin("Camera 3", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  if(module_enabled && ready && module_camera_show_EN && !stcam_node_tripple->record_images_EN){
    int view_w1=camera_view_width;
    int view_h1=view_w1 * height/width;

    int view_h2=camera_view_height;
    int view_w2=width*view_h2/height;

    int view_h=240;
    int view_w=320;

    if(view_w1<=camera_view_width && view_h1<=camera_view_height){
      view_h=view_h1;
      view_w=view_w1;
    }

    if(view_w2<=camera_view_width && view_h2<=camera_view_height){
      view_h=view_h2;
      view_w=view_w2;
    }
    if(stcam_node_tripple->cam1->use_opencv_cap){
        cv::resize(image3, image3, cv::Size(view_w, view_h), cv::INTER_LINEAR);
        cv_Mat_to_texture(image3, image3.cols, image3.rows, &image_texture3, image3.channels());
        //ImGui::Image( reinterpret_cast<void*>( static_cast<intptr_t>( image_texture ) ), ImVec2( image1.cols, image1.rows ) );
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture3, ImVec2(image3.cols,image3.rows));
        //ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);
    }else{
        unsigned char* temp=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));;
        image_resize(stb_image_in3, width, height, channels, temp, view_w, view_h, 0);
        unsigned char* temp2=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));
        memcpy(temp2, temp, view_w * view_h * channels* sizeof(unsigned char));
        stb_image_to_texture(temp2, view_w, view_h, &image_texture3, 3);
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture3, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);

        float top_trim_=top_trim*(view_h*1.0f/height);
        float bot_trim_=bot_trim*(view_h*1.0f/height);

        ImDrawList* drawList3 = ImGui::GetWindowDrawList();

        drawList3->AddText(ImVec2(viz_param->menu2_windwo_width+30, 20+viz_param->menu1_windwo_height+camera_view_height), IM_COL32(255, 255, 255, 255), "Camera 3");

        if(stcam_node_tripple->show_trimmed){
            drawList3->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height),
                          ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+top_trim_),IM_COL32(128, 128, 128, 150), 1.0f);

            drawList3->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10-bot_trim_),
                      ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10), IM_COL32(128, 128, 128, 150), 1.0f);
        }

        freeStr(&temp);
        freeStr(&temp2);
  }

  }else{
        int image_width = 0;
        int image_height = 0;

        bool ret = LoadTextureFromFile("../3Dviz_image_studio/images/camera.png", &image_texture3, &image_width, &image_height, 0.5);
        IM_ASSERT(ret);

        ImGui::SetCursorPos(ImVec2((camera_view_width-image_width)*0.5f, (camera_view_height-image_height)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture3, ImVec2(image_width,image_height));
  }

  ImGui::End();

  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  //Camera 04
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 40+viz_param->menu1_windwo_height+camera_view_height));
  ImGui::SetNextWindowSize(ImVec2(camera_view_width, camera_view_height));
  ImGui::Begin("Camera 4", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  if(module_enabled && ready && module_camera_show_EN && !stcam_node_tripple->record_images_EN){
    int view_w1=camera_view_width;
    int view_h1=view_w1 * height/width;

    int view_h2=camera_view_height;
    int view_w2=width*view_h2/height;

    int view_h=240;
    int view_w=320;

    if(view_w1<=camera_view_width && view_h1<=camera_view_height){
      view_h=view_h1;
      view_w=view_w1;
    }

    if(view_w2<=camera_view_width && view_h2<=camera_view_height){
      view_h=view_h2;
      view_w=view_w2;
    }

    if(stcam_node_tripple->cam1->use_opencv_cap){
        cv::resize(image4, image4, cv::Size(view_w, view_h), cv::INTER_LINEAR);
        cv_Mat_to_texture(image4, image4.cols, image4.rows, &image_texture4, image4.channels());
        //ImGui::Image( reinterpret_cast<void*>( static_cast<intptr_t>( image_texture ) ), ImVec2( image1.cols, image1.rows ) );
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture4, ImVec2(image4.cols,image4.rows));
        //ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);
    }else{

        unsigned char* temp=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));;
        image_resize(stb_image_in4, width, height, channels, temp, view_w, view_h, 0);
        unsigned char* temp2=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));
        memcpy(temp2, temp, view_w * view_h * channels* sizeof(unsigned char));
        stb_image_to_texture(temp2, view_w, view_h, &image_texture4, 3);
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture4, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);

        float top_trim_=top_trim*(view_h*1.0f/height);
        float bot_trim_=bot_trim*(view_h*1.0f/height);

        ImDrawList* drawList4 = ImGui::GetWindowDrawList();

        drawList4->AddText(ImVec2(viz_param->menu2_windwo_width+camera_view_width+30, 20+viz_param->menu1_windwo_height+camera_view_height), IM_COL32(255, 255, 255, 255), "Camera 4");

        if(stcam_node_tripple->show_trimmed){
            drawList4->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height),
                          ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+top_trim_),IM_COL32(128, 128, 128, 150), 1.0f);

            drawList4->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10-bot_trim_),
                      ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10), IM_COL32(128, 128, 128, 150), 1.0f);
        }

        freeStr(&temp);
        freeStr(&temp2);
  }

  }else{
        int image_width = 0;
        int image_height = 0;

        bool ret = LoadTextureFromFile("../3Dviz_image_studio/images/camera.png", &image_texture4, &image_width, &image_height, 0.5);
        IM_ASSERT(ret);

        ImGui::SetCursorPos(ImVec2((camera_view_width-image_width)*0.5f, (camera_view_height-image_height)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture4, ImVec2(image_width,image_height));
  }

  ImGui::End();


  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  // left window
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(0, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(viz_param->menu2_windwo_width, viz_param->height-viz_param->menu1_windwo_height-20));
  ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  ImGui::Text("Image Resolution : %d x %d", stcam_node_tripple->width, stcam_node_tripple->height);
  ImGui::Text("Image Trim : Top %d px | Bottom %d px" , stcam_node_tripple->top_trim, stcam_node_tripple->bot_trim);

  //ImGui::Text("Frames per Second : %d", cam_node->saving_fps);

  if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){

    // Tab 1
    if (ImGui::BeginTabItem("  Camera Settings  ")){
        ImGui::ColorEdit4("Intensity", &color_multipler.x);

        ImGui::Text("Camera calibration parameters.");
        ImGui::Text("_____________________________________________");

        if(module_enabled && ready ){
            if(!stcam_node_tripple->record_images_EN ){

                ImGui::Text("_____________________________________________");
                ImGui::Text("Image Type");

                ImGui::Checkbox("Gray Image", &stcam_node_tripple->image_type_gray);
                ImGui::Checkbox("Color Image", &stcam_node_tripple->image_type_color);

                if(stcam_node_tripple->image_type_gray){
                  stcam_node_tripple->image_type_color=false;
                  stcam_node_tripple->cam1->image_type="gray";
                  stcam_node_tripple->cam2->image_type="gray";
                  stcam_node_tripple->cam3->image_type="gray";
                  stcam_node_tripple->cam4->image_type="gray";

                }

                if(stcam_node_tripple->image_type_color){
                  stcam_node_tripple->image_type_gray=false;
                  stcam_node_tripple->cam1->image_type="color";
                  stcam_node_tripple->cam2->image_type="color";
                  stcam_node_tripple->cam3->image_type="color";
                  stcam_node_tripple->cam4->image_type="color";

                }

                ImGui::Text("_____________________________________________");
                ImGui::Text("Image Format");

                ImGui::Checkbox("JPEG", &stcam_node_tripple->image_format_jpg);
                ImGui::Checkbox("PNG", &stcam_node_tripple->image_format_png);
                ImGui::Checkbox("BMP", &stcam_node_tripple->image_format_bmp);

                if(stcam_node_tripple->image_format_jpg){
                  stcam_node_tripple->image_format_png=false;
                  stcam_node_tripple->image_format_bmp=false;
                  stcam_node_tripple->cam1->image_format="jpg";
                  stcam_node_tripple->cam2->image_format="jpg";
                  stcam_node_tripple->cam3->image_format="jpg";
                  stcam_node_tripple->cam4->image_format="jpg";
                }

                if(stcam_node_tripple->image_format_png){
                  stcam_node_tripple->image_format_jpg=false;
                  stcam_node_tripple->image_format_bmp=false;
                  stcam_node_tripple->cam1->image_format="png";
                  stcam_node_tripple->cam2->image_format="png";
                  stcam_node_tripple->cam3->image_format="png";
                  stcam_node_tripple->cam4->image_format="png";
                }

                if(stcam_node_tripple->image_format_bmp){
                  stcam_node_tripple->image_format_jpg=false;
                  stcam_node_tripple->image_format_png=false;
                  stcam_node_tripple->cam1->image_format="BMP";
                  stcam_node_tripple->cam2->image_format="BMP";
                  stcam_node_tripple->cam3->image_format="BMP";
                  stcam_node_tripple->cam4->image_format="BMP";
                }

                ImGui::Text("_____________________________________________");

                ImGui::Text("Image Saved : %d/50",stcam_node_tripple->image_by_image_num );

                ImGui::Text("_____________________________________________");
                ImGui::Text("Click Save to save one image at a time");
                if (ImGui::Button(" Save ")){
                  SoundEngine->play2D("../sounds/start.wav", false);
                  folder_path=new_directory3();
                  stcam_node_tripple->folder_path=folder_path;
                  //stcam_node->save_valid_images(); // 2022 Jan 24
                  stcam_node_tripple->save_image();
                }

                ImGui::Text("Image Saved : %d/50",stcam_node_tripple->image_by_image_num2);


                ImGui::Text("_____________________________________________");
                ImGui::Text("Clear to overwrite old data");
                if (ImGui::Button("Clear ")){
                  SoundEngine->play2D("../sounds/start.wav", false);
                  stcam_node_tripple->image_by_image_num=0;
                  stcam_node_tripple->image_by_image_num2=0;
                }

            }else{

                ImGui::Text("Stop image recording to save image by image");
            }

        }else{
          ImGui::Text("Message::Stereo camera is not enabled.");
          ImGui::Text("Please enable camera to record images.");
        }

      ImGui::EndTabItem();
    }

    //Tab 2
    if (ImGui::BeginTabItem("   Record Images  ")){
      ImGui::Checkbox("Right Click Recording EN", &stcam_node_tripple->right_click_recording_EN);
      ImGui::Text("_____________________________________________");
      ImGui::Text("Saves Videos By Default (Recommended).");
      ImGui::Text("Check Here to Save Images.");
      ImGui::Checkbox("Save Images", &stcam_node_tripple->images_or_video);
      stcam_node_tripple->cam1->images_or_video=stcam_node_tripple->images_or_video;
      stcam_node_tripple->cam2->images_or_video=stcam_node_tripple->images_or_video;
      stcam_node_tripple->cam3->images_or_video=stcam_node_tripple->images_or_video;
      stcam_node_tripple->cam4->images_or_video=stcam_node_tripple->images_or_video;

      if(module_enabled && ready ){
          if(!stcam_node_tripple->record_images_EN ){

              ImGui::Text("_____________________________________________");
              ImGui::Text("Image Type");

              ImGui::Checkbox("Gray Image", &stcam_node_tripple->image_type_gray);
              if(stcam_node_tripple->image_type_gray){
                stcam_node_tripple->image_type_color=false;
                stcam_node_tripple->cam1->image_type="gray";
                stcam_node_tripple->cam2->image_type="gray";
                stcam_node_tripple->cam3->image_type="gray";
                stcam_node_tripple->cam4->image_type="gray";

              }
              ImGui::Checkbox("Color Image", &stcam_node_tripple->image_type_color);
              if(stcam_node_tripple->image_type_color){
                stcam_node_tripple->image_type_gray=false;
                stcam_node_tripple->cam1->image_type="color";
                stcam_node_tripple->cam2->image_type="color";
                stcam_node_tripple->cam3->image_type="color";
                stcam_node_tripple->cam4->image_type="color";

              }

              ImGui::Text("_____________________________________________");
              ImGui::Text("Image Format");

              ImGui::Checkbox("JPEG", &stcam_node_tripple->image_format_jpg);
              ImGui::SliderInt("Quality", &stcam_node_tripple->JPEG_q, 50, 100);
              stcam_node_tripple->cam1->JPEG_q=stcam_node_tripple->JPEG_q;
              stcam_node_tripple->cam2->JPEG_q=stcam_node_tripple->JPEG_q;
              stcam_node_tripple->cam3->JPEG_q=stcam_node_tripple->JPEG_q;
              if(stcam_node_tripple->image_format_jpg){
                stcam_node_tripple->image_format_png=false;
                stcam_node_tripple->cam1->image_format="jpg";
                stcam_node_tripple->cam2->image_format="jpg";
                stcam_node_tripple->cam3->image_format="jpg";
                stcam_node_tripple->cam4->image_format="jpg";
              }
              ImGui::Checkbox("PNG", &stcam_node_tripple->image_format_png);
              ImGui::SliderInt("Compression", &stcam_node_tripple->PNG_COMPRESSION, 0, 9);
              stcam_node_tripple->cam1->PNG_COMPRESSION=stcam_node_tripple->PNG_COMPRESSION;
              stcam_node_tripple->cam2->PNG_COMPRESSION=stcam_node_tripple->PNG_COMPRESSION;
              stcam_node_tripple->cam3->PNG_COMPRESSION=stcam_node_tripple->PNG_COMPRESSION;
              stcam_node_tripple->cam4->PNG_COMPRESSION=stcam_node_tripple->PNG_COMPRESSION;

              if(stcam_node_tripple->image_format_png){
                stcam_node_tripple->image_format_jpg=false;
                stcam_node_tripple->cam1->image_format="png";
                stcam_node_tripple->cam2->image_format="png";
                stcam_node_tripple->cam3->image_format="png";
                stcam_node_tripple->cam4->image_format="png";
              }

              ImGui::Text("_____________________________________________");
              ImGui::Text("Click Start to Record");

              if (ImGui::Button("Start") || stcam_node_tripple->right_click_recording_EN && stcam_node_tripple->right_click_pressed){
                stcam_node_tripple->right_click_pressed=false;
                SoundEngine->play2D("../sounds/start.wav", false);

                folder_path=new_directory2();

                std::stringstream source_file_name, target_file_name;
                source_file_name<<"../camera_node/calibration/cam1.txt";
                target_file_name<<folder_path<<"/cam1.txt";
                copyFile(source_file_name.str().c_str(), target_file_name.str().c_str());

                std::stringstream source_file_name2, target_file_name2;
                source_file_name2<<"../camera_node/calibration/cam2.txt";
                target_file_name2<<folder_path<<"/cam2.txt";
                copyFile(source_file_name2.str().c_str(), target_file_name2.str().c_str());

                std::stringstream source_file_name3, target_file_name3;
                source_file_name3<<"../camera_node/calibration/stereo.txt";
                target_file_name3<<folder_path<<"/stereo.txt";
                copyFile(source_file_name3.str().c_str(), target_file_name3.str().c_str());

                stcam_node_tripple->folder_path=folder_path;
                stcam_node_tripple->record_images_EN=true;
                mvlink_node->folder_path=folder_path;
                mvlink_node->data_save_EN=true;
                mvlink_node->save_data_init();

              }
              ImGui::Text("Camera view will be automatically ");
              ImGui::Text("disabled when recording is started.");

          }else {

              ImGui::Text("_____________________________________________");
              ImGui::Text("Click Stop to Stop Recording.");
              if (ImGui::Button("Stop ") || stcam_node_tripple->right_click_recording_EN && stcam_node_tripple->right_click_pressed){
                stcam_node_tripple->right_click_pressed=false;
                SoundEngine->play2D("../sounds/stop.wav", false);
                stcam_node_tripple->record_images_EN=false;
                mvlink_node->save_data_close();
                mvlink_node->data_save_EN=false;
              }

              ImGui::Text("Images are Saved to-");
              ImGui::Text("Location: %s", folder_path.c_str());

              ImGui::Text("Number of Images Cam   I: %d", stcam_node_tripple->image_num1);
              ImGui::Text("Number of Images Cam  II: %d", stcam_node_tripple->image_num2);
              ImGui::Text("Number of Images Cam III: %d", stcam_node_tripple->image_num3);
              ImGui::Text("Number of Images Cam  IV: %d", stcam_node_tripple->image_num4);

              ImGui::Text("Folder Size : %f GB", stcam_node_tripple->image_num1*2*stcam_node_tripple->image_file_size/(1024*1024));
              ImGui::Text("Saving FPS cam   I : %d", stcam_node_tripple->saving_fps);
              ImGui::Text("Saving FPS cam  II : %d", stcam_node_tripple->saving_fps2);
              ImGui::Text("Saving FPS cam III : %d", stcam_node_tripple->saving_fps3);
              ImGui::Text("Saving FPS cam  IV : %d", stcam_node_tripple->saving_fps4);
          }

      }else{
        ImGui::Text("Message::Stereo camera is not enabled.");
        ImGui::Text("Please enable camera to record images.");
      }
      ImGui::EndTabItem();
    }


  ImGui::EndTabBar();
  }
  //ImGui::Text("Camera Settings");

  ImGui::End(); //left window


}

void viz::module_stereo_camera(bool ready, bool module_enabled, bool module_camera_show_EN,
                               unsigned char* stb_image_in,unsigned char* stb_image_in2,
                               int width, int height, int channels, int top_trim, int bot_trim,
                                cv::Mat image1, cv::Mat image2){

                                 //trying to add lines on top of my image here:


  // camera view
  int camera_view_width=viz_param->camera_view_width*0.5;
  int camera_view_height=viz_param->camera_view_height*0.5;

  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  // Left Camera
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(camera_view_width+10, camera_view_height+10));
  ImGui::Begin("Camera 1", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  if(module_enabled && ready && module_camera_show_EN && !stcam_node->record_images_EN){
    int view_w1=camera_view_width;
    int view_h1=view_w1 * height/width;

    int view_h2=camera_view_height;
    int view_w2=width*view_h2/height;

    int view_h=240;
    int view_w=320;

    if(view_w1<=camera_view_width && view_h1<=camera_view_height){
      view_h=view_h1;
      view_w=view_w1;
    }

    if(view_w2<=camera_view_width && view_h2<=camera_view_height){
      view_h=view_h2;
      view_w=view_w2;
    }

    if(stcam_node->calibration_mod_on){
      cv::resize(image1, image1, cv::Size(view_w, view_h), cv::INTER_LINEAR);
      cvMAT_to_texture(image1, &image_texture);
      ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);
    }else{

        unsigned char* temp=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));;
        image_resize(stb_image_in, width, height, channels, temp, view_w, view_h, 0);
        unsigned char* temp2=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));
        memcpy(temp2, temp, view_w * view_h * channels* sizeof(unsigned char));
        stb_image_to_texture(temp2, view_w, view_h, &image_texture, 3);
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);

        float top_trim_=top_trim*(view_h*1.0f/height);
        float bot_trim_=bot_trim*(view_h*1.0f/height);

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        drawList->AddText(ImVec2(viz_param->menu2_windwo_width+30, 20+viz_param->menu1_windwo_height), IM_COL32(255, 255, 255, 255), "Camera 1");

        if(stcam_node->show_trimmed){
            drawList->AddRectFilled(ImVec2(viz_param->menu2_windwo_width, 20+viz_param->menu1_windwo_height),
                              ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+top_trim_),IM_COL32(128, 128, 128, 150), 1.0f);

            drawList->AddRectFilled(ImVec2(viz_param->menu2_windwo_width, 20+viz_param->menu1_windwo_height+camera_view_height+10-bot_trim_),
                              ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10), IM_COL32(128, 128, 128, 150), 1.0f);
        }
        freeStr(&temp);
        freeStr(&temp2);
    }

  }else{
        int image_width = 0;
        int image_height = 0;

        bool ret = LoadTextureFromFile("../3Dviz_image_studio/images/camera.png", &image_texture, &image_width, &image_height, 0.5);
        IM_ASSERT(ret);

        ImGui::SetCursorPos(ImVec2((camera_view_width-image_width)*0.5f, (camera_view_height-image_height)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture, ImVec2(image_width,image_height));
  }

  ImGui::End();
  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  //Right Camera
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(camera_view_width+10, camera_view_height+10));
  ImGui::Begin("Camera 2", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  if(module_enabled && ready && module_camera_show_EN && !stcam_node->record_images_EN){
    int view_w1=camera_view_width;
    int view_h1=view_w1 * height/width;

    int view_h2=camera_view_height;
    int view_w2=width*view_h2/height;

    int view_h=240;
    int view_w=320;

    if(view_w1<=camera_view_width && view_h1<=camera_view_height){
      view_h=view_h1;
      view_w=view_w1;
    }

    if(view_w2<=camera_view_width && view_h2<=camera_view_height){
      view_h=view_h2;
      view_w=view_w2;
    }


    if(stcam_node->calibration_mod_on){
      cv::resize(image2, image2, cv::Size(view_w, view_h), cv::INTER_LINEAR);
      cvMAT_to_texture(image2, &image_texture2);
      ImGui::Image((void*)(intptr_t)image_texture2, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);
    }else{

        unsigned char* temp=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));;
        image_resize(stb_image_in2, width, height, channels, temp, view_w, view_h, 0);
        unsigned char* temp2=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));
        memcpy(temp2, temp, view_w * view_h * channels* sizeof(unsigned char));
        stb_image_to_texture(temp2, view_w, view_h, &image_texture2, 3);
        ImGui::SetCursorPos(ImVec2((camera_view_width-view_w)*0.5f, (camera_view_height-view_h)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture2, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);

        float top_trim_=top_trim*(view_h*1.0f/height);
        float bot_trim_=bot_trim*(view_h*1.0f/height);

        ImDrawList* drawList2 = ImGui::GetWindowDrawList();

        drawList2->AddText(ImVec2(viz_param->menu2_windwo_width+camera_view_width+30, 20+viz_param->menu1_windwo_height), IM_COL32(255, 255, 255, 255), "Camera 2");

        if(stcam_node->show_trimmed){
            drawList2->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height),
                          ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+top_trim_),IM_COL32(128, 128, 128, 150), 1.0f);

            drawList2->AddRectFilled(ImVec2(viz_param->menu2_windwo_width+camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10-bot_trim_),
                      ImVec2(viz_param->menu2_windwo_width+2*camera_view_width, 20+viz_param->menu1_windwo_height+camera_view_height+10), IM_COL32(128, 128, 128, 150), 1.0f);
        }

        freeStr(&temp);
        freeStr(&temp2);
      }
  }else{
        int image_width = 0;
        int image_height = 0;

        bool ret = LoadTextureFromFile("../3Dviz_image_studio/images/camera.png", &image_texture2, &image_width, &image_height, 0.5);
        IM_ASSERT(ret);

        ImGui::SetCursorPos(ImVec2((camera_view_width-image_width)*0.5f, (camera_view_height-image_height)*0.5f));
        ImGui::Image((void*)(intptr_t)image_texture2, ImVec2(image_width,image_height));
  }

  ImGui::End();
  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  ///////////////////////////////////
  // left window
  ///////////////////////////////////
  ImGui::SetNextWindowPos(ImVec2(0, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(viz_param->menu2_windwo_width, viz_param->height-viz_param->menu1_windwo_height-20));
  ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  ImGui::Text("Image Resolution : %d x %d", stcam_node->width, stcam_node->height);
  ImGui::Text("Image Trim : Top %d px | Bottom %d px" , stcam_node->top_trim, stcam_node->bot_trim);

  //ImGui::Text("Frames per Second : %d", cam_node->saving_fps);

  if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){

    // Tab 1
    if (ImGui::BeginTabItem("  Camera Settings  ")){
        ImGui::ColorEdit4("Intensity", &color_multipler.x);

        ImGui::Text("Camera calibration parameters.");
        ImGui::Text("_____________________________________________");
        ImGui::Text("Record Images for camera calibration");
        ImGui::Checkbox("Calibartion Mod ON", &stcam_node->calibration_mod_on);

        if(module_enabled && ready ){
            if(!stcam_node->record_images_EN ){

                ImGui::Text("_____________________________________________");
                ImGui::Text("Image Type");

                ImGui::Checkbox("Gray Image", &stcam_node->image_type_gray);
                ImGui::Checkbox("Color Image", &stcam_node->image_type_color);

                if(stcam_node->image_type_gray){
                  stcam_node->image_type_color=false;
                  stcam_node->cam1->image_type="gray";
                  stcam_node->cam2->image_type="gray";

                }

                if(stcam_node->image_type_color){
                  stcam_node->image_type_gray=false;
                  stcam_node->cam1->image_type="color";
                  stcam_node->cam2->image_type="color";

                }

                ImGui::Text("_____________________________________________");
                ImGui::Text("Image Format");

                ImGui::Checkbox("JPEG", &stcam_node->image_format_jpg);
                ImGui::Checkbox("PNG", &stcam_node->image_format_png);

                if(stcam_node->image_format_jpg){
                  stcam_node->image_format_png=false;
                  stcam_node->cam1->image_format="jpg";
                  stcam_node->cam2->image_format="jpg";
                }

                if(stcam_node->image_format_png){
                  stcam_node->image_format_jpg=false;
                  stcam_node->cam1->image_format="png";
                  stcam_node->cam2->image_format="png";
                }

                ImGui::Text("_____________________________________________");
                ImGui::Checkbox("Save Valid Images Automatically", &stcam_node->automatic_saving_for_calibartion);

                if (stcam_node->automatic_saving_for_calibartion){
                  SoundEngine->play2D("../sounds/start.wav", false);
                  folder_path=new_directory3();
                  stcam_node->folder_path=folder_path;
                  stcam_node->save_valid_images();
                  //stcam_node->save_image();
                }else{
                  stcam_node->image_by_image_num=0;
                }


                ImGui::Text("Image Saved : %d/50",stcam_node->image_by_image_num );

                ImGui::Text("_____________________________________________");
                ImGui::Text("Click Save to save one image at a time");
                if (ImGui::Button(" Save ")){
                  SoundEngine->play2D("../sounds/start.wav", false);
                  folder_path=new_directory3();
                  stcam_node->folder_path=folder_path;
                  //stcam_node->save_valid_images(); // 2022 Jan 24
                  stcam_node->save_image();
                }

                ImGui::Text("Image Saved : %d/50",stcam_node->image_by_image_num2);


                ImGui::Text("_____________________________________________");
                ImGui::Text("Clear to overwrite old data");
                if (ImGui::Button("Clear ")){
                  SoundEngine->play2D("../sounds/start.wav", false);
                  stcam_node->image_by_image_num=0;
                  stcam_node->image_by_image_num2=0;
                }

            }else{

                ImGui::Text("Stop image recording to save image by image");
            }

        }else{
          ImGui::Text("Message::Stereo camera is not enabled.");
          ImGui::Text("Please enable camera to record images.");
        }

      ImGui::Text("_____________________________________________");
      ImGui::Text("Calibration parameters camera I ");
      ImGui::Text("Fx: %f", stcam_node->cam1->fx);
      ImGui::Text("Fy: %f", stcam_node->cam1->fy);
      ImGui::Text("Cx: %f", stcam_node->cam1->cx);
      ImGui::Text("Cy: %f", stcam_node->cam1->cy);
      ImGui::Text("Error: %f", stcam_node->cam1->re_error);

      if (ImGui::Button(" CLBRT I")){
        ImGui::Text("Calibrating camera I ");
        SoundEngine->play2D("../sounds/start.wav", false);
        stcam_node->cam1->calibrate("cam1");

      }

      ImGui::Text("_____________________________________________");
      ImGui::Text("Calibration parameters camera II ");

      ImGui::Text("Fx: %f", stcam_node->cam2->fx);
      ImGui::Text("Fy: %f", stcam_node->cam2->fy);
      ImGui::Text("Cx: %f", stcam_node->cam2->cx);
      ImGui::Text("Cy: %f", stcam_node->cam2->cy);
      ImGui::Text("Error: %f", stcam_node->cam2->re_error);

      if (ImGui::Button("CLBRT II")){
        SoundEngine->play2D("../sounds/start.wav", false);
        ImGui::Text("Calibrating camera II ");
        stcam_node->cam2->calibrate("cam2");

      }

      ImGui::Text("_____________________________________________");
      ImGui::Text("Calibration parameters Stereo");

      ImGui::Text("Fx: %f", stcam_node->fx);
      ImGui::Text("Fy: %f", stcam_node->fy);
      ImGui::Text("Cx: %f", stcam_node->cx);
      ImGui::Text("Cy: %f", stcam_node->cy);
      ImGui::Text("B : %f", stcam_node->b);;

      if (ImGui::Button("CLBRT ST")){
        SoundEngine->play2D("../sounds/start.wav", false);
        ImGui::Text("Calibrating camera ST ");
        stcam_node->calibrate();

      }

      ImGui::EndTabItem();
    }

    //Tab 2
    if (ImGui::BeginTabItem("   Record Images  ")){
      ImGui::Checkbox("Right Click Recording EN", &stcam_node->right_click_recording_EN);
      ImGui::Text("_____________________________________________");
      ImGui::Text("Saves Videos By Default (Recommended).");
      ImGui::Text("Check Here to Save Images.");
      ImGui::Checkbox("Save Images", &stcam_node->images_or_video);


      if(module_enabled && ready ){
          if(!stcam_node->record_images_EN ){

              ImGui::Text("_____________________________________________");
              ImGui::Text("Image Type");

              ImGui::Checkbox("Gray Image", &stcam_node->image_type_gray);
              if(stcam_node->image_type_gray){
                stcam_node->image_type_color=false;
                stcam_node->cam1->image_type="gray";
                stcam_node->cam2->image_type="gray";

              }
              ImGui::Checkbox("Color Image", &stcam_node->image_type_color);
              if(stcam_node->image_type_color){
                stcam_node->image_type_gray=false;
                stcam_node->cam1->image_type="color";
                stcam_node->cam2->image_type="color";

              }

              ImGui::Text("_____________________________________________");
              ImGui::Text("Image Format");

              ImGui::Checkbox("JPEG", &stcam_node->image_format_jpg);
              ImGui::SliderInt("Quality", &stcam_node->JPEG_q, 50, 100);
              stcam_node->cam1->JPEG_q=stcam_node->JPEG_q;
              stcam_node->cam2->JPEG_q=stcam_node->JPEG_q;
              if(stcam_node->image_format_jpg){
                stcam_node->image_format_png=false;
                stcam_node->cam1->image_format="jpg";
                stcam_node->cam2->image_format="jpg";
              }
              ImGui::Checkbox("PNG", &stcam_node->image_format_png);
              ImGui::SliderInt("Compression", &stcam_node->PNG_COMPRESSION, 0, 9);
              stcam_node->cam1->PNG_COMPRESSION=stcam_node->PNG_COMPRESSION;
              stcam_node->cam2->PNG_COMPRESSION=stcam_node->PNG_COMPRESSION;

              if(stcam_node->image_format_png){
                stcam_node->image_format_jpg=false;
                stcam_node->cam1->image_format="png";
                stcam_node->cam2->image_format="png";
              }

              ImGui::Text("_____________________________________________");
              ImGui::Text("Click Start to Record");

              if (ImGui::Button("Start") || stcam_node->right_click_recording_EN && stcam_node->right_click_pressed){
                stcam_node->right_click_pressed=false;
                SoundEngine->play2D("../sounds/start.wav", false);

                folder_path=new_directory2();

                std::stringstream source_file_name, target_file_name;
                source_file_name<<"../camera_node/calibration/cam1.txt";
                target_file_name<<folder_path<<"/cam1.txt";
                copyFile(source_file_name.str().c_str(), target_file_name.str().c_str());

                std::stringstream source_file_name2, target_file_name2;
                source_file_name2<<"../camera_node/calibration/cam2.txt";
                target_file_name2<<folder_path<<"/cam2.txt";
                copyFile(source_file_name2.str().c_str(), target_file_name2.str().c_str());

                std::stringstream source_file_name3, target_file_name3;
                source_file_name3<<"../camera_node/calibration/stereo.txt";
                target_file_name3<<folder_path<<"/stereo.txt";
                copyFile(source_file_name3.str().c_str(), target_file_name3.str().c_str());

                stcam_node->folder_path=folder_path;
                stcam_node->record_images_EN=true;
                mvlink_node->folder_path=folder_path;
                mvlink_node->data_save_EN=true;
                mvlink_node->save_data_init();

              }
              ImGui::Text("Camera view will be automatically ");
              ImGui::Text("disabled when recording is started.");

          }else {

              ImGui::Text("_____________________________________________");
              ImGui::Text("Click Stop to Stop Recording.");
              if (ImGui::Button("Stop ") || stcam_node->right_click_recording_EN && stcam_node->right_click_pressed){
                stcam_node->right_click_pressed=false;
                SoundEngine->play2D("../sounds/stop.wav", false);
                stcam_node->record_images_EN=false;
                stcam_node->image_num=0;
                mvlink_node->save_data_close();
                mvlink_node->data_save_EN=false;
              }

              ImGui::Text("Images are Saved to-");
              ImGui::Text("Location: %s", folder_path.c_str());

              ImGui::Text("Number of Images Cam  I: %d", stcam_node->image_num);
              ImGui::Text("Number of Images Cam II: %d", stcam_node->image_num2);
              ImGui::Text("Folder Size : %f GB", stcam_node->image_num*2*stcam_node->image_file_size/(1024*1024));
              ImGui::Text("Saving FPS cam I : %d", stcam_node->saving_fps);
              ImGui::Text("Saving FPS cam II : %d", stcam_node->saving_fps2);
          }

      }else{
        ImGui::Text("Message::Stereo camera is not enabled.");
        ImGui::Text("Please enable camera to record images.");
      }
      ImGui::EndTabItem();
    }


  ImGui::EndTabBar();
  }
  //ImGui::Text("Camera Settings");

  ImGui::End(); //left window


}

void viz::module_camera(bool ready, bool module_enabled, bool module_camera_show_EN, unsigned char* stb_image_in, int width, int height, int channels){

  // camera view
  ImGui::SetNextWindowPos(ImVec2(viz_param->menu2_windwo_width, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(viz_param->camera_view_width+10, viz_param->camera_view_height+10));
  ImGui::Begin("Camera View", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

  if(module_enabled && ready && module_camera_show_EN && !cam_node->record_images_EN){
      // resize
      if(width>viz_param->camera_view_width||height>viz_param->camera_view_height){

            int view_w1=viz_param->camera_view_width;
            int view_h1=view_w1 * height/width;

            int view_h2=viz_param->camera_view_height;
            int view_w2=width*view_h2/height;

            int view_h=480;
            int view_w=640;

            if(view_w1<=viz_param->camera_view_width && view_h1<=viz_param->camera_view_height){
              view_h=view_h1;
              view_w=view_w1;
            }

            if(view_w2<=viz_param->camera_view_width && view_h2<=viz_param->camera_view_height){
              view_h=view_h2;
              view_w=view_w2;
            }

            unsigned char* temp=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));;
            image_resize(stb_image_in, width, height, channels, temp, view_w, view_h, 0);
            unsigned char* temp2=(unsigned char*)calloc(view_w*view_h*channels, sizeof(unsigned char));
            memcpy(temp2, temp, view_w * view_h * channels* sizeof(unsigned char));
            stb_image_to_texture(temp2, view_w, view_h, &image_texture, 3);
            ImGui::SetCursorPos(ImVec2((viz_param->camera_view_width-view_w)*0.5f, (viz_param->camera_view_height-view_h)*0.5f));
            ImGui::Image((void*)(intptr_t)image_texture, ImVec2(view_w,view_h),ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);

            freeStr(&temp);
            freeStr(&temp2);

        // No resizing
        }else{

            unsigned char* temp=(unsigned char*)calloc(width*height*channels, sizeof(unsigned char));
            memcpy(temp, stb_image_in, width * height * channels* sizeof(unsigned char));

            stb_image_to_texture(temp, width, height, &image_texture, channels);
            ImGui::SetCursorPos(ImVec2((viz_param->camera_view_width-width)*0.5f, (viz_param->camera_view_height-height)*0.5f));
            ImGui::Image((void*)(intptr_t)image_texture, ImVec2(width,height), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),color_multipler);

            freeStr(&temp);

        }

  }else{

    int image_width = 0;
    int image_height = 0;

    bool ret = LoadTextureFromFile("../3Dviz_image_studio/images/camera.png", &image_texture, &image_width, &image_height);
    IM_ASSERT(ret);

    ImGui::SetCursorPos(ImVec2((viz_param->camera_view_width-image_width)*0.5f, (viz_param->camera_view_height-image_height)*0.5f));
    ImGui::Image((void*)(intptr_t)image_texture, ImVec2(image_width,image_height));
  }

  ImGui::End();

  // left window
  ImGui::SetNextWindowPos(ImVec2(0, 20+viz_param->menu1_windwo_height));
  ImGui::SetNextWindowSize(ImVec2(viz_param->menu2_windwo_width, viz_param->height-viz_param->menu1_windwo_height-20));
  ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);


  ImGui::Text("Image Resolution  : %d x %d", cam_node->width, cam_node->height);
  //ImGui::Text("Frames per Second : %d", cam_node->saving_fps);

  if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)){

    // Tab 1
    if (ImGui::BeginTabItem("  Camera Settings  ")){
      ImGui::EndTabItem();

      ImGui::ColorEdit4("Intensity", &color_multipler.x);
    }

    //Tab 2
    if (ImGui::BeginTabItem("Record Images")){
      //ImGui::SetCursorPos(ImVec2(50,50));
      if(module_enabled && ready ){

          if(!cam_node->record_images_EN && module_enabled && ready ){

            ImGui::Text("Click Start to Record");

            if (ImGui::Button("Start")){
              folder_path=new_directory();
              cam_node->folder_path=folder_path;
              cam_node->record_images_EN=true;

            }
            ImGui::Text("Camera view will be automatically ");
            ImGui::Text("disabled when recording is started.");

          }else{
            ImGui::Text("Click Stop to Stop Recording.");

            if (ImGui::Button("Stop ")){
              cam_node->record_images_EN=false;
              cam_node->image_num=0;

            }

            ImGui::Text("Images are Saved to-");
            ImGui::Text("Location: %s", folder_path.c_str());

            ImGui::Text("Number of Images : %d", cam_node->image_num);
            ImGui::Text("Saving frames per Second : %d", cam_node->saving_fps);


          }
        }else{
          ImGui::Text("Message:: Camera is not enabled.");
          ImGui::Text("Please enable camera to record images.");
        }

      ImGui::EndTabItem();
    }


  ImGui::EndTabBar();
  }
  //ImGui::Text("Camera Settings");
  ImGui::End();

  stb_image_in=NULL;
}
