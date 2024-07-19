import bluetooth
import speech_recognition as sr

def find_esp32():
    target_name = "ESP32_BT_Control"  # Adjust to your ESP32 Bluetooth name
    target_address = None

    nearby_devices = bluetooth.discover_devices(duration=8, lookup_names=True, flush_cache=True, lookup_class=False)
    for addr, name in nearby_devices:
        if target_name == name:
            target_address = addr
            break

    if target_address is not None:
        print(f"Found target Bluetooth device with address {target_address}")
        return target_address
    else:
        print("Could not find target Bluetooth device nearby")
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

                    if 'forward' in command:
                        send_command(bt_socket, 'forward')  # Send 'forward' to move forward
                    elif 'backward' in command:
                        send_command(bt_socket, 'backward')  # Send 'backward' to move backward
                    elif 'left' in command:
                        send_command(bt_socket, 'left')  # Send 'left' to turn left
                    elif 'right' in command:
                        send_command(bt_socket, 'right')  # Send 'right' to turn right
                    elif 'bujji' in command:
                        send_command(bt_socket, 'bujji')  # Send 'bujji' to turn LED off
                    elif 'stop' in command:
                        send_command(bt_socket, 'stop')  # Send 'stop' to turn LED on
                    else:
                        print("Unrecognized command. Try saying 'forward', 'backward', 'left', 'right', 'bujji', or 'stop'.")

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

