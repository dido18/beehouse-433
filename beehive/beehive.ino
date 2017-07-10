
/*
 * beehive.ino  
 * 
 * Receives a "message" from the bee (bee.ino) and sounds a music
 * The music and the ligth can be controller also by a telecomando.
 * 
 * 
 */
#include <RCSwitch.h>    // 433.92 Mhz communication with the telecomando


#include <RH_ASK.h>    // RadioHead library for 433 Mhz communication(non blocking)
#include <SPI.h>      // Not actualy used but needed to RH_ASK

#include "pitches.h"     // notes defintion with frequencies
#include "framartino.h"  // melody[], noteDurations[] 


#define pinRx 2          // digital pin 433 Mhz receiver


RH_ASK driver(2000, pinRx);

RCSwitch mySwitch = RCSwitch(); 

#define pinSpeaker 3      // digital pin speaker output
#define maxSoundLevel  100   // max sound level received for starting the music

struct dataStruct{
  long sound ; 
  // float temp;
}message;

void setup()
{
    Serial.begin(9600); // Debugging only

  
   Serial.print("Beehive - Max level Sound:");
   Serial.print(maxSoundLevel);
   Serial.println("");
    if (!driver.init())
         Serial.println("init failed");
   mySwitch.enableReceive(0);         // Receiver on interrupt 0 => that is pin #2 (or #3 in other boards)
   Serial.println("Beehive - completed setup");
}


void loop()
{
   if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();

    if (value == 6000) {
      Serial.print("A button pressed");
      play_music();
    }
    else if  (value == 6001){
      Serial.print("B button pressed");
      playMusic = false;
    }
    else if (value==6002){
      Serial.print("C button pressed");
    }
    else if (value = 6003){
      Serial.print("D button pressed");
    }
    else{
      Serial.println("Unknow code received");
    }


   mySwitch.resetAvailable();
  }

    // receives from radioHead
//    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];  // buffers for receivein the packet (max 60 bytes) 
//    uint8_t buflen = sizeof(buf);
//    
//    if (driver.recv(buf, &buflen)) // Non-blocking
//    {
//      memcpy(&message, buf, sizeof(message)); 
//      if (message.sound < maxSoundLevel)
//      {
//        Serial.println("values < 100");
//      }
//      else // if (message.sound >= 100)
//      {
//        Serial.print("Level sound: ");
//        Serial.print(message.sound);
//        Serial.println("Music is starting...");
//        play_music();
//
//      }
// 
//    }
}

void play_music()
{
  
  int speed=180;  //higher value, slower notes
  for (int thisNote = 0; melody[thisNote]!=-1; thisNote++) {
      int noteDuration = speed*noteDurations[thisNote];
      tone(pinSpeaker, melody[thisNote],noteDuration*.95);
      delay(noteDuration);
  
      noTone(pinSpeaker);
  }


}

