
#include "viz.h"


int main(int argc, char* argv[]){


  viz GUI;
  GUI.init();
  GUI.run();
  GUI.~viz();


  return 0;
}
