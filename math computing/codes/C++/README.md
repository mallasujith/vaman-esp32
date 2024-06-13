This directory contains **(.cpp)** files which are an extension of code designed to support Vaman Esp32 Board.
<h2>Steps to Follow:</h2>
<h3>Setting up Directory in Ubuntu</h3>
<ol>
  <li>Creating Directory</li>
  <pre>mkdir geohard</pre>
  <li>Changing Directory</li>
  <pre>cd geohard</pre>
  <li>Creating src file</li>
  <pre>mkdir src</pre>
  <li>Changing to src directory</li>
  <pre>cd src</pre>
  <li>Download required <b>.cpp</b> file, along with <b>libs</b> folder and paste it in here.</li>
  <li>Modify <b>.cpp</b> file</li>
  Give your respective ssid, password. Save and exit.
  <pre>
const char* ssid = "Redmi";
const char* password = "987654321";</pre>
  <li>Returning Back to geohard</li>
  <pre>cd ..</pre>
  <li>Download the <b>platformio.ini</b> file and paste it in here</li>
</ol>
<h3>Generating .bin file</h3>
<ol>
  <li>Open Terminal, Goto geohard Directory</li>
  <li>Installing AsyncTCP</li>
  <pre>pio pkg install --library "me-no-dev/AsyncTCP@^1.1.1"</pre>
  <li>Installing ESP Async WebServer</li>
  <pre>pio pkg install --library "me-no-dev/ESP Async WebServer@^1.2.3"</pre>
  <li>Building bin file</li>
  <pre>pio run</pre>
</ol>
<h3>Flash vaman-ESP32 using Arduino</h3>
Click the <a href="https://github.com/gadepall/digital-design/blob/main/main.pdf">URL</a> to find out circuit Diagram between Arduino and Vaman.<br>
After Succesfully connecting both Vaman and Arduino, Connect Mobile to Arduino using Otg cable.<br>Then proceed to Uploading bin to vaman.

<h3>Uploading bin to Vaman</h3>
<ol>
  <li>Open Arduino Droid</li>
  <li>Select Board Type</li>
  <pre>Menu --> Settings --> Board type --> ESP32 --> DOIT ESP32 DEVKIT V1</pre>
  Flash Frequency: 80MHz<br>
  Upload Speed: 115200<br>
  Click ok
  <li>Upload <b>.bin</b> file</li>
  <pre>Menu --> Upload --> Upload precompiled --> location of bin</pre>
</ol>

<h3>Accessing Webpage</h3>
<ol>
  <li>After succesfull uploading, The Vaman connects to the Network whose credentials are provided in the <b>.cpp</b> file.</li>
  <li>Now find out the Ip address assigned to vaman</li>
  <pre>ifconfig
nmap -sn 192.168.xx.xx/24</pre>
  <li>After finding the IP, Paste it in the Web browser and use the problem values.</li>
</ol>

