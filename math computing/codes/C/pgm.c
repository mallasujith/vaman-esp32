#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "libs/matfun.h"
#include "libs/geofun.h"

#define PORT 8787
#define BUFFER_SIZE 2048

void sendHTMLForm(int client_fd, int x1, int y1, int x2, int y2, int x3, int y3, double mid1, double mid11, double mid2, double mid22, double mid3, double mid33) {
    char *html_template = "<!DOCTYPE html>\n"
                          "<html lang=\"en\">\n"
                          "<head>\n"
                          "<meta charset=\"UTF-8\">\n"
                          "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                          "<title> SOFTWARE SOLUTION</title>\n"
                          "</head>\n"
                          "<body>\n"
"<style>"
    "html {font-family: Times New Roman; display: inline-block; text-align: center;}"
    "h2 {font-size: 2.0rem; color: blue;}"
  "</style>"
                          "<h2>TRIANGLE CALCULATOR</h2>\n"
                          "<form method=\"post\">\n"
                          "    <label for=\"x1\">Vertex A:(x1,y1)</label><br>\n"
                          "    <input type=\"text\" id=\"x1\" name=\"x1\" value=\"%d\" step=\"any\" required><br><br>"
                          "    <input type=\"text\" id=\"y1\" name=\"y1\" value=\"%d\" step=\"any\" required><br><br>"
                          "    <label for=\"x2\">Vertex B:(x2,y2)</label><br>\n"
                          "    <input type=\"text\" id=\"x2\" name=\"x2\" value=\"%d\" step=\"any\" required><br><br>"
                          "    <input type=\"text\" id=\"y2\" name=\"y2\" value=\"%d\" step=\"any\" required><br><br>"
                          "    <label for=\"x3\">Vertex C:(x3,y3)</label><br>\n"
                          "    <input type=\"text\" id=\"x3\" name=\"x3\" value=\"%d\" step=\"any\" required><br><br>"
                          "    <input type=\"text\" id=\"y3\" name=\"y3\" value=\"%d\" step=\"any\" required><br><br>"
                          "    <button type=\"submit\">Calculate</button>\n"
                          "</form>\n"
                          "<h3>Result</h3>\n"
                          "<p>Result D1: %.2lf</p>\n"
                          "<p>Result D2: %.2lf</p>\n"
                          "<p>Result E1: %.2lf</p>\n"
                          "<p>Result E2: %.2lf</p>\n"
                          "<p>Result F1: %.2lf</p>\n"
                          "<p>Result F2: %.2lf</p>\n"
                          "</body>\n"
                          "</html>\n";

    char response[BUFFER_SIZE];
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
    snprintf(response + strlen(response), BUFFER_SIZE - strlen(response), html_template, x1, y1, x2, y2, x3, y3, mid1, mid11, mid2, mid22, mid3, mid33);

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
    printf("Server is listening on port 8787\n");

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
        
            int x1,y1,x2,y2,x3,y3;
            sscanf(body_start,"x1=%d&y1=%d&x2=%d&y2=%d&x3=%d&y3=%d", &x1, &y1, &x2, &y2, &x3, &y3);
            // Create matrices for the coordinates
            double **a = createMat(3, 2);
            a[0][0] = x1; a[0][1] = y1;
            a[1][0] = x2; a[1][1] = y2;
            a[2][0] = x3; a[2][1] = y3;

            // Create matrix b for scaling
            double **b = createMat(3, 1);
            b[0][0] = b[1][0] = b[2][0] = 1.0;

            // Call the Matsec function to calculate midpoints
            double **midpoints = Matsec(a, b, 3, 1.0);
            
            // Extract midpoints
            double mid1 = midpoints[0][0];
            double mid11 = midpoints[0][1];
            double mid2 = midpoints[1][0];
            double mid22 = midpoints[1][1];
            double mid3 = midpoints[2][0];
            double mid33 = midpoints[2][1];
            
            // Free memory allocated for matrices
            freeMat(a,3);
            freeMat(b,3);
            freeMat(midpoints,3);
            
            sendHTMLForm(client_fd, x1, y1, x2, y2, x3, y3, mid1, mid11, mid2, mid22, mid3, mid33);
            printf("Results sent to client\n");
        }
        
        close(client_fd);
    }

    return 0;
}
