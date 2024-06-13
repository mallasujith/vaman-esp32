#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include"libs/matfun.h"
#define BUFFER_SIZE 2048


void sendHTMLForm(int client_fd, double a, double b, double c, double d, double e, double f, double g, double h, double m1, double m2, double m3 ) {
    char *html_template = "<!DOCTYPE html>\n"
                          "<html lang=\"en\">\n"
                          "<head>\n"
                          "<meta charset=\"UTF-8\">\n"
                          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                          "<title>Angular Bisector</title>\n"
                          "<!--<style>body{text-align: center;}</style>-->"
                          "</head>\n"
                          "<body>\n"
                          "<h2>Angular Bisector of a Triangle</h2>\n"
                          "<h3>Enter the coordinates of the triangle A, B, C</h3>\n"
                          "<form method=\"post\">\n"
                          "    <label for=\"x1\">Vertex A:(x1,y1)</label><br>"
                          "    <input type=\"text\" id=\"x1\" name=\"x1\" step=\"any\" required value=\"1\">"
                          "    <input type=\"text\" id=\"y1\" name=\"y1\" step=\"any\" required value=\"-1\"><br><br>"
                          "    <label for=\"x2\">Vertex B:(x2,y2)</label><br>"
                          "    <input type=\"text\" id=\"x2\" name=\"x2\" step=\"any\" required value=\"-4\">"
                          "    <input type=\"text\" id=\"y2\" name=\"y2\" step=\"any\" required value=\"6\"><br><br>"
                          "    <label for=\"x3\">Vertex C:(x3,y3)</label><br>"
                          "    <input type=\"text\" id=\"x3\" name=\"x3\" step=\"any\" required value=\"-3\">"
                          "    <input type=\"text\" id=\"y3\" name=\"y3\" step=\"any\" required value=\"-5\"><br><br>"
                          "    <button type=\"submit\">Calculate</button>\n"
                          "</form>\n"
                          "<h3>Result</h3>\n"
                          
                          "<p>Result D: %lf, %lf</p>"
                          "<p>Result E: %lf, %lf</p>"
                          "<p>Result F: %lf, %lf</p>"
                          "<p>Incenter I: %lf, %lf</p>"
                          "<p>Distance AB: %lf</p>"
                          "<p>Distance BC: %lf</p>"
                          "<p>Distance CA: %lf</p>"
                          "</body>\n"
                          "</html>\n";

    char response[BUFFER_SIZE];
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
    snprintf(response + strlen(response), BUFFER_SIZE - strlen(response),
             html_template, a,b,c,d,e,f,g,h, m1, m2, m3);

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
    address.sin_port = htons(7500);

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
    printf("Server is listening on port localhost:7500\n");

    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("New connection accepted\n");

        char buffer[BUFFER_SIZE] = {0};
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Received data from client: %s\n", buffer);

        char *body_start = strstr(buffer, "\r\n\r\n");

        if (body_start) {

            body_start += 4;
            
            double **A,**B,**C,**D,**E,**F,**G,**eigenvalues,**eigenvectors,**I;
            double m1,m2,m3;
            double l1 = (m1 + m3 - m2) / 2;
            double l2 = (m1 + m2 - m3) / 2;
            double l3 = (m3+ m2 - m1) / 2;
            int m=2,n=1, x1, y1, x2, y2, x3, y3;;
            sscanf(body_start, "x1=%d&y1=%d&x2=%d&y2=%d&x3=%d&y3=%d", &x1, &y1, &x2, &y2, &x3, &y3);
            A = createMat(m,n);
            B = createMat(m,n);
            C = createMat(m,n);
            I = createMat(2, 1);
            A[0][0] = x1;
            A[1][0] = y1;
            B[0][0] = x2;
            B[1][0] = y2;
            C[0][0] = x3;
            C[1][0] = y3;
            m3 = Matnorm(Matsub(B, A, 2, 1), 2);
            m2 = Matnorm(Matsub(C, A, 2, 1), 2);
            m1 = Matnorm(Matsub(C, B, 2, 1), 2);
            G = Matscale(Matadd(Matadd(A, B, 2, 1), C, 2, 1), 2, 1, 1.0 / 3.0);
            eigenvalues = Mateigval(Matadd(Matadd(A, B, 2, 1), C, 2, 1));
            eigenvectors = Mateigvec(Matadd(Matadd(A, B, 2, 1), C, 2, 1), eigenvalues);
            I = Matsec(G, eigenvectors, 2, l1 / (l1 + l2 + l3));
    
      D = Matscale(Matadd(Matscale(C, 2, 1, l1), Matscale(B, 2, 1, l2), 2, 1), 2, 1, 1.0 / (l1 + l2));
      E = Matscale(Matadd(Matscale(A, 2, 1, l2), Matscale(C, 2, 1, l3), 2, 1), 2, 1, 1.0 / (l2 + l3));
      F = Matscale(Matadd(Matscale(B, 2, 1, l3), Matscale(A, 2, 1, l1), 2, 1), 2, 1, 1.0 / (l3 + l1));
   
  
      sendHTMLForm(client_fd, D[0][0], D[1][0], E[0][0], E[1][0], F[0][0], F[1][0], I[0][0], I[1][0], m1, m2, m3);

            printf("Results sent to client\n");
            
            
        }

        close(client_fd);
    }

    return 0;
}

    
   
