#include <Servo.h>

int servoPin = 3;
Servo Servo1;

//set motors pin
int leftForward=5;
int leftBackward=6;
int rightForward=A3;
int rightBackward=A2;

int leftForwardValue=0;
int leftBackwardValue=0;
int rightForwardValue=0;
int rightBackwardValue=0;
int headStatus = 0;

String inputString = "";

void setAllPinsOnLow() {
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBackward, LOW);
    analogWrite(rightForward, 0);
    analogWrite(rightBackward, 0);
    //servo
    useServo(90,500);
    // reserve 200 bytes for the inputString:
    //inputString.reserve(200);
}

void setup() {
    // We need to attach the servo to the used pin number
    pinMode(leftForward, OUTPUT);
    pinMode(leftBackward, OUTPUT);
    pinMode(rightForward, OUTPUT);
    pinMode(rightBackward, OUTPUT);

    //set all pins on low
    setAllPinsOnLow();
    Serial.begin(115200);
}
void loop() {
    while (Serial.available()) {
       inputString="";
        // get the new byte:
        String inChar = Serial.readStringUntil('\n'); 
      //String inChar = Serial.readString();
        // add it to the inputString:
      inputString += inChar;
      Serial.println(inputString);
      setAllPinsOnLow();
      
      leftForwardValue = getValue(inputString,',',0).toInt(); 
      leftBackwardValue = getValue(inputString,',',1).toInt();
      rightForwardValue = getValue(inputString,',',2).toInt();
      rightBackwardValue = getValue(inputString,',',3).toInt();
      headStatus = getValue(inputString,',',4).toInt();
  
      analogWrite(leftForward, leftForwardValue);
      analogWrite(leftBackward, leftBackwardValue);
      analogWrite(rightForward, rightForwardValue);
      analogWrite(rightBackward, rightBackwardValue);
      if (headStatus == 0)
      {
        useServo(90,500);
      }
      else if(headStatus == 1)  //left
      { 
        useServo(0,500);
        }
      else if(headStatus == 2) //right
      { 
        useServo(180,500);
        }
   
    }
} 
void useServo(int angle, int delays){
  Servo1.attach(servoPin);
  Servo1.write(angle);
  delay(delays);
  Servo1.detach() ;
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}



