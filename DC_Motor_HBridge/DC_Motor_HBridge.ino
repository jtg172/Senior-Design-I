/*************************************************************
H-Bridge DC Motor Controller

Robbie Lundine
Justin Gilmer

*************************************************************/
//Including Xbox USB libraries

#include <XBOXUSB.h>
#include <usbhub.h>
USB Usb;
USBHub Hub1(&Usb);
USBHub Hub2(&Usb);
XBOXUSB Xbox(&Usb);

//Define ports
const int M1_F = 7; //back left  //5
const int M1_R = 6; //4
const int M2_F = 4; //back right  //3
const int M2_R = 5;  //2
const int M34_U = 2; //back 
const int M34_D = 3; 
const int M56_U = 8; //front
const int M56_D = 9; 

const char xpin = A8;
const char ypin = A9;
const char zpin = A10;

int test;
int XboxConnected;
int dir;
int temp;
int mapped;



const float XPIN_L = 567;//568
const float YPIN_L = 436;
const float ZPIN_L = 388;
const float PIN_R = 30;

float accelOver = 0;
int tilt = 0;
int M_Val = 0;
int time = 0;

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
  pinMode(M1_F, OUTPUT);
  pinMode(M1_R, OUTPUT);

  //Setup Motor 2
  pinMode(M2_F, OUTPUT);
  pinMode(M2_R, OUTPUT);
  
  //Setup Motor 3 and 4
  pinMode(M34_U, OUTPUT);
  pinMode(M34_D, OUTPUT);
  
  //Setup Motor 5 and 6
  pinMode(M56_U, OUTPUT);
  pinMode(M56_D, OUTPUT);
  
  //Setup Accelerometer
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  pinMode(zpin, INPUT);
  
  test=0;
}

void loop(){
  // initialize motors
  float M1_F_S, M1_R_S, M2_F_S, M2_R_S, M34_U_S, M34_D_S, M56_U_S, M56_D_S;
  M1_F_S = M1_R_S = M2_F_S = M2_R_S = M34_U_S = M34_D_S = M56_U_S = M56_D_S = 0;
  
  // init/setup controls
  int CR_RL_S, CR_UD_S, CL_UD_S, CL_RL_S, LT_S, RT_S, LB_S, RB_S, A_S, B_S, Y_S;
  CR_RL_S = CR_UD_S = CL_UD_S = CL_RL_S = LT_S = RT_S = LB_S = RB_S = A_S = B_S = Y_S= 0;
  
  time = time +1;
  
  if (XboxConnected) {
    Usb.Task();
    
    //CR_UD_S = Xbox.getAnalogHat(RightHatY); //not used in new controls
    CR_RL_S = Xbox.getAnalogHat(RightHatX);
    CL_UD_S = Xbox.getAnalogHat(LeftHatY);
    //CL_RL_S = Xbox.getAnalogHat(LeftHatX); //not used at all
    
    LT_S = Xbox.getButton(L2);
    RT_S = Xbox.getButton(R2);
    
    LB_S = Xbox.getButton(L1);
    RB_S = Xbox.getButton(R1);
    
    A_S = Xbox.getButton(A);
    B_S = Xbox.getButton(B);
    Y_S = Xbox.getButton(Y);
  }
  
  // init/set up accelerometer 
  int XPIN_S, YPIN_S, ZPIN_S;
  XPIN_S = YPIN_S = ZPIN_S = 0;
  
  XPIN_S = analogRead(xpin);
  YPIN_S = analogRead(ypin);
  ZPIN_S = analogRead(zpin);
//  XPIN_S = XPIN_L;
//  YPIN_S = YPIN_L;
//  ZPIN_S = ZPIN_L;


if(time > 25)
{
  if (LB_S)
  {
     accelOver = accelOver + 1;
     tilt=1;
   }
  else if (RB_S)
  {
     accelOver = accelOver - 1;
     tilt=1;
  }
}  
  if (tilt)
  {
     if (accelOver > 20) accelOver=20;
     XPIN_S = XPIN_L + accelOver;
       
  }

 
  if (B_S)
    {
      tilt=0; 
    }
  
//  //make value stay if press A button
//  if (RB_S)
//  {
//     accelOver = accelOver + .1;
//     if (accelOver > (2*PIN_R) ) XPIN_S = XPIN_L - 2*PIN_R;
//     else XPIN_S = XPIN_L - accelOver;
//  }
//  else if (LB_S)
//  {
//     accelOver = accelOver + .1;
//     if (accelOver > (2*PIN_R) ) XPIN_S = XPIN_L + 2*PIN_R; 
//     else XPIN_S = XPIN_L + accelOver;
//  }
//  else
//  {
//     accelOver = 0; 
//  }
//  
//  
//  if (A_S)
//    {
//       lock = 1;
//       lockVal = XPIN_S;
//    }
//  else if (B_S)
//    {
//       lock = 0;
//       lockVal = 0;
//    }
//     
//  if (lock) XPIN_S = lockVal;
//  

  
  
  /***********************start of new controls*****************************/

  dir = 0;
  if (CL_UD_S > 800)
  {
    // M1 REVERSE, M2 REVERSE
     M1_F_S = M2_F_S = map(CL_UD_S, 801, 32767, 0, 255);
     M1_R_S = M2_R_S = LOW;
     dir = 1;
  }
  else if (CL_UD_S < -800)
  {
     // M1 FROWARD, M2 FORWARD
     M1_F_S = M2_F_S = LOW;
     M1_R_S = M2_R_S = map(CL_UD_S, -801, -32768, 0, 255);
     dir = 2;
  }

  if (CR_RL_S > 800)
  {
      if (dir==2) //both forward
      {
          M2_F_S = M2_F_S - map(CR_RL_S, 801, 32767, 0, 255);
          if (M2_F_S < 0)
          {
              M2_R_S = abs(M2_F_S);
              M2_F_S = LOW;
              M1_F_S = M1_F_S + M2_R_S;
          }
      }
      else if (dir==1) //both reverse
      {
          M1_R_S = M1_R_S - map(CR_RL_S, 801, 32767, 0, 255);
          if (M1_R_S < 0)
          {
              M1_F_S = abs(M1_R_S);
              M1_R_S = LOW;
              M2_R_S = M2_R_S + M1_F_S;
          }
      }
      else
      {
          M1_F_S = M2_R_S = map(CR_RL_S, 801, 32767, 0, 255);
      }
  }
  else if (CR_RL_S < -800)
  {
      if (dir==2) //both forward
      {
          M1_F_S = M1_F_S - map(CR_RL_S, -801, -32768, 0, 255);
          if (M1_F_S < 0)
          {
              M1_R_S = abs(M1_F_S);
              M1_F_S = LOW;
              M2_F_S = M2_F_S + M1_R_S;
          }
      }
      else if (dir==1) //both reverse
      {
          M2_R_S = M2_R_S - map(CR_RL_S, -801, -32768, 0, 255);
          if (M2_R_S < 0)
          {
              M2_F_S = abs(M2_R_S);
              M2_R_S = LOW;
              M1_R_S = M1_R_S + M2_F_S;
          }
      }
      else
      {
          M2_F_S = M1_R_S = map(CR_RL_S, -801, -32767, 0, 255);
      }
  } 
  /***********************finish of new controls***********************************/
  /***********************start of old controls*****************************//*  
  if(CR_UD_S > 800)
  {
     // M1 FROWARD, M2 FORWARD
     M1_F_S = M2_F_S = map(CR_UD_S, 801, 32767, 0, 255);
     M1_R_S = M2_R_S = LOW;

  } 
  else if( CR_UD_S < (-800) )
  {
     //M1 REVERSE, M2 REVERSE
     M1_F_S = M2_F_S = LOW;
     M1_R_S = M2_R_S = map(CR_UD_S, -799, -32768, 0, 255);
  }

  if(CR_RL_S > 800)
  {
     //M1 FORWARD, M2 REVERSE
     M1_F_S = M2_R_S = LOW;
     M1_R_S = M2_F_S = map(CR_RL_S, 801, 32767, 0, 255);
  }
  else if(CR_RL_S < (-800) )
  {
     //M1 REVERSE, M2 FORWARD
     M1_F_S = M2_R_S = map(CR_RL_S, -799, -32768, 0, 255);
     M1_R_S = M2_F_S = LOW;
  }
  /***********************finish of old controls*****************************/
  /***********************start of test controls*****************************//*
  if(CL_UD_S > 800)
  {
     // M1 FORWARD
     M1_F_S = map(CL_UD_S, 801, 32768, 0, 255);
     M1_R_S = LOW;

  } 
  else if( CL_UD_S < (-800) )
  {
     //M1 REVERSE
     M1_F_S = LOW;
     M1_R_S = map(CL_UD_S, -801, -32769, 0, 255);
  }

  if(CR_UD_S > 800)
  {
     // M2 FORWARD
     M2_F_S = map(CR_UD_S, 801, 32768, 0, 255);
     M2_R_S = LOW;

  } 
  else if( CR_UD_S < (-800) )
  {
     //M2 REVERSE
     M2_F_S = LOW;
     M2_R_S = map(CR_UD_S, -801, -32769, 0, 255);
  }
  /***********************finish of test controls*****************************/
  
  /************************start of accelerometer controls********************/
/* 
  
  }*/
 
 ///*
  mapped=0;
  
  if(time ==25)
  {
  time =0;
      if(XPIN_S > (XPIN_L+3))
      {
        if(tilt)
        {
           M56_D_S = map(XPIN_S, XPIN_L, (XPIN_L+PIN_R), 0, 255) / 2; //6 and 5 go down
           M34_U_S = map(XPIN_S, XPIN_L, (XPIN_L+PIN_R), 0, 255) / 2; //3 and 4 go up
        }  
        else if(M_Val < 200)
        {
          M_Val = M_Val+1;
          if(M_Val < 0) M56_U_S = M34_D_S =  abs(M_Val); //3 and 4 go down  6 and 5 go up 
          else M56_D_S = M34_U_S =  M_Val; //6 and 5 go down   3 and 4 go up
        {
      } 
      else if (XPIN_S < (XPIN_L-3) )
      {
        if(tilt)
        {
          M56_U_S = map(XPIN_S, XPIN_L, (XPIN_L-PIN_R), 0, 255) / 2; //6 and 5 go up
          M34_D_S = map(XPIN_S, XPIN_L, (XPIN_L-PIN_R), 0, 255) / 2; //3 and 4 go down
        }
        else if(M_Val > -200)
        {
          M_Val = M_Val -1;
          if(M_Val < 0)M56_U_S = M34_D_S =  abs(M_Val); //3 and 4 go down  6 and 5 go up
          else M56_D_S = M34_U_S =  M_Val; //6 and 5 go down   3 and 4 go up 
        }
      }else
      {
         if(M_Val>0)  M56_D_S = M34_U_S =  M_Val; //6 and 5 go down   3 and 4 go up
         else if(M_Val<0) M56_U_S = M34_D_S =  abs(M_Val); //3 and 4 go down  6 and 5 go up
      }
  }
  else 
  {
      if(M_Val>0)  M56_D_S = M34_U_S =  M_Val; //6 and 5 go down   3 and 4 go up
      else if(M_Val<0) M56_U_S = M34_D_S =  abs(M_Val); //3 and 4 go down  6 and 5 go up
  }
  
  //*/
//  if (YPIN_S < (YPIN_L-1) )
//  {
//      //3 AND 5 go down
//      //M34_U_S = M34_U_S = LOW;
//      M34_U_S = M34_U_S + map(YPIN_S, YPIN_L, (YPIN_L-PIN_R), 0, 255);
//      M56_U_S = M34_U_S + map(YPIN_S, YPIN_L, (YPIN_L-PIN_R), 0, 255);    
//  }
//  else if (YPIN_S > (YPIN_L+1) )
//  {
//     //4 and 6 go down
//      //M56_U_S = M56_U_S = LOW;
//      M34_U_S = M34_U_S + map(YPIN_S, YPIN_L, (YPIN_L+PIN_R), 0, 255); //4 DOWN
//      M56_U_S = M56_U_S + map(YPIN_S, YPIN_L, (YPIN_L+PIN_R), 0, 255);  //6 DOWN
//      if (M34_U_S > 255)
//      {
//          
//      }
//  }  

///*
//ZPIN_S = 315;
//  if ( (RT_S==0) && (LT_S==0) )
//  {
//    if( ZPIN_S < (ZPIN_L) )
//     {
//       //ROV RISING, MOTORS reverse/down
//      // LT_S = map(ZPIN_S, ZPIN_L, (ZPIN_L-100), 0, 255);
//     }
//     else if( ZPIN_S > (ZPIN_L) )
//     {
//       //ROV SINKING, MOTORS forward/up
//      // RT_S = map(ZPIN_S, ZPIN_L, (ZPIN_L+100), 0, 255);
//     }
//  }
  //*/
// /*
  if(LT_S > 0)
  {
    //goes reverse/down more
    if(M_Val > 0)//6 and 5 down, 4 and 3 up
    {
         mapped= map(LT_S, 0, 255, 0, (255 - M56_D_S));
         M56_D_S = M56_D_S + mapped;
                  
         M34_U_S = M34_U_S - mapped;//map(LT_S, 0, 255, 0, (255 - M56_D_S));//change when add y
         if (M34_U_S < 0)
         {
            M34_D_S = abs(M34_U_S);
            M34_U_S = LOW;  
         }
    }
    else if (M_Val < 0)//4 and 3 down, 6 and 5 up
    {
         mapped = map(LT_S, 0, 255, 0, (255 - M34_D_S));
         M34_D_S = M34_D_S + mapped;
         
         M56_U_S = M56_U_S - mapped;
         if (M56_U_S < 0)
         {
            M56_D_S = abs(M56_U_S);
            M56_U_S = LOW; 
         }
    }
    else if (M_Val == 0)
    {  
       M34_U_S = M56_U_S = LOW;
       M34_D_S = M56_D_S = LT_S;
    }
  }
  else if (RT_S > 0)
  {
    //goes forward/up more
    if(M_Val > 0)//6 and 5 down, 4 and 3 up
    {
         mapped = map(RT_S, 0, 255, 0, (255 - M34_U_S));
         M34_U_S = M34_U_S + mapped;
         
         M56_D_S = M56_D_S - mapped;
         if (M56_D_S < 0)
         {
            M56_U_S = abs(M56_D_S);
            M56_D_S = LOW; 
         }
    }
    else if (M_Val < 0)//4 and 3 down, 6 and 5 up
    {
         mapped = map(RT_S, 0, 255, 0, (255 - M56_U_S));
         M56_U_S = M56_U_S + mapped;
         
         M34_D_S = M34_D_S - mapped;
         if (M34_D_S < 0)
         {
            M34_U_S = abs(M34_D_S);
            M34_D_S = LOW; 
         }
    }
    else if (M_Val == 0)
    {
      M34_U_S = M56_U_S = RT_S;
      M34_D_S = M56_D_S = LOW;
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
     Serial.print("\n\n");
     
     Serial.print("RT_S=");
     Serial.print(RT_S, DEC);
     Serial.print("\tLT_S=");
     Serial.print(LT_S, DEC);
     Serial.print("\ttemp=");
     Serial.print(temp, DEC);     
     Serial.print("\n\n");
  
     Serial.print("M56_U_S=");
     Serial.print(M56_U_S, DEC);
     
     Serial.print("\nM56_D_S=");
     Serial.print(M56_D_S, DEC);   
     Serial.print("\n");  
     
     Serial.print("M34_U_S=");
     Serial.print(M34_U_S, DEC);
     
     Serial.print("\nM34_D_S=");
     Serial.print(M34_D_S, DEC);
     Serial.print("\n\n");
     
     Serial.print("CL_UD_S=");
    Serial.print(CL_UD_S, DEC);
    Serial.print("\n");
    
    Serial.print("CR_RL_S=");
    Serial.print(CR_RL_S, DEC);
    Serial.print("\n");
     
    Serial.print("M1_F_S=");
    Serial.print(M1_F_S, DEC);
    Serial.print(", M1_R_S=");
    Serial.print(M1_R_S, DEC);
    Serial.print("\n");
    
    Serial.print("M2_F_S=");
    Serial.print(M2_F_S, DEC);
    Serial.print(", M2_R_S=");
    Serial.print(M2_R_S, DEC);
    Serial.print("\n\n");
     
     test = 0; 
  }
  test= test+1;
  
  
///*
  //Set Motor 1 
  analogWrite(M1_F, M1_F_S);
  analogWrite(M1_R, M1_R_S);

  //Set Motor 2
  analogWrite(M2_F, M2_F_S);
  analogWrite(M2_R, M2_R_S);
  
  //Set Motor 3 and 4
  analogWrite(M34_U, M34_U_S);
  analogWrite(M34_D, M34_D_S);
  
  //Set Motor 5 and 6
  analogWrite(M56_U, M56_U_S);
  analogWrite(M56_D, M56_D_S);

  /*
  //Set Motor 1 test
//  analogWrite(M1_F, 100); //Establishes forward direction of Channel A
//  analogWrite(M1_R, 0);   //Disengage the Brake for Channel A
  
  //Set Motor 2 test
//  analogWrite(2, 200);  //Establishes backward direction of Channel B
//  analogWrite(3, 0);   //Disengage the Brake for Channel B
  
  //Set Motor 1 test
  analogWrite(M34_U, 255); //Establishes forward direction of Channel A
  analogWrite(M34_D, 0);   //Disengage the Brake for Channel A
  
    //Set Motor 1 test
  analogWrite(M34_U, 255); //Establishes forward direction of Channel A
  analogWrite(M34_D, 0);   //Disengage the Brake for Channel A
  
    //Set Motor 1 test
  analogWrite(M56_U, 255); //Establishes forward direction of Channel A
  analogWrite(M56_D, 0);   //Disengage the Brake for Channel A
  
    //Set Motor 1 test
  analogWrite(M56_U, 255); //Establishes forward direction of Channel A
  analogWrite(M56_D, 0);   //Disengage the Brake for Channel A
*/
}
