#include <WiFi.h>
#include <FirebaseESP32.h>
#include "DHT.h"
#define DHTPIN 4         // P
#define DHTTYPE DHT11   // DHT 11
#define FIREBASE_HOST "https://test4-8c9e7-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "BrQKjJhc16DBvEGPpqeWrwOTH6pqNwLKkgPkbzSV"
#define WIFI_SSID "HTPro-T1"
#define WIFI_PASSWORD "Ht88888888"
DHT dht(DHTPIN, DHTTYPE);
 float h = 0;
float t = 0;
//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;
unsigned long time_dht_delay=2000;
unsigned long dht_time=0;
int lastHumi , lastTemp;
void setup()
{
 
  Serial.begin(115200);
 
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
 
  /*
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  */
 
  //String path = "/data";
  
 
  Serial.println("------------------------------------");
  Serial.println("Connected...");
  
}
 
void loop()
{
   if(millis() -dht_time >= time_dht_delay)
   {
    readDHT11();
    dht_time=millis();
    }
   json.clear();
    json.add("tem", t);
    json.add("hum", h);

   Firebase.updateNode(firebaseData,"/data",json);
 
}
void readDHT11() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    lastHumi = h;
    lastTemp = t;
  }
  Serial.print("Humidity: ");
  Serial.print(lastHumi);
  Serial.print("%t");
  Serial.print("Temperature: ");
  Serial.print(lastTemp);
  Serial.print("°C");
  Serial.println();
}
