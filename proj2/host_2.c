#include "applicationLayer.c"
#include "reciever.c"

int main()
{
  printf("This device's MAC address is 8C-16-45-40-0D-AC\n");
  printf("This device's IP address is 192.168.1.0\n");
  // printf("-------------------------------------------------------------------\n");
  // printf("Captain, this is Jiaxuan Cai!!! We are testing our sending stack!!!\n");
  // printf("-------------------------------------------------------------------\n\n");
  // printf("-------------------------------------------------------------------\n");
  // printf("Length of usermessage is %d ,content of user message is:\n", strlen(usermessage));
  // for (int i = 0; i < strlen(usermessage); i++)
  // {
  //   printf("%x ", usermessage[i]);
  // }
  // printf("\n");
  // printf("-------------------------------------------------------------------\n\n");
  unsigned char usermessage[] = " ";
  recieve();
}