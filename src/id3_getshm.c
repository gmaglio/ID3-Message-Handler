/*
Author: Gregory Maglio
Date: 02/2011
Desc: retrieve data from shared buffer
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


void helper() {

    printf("id3_client <start offset> <end offset> <record index>\n");

}

int main(int argc, char *argv[]) {

   id3Buff *shmBuf;
   id3String title[TITLELENGTH];
   id3String album[ALBUMLENGTH];
   int recordCount = 0;
   int shmid = 0;
   int startOffset = 0;
   int endOffset = 0;
   int recordIndex = 0;
  
   if( (shmid = shmget( SHMKEY, sizeof(id3Buff), SHMPERM )) < 0 ) {
        perror("Failed to create Shared Memory Segment");
   }

   shmBuf = shmat( shmid, 0, 0);

   if(argc == 4) {
       startOffset = atoi(argv[1]);  
       endOffset = atoi(argv[2]);  
       recordIndex = atoi(argv[3]);  
   }
   else {
       printTags( *shmBuf );
       getTitle( title, shmBuf->buf[recordIndex] );
       printf("Title: %s\n", title->id3String);
       getAlbum( album, shmBuf->buf[recordIndex] );
       printf("Album: %s\n", album->id3String);
   }

   if( shmdt( shmBuf ) < 0 ) {
        perror("Failed to detach Shared Memory Segment");
   }
     
   return 0;

}
