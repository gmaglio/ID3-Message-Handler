/* 
Gregory Maglio
Feburay 2011
Definition of ID3 structure and message
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

#define EXTENSION ".mp3"
#define PATH "/home/gmaglio"
#define ID3_LENGTH 128 // Length of ID3 Tags
#define FN_LENGTH 1024 // Length of filename
#define BUFF_SIZE 8192 // Length of filename
#define SHMKEY 0x1702
#define SHMPERM ( SHM_R|SHM_W|SHM_R>>3|SHM_W>>3|SHM_R>>6|SHM_W>>6 )
#define MAX_PATH 1024 // used by dirwalk()


#define TITLE 3 // Offset for Title
#define TITLELENGTH 29 // Length of Title
#define getTitle( field, tag ) getField( field, tag, TITLE, TITLE + TITLELENGTH )

#define ALBUM 33 // Offset for Album
#define ALBUMLENGTH 20 // Length of Album
#define getAlbum( field, tag ) getField( field, tag, ALBUM, ALBUM + ALBUMLENGTH )


typedef struct id3String {

     char id3String[ID3_LENGTH];

} id3String;


// derived from struct id3String
typedef struct id3Buff {
 
     id3String buf[BUFF_SIZE];
     int bufSize;

} id3Buff;


// returns 1 if not mp3
// used by getFileList
int isMp3( char *filename ); 

// id3_file
void getFileList(char *d_name, GList **filelist);
void dirwalk(char *, GList **, void (*fcn)(char *, GList **));
int getTags( GList *filelist, id3String *tagBuff );

// id3_data
void printTags( id3Buff tagBuff );
id3String *copyTags( id3String *dest, id3String *src, const int size );
id3Buff *copyBuff( id3Buff *dest, id3Buff src );
id3String *getField( id3String *field, id3String tag, const unsigned int start, const unsigned int end );
