

int state = 0;
/*
state
0 = idle

*/
  unsigned long lastTime = 0;
  unsigned long time = 0; 
void setup() {
  initLCD();
  startFona();
  time = millis();
}

void loop() {

    time = millis();
  
    switch (state){
    case 0: {
      if (blockInfo(time)) { 
        main_menu();
      }
      break;
    }
}  
    loopFona();
}

#define BLOCK_INTERVAL 10000
boolean blockInfo(unsigned long time){
 if((time - lastTime) >= BLOCK_INTERVAL){
     lastTime = time;
    return true;
 }else{
   return false;
 }
}
