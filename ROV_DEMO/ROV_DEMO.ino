/*************************************************************
H-Bridge DC Motor Controller

Robbie Lundine
Justin Gilmer

*************************************************************/

//Define ports
const int M1_PW = 3;
const int M1_D = 12;
const int M2_PW = 11;
const int M2_D = 13;

const char CL_UD = A3;
const char CR_RL = A5;

int test = 0;
int dir = 0;
int XboxConnected;

void setup() {
  Serial.begin(9600);
  
  //Setup Motor 1
  pinMode(M1_D, OUTPUT);
  pinMode(M1_PW, OUTPUT);

  //Setup Motor 2
  pinMode(M2_D, OUTPUT);
  pinMode(M2_PW, OUTPUT);
  
  pinMode(CL_UD, INPUT);
  pinMode(CR_RL, INPUT);
  
  test=0;
}

void loop(){
  // initialize motors
  int M1_D_S, M1_PW_S, M2_D_S, M2_PW_S;
  M1_D_S = M1_PW_S = M2_D_S = M2_PW_S = 0;
  
  // init/setup controls
  int CR_RL_S, CR_UD_S, CL_UD_S, CL_RL_S;
  CR_RL_S = CR_UD_S = CL_UD_S = CL_RL_S = 0;
  
  float M1_F_S, M1_R_S, M2_F_S, M2_R_S, M34_U_S, M34_D_S, M56_U_S, M56_D_S;
  M1_F_S = M1_R_S = M2_F_S = M2_R_S = M34_U_S = M34_D_S = M56_U_S = M56_D_S = 0;
  
  CL_UD_S = analogRead(CL_UD);
  CR_RL_S = analogRead(CR_RL);
 
  /**********************start of new controls*****************************/

  dir = 0;
  if (CL_UD_S > 344)
  {
    // M1 FORWARD, M2 FORWARD
     M1_PW_S = M2_PW_S = map(CL_UD_S, 344, 690, 0, 255);
     M1_D_S = M2_D_S = HIGH;
     dir = 1;
  }
  else if (CL_UD_S < 339)
  {
     // M1 REVERSE, M2 REVERSE
     M1_PW_S = M2_PW_S = map(CL_UD_S, 339, 0, 0, 255);
     M1_D_S = M2_D_S = LOW;
     dir = 2;
  }

  if (CR_RL_S < 339 ) //GO RIGHT
  {
      if (dir==1) //both forward
      {
          M2_PW_S = M2_PW_S - map(CR_RL_S, 339, 0, 0, 255);
          if (M2_PW_S < 0)
          {
              M2_PW_S = abs(M2_PW_S);
              M2_D_S = LOW;
              M1_PW_S = M1_PW_S + M2_PW_S;
          }
      }
      else if (dir==2) //both reverse
      {
          M1_PW_S = M1_PW_S - map(CR_RL_S, 339, 0, 0, 255);
          if (M1_PW_S < 0)
          {
              M1_PW_S = abs(M1_PW_S);
              M1_D_S = LOW;
              M2_PW_S = M2_PW_S + M1_PW_S;
          }
      }
      else
      {
          M1_PW_S = M2_PW_S = map(CR_RL_S, 339, 0, 0, 255);
          M1_D_S = HIGH;
          M2_D_S = LOW;
      }
  }
  else if (CR_RL_S > 344)
  {
      if (dir==1) //both forward
      {
          M1_PW_S = M1_PW_S - map(CR_RL_S, 344, 690, 0, 255);
          if (M1_PW_S < 0)
          {
              M1_PW_S = abs(M1_PW_S);
              M1_D_S = LOW;
              M2_PW_S = M2_PW_S + M1_PW_S;
          }
      }
      else if (dir==2) //both reverse
      {
          M2_PW_S = M2_PW_S - map(CR_RL_S, 344, 690, 0, 255);
          if (M2_PW_S < 0)
          {
              M2_PW_S = abs(M2_PW_S);
              M2_D_S = LOW;
              M1_PW_S = M1_PW_S + M2_PW_S;
          }
      }
      else
      {
          M1_PW_S = M2_PW_S = map(CR_RL_S, 344, 690, 0, 255);
          M1_D_S = LOW;
          M2_D_S = HIGH;
      }
  } 
 
 
   if (test == 3000)
   { 
    Serial.print("CL_UD_S=");
    Serial.print(CL_UD_S, DEC);
    Serial.print("\n");
    
    Serial.print("CR_RL_S=");
    Serial.print(CR_RL_S, DEC);
    Serial.print("\n");
     
    Serial.print("M1_D_S=");
    Serial.print(M1_D_S, DEC);
    Serial.print(", M1_PW_S=");
    Serial.print(M1_PW_S, DEC);
    Serial.print("\n");
    
    Serial.print("M2_D_S=");
    Serial.print(M2_D_S, DEC);
    Serial.print(", M2_PW_S=");
    Serial.print(M2_PW_S, DEC);
    Serial.print("\n\n");
     
     test = 0; 
  }
  test= test+1;
  
  
  //Set Motor 1 
  digitalWrite(M1_D, M1_D_S);
  analogWrite(M1_PW, M1_PW_S);

  //Set Motor 2
  digitalWrite(M2_D, M2_D_S);
  analogWrite(M2_PW, M2_PW_S);
  

  
  //Set Motor 1 test
//  analogWrite(M1_PW, 100); //Establishes forward direction of Channel A
//  digitalWrite(13, HIGH);   //Disengage the Brake for Channel A
  
  //Set Motor 2 test
//  analogWrite(M2_PW, 100);  //Establishes backward direction of Channel B
//  digitalWrite(12, LOW);   //Disengage the Brake for Channel B
  
 
}
