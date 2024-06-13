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
  <title>ORTHOCENTER</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block;}
    h2 {font-size: 2.0rem; color: RED;}
  </style>
  <script>
    function updateResults(results) {
      document.getElementById("results").innerHTML = results;
    }
  </script>
  </head><body>
  <h2>Ortho center of the triangle</h2>
  <p>Enter the values of points A, B, C</p>
  <form id="triangleForm" onsubmit="submitForm(); return false;">
              Enter the values of Point A: <br>
              <input type="number" value = "1" name="input00">
              <input type="number" value = "-1" name="input01"><br>
              Enter the values of Point B: <br>
              <input type="number" value = "-4" name="input10">
              <input type="number" value = "6" name="input11"><br>
              Enter the values of Point C: <br>
              <input type="number" value = "-3" name="input20">
              <input type="number" value = "-5" name="input21"><br>
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


    
    int x1,y1,x2,y2,x3,y3,m=2,n=1,i,j,k=0;
    double **A,**B,**C,**m1,**m2,**m3,**H,**l,**N,**m3_t,**m1_t,**p0,**p1,**p,**P1,**P2;
    A = createMat(m,n);
    B = createMat(m,n);
    C = createMat(m,n);
    p = createMat(2,1);
    l = createMat(m,m);
    H = createMat(2,1);

    A[0][0] = Ax;
    A[1][0] = Ay;
    B[0][0] = Bx;
    B[1][0] = By;
    C[0][0] = Cx;
    C[1][0] = Cy;
    m1=Matsub(A,B,m,n);
    m2=Matsub(B,C,m,n);
    m3=Matsub(C,A,m,n);
   
    N=transposeMat(combineMat(m3,m1,m,n),m,m);
    m3_t=transposeMat(m3,m,n);
    m1_t=transposeMat(m1,m,n);
    p0=Matmul(m3_t,B,n,m,n);
    p1=Matmul(m1_t,C,n,m,n);
    p[0][0]=p0[0][0];
    p[1][0]=p1[0][0];
    // Solving the linear equations
    H[1][0]=(-1)*(p[0][0]*N[1][0]-N[0][0]*p[1][0])/Matdet(N);
    H[0][0]=(p[0][0]*N[1][1]-N[0][1]*p[1][0])/Matdet(N);
    // Send the results back to the client
    String result = "Orthocenter O: (" + String(H[0][0],2) + ", " + String(H[1][0], 2) + ")";
    request->send(200, "text/html", result);
    freeMat(A,2);
    freeMat(B,2);
    freeMat(C,2);
    freeMat(m1,2);
    freeMat(m2,2);
    freeMat(m3,2);
    freeMat(H,2);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Nothing to do here
}

