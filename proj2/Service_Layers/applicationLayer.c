#include "transmissionLayer_udp.c"

// define ip address of source host
ip_address ip1 = {192, 168, 1, 1};

// define ip address of destination host
ip_address ip2 = {192, 168, 1, 0};

// define port number of source process
source_port = 8000;

// define port mumber of destination process
destination_port = 8001;

/*
    Output the user data message
    input:
        unsigned char *usermessage: The array of user data message.
*/
void output(unsigned char *usermessage)
{
  send_datagram(strlen(usermessage), usermessage, source_port, destination_port, ip1, ip2);
}

/*
    input the user data message, using the services provided by transmission layer
*/
void input()
{
  recieve_datagram();
}