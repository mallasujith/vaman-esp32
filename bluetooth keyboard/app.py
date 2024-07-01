//pip3 install pybluez command before running this code
import bluetooth

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
    bt_socket.send(command)

if __name__ == "__main__":
    esp32_address = find_esp32()
    if esp32_address is not None:
        port = 1  # ESP32 usually uses port 1 for Bluetooth serial

        # Create the client socket
        bt_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        bt_socket.connect((esp32_address, port))

        print("Connected to ESP32")
        try:
            while True:
                command = input("Enter command (1 to turn on, 2 to turn off): ")
                if command in ['1', '2']:
                    send_command(bt_socket, command)
                else:
                    print("Invalid command. Please enter 1 or 2.")
        except KeyboardInterrupt:
            print("Disconnected")
        finally:
            bt_socket.close()
    else:
        print("Could not connect to ESP32")

