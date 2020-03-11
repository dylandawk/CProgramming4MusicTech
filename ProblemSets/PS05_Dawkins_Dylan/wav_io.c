#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define HEADER_SIZE 44

typedef struct {
    char head[HEADER_SIZE];
    //sound data
} Wav;

int main(int argc, char *argv[])
{
    int maxAmplitude;
	char *ifile, *ofile;
	FILE *ifp, *ofp;

	//Usage Error
	if(argc < 4){
        printf("ERROR: \n%s\nmax amplitude -- the maximum absolute level in ofile on the scale of 0 to 32767\ninputFile.wav\n outputFile.wav\n", argv[0]);
        return -1;
    } else { //assign values
        maxAmplitude = atoi(argv[1]);
        ifile = argv[2];
        ofile = argv[3];
    }


    //open input (read) file
    ifp = fopen(ifile, "r");
 	if (ifp == NULL)
    {
            printf("ERROR: cannot open %s\n", ifile);
            return -1;
    } 
    
    //open output (write) file
	ofp = fopen(ofile, "w");
	if (ofp == NULL)
    {
            printf("ERROR: cannot open %s\n", ofile);
            return -1;
    }	
    
    //read input file header into an array
    unsigned char header[HEADER_SIZE];
    if ( fread(header, sizeof(header), 1, ifp) == 0 ) {
        printf("ERROR: problem reading header file");
        return -1;
    }

    //print each byte in header
    for(int i = 0; i < HEADER_SIZE; i++){ 
        char c = isprint(header[i]) ? header[i] :  ' ';
        printf("%c %02x ", c, header[i]);
        if(i % 11 == 10) printf("\n");
    }

    //initialize and print values
    long numChannels = header[23]<<8 | header[22];
    printf("Number of Channels: %ld\n", numChannels);
    long sampleRate = header[27]<<24 | header[26]<<16 | header[25]<<8 | header[24];
    printf("Sample Rate: %ld\n", sampleRate);
    long bitsPerSample = header[35]<<8 | header[34];
    printf("Bits Per Sample: %ld\n", bitsPerSample);
    long dataSize = header[43]<<24 | header[42]<<16 | header[41]<<8 | header[40];
    printf("Size of data: %ld\n", dataSize);
    long numSamples = (numChannels * dataSize * 8) / bitsPerSample;
    printf("Number of Samples: %ld\n", numSamples);


    //allocate memory
    short *x;
    x = (short *)malloc(numSamples * sizeof(*x)); 
    if ( x == NULL) {
          printf("ERROR: Could not allocate pointer memory");
    }
    //read values into x array
    long count = fread(x, sizeof(*x), numSamples, ifp);
    if ( count != numSamples ) {
         printf("ERROR: Sample number calculated did not compare with file");
    }

    //calculate max value
    int max_x = 0;
    for (int i=0; i<numSamples; i++) {
        if (abs(x[i]) > max_x) { 
            max_x = abs(x[i]);
        } 
    }
    printf("Max Value = %d\n", max_x);

    //modify sample values to specified Max Amplitude
    for(int i = 0; i < numSamples; i++){
        x[i] = (short)(x[i] * ((float)maxAmplitude/(float)max_x) + 0.5);
    }
    //print new max value (Testing)
    int new_max_x = 0;
    for (int i=0; i<numSamples; i++) {
        if (abs(x[i]) > new_max_x) { 
            new_max_x = abs(x[i]);
        } 
    }
    printf("New Max Value = %d\n", new_max_x);
    

    //print header and PCM data to new file
    for(int i = 0; i < HEADER_SIZE; i++){
        fwrite(&header[i], 1, sizeof(header[0]), ofp);
    }
    for(int i = 0; i< numSamples; i++){
        fwrite(&x[i], 1, sizeof(x[0]), ofp);
    }   
    

    
    fclose(ifp);
    fclose(ofp);

    return (0);
}
