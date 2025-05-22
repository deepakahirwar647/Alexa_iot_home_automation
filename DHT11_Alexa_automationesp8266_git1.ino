#define BLYNK_TEMPLATE_ID "your_template_id"
#define BLYNK_TEMPLATE_NAME "DHT11"
#define BLYNK_AUTH_TOKEN "your_auth_token"

#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>

#define WIFI_SSID         "Dk"
#define WIFI_PASS         "wifi_password"
#define APP_KEY           "your_api_key"
#define APP_SECRET        "your_secret_key"

#define RELAY_PIN_1       D1
#define RELAY_PIN_2       D2
#define RELAY_PIN_3       D3

#define SWITCH_ID_1       "xyz"
#define SWITCH_ID_2       "xyz"
#define SWITCH_ID_3       "xyz"

#define DHTPIN            D4
#define DHTTYPE           DHT11

DHT dht(DHTPIN, DHTTYPE);



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

BlynkTimer timer;

bool onPowerState1(const String &deviceId, bool &state) {
  digitalWrite(RELAY_PIN_1, state ? LOW : HIGH);
  return true;
}

bool onPowerState2(const String &deviceId, bool &state) {
  digitalWrite(RELAY_PIN_2, state ? LOW : HIGH);
  return true;
}

bool onPowerState3(const String &deviceId, bool &state) {
  digitalWrite(RELAY_PIN_3, state ? LOW : HIGH);
  return true;
}

void setupWiFi() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void setupSinricPro() {
  SinricProSwitch &mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  SinricProSwitch &mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);

  SinricProSwitch &mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

void setup() {
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);

  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
  digitalWrite(RELAY_PIN_3, HIGH);

  setupWiFi();
  setupSinricPro();

  dht.begin();
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
  SinricPro.handle();
}
