/*
Author: Gregory Maglio
Date: 02/2011
Desc: Listen for id3 tag information via UDP Multicast.
*/


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../headers/id3shm.h"


int main(void) {

      char buffer[BUFF_SIZE];
      struct sockaddr_in to, buffer_sin;
      struct ip_mreq multiaddr;
      socklen_t addr_len;
      ssize_t numbytes;
      const int TTL = 64;
      int m;

      int sock; 
      if( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
             perror("socket");
             exit(1);
      }
      
      // setup UDP port 2007 and set address struct for any interface
      to.sin_family = AF_INET;
      to.sin_addr.s_addr = htonl(INADDR_ANY);
      to.sin_port = htons(2007);

      // bind
      if(bind(sock, (struct sockaddr *)&to, sizeof(to)) < 0) {
               perror("bind");
               exit(1);
       }

      // Join a multicast group 224.1.1.1 on all interfaces
      multiaddr.imr_multiaddr.s_addr = inet_addr("224.1.1.1");
      multiaddr.imr_interface.s_addr = htonl(INADDR_ANY);
      setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multiaddr, sizeof(multiaddr));
                     

      addr_len = sizeof(buffer_sin);
      //read here
      while (1) {
            memset(buffer, '\0', sizeof(buffer));
            if((numbytes = recvfrom(sock, buffer, BUFF_SIZE-1, 0, (struct sockaddr *)&buffer_sin, &addr_len)) < 0) {
                
                    perror("recvfrom");
                    exit(1);
            }
            printf("%s\n", buffer); 
      }


      // tell router we are no longer interested in this group
      close(sock);
      return 0;
}
