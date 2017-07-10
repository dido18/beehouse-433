
/*
 * beehibe.ino  (trasmitter / receiver)
 *
 * Receives/trasmit the signals to the bee(s) sensors.
 *
*/
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <RHDatagram.h>    //manager radio head

#include <ArduinoJson.h>

const uint16_t speed = 2000; //The desired bit rate in bits per second
const uint8_t rxPin=11;
const uint8_t txPin=12;

const uint8_t BEE_QUEEN_ADDRESS=0;
const uint8_t BEE_1_ADDRESS=1;


RH_ASK driver(speed, rxPin, txPin);

RHDatagram manager (driver, BEE_QUEEN_ADDRESS);


// byte operation:  3 ;   // 3 bits operation
struct MESSAGE {
  int  op ;         // 0 =register,
  uint8_t  data ;   // 2 bytes with no negative numbers = 65,535 (2^16) - 1)
};

struct MESSAGE *MessageIn;

// paylod message
struct {
  int op;
  uint8_t data;           // 2 bytes with no negative numbers = 65,535 (2^16) - 1)
} MESSAGE_RESPONSE;


void setup()
{
    Serial.begin(9600);   // Debugging only
    if (!manager.init())
         Serial.println("init failed");
}


// Dont put this on the stack:
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];



void loop(){
  
  if (receive_msg()){

   StaticJsonBuffer<100> jsonBuffer;

   JsonObject& root = jsonBuffer.createObject();
   root["src"] = manager.headerFrom();
   root["dst"] = manager.headerTo();
   root["op"] = MessageIn->op;
   root["data"] = MessageIn->data;

   root["status"] = "received";
   root.printTo(Serial);
   Serial.println();
  }
  
  send_msg();
}


bool receive_msg(){
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    if (manager.recvfrom(buf, &len))
    {
      //Serial.print("Received message from ");
      // Serial.print(manager.headerFrom());
      MessageIn = (struct MESSAGE*)buf;
    }
     return true;
  }
  else
    return false;
}

void send_msg()
{
     if (Serial.available() > 0){
      
        String val = Serial.readStringUntil('\n');
        
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(val);
        
         // Test if parsing succeeds.
        if (!root.success()) {
          Serial.println("parseObject() failed");
          return;
        }
        
        uint8_t src = root["src"];
        uint8_t dst = root["dst"];
        
        MESSAGE_RESPONSE.op =  root["op"];
        MESSAGE_RESPONSE.data = root["data"];

        // Broadcast to all the nodes; RH_BROADCAST_ADDRESS = 255
        // if (dst = 255)
        //    dst = RH_BROADCAST_ADDRESS; // broadcast to all the nodes
        
        if (manager.sendto((uint8_t*)&MESSAGE_RESPONSE, sizeof(MESSAGE_RESPONSE), dst))
          {
            // send to beehive server the ack
            root["status"] = "sent";
            root.printTo(Serial);
            Serial.println();
          }else
          {
            root["status"] = "error";  
            root.printTo(Serial);
            Serial.println();
          } 
          manager.waitPacketSent();
     }
     
}

