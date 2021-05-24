//LCD.BacklightConf(LOAD_TO_RAM, 20);
//LCD
#include <Wire.h>
#include <I2C_LCD.h>

I2C_LCD LCD;
uint8_t I2C_LCD_ADDRESS = 0x51; //Device address configuration, the default value is 0x51.



void initLCD(){
    Wire.begin();
    LCD.CleanAll(WHITE);    //Clean the screen with black or white.
      //8*16 font size, auto new line, black character on white back ground.
    LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
    LCD.CharGotoXY(0,0);
    LCD.BacklightConf(LOAD_TO_RAM, 20);
    LCD.println(F("Inicializando..."));
}

void navMenu (String ok, String nok) {
  LCD.DrawRectangleAt(0, 56, 127, 63, WHITE_FILL);
  LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
  LCD.CharGotoXY(0,56);
  LCD.print(ok);
  int local = 127 - (nok.length()*6);
  LCD.CharGotoXY(local, 56);
  LCD.print(nok);  
}


void main_menu(int8_t lastState){
  if (lastState != STATE_IDLE) {
    LCD.CleanAll(WHITE);   
    LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
  }
  
  
  
  // battery
  LCD.CharGotoXY(100,0);
  uint16_t battery = getBattery();
  LCD.print(battery); 
  LCD.print('%'); 
    
  // signal
  LCD.CharGotoXY(70,0);
  LCD.print(getRSSI()); 

  //network status
  LCD.CharGotoXY(0,32);
  LCD.print(networkStatus()); 
  navMenu("menu", "");

  int8_t smsnum = getSMSNumber();
  if (smsnum == 0) {
    Serial.println(F("no SMSs"));
  } else {
    drawMail(0,1);
    LCD.CharGotoXY(12,0);
    LCD.print(smsnum);
  }
  
}


void lcdRinging(char phoneNumber){
  LCD.CleanAll(WHITE);
  LCD.FontModeConf(Font_6x8, FM_ANL_AAA, BLACK_BAC); 
  LCD.CharGotoXY(0,0);
   
  LCD.println(F("RING!"));
  LCD.print(F("Phone Number:"));
  String myString = String(phoneNumber);
  LCD.println(myString);
  navMenu ("atender", "recusar");
}

void drawMail(int x, int y){
  LCD.DrawRectangleAt(x, y, 10, 6, BLACK_NO_FILL);
  LCD.DrawDotAt(x+1, y+1, BLACK);
  LCD.DrawDotAt(x+2, y+2, BLACK);
  LCD.DrawDotAt(x+3, y+2, BLACK);
  LCD.DrawDotAt(x+4, y+3, BLACK);
  LCD.DrawDotAt(x+5, y+3, BLACK);
  LCD.DrawDotAt(x+6, y+2, BLACK);
  LCD.DrawDotAt(x+7, y+2, BLACK);
  LCD.DrawDotAt(x+8, y+1, BLACK);
  
}

void drawBatt(int x, int y){
  LCD.DrawRectangleAt(x, y+1, 4, 6, BLACK_NO_FILL);
  LCD.DrawHLineAt(x+1, x+2, y, BLACK);
  
}

void drawAnt(int x, int y){
  LCD.DrawHLineAt(x, x+4, y, BLACK);
  LCD.DrawVLineAt(y+1, y + 5, x+2, BLACK);
  LCD.DrawDotAt(x + 0, y + 1, BLACK);
  LCD.DrawDotAt(x + 1, y + 2, BLACK);
  LCD.DrawDotAt(x + 3, y + 2, BLACK);
  LCD.DrawDotAt(x + 4, y + 1, BLACK);
}
