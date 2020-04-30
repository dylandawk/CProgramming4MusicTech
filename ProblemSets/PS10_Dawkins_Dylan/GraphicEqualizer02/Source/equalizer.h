#ifndef _EQUALIZER_H
#define _EQUALIZER_H

#define MAX_BAND		10
#define MAX_COEF		5
#define INIT_GAIN_DB	0
#define MIN_GAIN_DB		-80

/* constants */
typedef struct {
	int num_coef;
	double b[MAX_COEF];
	double a[MAX_COEF];	
} FiltCoef;

/* variables */
typedef struct {
	double gain_dB;
	double gain_lin;
} FiltGain;

class Equalizer {
public:
    Equalizer(); //constructor
    ~Equalizer(); //destructor
    /* member functions */
    int get_num_bands(void); 
    char* get_band_cf_str(int band, char *s);
    float get_gain_dB(int band);
    void set_gain_dB(int band, float gain_dB);
    void filter(const float *ibuf, float *obuf, int num_frames, int chan);
private:
	void filter_band(const float *ibuf, double *obuf_band, FiltCoef *pf, 
		float *ibuf_state, double *obuf_state, int num_frame, int num_coef);
	/* initialize in constructor */
	int num_bands;
	FiltGain filt_gain[MAX_BAND];
	float band_center_freq[MAX_BAND];
	FiltCoef *pfc; // access as pf[band].b[i] 
	/* array of State variables */
	float ibuf_state[MAX_CHAN][MAX_COEF];
	double obuf_state[MAX_CHAN][MAX_BAND][MAX_COEF]; 
    /* this is the filtered output for each band */
    double obuf_band[MAX_BAND][FRAMES_PER_BUFFER];
};

#endif
