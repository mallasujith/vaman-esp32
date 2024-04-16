#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal.h>

const char* ssid = "123456"; // Change to your WiFi SSID
const char* password = "12345678"; // Change to your WiFi password

AsyncWebServer server(80);
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>LCD Display</title>
</head>
<body>
  <h1>LCD Display</h1>
  <form action="/submit" method="post" name="LCD">
    <label for="input">Enter text:</label><br>
    <input type="text" id="input" name="input"><br>
    <input type="submit" value="Submit">
  </form>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Enter text:");
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Define web server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "POST request received");
    if (request->hasParam("input", true)) {
      AsyncWebParameter* p = request->getParam("input", true);
      String input = p->value();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Input:");
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
  });

  // Start web server
  server.begin();
}

void loop() {
  // Nothing to do here, all actions are handled by the web server
}

