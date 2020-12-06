#include "NetworkLayer.c"
#include "crc.h"
#define MTU 65535
unsigned char datagram[MTU];

/*
    Encapsulate the datagram using given informations.
    input:
        int userdatalength: length of the user message to be encapsulated into the datagram.
        unsigned char *userdata: array of user message to be encapsulated into the datagram.
        unsigned short src_port: number of port of the source process.
        unsigned short dst_port: number of port of the destination process.
        unsignde short total_length: Total length of the UDP datagram.
        unsigned char *result: the result of encapsulated datagram.
*/
void encapsulate_datagram(int userdatalength, unsigned char *userdata, unsigned short src_port, unsigned short dst_port,
                          unsigned short total_length, unsigned char *result)
{
  //2 bytes, source port.
  memcpy(&result[0], &src_port, 2);

  //2 bytes, destination port.
  memcpy(&result[2], &dst_port, 2);

  //2 bytes, total_length.
  memcpy(&result[4], &total_length, 6);

  //2 bytes, checksum
  unsigned short temp = 0;
  memcpy(&result[6], &temp, 8);

  //less than MTU-28 bytes, data.
  memcpy(&result[8], userdata, userdatalength);
}

/*
    Send the datagram, using servecices provided by Network layer.
    input:
        int userdatalength: length of user message.
        unsigned char *userdata: Array of the user message.
        unsigned short src_port: port number of the source process.
        unsigned short dst_port: port number of the destination process.
        ip_address ip1: ip address of the source host.
        ip_address ip2: ip address of the destination host.
*/
void send_datagram(int userdatalength, unsigned char *userdata, unsigned short src_port, unsigned short dst_port, ip_address ip1, ip_address ip2)
{
  encapsulate_datagram(userdatalength, userdata, src_port, dst_port, userdatalength + 8, datagram);
  printf("-------------------------------------------------------------------\n");
  printf("Successfully encapsultated into user datagram, the content of datagram:\n");
  for (int i = 0; i < userdatalength + 8; i++)
  {
    printf("%x ", datagram[i]);
  }
  printf("\n");
  printf("Length of userdatagram is %d \n", userdatalength + 8);

  fragment(userdatalength + 8, datagram, ip1, ip2);
}

/*
    input the datagram, using the services provided by network layer
*/
void recieve_datagram()
{
  recieve_packet();
}