#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include "libs/matfun.h" // Assuming you have defined matrix functions in matfun.h
#include "libs/geofun.h" // Assuming you have defined geometric functions in geofun.h

AsyncWebServer server(80);

const char* ssid = "nikhilsai";
const char* password = "nikhilsai";

const char* input_parameter00 = "input00";
const char* input_parameter01 = "input01";
const char* input_parameter10 = "input10";
const char* input_parameter11 = "input11";
const char* input_parameter20 = "input20";
const char* input_parameter21 = "input21";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>TRIANGLE CALCULATOR</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.0rem; color: blue;}
  </style>
  <script>
    function updateResults(results) {
      document.getElementById("results").innerHTML = results;
    }
  </script>
  </head><body>
  <h2>Number midpoint</h2>
  <p>Enter the values of points A, B, C</p>
  <form id="triangleForm" onsubmit="submitForm(); return false;">
    Vertex A:(x1,y1) <input type="number" name="input00"><br><input type="number" name="input01"><br>
    Vertex B:(x2,y2) <input type="number" name="input10"><br><input type="number" name="input11"><br>
    Vertex C(x3,y3): <input type="number" name="input20"><br><input type="number" name="input21"><br>
    <input type="submit" value="Submit">
  </form><br>
  <div id="results"></div>
  <br><a href="/">Return to Home Page</a>
  <script>
    function submitForm() {
      var formData = new FormData(document.getElementById("triangleForm"));
      fetch('/get', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(results => updateResults(results));
    }
  </script>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connecting...");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_POST, [] (AsyncWebServerRequest *request) {
    double Ax = request->arg(input_parameter00).toDouble();
    double Ay = request->arg(input_parameter01).toDouble();
    double Bx = request->arg(input_parameter10).toDouble();
    double By = request->arg(input_parameter11).toDouble();
    double Cx = request->arg(input_parameter20).toDouble();
    double Cy = request->arg(input_parameter21).toDouble();

    // Create matrices for the coordinates
    double **a = createMat(3, 2);
    a[0][0] = Ax; a[0][1] = Ay;
    a[1][0] = Bx; a[1][1] = By;
    a[2][0] = Cx; a[2][1] = Cy;

    // Create matrix b for scaling
    double **b = createMat(3, 1);
    b[0][0] = b[1][0] = b[2][0] = 1.0;

    // Call the Matsec function to calculate midpoints
    double **midpoints = Matsec(a, b, 3, 1.0);
    
    // Extract midpoints
    double MxAB = midpoints[0][0];
    double MyAB = midpoints[0][1];
    double MxBC = midpoints[1][0];
    double MyBC = midpoints[1][1];
    double MxCA = midpoints[2][0];
    double MyCA = midpoints[2][1];
    
    // Free memory allocated for matrices
    freeMat(midpoints, 3);
    freeMat(a, 3);
    freeMat(b, 3);

    // Send the results back to the client
    String result = "Midpoint AB: (" + String(MxAB, 2) + ", " + String(MyAB, 2) + ")<br>Midpoint BC: (" + String(MxBC, 2) + ", " + String(MyBC, 2) + ")<br>Midpoint CA: (" + String(MxCA, 2) + ", " + String(MyCA, 2) + ")";
    request->send(200, "text/html", result);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Nothing to do here
}
