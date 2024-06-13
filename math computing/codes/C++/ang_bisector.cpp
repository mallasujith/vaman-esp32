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

AsyncWebServer server(80);

const char* ssid = "Iqooneo6";
const char* password = "gps93093";

const char* input_parameter00 = "input00";
const char* input_parameter01 = "input01";
const char* input_parameter10 = "input10";
const char* input_parameter11 = "input11";
const char* input_parameter20 = "input20";
const char* input_parameter21 = "input21";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Angle Bisector</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block;}
    h2 {font-size: 2.0rem; color: blue;}
  </style>
  <script>
    function updateResults(results) {
      document.getElementById("results").innerHTML = results;
    }
  </script>
  </head><body>
  <h2>TO Find the Incenter and Inradius</h2>
  <p>Enter the values of points A, B, C</p>
  <form id="triangleForm" onsubmit="submitForm(); return false;">
    Enter the values of Point A: <input type="number" value="1" name="input00"><input type="number" value = "-1" name="input01"><br>
    Enter the values of Point B: <input type="number" value="-4" name="input10"><input type="number" value="6" name="input11"><br>
    Enter the values of Point C: <input type="number" value="-3" name="input20"><input type="number" value="-5" name="input21"><br>
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


    double **A, **B, **C,**eigen,a,b,c, **D, **E, **F;
    int m1 = 2, n1 = 1;
    A = createMat(m1, n1);
    B = createMat(m1, n1);
    C = createMat(m1, n1);
    eigen = createMat(m1, n1);
    
    A[0][0] = Ax;
    A[1][0] = Ay;
    B[0][0] = Bx;
    B[1][0] = By;
    C[0][0] = Cx;
    C[1][0] = Cy;

    double **diff_AB = Matsub(B, A, 2, 1); 
    double distance_AB = Matnorm(diff_AB, 2);  
    
    double **diff_AC = Matsub(C, A, 2, 1);
    double distance_AC = Matnorm(diff_AC, 2);  
    
    double **diff_BC = Matsub(C, B, 2, 1);
    double distance_BC = Matnorm(diff_BC, 2);
    
    a = distance_BC;
    b = distance_AC;
    c = distance_AB;
    
    double l1 = (a + c - b) / 2;
    double l2 = (a + b - c) / 2;
    double l3 = (c+ b - a) / 2;
    
    D = Matscale(Matadd(Matscale(C, 2, 1, l1), Matscale(B, 2, 1, l2), 2, 1), 2, 1, 1.0 / (l1 + l2));
    E = Matscale(Matadd(Matscale(A, 2, 1, l2), Matscale(C, 2, 1, l3), 2, 1), 2, 1, 1.0 / (l2 + l3));
    F = Matscale(Matadd(Matscale(B, 2, 1, l3), Matscale(A, 2, 1, l1), 2, 1), 2, 1, 1.0 / (l3 + l1));


    double **temp1 = Matscale(A, 2, 1, a);
    double **temp2 = Matscale(B, 2, 1, b);
    double **temp3 = Matscale(C, 2, 1, c);

    double **eigenvalues = Matadd(Matadd(temp1, temp2, 2, 1), temp3, 2, 1);
    double eigendenominator = a + b + c;

    eigen[0][0] = eigenvalues[0][0] / eigendenominator;
    eigen[1][0] = eigenvalues[1][0] / eigendenominator;
    
    String result = 
    "D: " + String(D[0][0],2) + ", " + String(D[1][0],2) + "<br>"
    "E: " + String(E[0][0],2) + ", " + String(E[1][0],2) + "<br>"
    "F: " + String(F[0][0],2) + ", " + String(F[1][0],2) + "<br>"
    "distance of AB: " + c + "<br>"
    "distance of BC: " + a + "<br>"
    "distance of CA: " + b + "<br>"
    "InCenter:"  + String(eigen[0][0],2) + ", " + String(eigen[1][0], 2) + "<br>"    
    ".";
    
    request->send(200, "text/html", result);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
}
