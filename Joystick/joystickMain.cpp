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
#define SENDSIZE 7

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

void jsRead(int socket, char jsVal[7], js_event jsE)
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
        sprintf(temp, "%03d", (int)value);
        strcat(jsVal, temp);
        strcat(jsVal, ">");
        puts(jsVal);
        send(socket,jsVal,SENDSIZE,0);
    }
}

int main(int argc, char *argv[])
{
    // Set up joystick.
    int done = 0;
    struct js_event jsE;
    char jsVal[SENDSIZE];
    jsInit(jsE);
    
    // Set up server socket.
    int sock_id, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    sock_id = socket(PF_INET, SOCK_STREAM, 0);
    
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(8080);
    /* Set IP address to localhost */
    //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
    /*---- Bind the address struct to the socket ----*/
    bind(sock_id, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(sock_id,1)==0)
        printf("Listening\n");
    else
        printf("Error\n");

    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    newSocket = accept(sock_id, (struct sockaddr *) &serverStorage, &addr_size);
    
    while (!done) {      
        jsRead(newSocket, jsVal, jsE);
    } 
    
    return 0;
}