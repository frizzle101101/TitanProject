#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "joystick.h"

#define AXIS 2
#define BUTTON 1

static int joystick_fd = -1;

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int open_joystick()
{
    joystick_fd = open(JOYSTICK_DEVNAME, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */
    if (joystick_fd < 0)
        return joystick_fd;

    return joystick_fd;
}

int read_joystick_event(struct js_event *jse)
{
    int bytes;

    bytes = read(joystick_fd, jse, sizeof(*jse)); 

    if (bytes == -1)
        return 0;

    if (bytes == sizeof(*jse))
        return 1;

    printf("Unexpected bytes from joystick:%d\n", bytes);

    return -1;
}

void close_joystick()
{
    close(joystick_fd);
}


int jsInit(js_event jsE)
{
    int jsCheck;
    
    // Open joystick and check.
    jsCheck = open_joystick();
    
    if (jsCheck < 0) {
        printf("Joystick failed to open.\n");
        exit(1);
    }
    
    return jsCheck;
}

void jsRead(char jsVal[7], js_event jsE)
{
    int readCheck;
    float value;
    char temp[4];
    readCheck = read_joystick_event(&jsE);
    jsVal[0] = '<';
    jsVal[2] = '\0';
    
    if (readCheck == 1) {
        
        if (jsE.type = 2)
        {
            // Axis has changed.
            switch (jsE.number)
            {
                case 0:
                    // X axis.
                    jsVal[1] = 'x';
                    value = (((float)jsE.value / (float)32767) * 100) + 100;
                    break;
                case 1:
                    // Y axis.
                    jsVal[1] = 'y';
                    value = (((float)jsE.value / (float)32767) * 100) + 100;
                    break;
                case 2:
                    // V axis.
                    jsVal[1] = 'v';
                    value = (((float)jsE.value / (float)32767) * 100) + 100;
                    break;
                case 3:
                    // W axis.
                    jsVal[1]= 'w';
                    value = (((float)jsE.value / (float)32767) * 100) + 100;
                    break;
            }
        } else {
            // Button has changed.
        }
        sprintf(temp, "%d", (int)value);
        strcat(jsVal, temp);
        strcat(jsVal, ">");
        puts(jsVal);
        
        
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    
    int done = 0;
    struct js_event jsE;
    char jsVal[7];
    jsInit(jsE);
    
    while (!done) {      
        jsRead(jsVal, jsE);
    }
    
    return 0;
}