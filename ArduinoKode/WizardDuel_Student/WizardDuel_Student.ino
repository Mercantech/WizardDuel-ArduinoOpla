/*
 * ========================================
 * WIZARD DUEL - STUDENT STARTER KIT
 * ========================================
 *
 * NYT: Fælles kit til Bomberman + Wizard i Games-repoet:
 *   https://github.com/Mercantech/Games/tree/main/arduino/MercantecGamesController
 *   Sæt GAME_MODE_WIZARD i config.h — denne fil er legacy/reference.
 *
 * Fælles Arduino-kontrakt (samme som Bomberman):
 *   POST {GAME_BASE_PATH}/api/controller/join
 *   POST {GAME_BASE_PATH}/api/controller/heartbeat
 *   POST {GAME_BASE_PATH}/api/controller/action
 *
 * Join:
 *   { "deviceId": "...", "name": "Elev" }
 *   → { "ok": true, "playerId": "..." }
 *
 * Heartbeat (hver 2-5 sek!):
 *   { "deviceId": "...", "playerId": "..." }
 *
 * Cast spell (action):
 *   {
 *     "deviceId": "...", "playerId": "...",
 *     "action": "cast",
 *     "params": { "spellKey": "FIREBALL", "targetId": 1 }
 *   }
 *
 * SPELLS: FIREBALL, LIGHTNING, SHIELD, HEAL, POWER_BOOST, DEATH_RAY
 *
 * Ability-profil (forslag):
 *   TOUCH0=FIREBALL, TOUCH1=LIGHTNING, TOUCH2=SHIELD,
 *   TOUCH3=HEAL, TOUCH4=DEATH_RAY  (eller POWER_BOOST)
 */

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;

// =============================================
// KONFIGURATION - UDFYLD DETTE!
// =============================================
#define WIFI_SSID       "WIFI_NAVN_HER"
#define WIFI_PASS       "WIFI_PASSWORD_HER"
#define SERVER_HOST     "games.mercantec.tech"
#define GAME_BASE_PATH  "/Wizard"     // Skift til "/Bomberman" for Bomberman
#define GAME_PIN        ""            // Wizard bruger kø – PIN valgfri
#define PLAYER_NAME     "Wizard"

// 1 = HTTPS (port 443). 0 = HTTP (port 80) – brug 0 kun lokalt.
#define USE_HTTPS       1

#if USE_HTTPS
  #define SERVER_PORT 443
#else
  #define SERVER_PORT 80
#endif
// =============================================

String deviceId;
String playerId;

#if USE_HTTPS
  WiFiSSLClient wifi;
#else
  WiFiClient wifi;
#endif
HttpClient http = HttpClient(wifi, SERVER_HOST, SERVER_PORT);

const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 240;
const unsigned long HEARTBEAT_MS = 3000;
unsigned long lastHeartbeat = 0;

String apiPath(const char* endpoint) {
  return String(GAME_BASE_PATH) + endpoint;
}

bool httpPost(const String& path, const String& jsonBody, String& responseBody) {
  Serial.print("POST ");
  Serial.println(path);

  http.beginRequest();
  http.post(path);
  http.sendHeader("Content-Type", "application/json");
  http.sendHeader("Content-Length", jsonBody.length());
  http.beginBody();
  http.print(jsonBody);
  http.endRequest();

  int status = http.responseStatusCode();
  responseBody = http.responseBody();

  int jsonStart = responseBody.indexOf('{');
  if (jsonStart >= 0) responseBody = responseBody.substring(jsonStart);

  Serial.println(status == 200 ? "OK" : "FAILED");
  return status == 200;
}

void drawCentered(const char* text, int y, int size, uint16_t color) {
  carrier.display.setTextSize(size);
  carrier.display.setTextColor(color);
  int charWidth = 6 * size;
  int textWidth = strlen(text) * charWidth;
  int x = (SCREEN_WIDTH - textWidth) / 2;
  carrier.display.setCursor(x, y);
  carrier.display.print(text);
}

void drawCentered(String text, int y, int size, uint16_t color) {
  drawCentered(text.c_str(), y, size, color);
}

void clearScreen(uint16_t color) {
  carrier.display.fillScreen(color);
}

void drawProgressBar(int x, int y, int width, int height,
                     int value, int maxValue, uint16_t fillColor) {
  carrier.display.fillRect(x, y, width, height, ST77XX_BLACK);
  carrier.display.drawRect(x, y, width, height, ST77XX_WHITE);
  int fillWidth = maxValue > 0 ? (width - 2) * value / maxValue : 0;
  carrier.display.fillRect(x + 1, y + 1, fillWidth, height - 2, fillColor);
}

bool doJoin() {
  String path = apiPath("/api/controller/join");
  String body = "{\"deviceId\":\"" + deviceId +
                "\",\"name\":\"" + String(PLAYER_NAME) + "\"";
  if (String(GAME_PIN).length() > 0) {
    body += ",\"pin\":\"" + String(GAME_PIN) + "\"";
  }
  body += "}";

  String resp;
  if (!httpPost(path, body, resp)) return false;

  JSONVar doc = JSON.parse(resp);
  if (JSON.typeof(doc) == "undefined") return false;

  if (doc.hasOwnProperty("playerId")) {
    playerId = (const char*)doc["playerId"];
  } else {
    playerId = deviceId;
  }
  return true;
}

bool doHeartbeat(String& resp) {
  String path = apiPath("/api/controller/heartbeat");
  String body = "{\"deviceId\":\"" + deviceId +
                "\",\"playerId\":\"" + playerId + "\"}";
  return httpPost(path, body, resp);
}

bool castSpell(const char* spellKey, int targetId = -1) {
  String path = apiPath("/api/controller/action");
  String body = "{\"deviceId\":\"" + deviceId +
                "\",\"playerId\":\"" + playerId +
                "\",\"action\":\"cast\",\"params\":{\"spellKey\":\"" +
                String(spellKey) + "\"";
  if (targetId >= 0) {
    body += ",\"targetId\":" + String(targetId);
  }
  body += "}}";

  String resp;
  return httpPost(path, body, resp);
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("================");
  Serial.println("WIZARD DUEL");
  Serial.println("================");

  carrier.noCase();
  carrier.begin();
  carrier.display.setRotation(0);
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextWrap(false);

  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  snprintf(buf, sizeof(buf), "OPLA_%02X%02X%02X", mac[3], mac[4], mac[5]);
  deviceId = String(buf);

  Serial.print("Device ID: ");
  Serial.println(deviceId);

  clearScreen(ST77XX_BLACK);
  drawCentered("Connecting", 80, 2, ST77XX_WHITE);
  drawCentered("to WiFi...", 110, 2, ST77XX_WHITE);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  clearScreen(ST77XX_GREEN);
  drawCentered("WiFi OK!", 100, 2, ST77XX_BLACK);
  delay(500);

  clearScreen(ST77XX_BLACK);
  drawCentered("Joining...", 100, 2, ST77XX_YELLOW);
  if (!doJoin()) {
    clearScreen(ST77XX_RED);
    drawCentered("Join fejl!", 100, 2, ST77XX_WHITE);
    return;
  }

  clearScreen(ST77XX_BLACK);
  drawCentered("WIZARD DUEL", 60, 2, ST77XX_YELLOW);
  drawCentered("In queue!", 120, 2, ST77XX_GREEN);
  drawCentered("Cast with buttons", 180, 1, ST77XX_WHITE);

  Serial.println("=== READY! ===");
  Serial.println(String("Server: ") + SERVER_HOST + GAME_BASE_PATH);
  lastHeartbeat = millis();
}

void loop() {
  if (playerId.length() == 0) {
    delay(1000);
    return;
  }

  unsigned long now = millis();
  if (now - lastHeartbeat > HEARTBEAT_MS) {
    String resp;
    doHeartbeat(resp);
    lastHeartbeat = now;

    // TODO (elev): parse hp/mana fra heartbeat og tegn progress bars
  }

  carrier.Buttons.update();

  // TODO (elev): map knapper til spells – eksempel:
  // if (carrier.Buttons.onTouchDown(TOUCH0)) castSpell("FIREBALL", 1);
  // if (carrier.Buttons.onTouchDown(TOUCH1)) castSpell("LIGHTNING");
  // if (carrier.Buttons.onTouchDown(TOUCH2)) castSpell("SHIELD");
  // if (carrier.Buttons.onTouchDown(TOUCH3)) castSpell("HEAL");
  // if (carrier.Buttons.onTouchDown(TOUCH4)) castSpell("DEATH_RAY", 1);

  if (carrier.Buttons.onTouchDown(TOUCH0)) {
    Serial.println("TOUCH0 - cast FIREBALL her");
  }
  if (carrier.Buttons.onTouchDown(TOUCH1)) {
    Serial.println("TOUCH1 - cast LIGHTNING her");
  }
  if (carrier.Buttons.onTouchDown(TOUCH2)) {
    Serial.println("TOUCH2 - cast SHIELD her");
  }
  if (carrier.Buttons.onTouchDown(TOUCH3)) {
    Serial.println("TOUCH3 - cast HEAL her");
  }
  if (carrier.Buttons.onTouchDown(TOUCH4)) {
    Serial.println("TOUCH4 - cast DEATH_RAY her");
  }

  delay(50);
}
