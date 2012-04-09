/*
Author: Gregory Maglio
Date: 02/2011
Desc: module to manage file i/o to the end of populating the buffer with Payload.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dirent.h>
#include <glib.h>
#include <glib/glist.h>
#include "../headers/id3shm.h"

int isMp3(char *filename) {

    char *extension = NULL;
    extension = strrchr(filename,'.');

    if( extension != NULL ) {  
        if( strcasecmp(extension, ".mp3") == 0 ) 
           return 0;
    }
    return 1;  

}

/* dirwalk: apply fcn to all files in dir */
void dirwalk(char *dir, GList **filelist, void (*fcn)(char *, GList **)) {

    char d_name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;
    char *fullpath = "\0";

    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }

    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;
                      /* skip self and parent */

        if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(d_name))
            fprintf(stderr, "dirwalk: d_name %s %s too long\n", dir, dp->d_name);
        else {
            sprintf(d_name, "%s/%s", dir, dp->d_name);
            (*fcn)(d_name, filelist);
        }
    }
    closedir(dfd);
}

void getFileList(char *d_name, GList **filelist) {

    struct stat stbuf;
    char *fullpath = "\0";

    if (stat(d_name, &stbuf) == -1) {
        fprintf(stderr, "getFileList: can't access %s\n", d_name);
        // return;
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(d_name, filelist, getFileList);
    if( isMp3(d_name) == 0 ) { 
        fullpath = calloc( FN_LENGTH, sizeof(char) );
        strcpy(fullpath, d_name);
        *filelist = g_list_prepend( *filelist, fullpath );
    }
}

int getTags( GList *filelist, id3String *tagBuff ) {

   FILE *fp;
   const int offset = -ID3_LENGTH;
   int i = 0;
 
   while(filelist != NULL) {
       if ( (fp = fopen(filelist->data,"r")) == NULL) {
          perror("Error: getTags(): file not opened");
          return -1;
       }
       fseek( fp, offset, SEEK_END );
       fgets( tagBuff[i].id3String, ID3_LENGTH, fp );
       i++;
       fclose(fp);
       filelist = g_list_next(filelist);

   }
   return i; 

}
