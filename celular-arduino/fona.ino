/***************************************************
  This is an example for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963
  ----> http://www.adafruit.com/products/2468
  ----> http://www.adafruit.com/products/2542

  These cellular modules use TTL Serial to communicate, 2 pins are
  required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/*
THIS CODE IS STILL IN PROGRESS!

Open up the serial console on the Arduino at 115200 baud to interact with FONA

Note that if you need to set a GPRS APN, username, and password scroll down to
the commented section below at the end of the setup() function.
*/
#include "Adafruit_FONA.h"

#define FONA_RX 13
#define FONA_TX 3
#define FONA_RST 4

// this is a large buffer for replies
char replybuffer[255];

// We default to using software serial. If you want to use hardware serial
// (because softserial isnt supported) comment out the following three lines 
// and uncomment the HardwareSerial line
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Hardware serial is also possible!
//  HardwareSerial *fonaSerial = &Serial1;

// Use this for FONA 800 and 808s
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
// Use this one for FONA 3G
//Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

uint8_t type;




// Note you need to map interrupt number to pin number
// for your board.  On an Uno & Mega interrupt 0 is
// digital pin 2, and on a Leonardo interrupt 0 is
// digital pin 3.  See this page for a complete table:
//   http://arduino.cc/en/Reference/attachInterrupt
// Make sure this interrupt pin is connected to FONA RI!
#define FONA_RI_INTERRUPT  0


void startFona(){
  while (!Serial);

  Serial.begin(19200);
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default: 
      Serial.println(F("???")); break;
  }

      // Enable incoming call notification.
  if(fona.callerIdNotification(true, FONA_RI_INTERRUPT)) {
    Serial.println(F("Caller id notification enabled."));
  }
  else {
    Serial.println(F("Caller id notification disabled"));
  }

  
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  fona.setGPRSNetworkSettings(F("vivo"), F("vivo"), F("vivo"));

  // Optionally configure HTTP gets to follow redirects over SSL.
  // Default is not to follow SSL redirects, however if you uncomment
  // the following line then redirects over SSL will be followed.
  //fona.setHTTPSRedirect(true);

 //custom
 setExternalAudio();
}

void loopFona(){
  // Create a small string buffer to hold incoming call number.
  char phone[32] = {0};
  // Check for an incoming call.  Will return true if a call is incoming.
  if(fona.incomingCallNumber(phone)){
    Serial.print("chamando: "); 
    Serial.println(phone);
    lcdRinging(phone);
    state = STATE_CALLING;
  }
  
}

//get battery percentage
uint16_t getBattery () {
  uint16_t vbat = -1;
  if (! fona.getBattPercent(&vbat)) {
    Serial.println(F("Failed to read battery"));
  } else {
    Serial.println(F("Battery ok"));
  }
  return vbat;
}

String networkStatus() {
        uint8_t n = fona.getNetworkStatus();
        String status = "error";
        if (n == 0) status = F("Nao registrado            ");
        if (n == 1) status = F("Registrado (home)         ");
        if (n == 2) status = F("Nao registrado (searching)");
        if (n == 3) status = F("Denied                    ");
        if (n == 4) status = F("Unknown                   ");
        if (n == 5) status = F("Registrado (roaming)      ");
        return status;
}

uint8_t getRSSI() {
  return fona.getRSSI();
}

int8_t getSMSNumber () {
  return fona.getNumSMS();
}

void setExternalAudio(){
        // Set External output
        if (! fona.setAudio(FONA_EXTAUDIO)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }

        fona.setMicVolume(FONA_EXTAUDIO, 5);
}
