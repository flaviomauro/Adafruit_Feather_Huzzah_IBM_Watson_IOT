/* 
 *******************************************************************************
 * Description:
 * Sample code to connect to WiFi and send data to IBM Watson IoT Platform.
 * GitHub: https://github.com/flaviomauro/Adafruit_Feather_Huzzah_IBM_Watson_IOT
 *******************************************************************************
 * Item list:
 * 
 * - 1 x Adafruit Feather Huzzah
 * - 1 x Micro USB cable
 *******************************************************************************
 * @author: Flavio H. C. Mauro
 * @date:   06-Dec-2017
 * LinkedIn: https://br.linkedin.com/in/flaviomauro
 *******************************************************************************
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//*********************************************************************************
// WiFi Connection Setup
//*********************************************************************************
const char* WIFI_SSID     = "*********"; //Add the WiFi SSID you want to connect to
const char* WIFI_PASSWORD = "*********"; //Add the password for the WiFi SSID

//*********************************************************************************
// IBM Watson IoT Platform device setup
//*********************************************************************************
//  Go to https://quickstart.internetofthings.ibmcloud.com/#/ to check the data 
//  sent from your device. 
//  Use the value from the DEVICE_ID to check data sent from it.
const char* DEVICE_ORG    = "quickstart"; // your organization
const char* DEVICE_TYPE   = "Adafruit-Feather-Huzzah"; // your device type
const char* DEVICE_ID     = "Add-an-unique-identifier-here"; // your device id
const char* DEVICE_TOKEN  = ""; // your device token (not necessary on quickstart)

const char* IBM_CLOUD_DOMAIN = ".messaging.internetofthings.ibmcloud.com";
const char* TOPIC = "iot-2/evt/status/fmt/json";
const char* AUTH_METHOD = "use-token-auth";

String server = String(DEVICE_ORG) + String(IBM_CLOUD_DOMAIN);
const char* SERVER_URL = server.c_str();

String clientId = "d:" + String(DEVICE_ORG) + ":" + String(DEVICE_TYPE) + ":" + String(DEVICE_ID);
const char* CLIENT_ID = clientId.c_str();
//*********************************************************************************


void callback(char* topic, byte* payload, unsigned int length) {
  String msgString = String((char*)payload);
  Serial.println();
  Serial.println("Payload Callback Message");
  Serial.println(msgString);
}


WiFiClient wifiClient;
PubSubClient client(SERVER_URL, 1883, callback, wifiClient); // 8883: the ssl enabled port number. ssl is not always supported on the arduino.


void setup() {
  Serial.begin(115200);
  delay(100);

  connectToWifi();
  printIoTSetupInfo();
}

 
void loop() 
{
  if (!!!client.connected()) {
    Serial.print("Connecting client to "); 
    Serial.println(SERVER_URL);
    
    while (!!!client.connect(CLIENT_ID, AUTH_METHOD, DEVICE_TOKEN)) 
    {
      Serial.print(".");
      delay(500);
    }
  
    Serial.println();
  }

  publishSensorData(getSensorData());
}


/*********************************************************************************
 * Returns JSON FORMATED sensor data
 ********************************************************************************/
String getSensorData()
{
  String payload = "{ \"d\" : {\"dummy-counter\":";
  payload += millis()/1000;
  payload += "}}";

  delay(3000);
  return payload;
}


/*********************************************************************************
 * Publishes Sensor data to IBM Watson IoT Platform
 ********************************************************************************/
void publishSensorData(String payload)
{
  Serial.print("Sending payload: "); 
  Serial.println(payload);
  
  if (client.publish(TOPIC, (char*) payload.c_str())) 
  {
    Serial.println("Message published.");
  } 
  else 
  {
    Serial.println("Message publish failed");
  }
}


/*********************************************************************************
 * Connect Adafruit Feather Huzzah to Wifi
 ********************************************************************************/
void connectToWifi()
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to SSID: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.print("Connected to SSID: ");
  Serial.println(WIFI_SSID);
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}


/*********************************************************************************
 * Print IOT setup and device info to Serial
 ********************************************************************************/
void printIoTSetupInfo()
{
  Serial.println();
//  Serial.print("DEVICE_ORG: ");
//  Serial.println(String(DEVICE_ORG));
//  Serial.print("DEVICE_TYPE: ");
//  Serial.println(String(DEVICE_TYPE));
//  Serial.print("DEVICE_ID: ");
//  Serial.println(String(DEVICE_ID));
//  Serial.print("DEVICE_TOKEN: ");
//  Serial.println(String(DEVICE_TOKEN));
// Serial.print("IBM_CLOUD_DOMAIN: ");
//  Serial.println(String(IBM_CLOUD_DOMAIN));
  Serial.print("SERVER_URL: ");
  Serial.println(String(SERVER_URL));
  Serial.print("CLIENT_ID: ");
  Serial.println(String(CLIENT_ID));
  Serial.println();
}
