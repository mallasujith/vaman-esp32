#include <ArduinoOTA.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include"libs/matfun.h"
#include"libs/geofun.h"

AsyncWebServer server(80);

const char* ssid = "RedmiNote10S";
const char* password = "arunkumar1";

const char* input_parameter00 = "input00";
const char* input_parameter01 = "input01";
const char* input_parameter10 = "input10";
const char* input_parameter11 = "input11";
const char* input_parameter20 = "input20";
const char* input_parameter21 = "input21";

const char index_html[] PROGMEM = R"rawliteral(
          <!DOCTYPE HTML><html><head>
            <title>Tangents</title>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <style>
              html {font-family: Times New Roman; display: inline-block;}
              h2 {font-size: 2.0rem; color: red;}
            </style>
            <script>
              function updateResults(results) {
                document.getElementById("results").innerHTML = results;
              }
            </script>
            </head><body>
            <h2>TO FIND THE TANGENTS</h2>
            <p>Enter the values of points A, B, C</p>
            <form id="triangleForm" onsubmit="submitForm(); return false;">
              Enter the values of Point A: <br><input type="number" value = "1" name="input00"><input type="number" value = "-1" name="input01"><br>
              Enter the values of Point B: <br><input type="number" value = "-4" name="input10"><input type="number" value = "6" name="input11"><br>
              Enter the values of Point C: <br><input type="number" value = "-3" name="input20"><input type="number" value = "-5" name="input21"><br>
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

    // Calculate midpoints of the triangle vertices
    double **A, **B, **C, **sol,**side_AB,**side_BC,**side_CA,a, b, c, m_val, n_val, p_val;
    int m = 2, n = 1;
    A = createMat(m, n);
    B = createMat(m, n);  
    C = createMat(m, n);
    sol = createMat(2,2);
    A[0][0] = Ax;
    A[1][0] = Ay;
    B[0][0] = Bx;
    B[1][0] = By;
    C[0][0] = Cx;
    C[1][0] = Cy;
    side_AB = Matsub(A,B,m,n);
    side_BC = Matsub(B,C,m,n);
    side_CA = Matsub(C,A,m,n);
    a = Matnorm(side_AB,m);
    b = Matnorm(side_BC,m);
    c = Matnorm(side_CA,m);
    p_val = (a + c - b) / 2;
    m_val = (a + b - c) / 2;
    n_val = (b + c - a) / 2;
    sol[0][0] = (m_val * A[0][0] + p_val * B[0][0]) / (m_val + p_val);
    sol[0][1] = (p_val * C[0][0] + n_val * A[0][0]) / (p_val + n_val);
    sol[1][0] = (m_val * A[1][0] + p_val * B[1][0]) / (m_val + p_val);
    sol[1][1] = (p_val * C[1][0] + n_val * A[1][0]) / (p_val + n_val);
  
    // Send the results back to the client
    String result = "E: (" + String(sol[0][0], 2) + ", " + String(sol[0][1], 2) + ")<br>F: (" + String(sol[1][0], 2) + ", " + String(sol[1][1], 2) + ")";

    request->send(200, "text/html", result);
    printf("Results sent to client\n");
    freeMat(A, 2);
    freeMat(B, 2);
    freeMat(C, 2);
    freeMat(sol, 2);
    freeMat(side_AB,2);
    freeMat(side_BC,2);
    freeMat(side_CA,2);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Nothing to do here
}


