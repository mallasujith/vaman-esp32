#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

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
    // Initialize LED pin as output
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // Ensure LED is initially off

    // Initialize motor pins
    pinMode(motor1PWM, OUTPUT);
    pinMode(motor1IN1, OUTPUT);
    pinMode(motor1IN2, OUTPUT);
    pinMode(motor2PWM, OUTPUT);
    pinMode(motor2IN1, OUTPUT);
    pinMode(motor2IN2, OUTPUT);

    // Begin Bluetooth serial at default baud rate
    SerialBT.begin("ESP32_BT_Control"); // Change the name as needed
    Serial.begin(115200); // For debugging purposes

    Serial.println("Bluetooth Device is ready to pair");
}

void loop() {
    // Check if data is available to read from Bluetooth serial
    if (SerialBT.available()) {
        // Read the incoming command
        String command = SerialBT.readStringUntil('\n');

        // Process the command
        if (command.equals("bujji")) {
            Serial.println("Received command: (Turn LED OFF)");
            digitalWrite(ledPin, HIGH); // Turn LED OFF

            // Stop motors (if moving)
            stopMotors();
        } else if (command.equals("stop")) {
            Serial.println("Received command:(Turn LED ON)");
            digitalWrite(ledPin, LOW); // Turn LED ON

            // Stop motors (if moving)
            stopMotors();
        } else if (command.equals("forward")) {
            Serial.println("Received command: forward (Move motors forward)");
            moveMotorsForward();
        } else if (command.equals("backward")) {
            Serial.println("Received command: backward (Move motors backward)");
            moveMotorsBackward();
        } else if (command.equals("left")) {
            Serial.println("Received command: left (Turn motors left)");
            turnMotorsLeft();
        } else if (command.equals("right")) {
            Serial.println("Received command: right (Turn motors right)");
            turnMotorsRight();
        } else {
            Serial.println("Invalid command received");
        }
    }

    // Add any other code you need to run continuously here
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

