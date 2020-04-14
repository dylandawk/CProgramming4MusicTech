#include <stdio.h>
#include <math.h>   //pow
#include "main.h"
#include "equalizer.h"
#include "coef.h"
#include "algorithm"

#define MY_DEBUG 0

static float octave_bands[] = {31.5, 63, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};

/* constructor */
Equalizer::Equalizer(void) {
    //Your code here
    num_bands = MAX_BAND;
    for(int i = 0; i < num_bands; i++){
        filt_gain[i].gain_dB = INIT_GAIN_DB; //inititalize gain decibels
        filt_gain[i].gain_lin = pow(10.0, filt_gain[i].gain_dB / 20.0); // initialize linear gain
    }
    //initialize band_center_freq[]
    int size  = sizeof(octave_bands) / sizeof(float);
    
    for(int i = 0; i < size; i ++){
        band_center_freq[i] = octave_bands[i];
    }
    // initialize pointer to filter coef
    pfc = &filt_coef[0];
    // zero out buffers
    std::fill(&ibuf_state[0][0], &ibuf_state[0][0] + sizeof(ibuf_state)/sizeof(float), 0);
    std::fill(&obuf_state[0][0][0], &obuf_state[0][0][0]+ sizeof(obuf_state) / sizeof(double), 0);
    std::fill(&obuf_band[0][0], &obuf_band[0][0] + sizeof(obuf_band)/sizeof(double), 0);

}

/* destructor */
Equalizer::~Equalizer() {;}

/* return number of bands in equalizer */
int Equalizer::get_num_bands(void) {
    //Your code here
    return num_bands;
}

/* this provides a short label for each band
 * which will be used in the VST plugin GUI
 */
void Equalizer::get_band_cf_str(int band, char *s)
{
    if (band_center_freq[band] < 1000) {
        sprintf(s, "%3.0f", band_center_freq[band]);
    }
    else {
        sprintf(s, "%2.0fk", band_center_freq[band]/1000);
    }
}

/* return current band gain setting, in dB */
float Equalizer::get_gain_dB(int band)
{
    //Your code here
    return filt_gain[band].gain_dB;
}

/* set band gain, as gain_dB and also gain_lin */
void Equalizer::set_gain_dB(int band, float gain_dB)
{
    //Your code here
    filt_gain[band].gain_dB = gain_dB;
    filt_gain[band].gain_lin = pow(10.0, filt_gain[band].gain_dB / 20.0);
}

/* filter all bands */
void Equalizer::filter(const float *ibuf, float *obuf, int num_frames, int chan)
{
#if (0)
    for (int i=0; i<num_frames; i++) 
        obuf[i] = ibuf[i];
#else
    /* filter each band */
    num_bands = get_num_bands();
    for (int band = 0; band < num_bands; band++){
        int num_coef = pfc[band].num_coef;
        filter_band(ibuf, &obuf_band[band][0], &pfc[band], &ibuf_state[chan][0], &obuf_state[chan][band][0], num_frames, num_coef);
    }

    /* compute output values */
    for(int i = 0; i < num_frames; i++){
        for (int band = 0; band < num_bands; band++){
            obuf[i] += filt_gain[band].gain_lin * (float)obuf_band[band][i];
        }
    }
    
    /* update input_state */
    for(int i = 0 ; i < MAX_COEF -1; i++){
        ibuf_state[chan][(MAX_COEF - 1) - i] = ibuf[(num_frames - 1) -i];
    }


#endif
}

/* apply one equalizer filter band to one channel of signal
 *
 * filters are designed in Matlab, and assume the following 
 * filtering equation (where Matlab indexes starting from 1!)
 *
 *  a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)
 *                        - a(2)*y(n-1) - ... - a(na+1)*y(n-na)
 *
 *  this code assumes that a(1) is always 1.0
 */                         
void Equalizer::filter_band(const float *ibuf, double *obuf_band, FiltCoef *pfc, 
    float *ibuf_state, double *obuf_state, int num_frames, int num_coef)
{
    //Your code here
    for(int n = 0; n < num_frames; n++){
        //output value
        double y = 0;

        // perform first convolution sum (b)
        for(int k = 0; k < num_coef; k++){
            if((n-k) < 0){
                y += ibuf_state[MAX_COEF+(n-k)] * pfc->b[k];
            } else {
                y += ibuf[n-k] * pfc->b[k];
            }
        }

        // perform second convolution sum (a)
        for(int k = 1; k < num_coef; k++){
            if((n-k) < 0){
                y -= obuf_state[MAX_COEF + (n-k)] * pfc->a[k];
            } else {
                y -= obuf_band[n-k] * pfc->a[k];
            }
        }

        obuf_band[n] = y;
    }
    // copy
    for(int i = 0; i < MAX_COEF-1; i++){
        obuf_state[(MAX_COEF -1) - i] = obuf_band[(num_frames - 1) - i];
    }
}

