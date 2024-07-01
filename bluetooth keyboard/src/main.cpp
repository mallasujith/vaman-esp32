#include <BluetoothSerial.h>

// Create a BluetoothSerial object
BluetoothSerial SerialBT;

const int ledPin = 2; // Change this to your actual LED pin

void setup() {
  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Ensure LED is initially off

  // Begin Bluetooth serial at default baud rate
  SerialBT.begin("ESP32_BT_LED_Control"); // Change this name as you like
  Serial.begin(115200); // For debugging purposes

  Serial.println("Bluetooth Device is ready to pair");
}

void loop() {
  // Check if data is available to read from Bluetooth serial
  if (SerialBT.available()) {
    // Read the incoming byte
    char command = SerialBT.read();

    // Process the command
    if (command == '1') {
      Serial.println("Turning LED ON");
      digitalWrite(ledPin, HIGH); // Turn LED ON
    } else if (command == '2') {
      Serial.println("Turning LED OFF");
      digitalWrite(ledPin, LOW); // Turn LED OFF
    } else {
      Serial.println("Invalid command received");
    }
  }

  // For debugging purposes
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  // Add any other code you need to run continuously here
}

