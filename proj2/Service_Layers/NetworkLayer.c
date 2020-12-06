#include "DataLinkLayer.c"
#include "crc.h"
#define MTU 50 // made to test the fragment, when in real use, we must set the MTU bigger. Just to clarify!!!
typedef unsigned char ip_address[4];

//this struct definition is used to spilt a
//byte into 2 parts, each part takes 4 bits.
struct first_byte
{
  unsigned char IHL : 4;
  unsigned char version : 4;
};

unsigned char packet[1000];
unsigned char frame[1000];

/*
    Fragment the datagram into small pieces.
    input:
        int datagramlength: Length of the datagram
        unsigned char *datagram: Array of the datagram
        ip_address ip1: ip address of the source host
        ip_address ip2: ip address of the destination host
*/
void fragment(int datagramlength, unsigned char *datagram, ip_address ip1, ip_address ip2)
{
  unsigned int total_len = datagramlength;
  struct first_byte first_byte1;
  first_byte1.version = 4;
  first_byte1.IHL = 5;
  // if the datagram is smaller than the MTU, directly encapsulate it.
  if (datagramlength + 8 < MTU)
  {
    packet_encapsulate(first_byte1, '0', strlen(datagram) + 8, 0, 0, '10', '5', ip1, ip2, strlen(datagram), datagram, &packet[0]);
  }
  //else if the datagram is bigger than the MTU, we must split it up.
  else
  {
    int number_of_fragments = datagramlength / (MTU - 8);
    printf("is divided into %d fragments\n", number_of_fragments + 1);
    printf("-------------------------------------------------------------------\n\n");
    for (int i = 0; i < number_of_fragments; i++)
    {
      unsigned short offset = i * (MTU - 8) / 8;
      unsigned short fragment = offset | 0x2000;
      packet_encapsulate(first_byte1, '0', MTU, 0, fragment, '10', '5', ip1, ip2, MTU - 8, &datagram[i * (MTU - 8)], &packet[0]);
      printf("this is frag %d, range of data is %d - %d \n", i + 1, i * (MTU - 8), i * (MTU - 8) + MTU - 8);
      printf("the sequence of framgment is %x \n", fragment);
      printf("-------------------------------------------------------------------\n\n");
    }
    unsigned short offset = number_of_fragments * (MTU - 8) / 8;
    unsigned short fragment = offset | 0x0000;
    packet_encapsulate(first_byte1, '0', total_len - number_of_fragments * (MTU - 8) + 20, 0, fragment, '10', '5', ip1, ip2, total_len - (number_of_fragments - 1) * (MTU - 8), &datagram[number_of_fragments * (MTU - 8)], &packet[0]);
    printf("this is frag %d, range of data is %d - %d \n", number_of_fragments + 1, number_of_fragments * (MTU - 8), number_of_fragments * (MTU - 8) + total_len - number_of_fragments * (MTU - 8));
    printf("the sequence of framgment is %x \n", fragment);
    printf("-------------------------------------------------------------------\n\n");
  }
}

/*
    Encapsulate the packet using given informations.
    input:
        mac_address *dst: 6 bytes, destination MAC address.
        mac_address *src: 6 bytes, source MAC address.
        unsigned short protocol: 2 bytes, protocol sequence.
        unsigned char *payload: 46-1500 bytes, The data encapsulated into the frame.
        int payloadlen: Length of the data.
        unsigned char *result: Result frame.
*/
void packet_encapsulate(struct first_byte version_ihl, unsigned char dif_services, unsigned short total_length, unsigned short identification, unsigned short fragment,
                        unsigned char ttl, unsigned char protocol, ip_address *src, ip_address *dst, int datalength, unsigned char *data, unsigned char *result)
{
  //1 bytes, version and IHL.
  memcpy(&result[0], &version_ihl, 1);

  //1 bytes, differentiated services.
  memcpy(&result[1], &dif_services, 1);

  //2 bytes, total length of this packet.
  memcpy(&result[2], &total_length, 2);

  //2 bytes, identification of this packet.
  memcpy(&result[4], &identification, 2);

  //2 bytes, fragment.
  memcpy(&result[6], &fragment, 2);

  //1 bytes, ttl.
  memcpy(&result[8], &ttl, 1);

  //1 bytes, protocol.
  memcpy(&result[9], &protocol, 1);

  //first set the checksum to be all zeros, checksum's length is 2 bytes
  unsigned short temp = 0;
  memcpy(&result[10], &temp, 2);

  //4 bytes, source address.
  memcpy(&result[12], src, 4);

  //4 bytes, destination address.
  memcpy(&result[16], dst, 4);

  //less than MTU-20 bytes, data.
  memcpy(&result[20], data, datalength);

  unsigned short crc16_result = crc16(result, 20);
  memcpy(&result[10], &crc16_result, 2);
  printf("-------------------------------------------------------------------\n");

  printf("Successfully encapsulated into packet. Content of this packet:\n");
  for (int i = 0; i < total_length; i++)
  {
    printf("%x ", packet[i]);
  }
  printf("\n");

  frame_encapsulate(&destination_mac, &source_mac, 0x86dd, result, total_length, &frame[0]);

  send_frame(frame, total_length + 18);
}

/*
    recieve packet, using the services provided by data link layer
*/
void recieve_packet()
{
  recieve_frame();
}