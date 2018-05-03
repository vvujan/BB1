#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
//mqtt
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
 
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345); 

const char* ssid = "ssid";
const char* password = "password";
const char* server = "server"; 
char* topic = "topic";   
const char* deviceName="device name";
const char* userName="username";
const char* userPass="userpass";
char message_buff[10];  
String msgData;

double refX,refY;
int stepxy = 5;
int speedy, speedx;
String directionxy="";
String prevDirectionxy="";

byte inMessage;
char in;
WiFiClient wifiClient;

// PubSubClient MQTT subscription function:
void callback(char* topic, byte* payload, unsigned int length) {
    int i = 0;
    Serial.println("Message arrived:  topic: " + String(topic));
    Serial.println("Length: " + String(length,DEC)); 
    // create character buffer with ending null terminator (string)
    for(i=0; i<length; i++) {
      message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';
}


PubSubClient client(server, 1883, callback, wifiClient);
  
String macToStr(const uint8_t* mac)
{
    String result;
    for (int i = 0; i < 6; ++i) {
      result += String(mac[i], 16);
      if (i < 5)
        result += ':';
    }
    return result;
}

 
void setup(void) 
{
  Serial.begin(115200);

    Serial.print("Connecting to ");
    Serial.println(ssid); 
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected"); 
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
   
  //  connection to broker script.
    if (client.connect("ctrlSender")) {
      client.subscribe(topic);
    } 
 
 /* Initialise the sensor */
  if(!accel.begin())
  {
   /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  
   }
   
   /* Set the range to whatever is appropriate for your project */
    accel.setRange(ADXL345_RANGE_16_G);
    //set referent value
    sensors_event_t event; 
    accel.getEvent(&event);
    refX=event.acceleration.x;
    refY=event.acceleration.y;
    Serial.print("Ref x:"); Serial.print(refX);Serial.print(" ");Serial.print("Ref y:"); Serial.print(refY);
  }

  void reconnect() {
  while (!client.connected()) {
    if (client.connect(deviceName, userName, userPass)) {
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
  void loop(void) 
  {
   /* Get a new sensor event */ 
    sensors_event_t event; 
    accel.getEvent(&event);

    //empty direction
    directionxy="";
    speedx = (int) (event.acceleration.x-refX)/stepxy; 
    speedy = (int) (event.acceleration.y-refY)/stepxy;
  
    directionxy.concat(speedx);
    directionxy.concat(',');
    directionxy.concat(speedy);
  
    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (client.connected() and prevDirectionxy != directionxy and directionxy != ""){
    
      Serial.println(directionxy);
      if (client.publish(topic, (char*) directionxy.c_str())) {
      }   
    
  }
  prevDirectionxy = directionxy;
  delay(40);
}

 

