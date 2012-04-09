/*
Author: Gregory Maglio
Date: 02/2011
Desc: Remove IPC element.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include <glib.h>
#include <glib/glist.h>
#include "../headers/id3shm.h"
   
int main() {
   
   int shmid = 0;

   if( (shmid = shmget( SHMKEY, ID3_LENGTH, SHMPERM )) < 0 ) {
        perror("Failed to create Shared Memory Segment");
   }
   if( shmctl( shmid, IPC_RMID, 0 ) < 0 ) {
        perror("Failed to remove shmid");
   }
   return 0;

}
