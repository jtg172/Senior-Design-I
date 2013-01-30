/*************************************************************
Motor Shield 2-Channel DC Motor Demo
by Randy Sarafan

For more information see:
http://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/
//Define ports

int M1_1 = 2;
int M1_2 = 3;
int M2_1 = 4;
int M2_2 = 5;

char CL_UD = A0;
char CL_RL = A1;

char CR_UD = A2;
char CR_RL = A3;

void setup() {
//  Serial.begin(9600);
  
  //Setup Motor 1
  pinMode(M1_1, OUTPUT);
  pinMode(M1_2, OUTPUT);

  //Setup Motor 2
  pinMode(M2_1, OUTPUT);
  pinMode(M2_2, OUTPUT);
  
  //Setup Controller
  pinMode(CL_UD, INPUT);
  pinMode(CR_UD, INPUT);
}

void loop(){
  
  int M1_1_S, M1_2_S, M2_1_S, M2_2_S;
  int CL_UD_S, CR_UD_S;
  
  M1_1_S = M1_2_S = M2_1_S = M2_2_S = 0;
  CL_UD_S = CR_UD_S = 0;
  
  CL_UD_S = analogRead(CL_UD);
  CR_UD_S = analogRead(CR_UD);
  
  /*
  Serial.print("CR_UD_S = ");
  Serial.print(CR_UD_S, DEC);
  delay(300);
 
  Serial.print(", CL_UD_S = ");
  Serial.print(CL_UD_S, DEC);
  delay(1000);
  */
   
 if(CR_UD_S > 521)
  {
     CR_UD_S = CR_UD_S - 521;
     M1_1_S = map(CR_UD_S, 0, 502, 0, 255) ;
     M1_2_S = LOW;
  } 
  else if(CR_UD_S <= 521)
  {
    M1_1_S = LOW;
    M1_2_S = map(CR_UD_S, 0, 521, 255, 0);
  }
  
  if(CL_UD_S > 521)
  {
    CL_UD_S = CL_UD_S - 521;
    M2_1_S = map(CL_UD_S, 0, 502, 0, 255);
    M2_2_S = LOW;
  } 
  else if(CL_UD_S <= 521)
  {
    M2_1_S = LOW;
    M2_2_S = map(CL_UD_S, 0, 521, 255, 0);
  }
 

  //Set Motor 1 
  analogWrite(M1_1, M1_1_S); //Establishes forward direction of Channel A
  analogWrite(M1_2, M1_2_S);   //Disengage the Brake for Channel A

  //Set Motor 2
  analogWrite(M2_1, M2_1_S);  //Establishes backward direction of Channel B
  analogWrite(M2_2, M2_2_S);   //Disengage the Brake for Channel B

  /*
  //Set Motor 1 test
  analogWrite(2, 0); //Establishes forward direction of Channel A
  analogWrite(3, 200);   //Disengage the Brake for Channel A
  
  //Set Motor 2 test
  analogWrite(4, 0);  //Establishes backward direction of Channel B
  analogWrite(5, 200);   //Disengage the Brake for Channel B
*/
}
