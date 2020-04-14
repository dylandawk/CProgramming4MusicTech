
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include "main.h"
#include "equalizer.h"


int main(int argc, char *argv[])
{
    float input[FRAMES_PER_BUFFER * MAX_CHAN];
    float output[FRAMES_PER_BUFFER * MAX_CHAN];
    char *ifile, *ofile;
    unsigned int count;
    int num_chan, num_frames;

    Equalizer eq, *pe = &eq;
    SNDFILE *isndfile, *osndfile;
    SF_INFO isfinfo, osfinfo;

    // Usage
    if(argc != 4){
        fprintf(stderr, "Usage: %s F|L|B|H input_file.wav output_file.wav \n Where mode F is flat, L is low-pas, B is band-pass, and H is high-pass \n", argv[0]);
        return -1;
    }

    char mode = argv[1][0];

    switch(mode){
        case 'F':
            break;
        case 'L':
            for(int i = 7; i < MAX_BAND; i++) pe->set_gain_dB(i, -80.0);
            break;
        case 'B':
            for(int i = 3; i < 7; i++) pe->set_gain_dB(i, -80.0);
            break;
        case 'H':
            for(int i = 0; i < 3; i++) pe->set_gain_dB(i, -80.0);
        default:
            fprintf(stderr, "Invalid mode: Please input F|L|B|H\n");
            return -1;

    }

    ifile = argv[2];
    ofile = argv[3];

    // set libsndfiles structure to zero
    memset(&isndfile, 0, sizeof( isndfile ));
    memset(&osndfile, 0, sizeof( osndfile ));
    memset(&isfinfo, 0, sizeof( isfinfo ));
    memset(&osfinfo, 0, sizeof( osfinfo ));

    //open input SNDFILE
    isndfile = sf_open(ifile, SFM_READ, &isfinfo);
    printf("Input audio file %s: \n", ifile);
    printf("Frames = %lld Channels = %d SampleRate = %d\n", isfinfo.frames, isfinfo.channels, isfinfo.samplerate);

    // set output wav file params to input wav file params
    osfinfo.frames = isfinfo.frames;
    osfinfo.samplerate = isfinfo.samplerate;
    osfinfo.channels = isfinfo.channels;
    osfinfo.format = isfinfo.format;
    osfinfo.sections = isfinfo.sections;
    osfinfo.seekable  = isfinfo.seekable;


    num_chan = isfinfo.channels;
    num_frames = FRAMES_PER_BUFFER;

    
    // open output wav file
    osndfile = sf_open(ofile, SFM_WRITE, &osfinfo);

    // declare single-channel buffers
    float icbuf[MAX_CHAN][FRAMES_PER_BUFFER];
    float ocbuf[MAX_CHAN][FRAMES_PER_BUFFER];

    //loop over input file
    while((count = sf_readf_float(isndfile, input, FRAMES_PER_BUFFER)) == FRAMES_PER_BUFFER){
        // de-interleave
        for(int i = 0; i < num_frames; i++){
            for(int chan = 0; chan < num_chan; chan++){
                icbuf[chan][i] = input[i*num_chan + chan];
            }
        }
        // filter
        for(int chan = 0; chan < num_chan; chan++ ){
            pe->filter(&icbuf[chan][0], &ocbuf[chan][0], num_frames, chan);
        }
        // re-interleave
        for(int i = 0; i < num_frames; i++){
            for(int chan = 0; chan < num_chan; chan++){
                output[num_chan * i + chan] = ocbuf[chan][i];
            }
        }
        
        sf_writef_float(osndfile, output, FRAMES_PER_BUFFER);
    }

    sf_close(osndfile);
    sf_close(isndfile);

}





