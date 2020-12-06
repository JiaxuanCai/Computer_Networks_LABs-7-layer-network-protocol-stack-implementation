// #include "transmissionLayer_udp.c"

unsigned char revieved_datagram[];

void recieve()
{
  /* initialize windows socket */
  WSADATA wsaData;

  if (WSAStartup(MAKEWORD(2, 1), &wsaData))
  {
    printf("winsock init failed!\n");
    return 1;
  }

  struct sockaddr_in local;
  struct sockaddr_in from;
  int len = sizeof(struct sockaddr_in);

  local.sin_family = AF_INET;         //Protocol cluster,in Socket:AF_INET
  local.sin_port = htons(8000);       // listening port
  local.sin_addr.s_addr = INADDR_ANY; //local

  /* create udp socket  */
  SOCKET sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  bind(sock_fd, (struct sockaddr *)&local, sizeof(local));

  unsigned char recieved_frame[1000];
  unsigned char datagram[1000];
  int counter = 0;
  while (1)
  {

    recieved_frame[0] = '\0';

    // printf("\nwaiting...\n");
    //get the size of received frame
    int frameSize = 0;
    if ((frameSize = recvfrom(sock_fd, recieved_frame, sizeof(recieved_frame), 0, (struct sockaddr *)&from, &len)) != SOCKET_ERROR)
    {
      // //1.
      // //check the size of the payload, if the payload's size is not within the
      // //field of 64-1518, then error will be generated.
      // if (frameSize > 1518 || frameSize < 64)
      // {
      //   printf("Error: Frame's payload length is %u, length error.", frameSize - 18);
      //   continue;
      // }

      // //2.
      // //check the MAC address of the received frame, if it is not the same with
      // //the local MAC address, then error will be generated.
      int flag = 1;
      for (int i = 0; i < 6; i++)
      {
        if (local_mac[i] != recieved_frame[i])
          flag = 0;
      }
      if (flag == 0)
      {
        printf("Error: Destination address error");
        continue;
      }

      // //3.
      // //check the CRC32 sequence, to know whether there is any mistakes in
      // //transmitting process. If the CRC32 sequence is wrong, then error will
      // //be generated.
      unsigned int crc32_result = crc32(recieved_frame, frameSize - 4);
      unsigned char crc32_result_c[4];
      memcpy(crc32_result_c, &crc32_result, sizeof(crc32_result));
      if (
          crc32_result_c[3] != recieved_frame[frameSize - 1] ||
          crc32_result_c[2] != recieved_frame[frameSize - 2] ||
          crc32_result_c[1] != recieved_frame[frameSize - 3] ||
          crc32_result_c[0] != recieved_frame[frameSize - 4])
      {
        printf("CRC check failed\n");
        continue;
      }

      //print out the results into the screen.
      printf("\n-------------------------------------------------------------------\n");
      printf("DATA LINK LAYER: \nframe From MAC:");
      int i = 0;
      for (i = 6; i < 12; ++i)
      {
        printf("%x", recieved_frame[i]);
        if (i != 11)
          printf("-");
      }
      printf("\nData Link Layer CRC check passed!!!\n");
      printf("Data Link Layer Protocol is : PPP\n");
      printf("frameSize is %d\n", frameSize);
      printf("-------------------------------------------------------------------\n\n");

      printf("-------------------------------------------------------------------\n");
      printf("NETWORK LAYER: \nFrom IP:");
      for (i = 30; i < 34; ++i)
      {
        printf("%d", recieved_frame[i]);
        if (i != 33)
          printf(".");
      }
      printf("\n");
      //print out data.
      printf("The decoded data(packet) is: \n");
      for (i = 14; i < frameSize - 4; ++i)
      {
        printf("%x ", recieved_frame[i]);
      }
      printf("\n");
      printf("Network Layer CRC check passed!!!\n");
      printf("Network Layer Protocol is : IPv4\n");
      printf("packet size is %d\n", frameSize - 18);
      printf("-------------------------------------------------------------------\n");

      // 合并分片into 一个完整的包Combination of packets
      // memcpy(recieved_frame[0], revieved_datagram[34], frameSize);

      printf("The datagram is: \n");
      for (i = 34; i < frameSize - 4; ++i)
      {
        printf("%x ", recieved_frame[i]);
      }
      printf("\n");

      if (counter == 0)
      {
        printf("The usermessage is: \n");
        for (i = 42; i < frameSize - 4; ++i)
        {
          printf("%c", recieved_frame[i]);
        }
        printf("\n");
      }
      else
      {
        printf("The usermessage is: \n");
        for (i = 34; i < frameSize - 4; ++i)
        {
          printf("%c", recieved_frame[i]);
        }
        printf("\n");
      }
      counter++;
    }
  }

  //close Socket
  closesocket(sock_fd);
  WSACleanup();
}
