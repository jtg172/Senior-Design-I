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
int M1_1 = 2; //left
int M1_2 = 3; 
int M2_1 = 4; //right
int M2_2 = 5; 
int M3_1 = 6; //bottom1
int M3_2 = 7; 
int M4_1 = 8; //bottom2
int M4_2 = 9; 
int M5_1 = 10; //bottom3
int M5_2 = 11; 

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
  
  //Setup Motor 3
  pinMode(M3_1, OUTPUT);
  pinMode(M3_2, OUTPUT);
  
  //Setup Motor 4
  pinMode(M4_1, OUTPUT);
  pinMode(M4_2, OUTPUT);
  
  //Setup Motor 5
  pinMode(M5_1, OUTPUT);
  pinMode(M5_2, OUTPUT);
  
  Xbox.setLedMode(ALTERNATING);
   
  //TCCR0B=0x02;
  //TCCR1B=0x02;
  //TCCR2B=0x02;
}

void loop(){
  Usb.Task();
  int M1_1_S, M1_2_S, M2_1_S, M2_2_S, M3_1_S, M3_2_S, M4_1_S, M4_2_S, M5_1_S, M5_2_S;
  int CR_RL_S, CR_UD_S, LT_S, RT_S;
  
  M1_1_S = M1_2_S = M2_1_S = M2_2_S = M3_1_S = M3_2_S = M4_1_S = M4_2_S = M5_1_S = M5_2_S = 0;
  CR_RL_S = CR_UD_S = LT_S = RT_S = 0;
  
  CR_UD_S = Xbox.getAnalogHat(RightHatY);
  CR_RL_S = Xbox.getAnalogHat(RightHatX);
  
  LT_S = Xbox.getButton(L2);
  RT_S = Xbox.getButton(R2);
  
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
  
  if(LT_S > 0)
  {
     M3_1_S = M4_1_S = M5_1_S = LOW;
     M3_2_S = M4_2_S = M5_2_S = LT_S;
  }
  else if (RT_S > 0)
  {
     M3_1_S = M4_1_S = M5_1_S = RT_S;
     M3_2_S = M4_2_S = M5_2_S = LOW;
  }
  
  /*
  Serial.print("LT_S=");
  Serial.print(LT_S, DEC);
  //delay(300);
  
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
  
  //Set Motor 3
  analogWrite(M3_1, M3_1_S);
  analogWrite(M3_2, M3_2_S);
  
  //Set Motor 2
  analogWrite(M4_1, M4_1_S);
  analogWrite(M4_2, M4_2_S);
  
  //Set Motor 2
  analogWrite(M5_1, M5_1_S);
  analogWrite(M5_2, M5_2_S);

  /*
  //Set Motor 1 test
  analogWrite(2, 0); //Establishes forward direction of Channel A
  analogWrite(3, 200);   //Disengage the Brake for Channel A
  
  //Set Motor 2 test
  analogWrite(4, 0);  //Establishes backward direction of Channel B
  analogWrite(5, 200);   //Disengage the Brake for Channel B
*/
}
