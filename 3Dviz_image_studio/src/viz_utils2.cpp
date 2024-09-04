
#include "viz_utils2.h"
// copy in binary mode
bool copyFile(const char *SRC, const char* DEST){
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

std::string new_directory(){

  //Main directory
  std::stringstream  path;
  path <<"../../ImageDATA";
  std::ifstream f(path.str().c_str());
  if(f.good()){
  }
  else{
    mkdir(path.str().c_str(), 0777);
  }

  std::stringstream  path2;
  path2 <<path.str().c_str()<<"/monocular";
  std::ifstream f2(path2.str().c_str());
  if(f2.good()){
  }
  else{
    mkdir(path2.str().c_str(), 0777);
  }

  std::stringstream pathx;
  int n=1;
  while(true){
    std::stringstream path3;
    path3 <<path2.str().c_str()<<"/data"<<n;
    std::ifstream f3(path3.str().c_str());
    if(f3.good()){
      n++;
    }
    else{
      mkdir(path3.str().c_str(), 0777);
      pathx<<path3.str().c_str();
      break;
    }
  }

  // sub folder
  std::stringstream path4;
  path4<<pathx.str().c_str()<<"/left";
  mkdir(path4.str().c_str(), 0777);

  return pathx.str().c_str();
}

std::string new_directory2(){

  //Main directory
  std::stringstream  path;
  path <<"../../ImageDATA";
  std::ifstream f(path.str().c_str());
  if(f.good()){
  }
  else{
    mkdir(path.str().c_str(), 0777);
  }

  std::stringstream  path2;
  path2 <<path.str().c_str()<<"/stereo";
  std::ifstream f2(path2.str().c_str());
  if(f2.good()){
  }
  else{
    mkdir(path2.str().c_str(), 0777);
  }

  std::stringstream pathx;
  int n=1;
  while(true){
    std::stringstream path3;
    path3 <<path2.str().c_str()<<"/data"<<n;
    std::ifstream f3(path3.str().c_str());
    if(f3.good()){
      n++;
    }
    else{
      mkdir(path3.str().c_str(), 0777);
      pathx<<path3.str().c_str();
      break;
    }
  }

  // sub folder
  std::stringstream path4;
  path4<<pathx.str().c_str()<<"/cam1";
  mkdir(path4.str().c_str(), 0777);

  std::stringstream path5;
  path5<<pathx.str().c_str()<<"/cam2";
  mkdir(path5.str().c_str(), 0777);

  std::stringstream path6;
  path6<<pathx.str().c_str()<<"/cam3";
  mkdir(path6.str().c_str(), 0777);

  std::stringstream path7;
  path7<<pathx.str().c_str()<<"/cam4";
  mkdir(path7.str().c_str(), 0777);

  return pathx.str().c_str();
}

std::string new_directory3(){

  //Main directory
  std::stringstream  path;
  path <<"../../ImageDATA";
  std::ifstream f(path.str().c_str());
  if(f.good()){
  }
  else{
    mkdir(path.str().c_str(), 0777);
  }

  std::stringstream  path2;
  path2 <<path.str().c_str()<<"/stereo";
  std::ifstream f2(path2.str().c_str());
  if(f2.good()){
  }
  else{
    mkdir(path2.str().c_str(), 0777);
  }

  std::stringstream path3;
  path3 <<path2.str().c_str()<<"/calibration_data";
  mkdir(path3.str().c_str(), 0777);

  // sub folder
  std::stringstream path4;
  path4<<path3.str().c_str()<<"/cam1";
  mkdir(path4.str().c_str(), 0777);

  std::stringstream path5;
  path5<<path3.str().c_str()<<"/cam2";
  mkdir(path5.str().c_str(), 0777);

  std::stringstream path6;
  path6<<path3.str().c_str()<<"/cam3";
  mkdir(path6.str().c_str(), 0777);

  std::stringstream path7;
  path7<<path3.str().c_str()<<"/cam4";
  mkdir(path7.str().c_str(), 0777);

  return path3.str().c_str();
}
