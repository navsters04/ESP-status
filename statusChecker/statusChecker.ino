#include <WiFi.h>        
#include <HTTPClient.h>  

const char* SSID = "name";
const char* PASS = "password";
const char* URL  = "https://esp-status.vercel.app/api/status";
const char* TOKEN = "kX9f2mQp7Lw3Rz8VbT4nJc6Ys1";

int  OUTPUT_CLOSE_PIN = 33;
int  OUTPUT_OPEN_PIN = 25;
int  OUTPUT_AWAY_PIN = 26;
int  INPUT_CLOSE_PIN = 33;
int  INPUT_OPEN_PIN = 14;
int  INPUT_AWAY_PIN = 12;

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("Connected");

  pinMode(OUTPUT_CLOSE_PIN, OUTPUT);
  pinMode(OUTPUT_OPEN_PIN, OUTPUT);
  pinMode(OUTPUT_AWAY_PIN, OUTPUT);
  pinMode(INPUT_CLOSE_PIN, INPUT_PULLUP);
  pinMode(INPUT_CLOSE_PIN, INPUT_PULLUP);
  pinMode(INPUT_CLOSE_PIN, INPUT_PULLUP);
}

void loop() {

  delay(100);
}

void sendState(const char* state) {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", String("Bearer ") + TOKEN);
  int code = http.POST(String("{\"state\":\"") + state + "\"}");
  Serial.printf("Sent '%s' → HTTP %d\n", state, code);
  http.end();
} 