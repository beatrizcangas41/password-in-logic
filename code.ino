#include <EEPROM.h>
#include <Servo.h>

Servo servo;        //initialize a servo object for the connected servo  
int angle = 0;    


//const int PROGRAMMING_MODE = 3;

int analogInPin = A0;
int motorLight  = A1;
int programmingButton  = A2;

int enterPass = 4;
int led_Right = 2;
int led_Left  = 3;

int programming = 1;
long value = 0;

double sensorValue = 0;        // value read from the photoresistor
double roomL;
double handL;

bool lightC = true;
bool lightF = true;
bool stopAll = 0;

double lightCloseRange[] = {0,0};
double lightFarRange[] =   {0,0};


void setup() {
  // put your setup code here, to run once:
    servo.attach(5);      // attach the signal pin of servo to pin5 of arduino

    pinMode (analogInPin, INPUT);
    pinMode (led_Right, OUTPUT);
    pinMode (led_Left, OUTPUT);
    pinMode (motorLight, OUTPUT);

    pinMode (enterPass, INPUT);
    pinMode (programmingButton, INPUT);

    Serial.begin(9600);
    roomL = getRoomLight();
}

void loop() {
//   for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
//  {                                  
//    servo_test.write(angle);                 //command to rotate the servo to the specified angle
//    delay(15); }           
  //Higher values of sensorValue represent more shade on the potentiometer
  //Lower values of sensorValue represent more light on the potentiometer
    int programmingMode = analogRead(programmingButton);
    int enterP = digitalRead(enterPass);
    int readMem = EEPROM.read(0);      

     if(programmingMode > 100){
        delay(1050);
        digitalWrite(led_Right,HIGH);
        digitalWrite(led_Left,HIGH);
        delay(800);
        digitalWrite(led_Right,LOW);
        digitalWrite(led_Left,LOW);  
        Serial.print("now entering programming mode");
        Serial.print("\n");
        delay(2000);
        programming_Mode();
        delay(2000);
      }


      if(enterP == HIGH){
        delay(200);
        digitalWrite(led_Right,HIGH);
        delay(75);
        digitalWrite(led_Right,LOW);
        delay(75);
        digitalWrite(led_Right,HIGH);
        delay(75);
        digitalWrite(led_Right,LOW);
        delay(75);
        digitalWrite(led_Right,HIGH);
        delay(75);
        digitalWrite(led_Right,LOW);
        delay(100);
        enterPassword();
      }


      if(readMem == 0) {
         digitalWrite(led_Left,LOW);
         analogWrite(motorLight,0);
      }
      
      if(readMem == 20){

            Serial.print("counterclockwise");
            Serial.print("\n");
          servo.write(180);
        for(angle = 0; angle <= 180; angle+=10)    // command to move from 0 degrees to 180 degrees 
        {
          analogWrite(motorLight,255);
          delay(200);
          analogWrite(motorLight,0);
          delay(200);                                  
          servo.write(angle);                 //command to rotate the servo to the specified angle
//          delay(10);                       
        }          


        }

       if(readMem == 21){
            Serial.print("clockwise");
            Serial.print("\n");
            servo.write(180);
//         analogWrite(motorLight,255);
//         delay(50);
//         analogWrite(motorLight,0);
//         delay(50);
         for(angle = 180; angle>=0; angle-=10)     // command to move from 180 degrees to 0 degrees 
          { 
            analogWrite(motorLight,255);
            delay(75);
            analogWrite(motorLight,0);
            delay(75);                               
            servo.write(angle);              //command to rotate the servo to the specified angle
            delay(75);                       
          }

               
        
        }
}

double getRoomLight(){
  int roomLighting = 0;
  int i = 0;
  
  do{
    sensorValue = analogRead(analogInPin);
    roomLighting += sensorValue;
    i++;
    delay(10);
      }while(i<=6);
    digitalWrite(led_Right,HIGH);
    delay(800);
    digitalWrite(led_Right,LOW);
    delay(800);
    digitalWrite(led_Left,HIGH);
    delay(800);
    digitalWrite(led_Left,LOW);
    delay(800);
    digitalWrite(led_Right,HIGH);
    digitalWrite(led_Left,HIGH);
    delay(800);
    digitalWrite(led_Right,LOW);
    digitalWrite(led_Left,LOW);

  return (roomLighting = (roomLighting/7));
  }

  void enterPassword(){
    Serial.print("Please enter password");
    Serial.print("\n");
    delay(800);
    int storedVal = EEPROM.read(0);
    char passwordInput[] = "00000";

    
    for(int i = 7; i >= 3; i--){
    passwordInput[i] = handProximity();
    Serial.print(passwordInput[i]);
    Serial.print("\n");
    }
    
    Serial.print(passwordInput);
    Serial.print("\n");
    value = strtol(passwordInput, NULL, 2);
    if(value == storedVal){
      Serial.print("Password Correct!");
      Serial.print("\n");      
      digitalWrite(led_Left,HIGH);
      analogWrite(motorLight,200);
      }
     else{
        Serial.print("Password incorrect!");
        Serial.print("\n");       
      }
    
    value = 0;
    delay(800);

    }

  void programming_Mode(){

    char passwordStore[] = "00000";
     
    for(int i = 7; i >= 3; i--){
    passwordStore[i] = handProximity();
    Serial.print(passwordStore[i]);
    Serial.print("\n");
    }
    Serial.print(passwordStore);
    Serial.print("\n");
    value = strtol(passwordStore, NULL, 2);
    Serial.print(value);
    Serial.print("\n");
    EEPROM.write(0, value);
    value = 0;
    }


  char handProximity(){
  
        handL = analogRead(analogInPin);
      if(handL>810){
        handClose();
        Serial.print(handL);
        Serial.print("hand close");
        Serial.print("\n");
        return '1';
        }

       if (handL<800) {
        handFar();
         Serial.print(handL);
         Serial.print("hand far");
         Serial.print("\n");
         return '0';
        }
        
        delay(150);
    }

   void handClose(){
      digitalWrite(led_Right,HIGH);
      delay(200);
      digitalWrite(led_Right,LOW);
      delay(200);
      digitalWrite(led_Right,HIGH);
      delay(200);
      digitalWrite(led_Right,LOW);
      delay(1000);
    }

    void handFar(){
      delay(100);
      digitalWrite(led_Right,HIGH);
      delay(1000);
      digitalWrite(led_Right,LOW);
      delay(1000);
    }
