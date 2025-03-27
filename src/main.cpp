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

const char *ledTopic = "ESP32_SENSOR/ledControl"; // Corrigido para o tópico certo

byte led = 2;

/*------------------------------FUNCTIONS------------------------------*/

bool connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Connecting to WiFi...");
  
  byte tentativas = 0;
  while (!WiFi.isConnected() && tentativas < 10) {
    Serial.print("Status: ");
    Serial.println(WiFi.status());
    delay(1000);
    tentativas++;
  }
  
  if (WiFi.isConnected()) {
    Serial.println("Connected to WiFi!");
  } else {
    Serial.println("Failed to connect to WiFi");
  }
  return WiFi.isConnected();
}

bool connectToBroker() {
  byte tentativas = 0;
  mqttClient.setServer(BROKER, 8883);

  while (!mqttClient.connected() && tentativas < 10) {
    if (mqttClient.connect(MQTT_BOARD_ID, MQTT_USERNAME, MQTT_PASS)) {
      Serial.println("Connected to broker!");
      mqttClient.setCallback(messageReceived);
      mqttClient.subscribe(ledTopic); // Assina o tópico correto
    } else {
      Serial.println("Failed to connect to broker");
      delay(1000);
      tentativas++;
    }
  }
  return mqttClient.connected();
}

void messageReceived(const char* topic, byte* message, unsigned int length) {
  String status;
  for (int i = 0; i < length; i++) {
    status += (char)message[i];
  }
  
  Serial.print("Recebido do tópico ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(status);
  
  if (strcmp(topic, ledTopic) == 0) {
    if (status == "Acender") {
      digitalWrite(led, HIGH);
      Serial.println("LED Ligado");
    } else if (status == "Apagar") {
      digitalWrite(led, LOW);
      Serial.println("LED Desligado");
    }
  }
}

/*---------------------------------MAIN LOOPS--------------------------------------*/
void setup() {
  client.setInsecure();
  Serial.begin(9600);

  connectToWifi();
  connectToBroker();

  pinMode(led, OUTPUT);
}

void loop() {
  if (!WiFi.isConnected()) {
    connectToWifi();
  }

  if (!mqttClient.connected()) {
    connectToBroker();
  }

  mqttClient.loop();
  delay(100);
}
