/*
Author: Gregory Maglio
Date: 02/2011
Desc: Send id3 tag information through UDP Multicast.
*/



#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../headers/id3shm.h"


int main(void) {

    id3Buff *shmBuf; // id3shm
    const int TTL = 64;
    struct sockaddr_in to;
    struct ip_mreq multiaddr;
    int shmid = 0; // id3shm
    int m = 0;
    int j = 0;
    int len = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
   
 
    if( (shmid = shmget( SHMKEY, sizeof(id3Buff), SHMPERM )) < 0 ) { // id3shm
        perror("Failed to create Shared Memory Segment");           // id3shm
 	exit(EXIT_FAILURE);
    }                                                                // id3shm
    shmBuf = shmat( shmid, 0, 0);                                    //id3shm

    // Join a multicast group 224.1.1.1 on all interfaces
    multiaddr.imr_multiaddr.s_addr = inet_addr("224.1.1.1");
    multiaddr.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multiaddr, sizeof(multiaddr));
                     

    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &TTL, sizeof(TTL));
                     

    // setup the destination address of 224.1.1.1 and UDP port 2007
    to.sin_family = AF_INET;
    to.sin_addr.s_addr = inet_addr("224.1.1.1");
    to.sin_port = htons(2007);

    // write some data
    for (j = 3; j < shmBuf->bufSize; j++) {
        len = strlen(shmBuf->buf[j].id3String) + 1;
                /* +1 for terminating null byte */

        if (len + 1 > BUFF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }

        if(sendto(sock, shmBuf->buf[j].id3String, len, 0, (const struct sockaddr*)&to,sizeof(to)) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

    }
             

      // tell router we are no longer interested in this group
      close(sock);
      return 0;
}

