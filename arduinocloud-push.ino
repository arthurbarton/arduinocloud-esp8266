#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoCloud.h>
/* a sensor to get some data from */
#include <DHT.h>

/* Wifi */
const char* ssid    = "x";
const char* wifikey = "y";

/* Arduino Cloud config */
WiFiClientSecure  net;
ArduinoCloudThing yourThingName;
const char userName[]   = "myname";
const char thingName[]  = "cloudthingname";
const char thingId[]    = "cloudthing-id";
const char thingPsw[]   = "cloudthing-pw";

/* Sensor config */
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float   currentHumidity;
float   currentTemperature;
float   currentHeadIndex;


void setup() {
  
  /* Set WiFi to station mode 
   * Disconnect from an AP if it was previously connected
  */
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, wifikey);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  dht.begin();
  delay(1000);

  yourThingName.begin(thingName, userName, thingId, thingPsw, net);
  // yourThingName.enableDebug();
  // define the properties
  yourThingName.addProperty("humidity", FLOAT, R);
  yourThingName.addProperty("temperature", TEMPERATURE_C, R);

}

void updateSensorValues() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  currentHumidity     = dht.readHumidity();
  currentTemperature  = dht.readTemperature();  // Celcius is the default

  // Compute heat index in Celsius (isFahreheit = false)
  // Why one defaults to C and the other F is perfectly irritating
  currentHeadIndex = dht.computeHeatIndex(currentTemperature, currentHumidity, false);
}

void loop() {

  // This takes approx 500ms
  updateSensorValues();

  // Internet depending - approx 300ms
  yourThingName.poll();
  if (!isnan(currentHumidity)){
    yourThingName.writeProperty("humidity", currentHumidity);
  }
  if (!isnan(currentTemperature)){
    yourThingName.writeProperty("temperature", currentTemperature);
  }
  delay(5000);
}
