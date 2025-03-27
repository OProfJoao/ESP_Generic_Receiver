const char *WIFI_SSID = "TesteFerrorama";
const char *WIFI_PASS = "Ferrorama1";

const char *BROKER = "9ba2cb32944a4266a47c6f2a46d1bd26.s1.eu.hivemq.cloud";
const char *MQTT_USERNAME = "GenericReceiver";
const char *MQTT_PASS = "GenericReceiver1";

const char *MQTT_BOARD_ID = "ESP32_RECEIVER";

#ifndef WIFI_CONNECTION
#define WIFI_CONNECTION

#define WIFI_SSID WIFI_SSID
#define WIFI_PASS WIFI_PASS
#endif

#ifndef MQTT_CONNECTION
#define MQTT_CONNECTION
#define MQTT_BOARD_ID MQTT_BOARD_ID

#define BROKER BROKER
#define MQTT_USERNAME MQTT_USERNAME
#define MQTT_PASS MQTT_PASS
#endif