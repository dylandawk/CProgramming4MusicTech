#ifndef _SYNTH_H_
#define _SYNTH_H_

#define NUM_KEYS		    12  /* for 12 semitones in an octave */
#define NUM_OCTAVES         3   /* three octaves */
#define SAMP_RATE           48000
#define NUM_CHAN	        2
#define FRAMES_PER_BUFFER   2048
#define FS_AMPL             0.5 /* -6 dB FS */
//#define ATTACK_FACTOR       0.99980 /* attack time constant of 100 ms */
//#define ATTACK_FACTOR       0.99960 /* attack time constant of 50 ms */
#define ATTACK_FACTOR       0.99800 /* attack time constant of 10 ms */
#define DECAY_FACTOR        0.99998 /* decay time constant of 1.0 sec */
#define DROP_LEVEL          0.01  /* -40 dBFS */

#define PI                  3.14159265358979323846

/* for tone playing */
typedef struct {
    int key; /* index into freq array */   
    double phase_inc; /* phase increment per sample to realize freq */
    double phase; /* save phase value for next sample */
    double attack_factor;
    double decay_factor;
    double attack_amp; /* save attack amplitude for next sample */
    double decay_amp; /* save decay amplitude for next sample */
} Tone;

/* data structure to pass to callback */
typedef struct {
    int num_chan;
    Tone tone;
#if (DB_WAV_OUT)
    SNDFILE *sndfile; //for debug file
#endif
} Buf;

#endif 