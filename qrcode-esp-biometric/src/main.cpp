#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

const char* ssid = "TP-Link_Guest_EFDC";
const char* password = "Fwciith@2022";

AsyncWebServer server(80);

// Stores the list of client IPs
String clientIPs = "";

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Route for /sujith
  server.on("/sujith", HTTP_GET, [](AsyncWebServerRequest *request){
    IPAddress clientIP = request->client()->remoteIP();
    String clientIPStr = clientIP.toString();

    // Add the client's IP to the list
    clientIPs += clientIPStr + "<br>";

    // Print client IP to Serial Monitor
    Serial.print("Client connected: ");
    Serial.println(clientIPStr);

    // Send the list of client IPs as response
    String response = "Hello! Here are the connected client IPs:<br><br>" + clientIPs;
    request->send(200, "text/html", response);
  });

  server.begin();
}

void loop() {
  // No need for code in loop as we're using an asynchronous server
}

