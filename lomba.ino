
#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include <time.h>

// ==========================================
// 0. CONFIGURATION - ISI DI KOMPUTER SENDIRI
// ==========================================

#define API_KEY "AIzaSyDOr7Ra3-hAniYo1u19kmIH4i64I2NpDJQ"
#define USER_EMAIL "aress.mit.liebe@gmail.com"
#define USER_PASSWORD "aress12345##"
#define DATABASE_URL "https://sppg-loop-default-rtdb.firebaseio.com/"

const char* ssid = "ROBBEN";
const char* wifiPassword = "12345678";

// ==========================================
// 1. PIN CONFIGURATION
// ==========================================

#define LCD_SDA 21
#define LCD_SCL 22

#define RFID_SS 5
#define RFID_RST 27
#define RFID_SCK 18
#define RFID_MISO 19
#define RFID_MOSI 23

#define HX711_DT 32
#define HX711_SCK 33

// ==========================================
// 2. HARDWARE & FIREBASE OBJECTS
// ==========================================

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(RFID_SS, RFID_RST);
HX711 scale;
WebServer server(80);

WiFiClientSecure sslClientAuth;
WiFiClientSecure sslClientDB;

using AsyncClient = AsyncClientClass;
AsyncClient authClient(sslClientAuth);
AsyncClient dbClient(sslClientDB);

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD, 3000);
FirebaseApp app;
RealtimeDatabase Database;

// ==========================================
// 3. GLOBAL VARIABLES
// ==========================================

String adminUID = "4B85AB72";
bool adminAuthorized = false;
unsigned long accessExpiry = 0;
const unsigned long ACCESS_DURATION = 300000UL;

float calibrationFactor = 1.0;
float currentWeight = 0;

unsigned long lastWeightRead = 0;
unsigned long lastLCDUpdate = 0;

bool firebaseStarted = false;

// ==========================================
// 4. LCD
// ==========================================

void showLCD(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1.substring(0, 16));
  lcd.setCursor(0, 1);
  lcd.print(line2.substring(0, 16));
}

// ==========================================
// 5. RFID
// ==========================================

String readRFID() {
  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();
  return uid;
}

void checkRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String scannedUID = readRFID();

  Serial.println("======================");
  Serial.println("RFID CARD DETECTED");
  Serial.print("UID: ");
  Serial.println(scannedUID);

  if (scannedUID == adminUID) {
    adminAuthorized = true;
    accessExpiry = millis() + ACCESS_DURATION;
    Serial.println("ACCESS GRANTED");
    showLCD("ADMIN VERIFIED", "Access Granted");
  } else {
    Serial.println("ACCESS DENIED");
    showLCD("UNKNOWN CARD", "Access Denied");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(700);
}

void checkAccess() {
  if (adminAuthorized && (long)(millis() - accessExpiry) >= 0) {
    adminAuthorized = false;
    Serial.println("ADMIN SESSION EXPIRED");
    showLCD("SESSION EXPIRED", "Scan RFID Card");
  }
}

bool checkAdmin() {
  if (!adminAuthorized) return false;

  if ((long)(millis() - accessExpiry) >= 0) {
    adminAuthorized = false;
    return false;
  }

  return true;
}

// ==========================================
// 6. LOAD CELL
// ==========================================

void readWeight() {
  if (scale.is_ready()) {
    float weight = scale.get_units(5);

    if (weight < 0 && weight > -3) {
      weight = 0;
    }

    currentWeight = weight;
  } else {
    Serial.println("HX711 NOT READY");
  }
}

// ==========================================
// 7. FIREBASE
// ==========================================

void auth_debug_print(AsyncResult &aResult) {
  if (aResult.isEvent()) {
    Serial.printf("Auth event: %s\n",
                  aResult.eventLog().message().c_str());
  }

  if (aResult.isDebug()) {
    Serial.printf("Auth debug: %s\n",
                  aResult.debug().c_str());
  }

  if (aResult.isError()) {
    Serial.printf("Auth error: %s\n",
                  aResult.error().message().c_str());
  }
}

void setupFirebase() {
  // Pengaturan ini melewati pemeriksaan sertifikat TLS.
  // Cocok hanya untuk prototipe/pengujian, bukan produksi.
  sslClientAuth.setInsecure();
  sslClientDB.setInsecure();

  sslClientAuth.setConnectionTimeout(1000);
  sslClientAuth.setHandshakeTimeout(5);
  sslClientDB.setConnectionTimeout(1000);
  sslClientDB.setHandshakeTimeout(5);

  Serial.println("Starting Firebase authentication...");

  initializeApp(
    authClient,
    app,
    getAuth(user_auth),
    auth_debug_print,
    "authTask"
  );

  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);

  // Waktu Indonesia Barat (UTC+7)
  configTime(7 * 3600, 0, "pool.ntp.org", "time.google.com");

  firebaseStarted = true;
}

// Membuat tanggal dan waktu lokal dari NTP
void getDateTime(String &dateText, String &timeText) {
  struct tm timeInfo;

  if (getLocalTime(&timeInfo, 1000)) {
    char dateBuffer[16];
    char timeBuffer[16];

    strftime(dateBuffer, sizeof(dateBuffer), "%d-%m-%Y", &timeInfo);
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeInfo);

    dateText = String(dateBuffer);
    timeText = String(timeBuffer);
  } else {
    dateText = "Belum sinkron";
    timeText = "--:--:--";
  }
}

// Menyimpan satu sesi lengkap: edible + inedible
bool saveHistoryToFirebase(float edible, float inedible) {
  if (!app.ready()) {
    Serial.println("Firebase belum siap / belum login.");
    return false;
  }

  String dateText, timeText;
  getDateTime(dateText, timeText);

  float total = edible + inedible;

  String json = "{";
  json += "\"edible\":" + String(edible, 2) + ",";
  json += "\"inedible\":" + String(inedible, 2) + ",";
  json += "\"total\":" + String(total, 2) + ",";
  json += "\"date\":\"" + dateText + "\",";
  json += "\"time\":\"" + timeText + "\"";
  json += "}";

  Serial.println("Sending record to Firebase:");
  Serial.println(json);

  String pushName = Database.push<object_t>(
    dbClient,
    "/history",
    object_t(json)
  );

  if (dbClient.lastError().code() == 0 && pushName.length() > 0) {
    Serial.print("Firebase save success. Key: ");
    Serial.println(pushName);
    return true;
  }

  Serial.print("Firebase save failed. Code: ");
  Serial.println(dbClient.lastError().code());
  Serial.print("Message: ");
  Serial.println(dbClient.lastError().message());

  return false;
}

// Membaca seluruh riwayat dari Firebase
String readHistoryFromFirebase() {
  if (!app.ready()) {
    return "";
  }

  String result = Database.get<String>(dbClient, "/history");

  if (dbClient.lastError().code() != 0) {
    Serial.print("Firebase read failed: ");
    Serial.println(dbClient.lastError().message());
    return "";
  }

  if (result.length() == 0 || result == "null") {
    return "{}";
  }

  return result;
}

// ==========================================
// 8. WEB DASHBOARD
// ==========================================

#include "webpage.h"

// ==========================================
// 9. WEB SERVER ROUTES
// ==========================================

void setupWebServer() {
  server.on("/", HTTP_GET, []() {
    if (!checkAdmin()) {
      server.send(401, "text/plain",
                  "ACCESS DENIED. PLEASE SCAN ADMIN RFID CARD.");
      return;
    }

    server.send_P(200, "text/html", webpage);
  });

  server.on("/data", HTTP_GET, []() {
    if (!checkAdmin()) {
      server.send(401, "application/json", "{\"error\":\"Unauthorized\"}");
      return;
    }

    String json = "{\"weight\":" + String(currentWeight, 2) + "}";
    server.send(200, "application/json", json);
  });

  // Menyimpan satu sesi lengkap ke Firebase
  server.on("/save", HTTP_POST, []() {
    if (!checkAdmin()) {
      server.send(401, "application/json",
                  "{\"success\":false,\"message\":\"Unauthorized\"}");
      return;
    }

    if (!app.ready()) {
      server.send(503, "application/json",
                  "{\"success\":false,\"message\":\"Firebase belum siap\"}");
      return;
    }

    String edibleText = server.arg("edible");
    String inedibleText = server.arg("inedible");

    if (edibleText.length() == 0 || inedibleText.length() == 0) {
      server.send(400, "application/json",
                  "{\"success\":false,\"message\":\"Data tidak lengkap\"}");
      return;
    }

    float edible = edibleText.toFloat();
    float inedible = inedibleText.toFloat();

    if (edible < 0 || inedible < 0) {
      server.send(400, "application/json",
                  "{\"success\":false,\"message\":\"Berat tidak valid\"}");
      return;
    }

    bool success = saveHistoryToFirebase(edible, inedible);

    if (success) {
      server.send(200, "application/json", "{\"success\":true}");
    } else {
      server.send(500, "application/json",
                  "{\"success\":false,\"message\":\"Firebase write gagal\"}");
    }
  });

  // Mengambil riwayat dari Firebase
  server.on("/history", HTTP_GET, []() {
    if (!checkAdmin()) {
      server.send(401, "application/json", "{\"error\":\"Unauthorized\"}");
      return;
    }

    if (!app.ready()) {
      server.send(503, "application/json", "{\"error\":\"Firebase belum siap\"}");
      return;
    }

    String history = readHistoryFromFirebase();

    if (history.length() == 0) {
      server.send(500, "application/json",
                  "{\"error\":\"Gagal membaca Firebase\"}");
      return;
    }

    server.send(200, "application/json", history);
  });

  server.on("/logout", HTTP_GET, []() {
    adminAuthorized = false;
    server.send(200, "text/plain", "LOGGED OUT");
    Serial.println("ADMIN LOGOUT");
  });

  server.begin();
  Serial.println("WEB SERVER STARTED");
}

// ==========================================
// 10. SETUP
// ==========================================

void setup() {
  Serial.begin(115200);

  // LCD
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.init();
  lcd.backlight();
  showLCD("SPPG LOOP", "Initializing...");

  // HX711
  scale.begin(HX711_DT, HX711_SCK);

  if (!scale.wait_ready_timeout(5000)) {
    Serial.println("HX711 ERROR");
    showLCD("HX711 NOT FOUND", "Check Wiring!");
    while (true) delay(100);
  }

  scale.set_scale(calibrationFactor);
  scale.tare(20);
  Serial.println("LOAD CELL READY");

  // RFID
  SPI.begin(RFID_SCK, RFID_MISO, RFID_MOSI, RFID_SS);
  rfid.PCD_Init();
  Serial.println("RFID READY");

  // Wi-Fi
  WiFi.begin(ssid, wifiPassword);
  showLCD("Connecting WiFi", "Please Wait...");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WIFI CONNECTED SUCCESSFULLY!");
    Serial.print("IP ADDRESS: http://");
    Serial.println(WiFi.localIP());

    setupFirebase();
    setupWebServer();

    showLCD(WiFi.localIP().toString(), "Scan Admin RFID");
  } else {
    Serial.println();
    Serial.println("WIFI CONNECTION FAILED");
    showLCD("WiFi Failed", "Check Settings");
  }
}

// ==========================================
// 11. MAIN LOOP
// ==========================================

void loop() {
  // Menjaga proses autentikasi dan token Firebase
  if (firebaseStarted) {
    app.loop();
  }

  checkRFID();
  checkAccess();

  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  if (millis() - lastWeightRead >= 250) {
    lastWeightRead = millis();
    readWeight();

    Serial.print("WEIGHT: ");
    Serial.print(currentWeight, 2);
    Serial.println(" gram");
  }

  if (millis() - lastLCDUpdate >= 500) {
    lastLCDUpdate = millis();

    if (adminAuthorized) {
      showLCD("Wt: " + String(currentWeight, 1) + " g",
              "ADMIN: ACTIVE");
    } else if (WiFi.status() == WL_CONNECTED) {
      showLCD(WiFi.localIP().toString(),
              "Scan Admin RFID");
    } else {
      showLCD("WiFi Offline", "Check Settings");
    }
  }
}