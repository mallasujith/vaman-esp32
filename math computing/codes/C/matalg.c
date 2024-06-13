#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "libs/matfun.h"
#include "libs/geofun.h"
#define PORT 5500
#define BUFFER_SIZE 8192

void sendHTMLForm(int client_fd, double AB, double BC, double CA,double hx, double hy, double ABx, double ABy, double BCx, double BCy, double CAx, double CAy, double Dx, double Dy, double Ex, double Ey, double Fx, double Fy, double Ix, double Iy, double DAB, double DBC, double DCA) {
    char *html_template = "<!DOCTYPE html>\n"
                          "<html lang=\"en\">\n"
                          "<head>\n"
                          "<meta charset=\"UTF-8\">\n"
                          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                          "<title> Mat_Alg </title>\n"
                          "</head>\n"
                          "<body>\n"
                          "<style>"
    "html {font-family: Times New Roman; display: inline-block; text-align: center;}"
    "h2 {font-size: 2.0rem; color: blue;}"
  "</style>" 
                          "<h2>Mat_Alg</h2>\n"
                          "<form method=\"post\">\n"
                          "    <label for=\"x1\">Vertex A:(x1,y1)</label><br>\n"
                          "    <input type=\"text\" id=\"x1\" name=\"x1\" value=\"1\" step=\"any\" required>"
                          "    <input type=\"text\" id=\"y1\" name=\"y1\" value=\"-1\" step=\"any\" required><br><br>"
                          "    <label for=\"x2\">Vertex B:(x2,y2)</label><br>\n"
                          "    <input type=\"text\" id=\"x2\" name=\"x2\" value=\"-4\" step=\"any\" required>"
                          "    <input type=\"text\" id=\"y2\" name=\"y2\" value=\"6\" step=\"any\" required><br><br>"
                          "    <label for=\"x3\">Vertex C:(x3,y3)</label><br>\n"
                          "    <input type=\"text\" id=\"x3\" name=\"x3\" value=\"-3\" step=\"any\" required>"
                          "    <input type=\"text\" id=\"y3\" name=\"y3\" value=\"-5\" step=\"any\" required><br><br>"
                          "    <button type=\"submit\">Calculate</button>\n"
                          "<h3>Results</h3>"
                          "<p>********************Medians********************</p>"
                          "<p>Median AD: %.6f</p>"
                          "<p>Median BE: %.6f</p>"
                          "<p>Median CF: %.6f</p>"
                          "<p>******************Orthocenter******************</p>"
                          "<p>Orthocenter H: %lf, %lf</p>\n"
                          "<p>*************Perpendicular Bisector**************</p>"
                          "<p>Bisector of AB:%.6f,%.6f</p>\n"
                          "<p>Bisector of BC:%.6f,%.6f</p>\n"
                          "<p>Bisector of CA:%.6f,%.6f</p>\n"
                          "<p>****************Angular Bisector*****************</p>"
                          "<p></p>"
                          "<p>Bisector of A : %.6f, %.6f</p>"
                          "<p>Bisector of B: %.6f, %.6f</p>"
                          "<p>Bisector of C: %.6f, %.6f</p>"
                          "<p>Incenter I: %.6f, %.6f</p>"
                          "<p>Distance AB: %.6f</p>"
                          "<p>Distance BC: %.6f</p>"
                          "<p>Distance CA: %.6f</p>"
                          "</body>\n"
                          "</html>\n";

    char response[BUFFER_SIZE];
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
    snprintf(response + strlen(response), BUFFER_SIZE - strlen(response), html_template, AB,BC,CA,hx, hy,ABx,ABy,BCx,BCy,CAx,CAy,Dx,Dy,Ex,Ey,Fx,Fy,Ix,Iy,DAB,DBC,DCA);

    send(client_fd, response, strlen(response), 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Print server address
    printf("Server is listening on port localhost:5500\n");

    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("New connection accepted\n");
        
        char buffer[BUFFER_SIZE] ={0};
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Received data from client: %s\n", buffer);
        
        char *body_start = strstr(buffer,"\r\n\r\n");
        
        if(body_start){
        
            body_start +=4;
        
            int x1,y1,x2,y2,x3,y3,m=2,n=1,i,j,k=0;
            double **A,**B,**C,**m1,**m2,**m3,**H,**l,**N,**m3_t,**m1_t,**p0,**p1,**p,**P1,**P2;
            sscanf(body_start,"x1=%d&y1=%d&x2=%d&y2=%d&x3=%d&y3=%d  ", &x1, &y1, &x2, &y2, &x3, &y3);
		A = createMat(m,n);
		B = createMat(m,n);
		C = createMat(m,n);
		p = createMat(2,1);
		l = createMat(m,m);
		H = createMat(2,1);
		A[0][0] = x1;
		A[1][0] = y1;
		B[0][0] = x2;
		B[1][0] = y2;
		C[0][0] = x3;
		C[1][0] = y3;
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

		
			
            
            sendHTMLForm(client_fd, sideAB, sideBC, sideCA,H[0][0], H[1][0],bisectorABMidpoint[0][0], bisectorABMidpoint[1][0], bisectorBCMidpoint[0][0],bisectorBCMidpoint[1][0], bisectorCAMidpoint[0][0], bisectorCAMidpoint[1][0],Da[0][0], Da[1][0], Ea[0][0], Ea[1][0], Fa[0][0], Fa[1][0], I[0][0], I[1][0], c, a, b);
            printf("Results sent to client\n");
            freeMat(A,2);
            freeMat(B,2);
            freeMat(C,2);
            freeMat(m1,2);
            freeMat(m2,2);
            freeMat(m3,2);
            freeMat(H,2);
        }
        
        close(client_fd);
    }

    return 0;
}
