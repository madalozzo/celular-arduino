

int8_t state = 0;
int8_t lastState = -1;

#define STATE_IDLE 0

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
    case STATE_IDLE: {
      if (blockInfo(time)) { 
        main_menu(lastState);
        lastState = STATE_IDLE;
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
