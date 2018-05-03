#include <Servo.h>

int servoPin = 3;
Servo Servo1;

//set motors pin
int leftForward=5;
int leftBackward=6;
int rightForward=9;
int rightBackward=10;

int leftForwardValue=0;
int leftBackwardValue=0;
int rightForwardValue=0;
int rightBackwardValue=0;
int headStatus = 0;

String inputString = "";

void setAllPinsOnLow() {
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBackward, LOW);
    digitalWrite(rightForward, LOW);
    digitalWrite(rightBackward, LOW);
    //servo
    Servo1.write(90);

    // reserve 200 bytes for the inputString:
    //inputString.reserve(200);
}

void setup() {
    // We need to attach the servo to the used pin number
    Servo1.attach(servoPin);
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
        Servo1.write(90); 
      else if(headStatus == 1)  {  //left
      
        Servo1.write(0);
        }
      else if(headStatus == 2) { //right
        
        Servo1.write(180);
        }
   
    }
        
}

void left(int angle){
  for (int pos = 0; pos <= angle; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Servo1.write(pos); 
    delay(1);                       // waits 15ms for the servo to reach the position
  }
  Servo1.write(0);
}
void right(int angle){
  for (int pos = angle; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    Servo1.write(pos);  
    delay(1);                       // waits 15ms for the servo to reach the position
  }
  Servo1.write(0);
  
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



