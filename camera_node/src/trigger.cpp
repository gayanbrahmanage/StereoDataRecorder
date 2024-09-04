#include <trigger.h>

trigger::trigger(){

}

trigger::~trigger(){

}

int trigger::init(){
    serial_port = open("/dev/ttyACM0", O_RDWR);
   //Opening device file
}

void trigger::terminate(){

  close(serial_port);

}

void trigger::send_trigger(){


  unsigned char msg[] = {'5'};
  write(serial_port, msg, sizeof(msg));

  std::cout<<"sent trigger"<<std::endl;

}
