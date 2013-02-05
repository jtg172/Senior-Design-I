/*************************************************************
H-Bridge DC Motor Controller

Robbie Lundine
Justin Gilmer

*************************************************************/
//Including Xbox USB libraries

#include <XBOXUSB.h>
USB Usb;
XBOXUSB Xbox(&Usb);

//Define ports
int M1_1 = 5; //left //3
int M1_2 = 9; //5
int M2_1 = 6; //right //6
int M2_2 = 10; //9

void setup() {
  Serial.begin(9600);
//  Serial.begin(115200);

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }  
  Serial.print(F("\r\nXBOX USB Library Started"));

  //Setup Motor 1
  pinMode(M1_1, OUTPUT);
  pinMode(M1_2, OUTPUT);

  //Setup Motor 2
  pinMode(M2_1, OUTPUT);
  pinMode(M2_2, OUTPUT);
  
  Xbox.setLedMode(ALTERNATING);
   
  //TCCR0B=0x02;
  //TCCR1B=0x02;
  //TCCR2B=0x02;
}

void loop(){
  Usb.Task();
  int M1_1_S, M1_2_S, M2_1_S, M2_2_S;
  int CR_RL_S, CR_UD_S;
  
  M1_1_S = M1_2_S = M2_1_S = M2_2_S = 0;
  CR_RL_S = CR_UD_S = 0;
  
  CR_UD_S = Xbox.getAnalogHat(RightHatY);
  CR_RL_S = Xbox.getAnalogHat(RightHatX);
  
  ///* 
  if(CR_UD_S > 800)
  {
     // M1 FROWARD, M2 FORWARD
     M1_1_S = M2_1_S = map(CR_UD_S, 801, 32767, 0, 255);
     M1_2_S = M2_2_S = LOW;

  } 
  else if( CR_UD_S < (-800) )
  {
     //M1 REVERSE, M2 REVERSE
     M1_1_S = M2_1_S = LOW;
     M1_2_S = M2_2_S = map(CR_UD_S, -799, -32768, 0, 255);
  }
  //*/
  ///*
  if(CR_RL_S > 800)
  {
     //M1 REVERSE, M2 FORWARD
     M1_1_S = M2_2_S = LOW;
     M1_2_S = M2_1_S = map(CR_RL_S, 801, 32767, 0, 255);
  }
  else if(CR_RL_S < (-800) )
  {
     //M1 FORWARD, M2 REVERSE
     M1_1_S = M2_2_S = map(CR_RL_S, -799, -32768, 0, 255);
     M1_2_S = M2_1_S = LOW;
  }
  //*/
  /*
  Serial.print("M1_1_S=");
  Serial.print(M1_1_S, DEC);
  Serial.print(", M1_2_S=");
  Serial.print(M1_2_S, DEC);
  Serial.print("\n");
  
  Serial.print("M2_1_S=");
  Serial.print(M2_1_S, DEC);
  Serial.print(", M2_2_S=");
  Serial.print(M2_2_S, DEC);
  Serial.print("\n");
  
  Serial.print("CR_UD_S=");
  Serial.print(CR_UD_S, DEC);
  Serial.print("\n\n");
  delay(300);
  */

  //Set Motor 1 
  analogWrite(M1_1, M1_1_S);
  analogWrite(M1_2, M1_2_S);

  //Set Motor 2
  analogWrite(M2_1, M2_1_S);
  analogWrite(M2_2, M2_2_S);

  /*
  //Set Motor 1 test
  analogWrite(2, 0); //Establishes forward direction of Channel A
  analogWrite(3, 200);   //Disengage the Brake for Channel A
  
  //Set Motor 2 test
  analogWrite(4, 0);  //Establishes backward direction of Channel B
  analogWrite(5, 200);   //Disengage the Brake for Channel B
*/
}
