#include "include/global.h"

Bounce debouncerBP0  = Bounce(BP0,5);
Bounce debouncerBP1  = Bounce(BP1,5);
Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce btn[3] = {debouncerBP0,debouncerBP1,debouncerBPEN};

int getBtn(){
  bool btnVal[2] = {false,false};
  btn[0].update();
  btn[1].update();
  btn[2].update();

  if(btn[2].rose()){
    btnVal[0] = btn[0].read();
    btnVal[1] = btn[1].read();

    if     ( !btnVal[1] && !btnVal[0])
      return 1;
    else if( !btnVal[1] &&  btnVal[0])
      return 2;
    else if( btnVal[1]  && !btnVal[0])
      return 3;
    else if( btnVal[1]  &&  btnVal[0])
      return 4;
    else
      return -1;
  }
  else
    return 0;
}
