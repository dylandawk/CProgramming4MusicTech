/* Program for a sinewave generation synthesizer
 * which can play simultaneous tones
 * 
 * Demonstrates use of NCurses and PortAudio (also libsnd for debugging file) 
 */

#define DB_WAV_OUT      0   /* write output to wav file for debugging */

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
#include "synth.h"      /* #defines and declare structures used in synth.c */
#include "support.h"    /* function prototype for support.c */
#include "freq.h"       /* note frequences for freq[] */
#include "paUtils.h"    /* portaudio function prototypes */

/* PortAudio callback function protoype */
static int paCallback( 
    const void *inputBuffer, 
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData );


int main(int argc, char *argv[])
{
    char ch;
    int key2index[128];
    int samp_rate, num_chan;
	/* portaudio callback data structure */
	Buf buf, *pb = &buf;
    Tone *pt = &buf.tone;
	/* PortAudio data structures */
    PaStream *stream;
#if (DB_WAV_OUT)
    /* libsndfile data structures */
    SF_INFO sfinfo;
	/* zero libsndfile structures */
	memset(&sfinfo, 0, sizeof(sfinfo));
#endif

    /* initialize variables */
    samp_rate = SAMP_RATE;
    num_chan = NUM_CHAN;

    /* initialize key table */
    init_key_index(key2index);

    /* initialize portaudio callback data structure */
    //Your code here
    pt->key = -1;
    pt->phase_inc = -1;
    pt->phase = 0.0;
    pt->attack_factor = ATTACK_FACTOR;
    pt->decay_factor = DECAY_FACTOR;
    pt->attack_amp = 0.0;   
    pt->decay_amp = 1.0;

    /* start up Port Audio */
    stream = startupPa(num_chan,num_chan, samp_rate, FRAMES_PER_BUFFER, paCallback, pb);

#if (DB_WAV_OUT)
        /* open debug output file */
        memset(&sfinfo, 0, sizeof(sfinfo));
        sfinfo.samplerate = samp_rate;
    	sfinfo.channels = num_chan;
    	sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
        if ( (pb->sndfile = sf_open ("test_file.wav", SFM_WRITE, &sfinfo)) == NULL ) {
            fprintf (stderr, "Error: could not open test wav file\n");
            return -1;
        }
#endif

	/* Initialize ncurses
     * to permit interactive character input
     *
     * ncurses loop waits for key press
     * If valid key, then initialize Tone structure
     * and refresh ncurses display buffer
     */
    //Your code here
    initscr();
    cbreak();
    noecho();

    printw("Welcome to my Synthesizer!\n"); // line 0
    printw("Keyboard to piano key mapping is:\n"); // line 1
    printw("qwertyQWERTY -> C3 to B3\n");// line 2
    printw("asdfghASDFGH -> C4 to B4\n");// line 3
    printw("zxcvbnZXCVBN -> C5 to B5\n");// line 4
    printw("SpaceBar to quit\n");// line 5
    printw("\n");// line 6
    printw("Key: ");// line 7
    refresh();

    ch = 0;
    while (ch != ' ') {
        printf("1\n");
        ch =getch();
        if(key2index[(int)ch] != -1){
            int freq_index = key2index[(int)ch];
            double f0 = freq[freq_index];
            pt->phase_inc = 2*PI*f0/samp_rate;
            pt->phase = 0.0;
            pt->attack_amp = 0.0;
            pt->decay_amp = 1.0;
            mvprintw(7,0, "Key %3d,     New key: ", freq_index);
        } else {
            
            mvprintw(7,0, "Invalid key. New key: ");
        }
        refresh();
    }
    endwin();



#if (DB_WAV_OUT)
    /* close debugging output wav file */
    sf_close (pb->sndfile);
#endif

    /* shut down Port Audio */
    //Your code here
    shutdownPa(stream);
    return 0;
}



/* This routine will be called by the PortAudio engine when audio is needed.
 * It will be called in the "real-time" thread, so don't do anything
 * in the routine that requires significant time or resources.
 */
static int paCallback(
    const void *inputBuffer, 
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    //Your code here
    Buf *pb = (Buf *)userData;
    Tone *pt = &pb->tone;
    float *output = (float *)outputBuffer;
    double v = 0.0;

    // loop over outputframes
    
    for(int i = 0; i< framesPerBuffer; i ++){
        v = 0.0;
        if(pt->phase_inc != -1){
            v += FS_AMPL * sin(pt->phase) * (1-pt->attack_amp) * (pt->decay_amp);
            pt->phase += pt->phase_inc;
            pt->decay_amp *= pt->decay_factor;
            pt->attack_amp *= pt->attack_factor;
        }

        for(int j = 0; j < pb->num_chan; j++ ){
           //loop over channels in frame and write v to channel 
               output[i*pb->num_chan+j] = v;
        }
        if(v < DROP_LEVEL) pt->phase_inc = -1;

    }


    return 0;
}
