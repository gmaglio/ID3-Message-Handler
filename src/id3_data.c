
/*
Author: Gregory Maglio
Date: 02/2011
Desc: module to handle transversale of allocated buffer of type id3Buff
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

void printTags( id3Buff tagBuff ) {

   const int recordCount = tagBuff.bufSize;
   int i = 0;

   for( i = 0; i < recordCount;  i++) { 
        printf("%s\n", tagBuff.buf[i].id3String); 
   }
   printf("record count: %i\n", recordCount); 

}

id3String *copyTags( id3String *dest, id3String *src, const int size ) {

   int i = 0;

   for(i = 0; i < size; i++) {
        strncpy( dest[i].id3String, src[i].id3String, ID3_LENGTH );
   }
   return dest;

}

id3Buff *copyBuff( id3Buff *dest, id3Buff src ) {

   dest->bufSize = src.bufSize;
   copyTags(dest->buf, src.buf, src.bufSize);
   return dest;

}

id3String *getField( id3String *field, id3String tag, const unsigned int start, const unsigned int end ) {

   int i = 0;
   int j = 0;

   for( i = 0, j = start; j <= end; i++, j++ ) { field->id3String[i] = tag.id3String[j]; }
      
   return field; 

}
