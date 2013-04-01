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
const int M1_1 = 5; //back left  //5
const int M1_2 = 4; //4
const int M2_1 = 3; //back right  //3
const int M2_2 = 2;  //2
const int M3_1 = 6; //back left bottom1
const int M3_2 = 7; 
const int M4_1 = 8; //back right bottom2
const int M4_2 = 9; 
const int M5_1 = 10; //front left bottom3
const int M5_2 = 11; 
const int M6_1 = 12; //front right bottom4
const int M6_2 = 13;

const int xpin = A8;
const int ypin = A9;
const int zpin = A10;

int test;
int XboxConnected;
int dir;
int temp;

const int XPIN_L = 344;
const int YPIN_L = 337;
const int ZPIN_L = 300;//?
const int PIN_R = 75;


void setup() {
  Serial.begin(9600);

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    XboxConnected = LOW;
  }
  else {
    Serial.print(F("\r\nXBOX USB Library Started"));
    Xbox.setLedMode(ALTERNATING);
    XboxConnected = HIGH;
  }
  
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
  
  //Setup Motor 6
  pinMode(M6_1, OUTPUT);
  pinMode(M6_2, OUTPUT);
  
  //Setup Accelerometer
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  pinMode(zpin, INPUT);
  
  //TCCR0B=0x02;
  //TCCR2B=0x02;
  
  test = 0;
  
}

void loop(){
  // initialize motors
  int M1_1_S, M1_2_S, M2_1_S, M2_2_S, M3_1_S, M3_2_S, M4_1_S, M4_2_S, M5_1_S, M5_2_S, M6_1_S, M6_2_S;
  M1_1_S = M1_2_S = M2_1_S = M2_2_S = M3_1_S = M3_2_S = M4_1_S = M4_2_S = M5_1_S = M5_2_S = M6_1_S = M6_2_S = 0;
  
  // init/setup controls
  int CR_RL_S, CR_UD_S, CL_UD_S, CL_RL_S, LT_S, RT_S;
  CR_RL_S = CR_UD_S = CL_UD_S = CL_RL_S = LT_S = RT_S = 0;
  
  if (XboxConnected) {
    Usb.Task();
    
    //CR_UD_S = Xbox.getAnalogHat(RightHatY); //not used in new controls
    CR_RL_S = Xbox.getAnalogHat(RightHatX);
    CL_UD_S = Xbox.getAnalogHat(LeftHatY);
    //CL_RL_S = Xbox.getAnalogHat(LeftHatX); //not used at all
    
    LT_S = Xbox.getButton(L2);
    RT_S = Xbox.getButton(R2);
  }
  
  // init/set up accelerometer 
  int XPIN_S, YPIN_S, ZPIN_S;
  XPIN_S = YPIN_S = ZPIN_S = 0;
  
  XPIN_S = analogRead(xpin);
  YPIN_S = analogRead(ypin);
  ZPIN_S = analogRead(zpin);
  
  /***********************start of new controls*****************************/

  dir = 0;
  if (CL_UD_S > 800)
  {
    // M1 REVERSE, M2 REVERSE
     M1_1_S = M2_1_S = LOW;
     M1_2_S = M2_2_S = map(CL_UD_S, -801, -32768, 0, 255);
     dir = 1;
  }
  else if (CL_UD_S < -800)
  {
     // M1 FROWARD, M2 FORWARD
     M1_1_S = M2_1_S = map(CL_UD_S, 801, 32767, 0, 255);
     M1_2_S = M2_2_S = LOW;
     dir = 2;
  }

  if (CR_RL_S > 800)
  {
      if (dir==2) //both forward
      {
          M2_1_S = M2_1_S - map(CR_RL_S, 801, 32767, 0, 255);
          if (M2_1_S < 0)
          {
              M2_2_S = abs(M2_1_S);
              M2_1_S = LOW;
              M1_1_S = M1_1_S + M2_2_S;
          }
      }
      else if (dir==1) //both reverse
      {
          M1_2_S = M1_2_S - map(CR_RL_S, 801, 32767, 0, 255);
          if (M1_2_S < 0)
          {
              M1_1_S = abs(M1_2_S);
              M1_2_S = LOW;
              M2_2_S = M2_2_S + M1_1_S;
          }
      }
      else
      {
          M1_1_S = M2_2_S = map(CR_RL_S, 801, 32767, 0, 255);
      }
  }
  else if (CR_RL_S < -800)
  {
      if (dir==2) //both forward
      {
          M1_1_S = M1_1_S - map(CR_RL_S, -801, -32768, 0, 255);
          if (M1_1_S < 0)
          {
              M1_2_S = abs(M1_1_S);
              M1_1_S = LOW;
              M2_1_S = M2_1_S + M1_2_S;
          }
      }
      else if (dir==1) //both reverse
      {
          M2_2_S = M2_2_S - map(CR_RL_S, -801, -32768, 0, 255);
          if (M2_2_S < 0)
          {
              M2_1_S = abs(M2_2_S);
              M2_2_S = LOW;
              M1_2_S = M1_2_S + M2_1_S;
          }
      }
      else
      {
          M2_1_S = M1_2_S = map(CR_RL_S, -801, -32767, 0, 255);
      }
  } 
  /***********************finish of new controls***********************************/
  /***********************start of old controls*****************************//*  
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
     //M1 FORWARD, M2 REVERSE
     M1_1_S = M2_2_S = LOW;
     M1_2_S = M2_1_S = map(CR_RL_S, 801, 32767, 0, 255);
  }
  else if(CR_RL_S < (-800) )
  {
     //M1 REVERSE, M2 FORWARD
     M1_1_S = M2_2_S = map(CR_RL_S, -799, -32768, 0, 255);
     M1_2_S = M2_1_S = LOW;
  }
  /***********************finish of old controls*****************************/
  /***********************start of test controls*****************************//*
  if(CL_UD_S > 800)
  {
     // M1 FORWARD
     M1_1_S = map(CL_UD_S, 801, 32768, 0, 255);
     M1_2_S = LOW;

  } 
  else if( CL_UD_S < (-800) )
  {
     //M1 REVERSE
     M1_1_S = LOW;
     M1_2_S = map(CL_UD_S, -801, -32769, 0, 255);
  }

  if(CR_UD_S > 800)
  {
     // M2 FORWARD
     M2_1_S = map(CR_UD_S, 801, 32768, 0, 255);
     M2_2_S = LOW;

  } 
  else if( CR_UD_S < (-800) )
  {
     //M2 REVERSE
     M2_1_S = LOW;
     M2_2_S = map(CR_UD_S, -801, -32769, 0, 255);
  }
  /***********************finish of test controls*****************************/
  
  /************************start of accelerometer controls********************/
  
  temp=0;
  if(XPIN_S < (XPIN_L-1))
  {
      temp=0;
      M6_1_S = M5_1_S = map(XPIN_S, XPIN_L, (XPIN_L-PIN_R), 0, 255) / 2; //6 and 5 go down
      M3_2_S = M4_2_S = map(XPIN_S, XPIN_L, (XPIN_L-PIN_R), 0, 255) / 2; //3 and 4 go up
  } 
  else if (XPIN_S > (XPIN_L+1) )
  {
      temp=1;
      M6_2_S = M5_2_S = map(XPIN_S, XPIN_L, (XPIN_L-PIN_R), 0, 255) / 2; //6 and 5 go up
      M3_1_S = M4_1_S = map(XPIN_S, XPIN_L, (XPIN_L-PIN_R), 0, 255) / 2; //3 and 4 go down
  }
  
//  if (YPIN_S < (YPIN_L-1) )
//  {
//      //3 AND 5 go down
//      //M3_1_S = M4_1_S = LOW;
//      M3_1_S = M3_1_S + map(YPIN_S, YPIN_L, (YPIN_L-PIN_R), 0, 255);
//      M5_1_S = M4_1_S + map(YPIN_S, YPIN_L, (YPIN_L-PIN_R), 0, 255);    
//  }
//  else if (YPIN_S > (YPIN_L+1) )
//  {
//     //4 and 6 go down
//      //M5_1_S = M6_1_S = LOW;
//      M4_1_S = M4_1_S + map(YPIN_S, YPIN_L, (YPIN_L+PIN_R), 0, 255); //4 DOWN
//      M6_1_S = M6_1_S + map(YPIN_S, YPIN_L, (YPIN_L+PIN_R), 0, 255);  //6 DOWN
//      if (M4_1_S > 255)
//      {
//          
//      }
//  }  


  if( ZPIN_S < (ZPIN_L-1) )
     {
       //ROV SINKING, MOTORS FORWARD/UP?
       RT_S = map(ZPIN_S, ZPIN_L, (ZPIN_L-100), 0, 255);
     }
     else if( ZPIN_S > (ZPIN_L+1) )
     {
       //ROV RISING, MOTORS REVERSE/DOWN?
       LT_S = map(ZPIN_S, ZPIN_L, (ZPIN_L+100), 0, 255);
     }
  
  if(LT_S > 0)
  {
    //goes DOWN
    if(!temp)
    {
         M6_1_S = M6_1_S + map(LT_S, 0, 255, 0, (255 - M6_1_S));
         M5_1_S = M5_1_S + map(LT_S, 0, 255, 0, (255 - M5_1_S));
         
         M4_2_S = M4_2_S - map(LT_S, 0, 255, 0, (255 - M6_1_S));
         if (M4_2_S < 0)
         {
            M4_1_S = abs(M4_2_S);
            M4_2_S = LOW; 
         }
         M3_2_S = M3_2_S - map(LT_S, 0, 255, 0, (255 - M5_1_S));
         if (M3_2_S < 0)
         {
            M3_1_S = abs(M3_2_S);
            M3_2_S = LOW; 
         }
    }
    else
    {
         M4_1_S = M4_1_S + map(LT_S, 0, 255, 0, (255 - M4_1_S));
         M3_1_S = M3_1_S + map(LT_S, 0, 255, 0, (255 - M3_1_S));
         
         M6_2_S = M6_2_S - map(LT_S, 0, 255, 0, (255 - M4_1_S));
         if (M6_2_S < 0)
         {
            M6_1_S = abs(M6_2_S);
            M6_2_S = LOW; 
         }
         M5_2_S = M5_2_S - map(LT_S, 0, 255, 0, (255 - M3_1_S));
         if (M5_2_S < 0)
         {
            M5_1_S = abs(M5_2_S);
            M5_2_S = LOW; 
         }
    }
  }
  else if (RT_S > 0)
  {
    //goes UP
    if(!temp)
    {
         M4_2_S = M4_2_S + map(LT_S, 0, 255, 0, (255 - M4_2_S));
         M3_2_S = M3_2_S + map(LT_S, 0, 255, 0, (255 - M3_2_S));
         
         M6_1_S = M6_1_S - map(LT_S, 0, 255, 0, (255 - M4_2_S));
         if (M6_1_S < 0)
         {
            M6_2_S = abs(M6_1_S);
            M6_1_S = LOW; 
         }
         M5_1_S = M5_1_S - map(LT_S, 0, 255, 0, (255 - M3_2_S));
         if (M5_1_S < 0)
         {
            M5_2_S = abs(M5_1_S);
            M5_1_S = LOW; 
         }
    }
    else
    {
         M6_2_S = M6_2_S + map(LT_S, 0, 255, 0, (255 - M6_2_S));
         M5_2_S = M5_2_S + map(LT_S, 0, 255, 0, (255 - M5_2_S));
         
         M4_1_S = M4_1_S - map(LT_S, 0, 255, 0, (255 - M6_2_S));
         if (M4_1_S < 0)
         {
            M4_2_S = abs(M4_1_S);
            M4_1_S = LOW; 
         }
         M3_1_S = M3_1_S - map(LT_S, 0, 255, 0, (255 - M5_2_S));
         if (M3_1_S < 0)
         {
            M3_2_S = abs(M3_1_S);
            M3_1_S = LOW; 
         }  
    }
  }
  
  /***************************END ACCELEROMETER*****************************/    
 
 
   if (test == 3000)
   {
     // print the sensor values:
     Serial.print("X=");
     Serial.print(XPIN_S, DEC);
     Serial.print("\tY=");
     Serial.print(YPIN_S, DEC);
     Serial.print("\tZ=");
     Serial.print(ZPIN_S, DEC);
     Serial.print("\n");
  
     Serial.print("M5_1_S=");
     Serial.print(M5_1_S, DEC);
     Serial.print("\tM6_1_S=");
     Serial.print(M6_1_S, DEC);
     
     Serial.print("\nM5_2_S=");
     Serial.print(M5_2_S, DEC);   
     Serial.print("\tM6_2_S=");
     Serial.print(M6_2_S, DEC);
     Serial.print("\n");  
     
     Serial.print("M3_1_S=");
     Serial.print(M3_1_S, DEC);
     Serial.print("\tM4_1_S=");
     Serial.print(M4_1_S, DEC);
     
     Serial.print("\nM3_2_S=");
     Serial.print(M3_2_S, DEC);
     Serial.print("\tM4_2_S=");
     Serial.print(M4_2_S, DEC);
     Serial.print("\n\n");
     
     test = 0; 
  }
  test= test+1;
  
  /*
  //Serial.print("LT_S=");
  //Serial.print(LT_S, DEC);
  //delay(300);
  
  if ( test == 4000 )
  {  
    test=0;
    Serial.print("CL_UD_S=");
    Serial.print(CL_UD_S, DEC);
    Serial.print("\n");
    
    Serial.print("CR_RL_S=");
    Serial.print(CR_RL_S, DEC);
    Serial.print("\n");
    
    Serial.print("M1_1_S=");
    Serial.print(M1_1_S, DEC);
    Serial.print(", M1_2_S=");
    Serial.print(M1_2_S, DEC);
    Serial.print("\n");
    
    Serial.print("M2_1_S=");
    Serial.print(M2_1_S, DEC);
    Serial.print(", M2_2_S=");
    Serial.print(M2_2_S, DEC);
    Serial.print("\n\n");
    
//    Serial.print("M6_1_S=");
//    Serial.print(M6_1_S, DEC);
//    Serial.print(", M6_2_S=");
//    Serial.print(M6_2_S, DEC);
//    Serial.print("\n\n\n");
  }
  test=test+1;
  
  //delay(3000);
  */
///*
  //Set Motor 1 
  analogWrite(M1_1, M1_1_S);
  analogWrite(M1_2, M1_2_S);

  //Set Motor 2
  analogWrite(M2_1, M2_1_S);
  analogWrite(M2_2, M2_2_S);
  
  //Set Motor 3
  analogWrite(M3_1, M3_1_S);
  analogWrite(M3_2, M3_2_S);
  
  //Set Motor 4
  analogWrite(M4_1, M4_1_S);
  analogWrite(M4_2, M4_2_S);
  
  //Set Motor 5
  analogWrite(M5_1, M5_1_S);
  analogWrite(M5_2, M5_2_S);
  
  //Set Motor 6
  analogWrite(M6_1, M6_1_S);
  analogWrite(M6_2, M6_2_S);
//*/
  /*
  //Set Motor 1 test
  analogWrite(2, 200); //Establishes forward direction of Channel A
  analogWrite(3, 200);   //Disengage the Brake for Channel A
  
  //Set Motor 2 test
  analogWrite(4, 200);  //Establishes backward direction of Channel B
  analogWrite(5, 200);   //Disengage the Brake for Channel B
*/
}
