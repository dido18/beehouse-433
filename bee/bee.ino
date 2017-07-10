/*
   bee.ino  (trasmitter)

   It has a analog sound sensor and the code are sent to the beehive.

*/

#include <RH_ASK.h>
#include <SPI.h>            // Not actually used but needed to compile
#include <RHDatagram.h>    // Radio Head manager: Addressed, unreliable variable length messages, with optional broadcast facilities.


const uint16_t speed = 2000; //The desired bit rate in bits per second
const uint8_t rxPin = 11;
const uint8_t txPin = 12;

#define BEE_ADDRESS 1

#define BEE_QUEEN_ADDRESS 0


RH_ASK driver(speed, rxPin, txPin);
RHDatagram manager (driver, BEE_ADDRESS);

// paylod message
struct {
  int op;
  uint8_t data;           // 2 bytes with no negative numbers = 65,535 (2^16) - 1)
} MESSAGE;


struct MESSAGE_RESPONSE{
  int  op ;       // 0 =register, 
  uint8_t  data ;   // 2 bytes with no negative numbers = 65,535 (2^16) - 1)
};

struct MESSAGE_RESPONSE *MessageIn;

int sensorPin = A0;    // Input analog pin of the sound sensor

const int ledPin = LED_BUILTIN; // 3;      // integrated arduino Led pin = 13
const int threshold = 110;  // value of sound ath which the led is switch on


void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);   // Debugging only
   if (!manager.init())
    Serial.println("init failed");
}


// Dont put this on the stack: buf for receiving a message
uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];

void loop()
{
    Serial.println(ledPin);
    // read_sound() ;
    send_msg_beequeen();

    if(receive_msg()){
    Serial.print(" { Operation: ");
    Serial.print(MessageIn->op);
    Serial.print(", Data ");
    Serial.print(MessageIn->data);
    Serial.println("} ");
      
    }
    
    delay(2000);
    
    // read_sound() ;
    //int sample;
    //sample = analogRead(sensorPin);
    //Serial.println(sample);
    
   // delay(2000);
}

bool receive_msg(){
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfrom(buf, &len, &from))
    {
      
      Serial.print("Received message from ");
      Serial.print(manager.headerFrom());
      MessageIn = (struct MESSAGE_RESPONSE*)buf;
      Serial.print(MessageIn->data);
      switch (MessageIn->op) {
        case 1:
          //do something when var equals 1
          break;
        case 2:  //COMMAND message
          if(MessageIn->data == 1)
            digitalWrite(ledPin, HIGH);
          else
            digitalWrite(ledPin, LOW);
          break;
        default: 
          // if nothing else matches, do the default
          // default is optional
        break;
      }
      
    }
     return true;
  }
  else
    return false;
}

void send_msg_beequeen()
{
  MESSAGE.op = 1;
  MESSAGE.data = 50;

  if (manager.sendto((uint8_t*)&MESSAGE, sizeof(MESSAGE), BEE_QUEEN_ADDRESS))
  {
    Serial.print("Bee");
    Serial.print(BEE_ADDRESS);
    Serial.print("- Message sent to BEEE QUEEN address: "); 
    Serial.print(BEE_QUEEN_ADDRESS);
    Serial.print(" OP: ");
    Serial.print(MESSAGE.op);
    Serial.print(" Data: ");
    Serial.print(MESSAGE.data);
    Serial.println("");
  }
  else
  {
    Serial.print("Message NOT sent to Bee queen");
  }
  manager.waitPacketSent( ) ;
  //Serial.print("sent message!");
}


//
//
//const int middleValue = 90;      //512 in the middle of the analog value
//const int numberOfSamples = 128; // how many reads every cycle
//int sample;                      // value of the michopone read every time
//long signal;
//
//long averageReading;    // media del ciclo di lettura
//
//long runningAverage = 0;  // la media incorso dei valori calcolati
//const int averageOver = 16;  //quanto velocemente i nuovi valori influenzano la media corrente, più grandi sono i numeri menore è la velcoitò
//
//
//
//long  read_sound()
//{
//  long sumOfSquares = 0;
//  for (int i = 0; i < numberOfSamples ; i++)
//  {

//    sample = analogRead(0); //sensorPin
//    //Serial.println(sample);
//    signal = (sample - middleValue);  // calculate the shift from the middle value
//    signal *= signal;  // power the signal for having positive values
//    sumOfSquares += signal;
//  }
//
//  averageReading = sumOfSquares/numberOfSamples;              // actual average
//  runningAverage = (((averageOver-1)*runningAverage)+averageReading)/averageOver; //
//
//  Serial.println(runningAverage);
//  if (runningAverage >  threshold)
//  {
//    digitalWrite(ledPin, HIGH);
//    Serial.println("accendi ked");
//  }
//  else
//  {
//    digitalWrite(ledPin,LOW);
//  }
//  return runningAverage;
//}
