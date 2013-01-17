/*************************************************************
Motor Shield 2-Channel DC Motor Demo
by Randy Sarafan

For more information see:
http://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/
int LRR = 0;
int UDR = 0;
int LRL = 0;
int UDL = 0;

int M1SPEED=0; //EN1,2
int M2SPEED=0; //EN3,4

int DIRM1_1 = 0; //1A
int DIRM1_2 = 0; //2A

int DIRM2_1 = 0; //3A
int DIRM2_2 = 0; //4A
void setup() {
  //Serial.begin(9600);
  
  //Setup Motor 1
  pinMode(3, OUTPUT);//EN1,2
  pinMode(4, OUTPUT);//1A
  pinMode(2, OUTPUT);//2A

  //Setup Motor 2
  pinMode(6, OUTPUT);//EN3,4
  pinMode(7, OUTPUT);//3A
  pinMode(8, OUTPUT);//4A
  
  //Setup Controller
  pinMode(A5, INPUT);
  pinMode(A2, INPUT);
}

void loop(){
  UDR = analogRead(A5);
  UDL = analogRead(A2);
  
  //Serial.print("UD = ");
  //Serial.print(UDR, DEC);
  //delay(300);
  
  ///*
  M1SPEED=0;
  M2SPEED=0;
    
  if(UDR > 521)
  {
     UDR = UDR-521;
     DIRM1_1 = HIGH;
     DIRM1_2 = LOW;
     M1SPEED= map(UDR, 0,502, 0, 255) ;
  } else if(UDR <521)
  {
    DIRM1_1 = LOW;
    DIRM1_2 = HIGH;
    M1SPEED = map(UDR, 0,521, 255, 0);
  }
  
  if(UDL > 521)
  {
     UDL = UDL-521;
     DIRM2_1 = HIGH;
     DIRM2_2 = LOW;
     M2SPEED= map(UDL, 0,502, 0, 255) ;
  } else if(UDL <521)
  {
    DIRM2_1 = LOW;
    DIRM2_2 = HIGH;
    M2SPEED = map(UDL, 0,521, 255, 0);
  }
  
  //Motor A forward @ full speed
  digitalWrite(4, DIRM1_1); //Establishes forward direction of Channel A
  digitalWrite(2, DIRM1_2);   //Disengage the Brake for Channel A
  analogWrite(3, M1SPEED);   //Spins the motor on Channel A at full speed

  //Motor B backward @ half speed
  digitalWrite(7, DIRM2_1);  //Establishes backward direction of Channel B
  digitalWrite(8, DIRM2_2);   //Disengage the Brake for Channel B
  analogWrite(6, M2SPEED);    //Spins the motor on Channel B at half speed
//*/
}
