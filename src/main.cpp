/*------------------------------IMPORTED LIBRARIES------------------------------*/
#include <Arduino.h>
#include <pubSubClient.h>
#include <WiFiClientSecure.h>
#include <env.h>


/*------------------------------FUNCTION HEADERS------------------------------*/

bool connectToWifi();
bool connectToBroker();
void messageReceived(const char* topic, byte* message, unsigned int length);

/*------------------------------DEFINED VARIABLES------------------------------*/

WiFiClientSecure client;
PubSubClient mqttClient(client);

const char *lightTopic = "ESP32_SENSOR/lightSensor";

byte led = 2;

/*------------------------------FUNCTIONS------------------------------*/

bool connectToWifi(){
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  Serial.println("Connecting to WiFi");
  byte tentativas;
  while(WiFi.isConnected() == false && tentativas < 10){
    Serial.print("Status: "); 
    Serial.println(WiFi.status());
    delay(1000);
    tentativas ++;
  }
  if(WiFi.isConnected()){
    Serial.println("Connected to WiFi");
  }
  else{
    Serial.println("Failed to connect to WiFi");
  }
  return WiFi.isConnected();
}

bool connectToBroker(){
  byte tentativas;
  while(mqttClient.connected() == false && tentativas < 10){
    mqttClient.setServer(BROKER,8883);
    mqttClient.connect("ESP32_SENSOR",MQTT_USERNAME,MQTT_PASS);
    if(mqttClient.connected() == false){
      Serial.println("Failed to connect to broker");
      delay(1000);
      tentativas ++;
    }else{
      Serial.println("Connected to broker");
      mqttClient.setCallback(messageReceived);

      mqttClient.subscribe(lightTopic);
    }
    delay(1000);
  }
  return mqttClient.connected();
}

void messageReceived(const char* topic, byte* message, unsigned int length){
  //Do something if receives any message
  String status;
  for (int i = 0; i<length;i++){
    char c = (char)message[i];
    status += c;
  }
  if(status == "Acender"){
    digitalWrite(led,HIGH);
  }
  else if(status == "Apagar"){
    digitalWrite(led,LOW);
  }
}

/*---------------------------------MAIN LOOPS--------------------------------------*/

void setup()
{
  client.setInsecure();

  Serial.begin(9600);

  connectToWifi();
  connectToBroker();

  pinMode(led,OUTPUT);
}

void loop()
{
  if(WiFi.isConnected() == false){
    connectToWifi();
  }

  if(mqttClient.connected() == false){
    connectToBroker();
  }

  mqttClient.loop();
  delay(1000);
}