#include <WiFi.h>        
#include <HTTPClient.h>  

const char* SSID = "name";
const char* PASS = "pass";
const char* URL  = "https://web/api/status";
const char* TOKEN = "key";

int  OUTPUT_CLOSE_PIN = 33;
int  OUTPUT_OPEN_PIN = 25;
int  OUTPUT_AWAY_PIN = 26;
int  INPUT_CLOSE_PIN = 12;
int  INPUT_OPEN_PIN = 14;
int  INPUT_AWAY_PIN = 27;

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("Connected");

  pinMode(OUTPUT_CLOSE_PIN, OUTPUT);
  pinMode(OUTPUT_OPEN_PIN, OUTPUT);
  pinMode(OUTPUT_AWAY_PIN, OUTPUT);
  pinMode(INPUT_CLOSE_PIN, INPUT_PULLUP);
  pinMode(INPUT_OPEN_PIN, INPUT_PULLUP);
  pinMode(INPUT_AWAY_PIN, INPUT_PULLUP);

  // input is mirrored because using pullup, 0=1 & 1=0
  digitalWrite(OUTPUT_OPEN_PIN,HIGH);
}

String lastState = "open";
void loop() {
  Serial.print("close : ");
  Serial.println(digitalRead(INPUT_CLOSE_PIN));
  Serial.print("open : ");
  Serial.println(digitalRead(INPUT_OPEN_PIN));
  Serial.print("away : ");
  Serial.println(digitalRead(INPUT_AWAY_PIN));

  String current = readDoorState();

  if (current != lastState) {
    lastState = current;
    changeLight(current);
    sendState(current);
  }
  delay(100);
}

void changeLight(String state){
  digitalWrite(OUTPUT_CLOSE_PIN,LOW);
  digitalWrite(OUTPUT_OPEN_PIN,LOW);
  digitalWrite(OUTPUT_AWAY_PIN,LOW);

  if(state == "close"){
    digitalWrite(OUTPUT_CLOSE_PIN,HIGH);
  }else if(state == "open"){
    digitalWrite(OUTPUT_OPEN_PIN,HIGH);
  }else if(state == "away"){
    digitalWrite(OUTPUT_AWAY_PIN,HIGH);
  }
}

String readDoorState(){
  if(digitalRead(INPUT_CLOSE_PIN) == 0){
    return "close";
  }else if(digitalRead(INPUT_AWAY_PIN) == 0){
    return "away";
  }else  if(digitalRead(INPUT_OPEN_PIN) == 0 || lastState == "close"){
    return "open";
  }
  return lastState;
}

void sendState(String state) {
  if (WiFi.status() != WL_CONNECTED) {
    static unsigned long lastTry = 0;
    if (millis() - lastTry > 10000) {
      lastTry = millis();
      Serial.println("WiFi lost — reconnecting...");
      WiFi.disconnect();
      WiFi.begin(SSID, PASS);
    }
  }

  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", String("Bearer ") + TOKEN);
  int code = http.POST(String("{\"state\":\"") + state + "\"}");
  Serial.printf("Sent '%s' → HTTP %d\n", state, code);
  http.end();
}