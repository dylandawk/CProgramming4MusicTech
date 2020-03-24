/*****************************************************************************
 * play_wavfiles.c
 *
 * Plays one of a set of WAV files to speaker using PortAudio
 * Selection is via keyboard input using Ncurses
 * Wav file access uses Sndfile
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h> 	/* malloc() */
#include <unistd.h>     /* sleep() */
#include <stdbool.h>	/* true, false */
#include <string.h>		/* memset() */
#include <ctype.h>		/* lolower() */
#include <math.h>		/* sin() */
#include <sndfile.h>	/* libsndfile */
#include <portaudio.h>	/* portaudio */
#include <ncurses.h> 	/* This library is for getting input without hitting return */
#include <stdatomic.h>  /* permits write/read of "slection" to be atomic */
#include "paUtils.h"

#define MAX_PATH_LEN        256
#define MAX_IFILES		      8
#define MAX_CHN	            2
#define FRAMES_PER_BUFFER   512 /* must be divisible by 2 -- see fade-out window code */

  

/* data structure to pass to callback */
typedef struct {
    /* libsndfile data structures */
    SNDFILE *sndfile[MAX_IFILES]; 
    SF_INFO sfinfo[MAX_IFILES];
    unsigned int num_chan;
    atomic_int selection;
} Buf;

/* PortAudio callback function protoype */
static int paCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData );

int main(int argc, char *argv[])
{
  	char ifilename[MAX_IFILES][MAX_PATH_LEN], ch;
  	int i, selection;
  	unsigned int num_input_files, samp_rate, num_chan;
  	FILE *fp;
  	/* my data structure */
  	Buf iBuf, *p = &iBuf;
  	/* PortAudio stream */
    PaStream *stream;

  	/* zero libsndfile structures */
  	for (i=0; i<MAX_IFILES; i++) {
    		memset(&p->sfinfo[i], 0, sizeof(p->sfinfo[i]));
  	}

  	/* 
  	 * Parse command line and open all files 
  	 */
    //Your code here

    // Usage
    if(argc != 2){
        printf("ERROR %s inputFile.txt (where inputFile.txt contails list of playable WAV audio files) \n", argv[0]);
        return -1;
    } 

  	/* open list of files */
    //Your code here
    fp = fopen(argv[1], "r");
	  if (fp == NULL)
    {
            printf("ERROR: cannot open %s\n", argv[1]);
            return -1;
    }

    /* read WAV filenames from the list in ifile_list.txt */
    /* print information about each WAV file */
    //Your code here
    num_input_files = 0;
    while((fscanf(fp, "%s", ifilename[num_input_files]) != EOF)){
      p -> sndfile[num_input_files] = sf_open(ifilename[num_input_files], SFM_READ, &p -> sfinfo[num_input_files]);
      num_input_files ++;
    }

    printf("Number of input files: %d\n", num_input_files);



   	/* check for compatibility of input WAV files
     * If sample rates don't match, print error and exit 
     * If number of channels don't match or too many channels, print error and exit
     * if too many channels, print error and exit
     */
    //Your code here
    for(int i = 0; i < num_input_files; i++){
      //compare sample rates and channels
      if(i != num_input_files -1){
        if(iBuf.sfinfo[i].samplerate != iBuf.sfinfo[i+1].samplerate){
          printf("ERROR: Sample rates of file %d and %d do not match\n", i, i+1);
          printf("Sample rate of file %d = %d\n", i+1, iBuf.sfinfo[i+1].samplerate);
          return -1;
        } else if(iBuf.sfinfo[i].channels != iBuf.sfinfo[i+1].channels){
          printf("ERROR: Channels of file %d and %d do not match\n", i, i+1);
          return -1;
        }
      } 
      //check wav format
      if(iBuf.sfinfo[i].format != SF_FORMAT_WAV){
        printf("ERROR: file %d is not a WAV file. File Format = %d, WAV Format = %d\n", i, iBuf.sfinfo[i].format, SF_FORMAT_WAV);
        return -1;
      }
      //check # of channels
      if(iBuf.sfinfo[i].channels > 2){
        printf("ERROR: file %d have too many channels\n", i);
        return -1;
      }
      printf("Files checked successfully\n");
  
    }

  	/* initialize data structure 
     * set number of channels 
     * and set selection to -1 so initially only "zeros" are played
     */
    
    /* pause so user can read console printout */
    sleep(2);

    /* start up Port Audio */
    stream = startupPa(2, p->sfinfo[0].channels, 
      samp_rate, FRAMES_PER_BUFFER, paCallback, &iBuf);

  	/* Initialize ncurses 
     * to permit interactive character input 
     */
  	initscr(); /* Start curses mode */
  	cbreak();  /* Line buffering disabled */
  	noecho(); /* Uncomment this if you don't want to echo characters when typing */  

    printw("Select input file by number:\n");
    //Your code here
    //moveprintw

  	/* shut down Ncurses */
  	endwin();

    /* shut down Port Audio */
    shutdownPa(stream);
     
  	/* Close all WAV files */
  	for (i=0; i<num_input_files; i++) {
    		sf_close (p->sndfile[i]);
  	}

    return 0;
}


/* This routine will be called by the PortAudio engine when audio is needed.
 * It will be called in the "real-time" thread, so don't do anything
 * in the routine that requires significant time or resources.
 */
static int paCallback(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    Buf *p = (Buf *)userData; /* Cast data passed through stream to our structure. */
    float *output = (float *)outputBuffer;
    //float *input = (float *)inputBuffer; /* input not used in this code */
    unsigned int num_samples = framesPerBuffer * p->num_chan; /* number or samples in buffer */
    unsigned int selection, i, count;
 
    /* if selection is -1, then fill output buffer with zeros
     * otherwise, read from selected WAV file into output buffer
     * if not enough samples (i.e. count < framesPerBuffer)
     * then rewind and read remaining samples
     */
    //Your code here

    return 0;
}
