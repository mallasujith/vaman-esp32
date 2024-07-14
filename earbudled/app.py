import bluetooth
import speech_recognition as sr

def find_esp32():
    target_name = "ESP32_BT_LED_Control"  # Change this to your ESP32 Bluetooth name
    target_address = None

    nearby_devices = bluetooth.discover_devices(duration=8, lookup_names=True, flush_cache=True, lookup_class=False)
    for addr, name in nearby_devices:
        if target_name == name:
            target_address = addr
            break

    if target_address is not None:
        print(f"Found target bluetooth device with address {target_address}")
        return target_address
    else:
        print("Could not find target bluetooth device nearby")
        return None

def send_command(bt_socket, command):
    bt_socket.send(command.encode())

if __name__ == "__main__":
    esp32_address = find_esp32()
    if esp32_address is not None:
        port = 1  # ESP32 usually uses port 1 for Bluetooth serial

        # Create the client socket
        bt_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        bt_socket.connect((esp32_address, port))

        print("Connected to ESP32")

        # Initialize speech recognition
        recognizer = sr.Recognizer()

        try:
            while True:
                with sr.Microphone() as source:
                    print("Listening...")
                    audio = recognizer.listen(source)

                try:
                    command = recognizer.recognize_google(audio).lower()
                    print(f"Recognized command: {command}")

                    if 'luffy' in command:
                        send_command(bt_socket, 'luffy')  # Send 'luffy' to turn LED on
                    elif 'zoro' in command:
                        send_command(bt_socket, 'zoro')  # Send 'zoro' to turn LED off
                    else:
                        print("Unrecognized command. Try saying 'luffy' or 'zoro'.")

                except sr.UnknownValueError:
                    print("Could not understand audio")
                except sr.RequestError as e:
                    print(f"Could not request results; {e}")

        except KeyboardInterrupt:
            print("Disconnected")
        finally:
            bt_socket.close()
    else:
        print("Could not connect to ESP32")

