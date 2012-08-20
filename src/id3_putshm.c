/*
Author: Gregory Maglio
Date: 02/2011
Desc: Retrieve id3 info from files withing specified directory tree.  Copy id3 info to shared buffer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include <glib.h>
#include "../headers/id3shm.h"


int main(int argc, char *argv[]) {

   id3Buff *shmBuff = NULL;
   id3Buff tagBuff;
   GList *filelist = NULL;
   char dirPath[FN_LENGTH] = "\0";
   int shmid = 0;
   int i = 0;

   if( argc < 2 ) {
        printf("Missing argument.\n"); 
        return -1;
   } 

   strcpy(dirPath,argv[1]);   // get argument from user
   getFileList(dirPath, &filelist); 
   tagBuff.bufSize = getTags(filelist, tagBuff.buf);
    
   printf("record count: %i\n", tagBuff.bufSize);
   if( tagBuff.bufSize > 1 ) { 
       if( (shmid = shmget( SHMKEY, sizeof(id3Buff), IPC_CREAT|IPC_EXCL|SHMPERM )) < 0 ) {
           perror("Failed to create Shared Memory Segment");
           return -1;
       }
       printf("shmget() is successful.\n");
   }
   else {
       printf("No records to process.\n");
       return tagBuff.bufSize;;
   }

   if( ( shmBuff = shmat( shmid, 0, 0)) < 0 ) {
        perror("Failed to attach to Shared Memory Segment");
   }
   copyBuff( shmBuff, tagBuff );
   if( shmdt( shmBuff ) < 0 ) {
        perror("Failed to detach Shared Memory Segment");
   }
   
   return 0;

}
