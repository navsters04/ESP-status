#include <WiFi.h>        
#include <HTTPClient.h>  

const char* SSID = "RumahTalenta_BCA";
const char* PASS = "rtbcaokee";
const char* URL  = "https://esp-status.vercel.app/api/status";
const char* TOKEN = "kX9f2mQp7Lw3Rz8VbT4nJc6Ys1";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("Connected");
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