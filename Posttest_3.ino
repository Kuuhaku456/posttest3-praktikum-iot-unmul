#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>

#define DHTPIN D5
#define DHTTYPE DHT11
#define LED_RED D1
#define LED_YELLOW D2
#define LED_GREEN D3
const int BUZZER_PIN = D6;

DHT dht(DHTPIN, DHTTYPE);

unsigned long CHANNEL_ID = 2479777;
const char *ssid = "Universitas Mulawarman";
const char *password = "";
const char *thingSpeakApiKey = "5LQVXE5ZH09OXVJM";
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  ThingSpeak.begin(client);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  float Celcius = dht.readTemperature();
  float Fahrenheit = (Celcius * 9/5) + 32;
  float Reamur = Celcius * 4/5;
  float Kelvin = Celcius + 273;

  if (isnan(Celcius)) {
    Serial.println("Sensor tidak terbaca!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.println(Celcius);


  // Kontrol LED berdasarkan suhu
  if (Celcius < 30) {
    digitalWrite(LED_GREEN, HIGH); // Hijau
    digitalWrite(LED_YELLOW, LOW); // Hijau
    digitalWrite(LED_RED, LOW); // Hijau
    noTone(BUZZER_PIN);
  } else if (Celcius >= 30 && Celcius <= 36) {
    digitalWrite(LED_GREEN, LOW); // Hijau
    digitalWrite(LED_YELLOW, HIGH); // Kuning
    digitalWrite(LED_RED, LOW); // Hijau
    noTone(BUZZER_PIN);
  } else {
    digitalWrite(LED_GREEN, LOW); // Hijau
    digitalWrite(LED_YELLOW, LOW); // Kuning
    digitalWrite(LED_RED, HIGH); // Merah
    tone(BUZZER_PIN, 500);
  }

  // Kirim data suhu ke ThingSpeak
  ThingSpeak.setField(1, Celcius);
  ThingSpeak.setField(2, Fahrenheit);
  ThingSpeak.setField(3, Reamur);
  ThingSpeak.setField(4, Kelvin);
  ThingSpeak.writeFields(CHANNEL_ID, thingSpeakApiKey);

  delay(8000); // Delay untuk baca Suhu 
}