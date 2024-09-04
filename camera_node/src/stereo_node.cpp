#include "camera_node.h"

stereo_node::stereo_node(){

}

int stereo_node::init(){

  if(!initialized){
      camera* camera1=new camera;
      int cam_no=camera1->init(0);

      camera* camera2=new camera;
      int cam_no2=camera2->init(2);

      if(cam_no==1){
        cam2=camera1;
        cam2->set_params(width, height,2, top_trim, bot_trim, image_format);
      }else{
        cam1=camera1;
        cam1->set_params(width, height,1, top_trim, bot_trim, image_format);
      }

      if(cam_no2==2){
        cam1=camera2;
        cam1->set_params(width, height,1, top_trim, bot_trim, image_format);
      }else{
        cam2=camera2;
        cam2->set_params(width, height,2, top_trim, bot_trim, image_format);
      }

      //cam1->init(0);
      //cam1->set_params(width, height);

      //cam2->init(2);
      //cam2->set_params(width, height);


      t = clock(); //start time
      initialized=true;
    }
}

stereo_node::~stereo_node(){

}


void stereo_node::save_image(){

  cam1->save_image(buffer1_flag, folder_path , "cam1", image_by_image_num2 );
  cam2->save_image(buffer2_flag, folder_path , "cam2", image_by_image_num2 );
  image_by_image_num2++;

}

int stereo_node::calibrate(){
  // Defining the dimensions of checkerboard
  int CHECKERBOARD[2]{5,8};

  // Creating vector to store vectors of 3D points for each checkerboard image
  std::vector<std::vector<cv::Point3f> > objpoints;

  // Creating vector to store vectors of 2D points for each checkerboard image
  std::vector<std::vector<cv::Point2f> > imgpointsL, imgpointsR;

  // Defining the world coordinates for 3D points
  double cube_size=77;
  std::vector<cv::Point3f> objp;
  for(int i{0}; i<CHECKERBOARD[1]; i++)
  {
    for(int j{0}; j<CHECKERBOARD[0]; j++)
      objp.push_back(cv::Point3f(j*cube_size,i*cube_size,0));
  }

  // Extracting path of individual image stored in a given directory
  std::vector<cv::String> imagesL, imagesR;
  // Path of the folder containing checkerboard images

  std::string pathL = "../../ImageDATA/stereo/calibration_data/cam2/*.jpg";
  std::string pathR = "../../ImageDATA/stereo/calibration_data/cam1/*.jpg";

  cv::glob(pathL, imagesL);
  cv::glob(pathR, imagesR);

  cv::Mat frameL, frameR, grayL, grayR;
  // vector to store the pixel coordinates of detected checker board corners
  std::vector<cv::Point2f> corner_ptsL, corner_ptsR;
  bool successL, successR;

  // Looping over all the images in the directory
  for(int i{0}; i<imagesL.size(); i++)
  {
    frameL = cv::imread(imagesL[i]);
    cv::cvtColor(frameL,grayL,cv::COLOR_BGR2GRAY);

    frameR = cv::imread(imagesR[i]);
    cv::cvtColor(frameR,grayR,cv::COLOR_BGR2GRAY);

    // Finding checker board corners
    // If desired number of corners are found in the image then success = true
    successL = cv::findChessboardCorners(
      grayL,
      cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]),
      corner_ptsL);
      // cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

    successR = cv::findChessboardCorners(
      grayR,
      cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]),
      corner_ptsR);
      // cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
    /*
      * If desired number of corner are detected,
      * we refine the pixel coordinates and display
      * them on the images of checker board
    */
    if((successL) && (successR))
    {
      cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

      // refining pixel coordinates for given 2d points.
      cv::cornerSubPix(grayL,corner_ptsL,cv::Size(11,11), cv::Size(-1,-1),criteria);
      cv::cornerSubPix(grayR,corner_ptsR,cv::Size(11,11), cv::Size(-1,-1),criteria);

      // Displaying the detected corner points on the checker board
      cv::drawChessboardCorners(frameL, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_ptsL,successL);
      cv::drawChessboardCorners(frameR, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_ptsR,successR);

      objpoints.push_back(objp);
      imgpointsL.push_back(corner_ptsL);
      imgpointsR.push_back(corner_ptsR);
    }
  }

  cv::Mat mtxL,distL,R_L,T_L;
  cv::Mat mtxR,distR,R_R,T_R;
  cv::Mat Rot, Trns, Emat, Fmat;
  cv::Mat new_mtxL, new_mtxR;

  // Calibrating left camera
  cv::calibrateCamera(objpoints,
                      imgpointsL,
                      grayL.size(),
                      mtxL,
                      distL,
                      R_L,
                      T_L);

  new_mtxL = cv::getOptimalNewCameraMatrix(mtxL,
                                distL,
                                grayL.size(),
                                1,
                                grayL.size(),
                                0);

  // Calibrating right camera
  cv::calibrateCamera(objpoints,
                      imgpointsR,
                      grayR.size(),
                      mtxR,
                      distR,
                      R_R,
                      T_R);

  new_mtxR = cv::getOptimalNewCameraMatrix(mtxR,
                                distR,
                                grayR.size(),
                                1,
                                grayR.size(),
                                0);


  cv::Mat STRot, STTrns, STEmat, STFmat;
  int flag = 0;
  flag |= cv::CALIB_FIX_INTRINSIC;

  // This step is performed to transformation between the two cameras and calculate Essential and
  // Fundamenatl matrix
  cv::stereoCalibrate(objpoints,
                      imgpointsL,
                      imgpointsR,
                      new_mtxL,
                      distL,
                      new_mtxR,
                      distR,
                      grayR.size(),
                      STRot,
                      STTrns,
                      STEmat,
                      STFmat,
                      flag,
                      cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 1e-6));

  // std::cout<< "R "<<STRot<<std::endl<<std::endl;
  // std::cout<< "T "<<STTrns<<std::endl<<std::endl;
  // std::cout<< "STEmat "<<STEmat<<std::endl<<std::endl;
  // std::cout<< "STFmat "<<STFmat<<std::endl<<std::endl;

  std::stringstream file_name;
  file_name<<"../camera_node/calibration/stereo.txt";
  std::ofstream califile;
  califile.open (file_name.str().c_str());

  std::stringstream line;

  line<<"RM "<<STRot.at<double>(0,0)<< " "<<STRot.at<double>(0,1) << " " << STRot.at<double>(0,2)<< " "
             <<STRot.at<double>(1,0)<< " "<<STRot.at<double>(1,1) << " " << STRot.at<double>(1,2)<< " "
             <<STRot.at<double>(2,0)<< " "<<STRot.at<double>(2,1) << " " << STRot.at<double>(2,2)<<" \n"

      <<"TV "<<STTrns.at<double>(0,0)<< " "<<STTrns.at<double>(1,0) << " " << STTrns.at<double>(2,0)<<" \n"

      <<"EM "<<STEmat.at<double>(0,0)<< " "<<STEmat.at<double>(0,1) << " " << STEmat.at<double>(0,2)<< " "
             <<STEmat.at<double>(1,0)<< " "<<STEmat.at<double>(1,1) << " " << STEmat.at<double>(1,2)<< " "
             <<STEmat.at<double>(2,0)<< " "<<STEmat.at<double>(2,1) << " " << STEmat.at<double>(2,2)<<" \n"

      <<"FM "<<STFmat.at<double>(0,0)<< " "<<STFmat.at<double>(0,1) << " " << STFmat.at<double>(0,2)<< " "
             <<STFmat.at<double>(1,0)<< " "<<STFmat.at<double>(1,1) << " " << STFmat.at<double>(1,2)<< " "
             <<STFmat.at<double>(2,0)<< " "<<STFmat.at<double>(2,1) << " " << STFmat.at<double>(2,2)<<" \n";

  califile<<line.str().c_str();

  califile.close();


}
void stereo_node::save_valid_images(){

  if(image_by_image_num==0){
    start = std::chrono::system_clock::now();
  }

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;

  if(valid_for_calibration){

    if((int)elapsed_seconds.count()>=image_by_image_num &&image_by_image_num<=50){
        std::stringstream image_name;
        image_name<< folder_path<<"/cam1/img"<<image_by_image_num<<".jpg";
        cv::imwrite(image_name.str().c_str(), cv_image1_original, {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, JPEG_q});

        std::stringstream image_name2;
        image_name2<< folder_path<<"/cam2/img"<<image_by_image_num<<".jpg";
        cv::imwrite(image_name2.str().c_str(), cv_image2_original, {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, JPEG_q});
        image_by_image_num++;
    }
  }
  valid_for_calibration=false;
}

void stereo_node::find_checkboard_patterns(cv::Mat& src, cv::Mat& dst , bool& found){


}

void stereo_node::run(){

  if(module_enabled){
    if(!running){
      running=true;
      // Start camera streamig threads here
      // Camera 1
      std::thread cam1_thread(&camera::get_sync_data, cam1,
      std::ref(module_enabled),
      std::ref(running),
      std::ref(buffer1_flag),
      std::ref(st_sync_flag));
      cam1_thread.detach();

      //camera 2
      std::thread cam2_thread(&camera::get_sync_data, cam2,
      std::ref(module_enabled),
      std::ref(running),
      std::ref(buffer2_flag),
      std::ref(st_sync_flag));
      cam2_thread.detach();

    }else{

        if(record_images_EN){
            if(!recording){
              st_sync_flag=false;
              recording=true;

              // Start recording thread here
              if(images_or_video){
                  std::thread cam1_rec_thread(&camera::save_image_m,
                    cam1,std::ref(record_images_EN),
                    std::ref(buffer1_flag),
                    std::ref(folder_path),
                    std::ref(image_num),
                    std::ref(image_num2),
                    std::ref(st_sync_flag));
                  cam1_rec_thread.detach();

                  std::thread cam2_rec_thread(&camera::save_image_s,
                    cam2,std::ref(record_images_EN),
                    std::ref(buffer2_flag),
                    std::ref(folder_path),
                    std::ref(image_num),
                    std::ref(image_num2),
                    std::ref(st_sync_flag));
                  cam2_rec_thread.detach();
              }else{
                  std::thread cam1_rec_thread(&camera::save_video_m,
                    cam1,std::ref(record_images_EN),
                    std::ref(buffer1_flag),
                    std::ref(folder_path),
                    std::ref(image_num),
                    std::ref(image_num2),
                    std::ref(st_sync_flag));
                  cam1_rec_thread.detach();

                  std::thread cam2_rec_thread(&camera::save_video_s,
                    cam2,std::ref(record_images_EN),
                    std::ref(buffer2_flag),
                    std::ref(folder_path),
                    std::ref(image_num),
                    std::ref(image_num2),
                    std::ref(st_sync_flag));
                  cam2_rec_thread.detach();
              }

              usleep(10000);

            }else{

              if(!st_sync_flag){
                st_sync_flag=true;
              }

              if(buffer1_flag){
                saving_fps=cam1->fps;
              }
              if(buffer2_flag){
                saving_fps2=cam2->fps;
              }

            }

        }else{

          if(calibration_mod_on){
            bool found1=false;
            bool found2=false;

            //clear data
            st_sync_flag=true;
            //camera 1

            if(!buffer1_flag){
              cv_image1_original.release();
              cv_image1.release();
              cv::Mat cv_image=cv::Mat(width, height, CV_8UC3, cam1->buffer);
              cv_image1_original= cv::imdecode(cv_image,cv::IMREAD_COLOR);
              buffer1_flag=true;

            }

            //camera 1I
            if(!buffer2_flag){
              cv_image2_original.release();
              cv_image2.release();
              cv::Mat cv_image=cv::Mat(width, height, CV_8UC3, cam2->buffer);
              cv_image2_original= cv::imdecode(cv_image,cv::IMREAD_COLOR);
              buffer2_flag=true;
            }

            std::thread fcp1(&stereo_node::find_checkboard_patterns,this, std::ref(cv_image1_original),std::ref(cv_image1),std::ref(found1));
            std::thread fcp2(&stereo_node::find_checkboard_patterns,this, std::ref(cv_image2_original),std::ref(cv_image2),std::ref(found2));
            fcp1.join();
            fcp2.join();

            valid_for_calibration=found1 && found2;

            ready1=true;
            ready2=true;
            ready=ready1 && ready2;

          }else{
            image_by_image_num=0;
            st_sync_flag=true;
              //camera 1
              if(!buffer1_flag){
                //using stbi image
                freeStr(&image_data1);
                image_data1=load_image_from_memory(cam1->buffer, width * height * channels*sizeof(unsigned char), width, height, channels, channels);
                buffer1_flag=true;
                ready1=true;
              }

              //camera 2
              if(!buffer2_flag){
                freeStr(&image_data2);
                image_data2=load_image_from_memory(cam2->buffer, width * height * channels*sizeof(unsigned char), width, height, channels, channels);
                buffer2_flag=true;
                ready2=true;
              }

              ready=ready1 && ready2;
              recording=false;
          }
        }
    }

  }else{
    if(running){
      running=false;
      buffer1_flag=true;
      ready1=false;
      buffer2_flag=true;
      ready2=false;
      ready=false;
      recording=false;
    }
  }

}
