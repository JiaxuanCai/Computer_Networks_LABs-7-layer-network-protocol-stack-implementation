#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>
#include "crc.h"
#include "reciever.h"
#define SERVER_PORT 8000
//Rename the char array, to conveniently connote the MAC address.
typedef unsigned char mac_address[6];

//The MAC address of local sender
mac_address source_mac = {0x8c, 0x16, 0x45, 0x40, 0x0d, 0xac};

//the MAC address of destination receiver
mac_address destination_mac = {0x8d, 0x17, 0x46, 0x41, 0x0e, 0xad};

//The MAC address of local receiver
mac_address local_mac = {0x8d, 0x17, 0x46, 0x41, 0x0e, 0xad};

/*
    Encapsulate the frame using given informations.
    input:
        mac_address *dst: 6 bytes, destination MAC address.
        mac_address *src: 6 bytes, source MAC address.
        unsigned short protocol: 2 bytes, protocol sequence.
        unsigned char *payload: 46-1500 bytes, The data encapsulated into the frame.
        int payloadlen: Length of the data.
        unsigned char *result: Result frame.
*/
void frame_encapsulate(mac_address *dst, mac_address *src, unsigned short protocol, unsigned char *payload, int payloadlength, unsigned char *result)
{
  //6 bytes, destination MAC address.
  memcpy(&result[0], dst, 6);

  //6 bytes, source MAC address.
  memcpy(&result[6], src, 6);

  //2 bytes, protocol sequence.
  memcpy(&result[12], &protocol, sizeof(protocol));

  //46-1500 bytes, The data encapsulated into the frame.
  memcpy(&result[14], payload, payloadlength);

  //CRC sequence, 4 bytes.
  unsigned int crc32_result = crc32(result, payloadlength + 14);
  memcpy(&result[14 + payloadlength], &crc32_result, sizeof(crc32_result));
  // printf("Successfully encapsulated into frame! Content of this frame is \n");
  // for (int i = 0; i < payloadlength + 18; i++)
  // {
  //   printf("%x ", result[i]);
  // }
  // printf("\n");
}

/*
    Send the frame using Socket.
    input:
        unsigned char *frame: The frame to be sent through the Socket.
        unsigned int frame_len: Length of the frame,
*/
void send_frame(unsigned char *frame, unsigned int framelength)
{
  //initialize windows Socket
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 1), &wsaData))
  {
    printf("winsock init failed!\n");
    return 1;
  }

  struct sockaddr_in server;
  int len = sizeof(server);

  server.sin_family = AF_INET;                     //Protocol cluster,in Socket:AF_INET
  server.sin_port = htons(8000);                   //server listening port
  server.sin_addr.s_addr = inet_addr("127.0.0.1"); //address of the server
  SOCKET sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sendto(sock_fd, frame, framelength, 0, (struct sockaddr *)&server, sizeof(struct sockaddr)) != SOCKET_ERROR)
  {
    // printf("Successfully sent\n");
    return 0;
  }
  else
  {
    printf("Failed in sent\n");
  }
}

/*
    recieve the frame, using the services provided by reviever.c
*/
void recieve_frame()
{
  recieve();
}