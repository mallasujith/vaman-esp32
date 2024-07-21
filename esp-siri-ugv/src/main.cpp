#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "123456";  // Replace with your network SSID
const char* password = "12345678";  // Replace with your network password

AsyncWebServer server(80);

// Pin Definitions
const int ledPin = 2;      // Change this to your actual LED pin
const int motor1PWM = 14;  // Motor 1 PWM pin
const int motor1IN1 = 16;  // Motor 1 IN1 pin
const int motor1IN2 = 17;  // Motor 1 IN2 pin
const int motor2PWM = 15;  // Motor 2 PWM pin
const int motor2IN1 = 18;  // Motor 2 IN1 pin
const int motor2IN2 = 19;  // Motor 2 IN2 pin

void stopMotors();
void moveMotorsForward();
void moveMotorsBackward();
void turnMotorsLeft();
void turnMotorsRight();

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // Ensure LED is initially off

    // Initialize motor pins
    pinMode(motor1PWM, OUTPUT);
    pinMode(motor1IN1, OUTPUT);
    pinMode(motor1IN2, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
    pinMode(motor2IN1, OUTPUT);
    pinMode(motor2IN2, OUTPUT);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Define web server routes
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Welcome to ESP32!");
    });

    server.on("/command", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("cmd")) {
            String command = request->getParam("cmd")->value();
            Serial.print("Command received: ");
            Serial.println(command);
            if (command == "ON") {
                digitalWrite(ledPin, LOW); // Turn LED ON
                request->send(200, "text/plain", "LED is ON");
                Serial.println("LED is ON");
            } else if (command == "OFF") {
                digitalWrite(ledPin, HIGH); // Turn LED OFF
                request->send(200, "text/plain", "LED is OFF");
                Serial.println("LED is OFF");
            } else if (command == "forward") {
                Serial.println("Received command: forward (Move motors forward)");
                moveMotorsForward();
                request->send(200, "text/plain", "Moving forward");
            } else if (command == "backward") {
                Serial.println("Received command: backward (Move motors backward)");
                moveMotorsBackward();
                request->send(200, "text/plain", "Moving backward");
            } else if (command == "left") {
                Serial.println("Received command: left (Turn motors left)");
                turnMotorsLeft();
                request->send(200, "text/plain", "Turning left");
            } else if (command == "right") {
                Serial.println("Received command: right (Turn motors right)");
                turnMotorsRight();
                request->send(200, "text/plain", "Turning right");
            } else {
                request->send(200, "text/plain", "Unknown command");
                Serial.println("Unknown command");
            }
        } else {
            request->send(200, "text/plain", "No command received");
            Serial.println("No command received");
        }
    });

    // Start web server
    server.begin();
}

void loop() {
    // Nothing to do here, all actions are handled by the web server
}

void stopMotors() {
    // Stop motors
    analogWrite(motor1PWM, 0);
    digitalWrite(motor1IN1, LOW);
    digitalWrite(motor1IN2, LOW);
    analogWrite(motor2PWM, 0);
    digitalWrite(motor2IN1, LOW);
    digitalWrite(motor2IN2, LOW);
}

void moveMotorsForward() {
    // Move motors forward
    analogWrite(motor1PWM, 255); // Adjust speed as needed
    digitalWrite(motor1IN1, HIGH);
    digitalWrite(motor1IN2, LOW);
    analogWrite(motor2PWM, 255); // Adjust speed as needed
    digitalWrite(motor2IN1, HIGH);
    digitalWrite(motor2IN2, LOW);

    delay(3000); // Run for 3 seconds
    stopMotors();
}

void moveMotorsBackward() {
    // Move motors backward
    analogWrite(motor1PWM, 255); // Adjust speed as needed (reverse)
    digitalWrite(motor1IN1, LOW);
    digitalWrite(motor1IN2, HIGH);
    analogWrite(motor2PWM, 255); // Adjust speed as needed (reverse)
    digitalWrite(motor2IN1, LOW);
    digitalWrite(motor2IN2, HIGH);

    delay(3000); // Run for 3 seconds
    stopMotors();
}

void turnMotorsLeft() {
    // Turn motors left
    analogWrite(motor1PWM, 255); // Adjust speed as needed (turn left)
    digitalWrite(motor1IN1, LOW);
    digitalWrite(motor1IN2, HIGH);
    analogWrite(motor2PWM, 255); // Adjust speed as needed (turn left)
    digitalWrite(motor2IN1, HIGH);
    digitalWrite(motor2IN2, LOW);

    delay(3000); // Run for 3 seconds
    stopMotors();
}

void turnMotorsRight() {
    // Turn motors right
    analogWrite(motor1PWM, 255); // Adjust speed as needed (turn right)
    digitalWrite(motor1IN1, HIGH);
    digitalWrite(motor1IN2, LOW);
    analogWrite(motor2PWM, 255); // Adjust speed as needed (turn right)
    digitalWrite(motor2IN1, LOW);
    digitalWrite(motor2IN2, HIGH);

    delay(3000); // Run for 3 seconds
    stopMotors();
}

