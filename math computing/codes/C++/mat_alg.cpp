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
  <title>MATALG</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  "<style>"
    "html {font-family: Times New Roman; display: inline-block; text-align: center;}"
    "h2 {font-size: 2.0rem; color: blue;}"
  "</style>" 
  <script>
    function updateResults(results) {
      document.getElementById("results").innerHTML = results;
    }
  </script>
  </head><body>
  <h2>Mat_Alg</h2>
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
    
    
    //Medinas
	double **D = Matsec(A, B, 2, 1.0);
	double **E = Matsec(B, C, 2, 1.0);
	double **F = Matsec(C, A, 2, 1.0);           

	double **s_ab, **s_bc, **s_ca;
	s_ab = Matsub(A,D,m,n);//A-D
	s_bc = Matsub(B,E,m,n);//B-E
	s_ca = Matsub(C,F,m,n);//C-F
	double sideAB = Matnorm(s_ab,m);
	double sideBC = Matnorm(s_bc,m); 
	double sideCA = Matnorm(s_ca,m);
	
	//Orthocenter
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
    
     //Perpedicular bisector			
	double **_ab, **_bc, **_ca, **bisectorABMidpoint, **bisectorBCMidpoint, **bisectorCAMidpoint;
	_ab = Matsub(A,B,m,n);//A-B
	_bc = Matsub(B,C,m,n);//B-C
	_ca = Matsub(C,A,m,n);//C-A
	bisectorABMidpoint = normVec(_ab);
	bisectorBCMidpoint = normVec(_bc);
	bisectorCAMidpoint = normVec(_ca);
	
	//Angular Bisector
	double **I,a,b,c, **Da, **Ea, **Fa;
	int ma1 = 2, na1 = 1;
	I = createMat(ma1, na1);
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
	Da = Matscale(Matadd(Matscale(C, 2, 1, l1), Matscale(B, 2, 1, l2), 2, 1), 2, 1, 1.0 / (l1 + l2));
	Ea = Matscale(Matadd(Matscale(A, 2, 1, l2), Matscale(C, 2, 1, l3), 2, 1), 2, 1, 1.0 / (l2 + l3));
	Fa = Matscale(Matadd(Matscale(B, 2, 1, l3), Matscale(A, 2, 1, l1), 2, 1), 2, 1, 1.0 / (l3 + l1));
	double **temp1 = Matscale(A, 2, 1, a);
	double **temp2 = Matscale(B, 2, 1, b);
	double **temp3 = Matscale(C, 2, 1, c);
	double **eigenvalues = Matadd(Matadd(temp1, temp2, 2, 1), temp3, 2, 1);
	double eigendenominator = a + b + c;
	I[0][0] = eigenvalues[0][0] / eigendenominator;
	I[1][0] = eigenvalues[1][0] / eigendenominator;
	
    // Send the results back to the client
    String result = "Results <br><br> Median AD: " + String(sideAB, 2) + "<br>Median BE: " + String(sideBC, 2) + "<br>Median CF: " + String(sideCA, 2) + "<br><br>";
    result += "Orthocenter O: (" + String(H[0][0],2) + ", " + String(H[1][0], 2) + ")<br><br>";
    result += "Bisector AB: (" + String(bisectorABMidpoint[0][0], 2) +", "+ String(bisectorABMidpoint[1][0],2) + ")<br>Bisector BC: (" + String(bisectorBCMidpoint[0][0], 2) +", "+ String(bisectorBCMidpoint[0][0], 2)+ ")<br>Bisector CA: (" + String(bisectorCAMidpoint[0][0], 2) +", "+ String(bisectorCAMidpoint[0][0],2) + ")<br><br>";
    result += "Bisector of A :(" + String(D[0][0],2) + ", " + String(D[1][0],2) + ")<br>Bisector of B :(" + String(E[0][0],2) + ", " + String(E[1][0],2) + ")<br>Bisector of  :(" + String(F[0][0],2) + ", " + String(F[1][0],2) + ")<br>distance of AB: " + c + "<br>distance of BC: " + a + "<br>distance of CA: " + b + "<br>InCenter:("  + String(I[0][0],2) + ", " + String(I[1][0], 2) + ")<br>.";
    
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
