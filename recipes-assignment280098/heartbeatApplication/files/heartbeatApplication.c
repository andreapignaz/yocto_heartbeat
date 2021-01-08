#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define q	11		    /* for 2^11 points */
#define N	(1<<q)		/* N-point FFT, iFFT */
#define q	11		    /* for 2^11 points */
#define N	(1<<q)		/* N-point FFT, iFFT */
#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif
typedef float real;
typedef struct{real Re; real Im;} complex;

//GLOBAL VARIABLES

int fd = -1;
int fft_values[2048];

//THREAD FOR FETCHING HEARTBEAT VALUE

void *heartbeatFetch(void *arg) {
	char read_value[5];
	int value_int;

	read( fd, read_value, 5 );
	read_value[5] = '\0';
	sscanf(read_value, "%d", &value_int);
	//critical section
	fft_values[*(int *)arg] = value_int;
	//end of critical section
	printf("Read value %d. Thread closes.",read_value);
	pthread_exit(NULL);
}

//FOURIER TRANSFORM FUNCTION

void fft( complex *v, int n, complex *tmp )
{
  if(n>1) {			/* otherwise, do nothing and return */
    int k,m;    complex z, w, *vo, *ve;
    ve = tmp; vo = tmp+n/2;
    for(k=0; k<n/2; k++) {
      ve[k] = v[2*k];
      vo[k] = v[2*k+1];
    }
    fft( ve, n/2, v );		/* FFT on even-indexed elements of v[] */
    fft( vo, n/2, v );		/* FFT on odd-indexed elements of v[] */
    for(m=0; m<n/2; m++) {
      w.Re = cos(2*PI*m/(double)n);
      w.Im = -sin(2*PI*m/(double)n);
      z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	/* Re(w*vo[m]) */
      z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	/* Im(w*vo[m]) */
      v[  m  ].Re = ve[m].Re + z.Re;
      v[  m  ].Im = ve[m].Im + z.Im;
      v[m+n/2].Re = ve[m].Re - z.Re;
      v[m+n/2].Im = ve[m].Im - z.Im;
    }
  }
  return;
}

//MAIN BODY

int main(int argc, char **argv)
{
	complex v[N], scratch[N];
	float abs[N];
	int ret;
	pthread_t myThread;
	char *app_name = argv[0];
	char *dev_name = "/dev/mymod";
	int i = 0;
	int m, minIdx, maxIdx;
	
	char read_value[5];
	int value_int;
	
	//Open device
	if ((fd = open(dev_name, O_RDWR)) < 0)
	{
		fprintf(stderr, "%s: unable to open %s: %s\n", app_name, dev_name, strerror(errno));
		return( 1 );
	}
	
	//Fetch values, compute FFT, compute PSD, print result (infinite loop)
	while(1) {
		//Fetch N value from device
		for(i=0; i<N; i++) {
			//ret = pthread_create(&myThread, NULL, heartbeatFetch, &i);
			read( fd, read_value, 5 );
		read_value[5] = '\0';
		sscanf(read_value, "%d", &value_int);
		//critical section
		fft_values[i] = value_int;
		//end of critical section
		printf("Read value %d. Thread closes.",value_int);
			if (ret != 0) {
				printf("Can't create thread (%s)\n", strerror( errno));
				return -1;
			}
			//printf("Created thread %d\n", i);
			usleep(1000000/50);  //microseconds for 50Hz
		}
		//Initialize complex array and do FFT computation
		//Critical section
		for(i=0; i<N; i++) {
			v[i].Re = fft_values[i];
			v[i].Im = 0;
		}
		//End of critical section
		fft(v, N, scratch);
		//Do PSD computation
		for(i=0; i<N; i++) 
			abs[i] = (50.0/2048)*((v[i].Re*v[i].Re)+(v[i].Im*v[i].Im)); 
		minIdx = (0.5*2048)/50;   // position in the PSD of the spectral line corresponding to 30 bpm
 		 maxIdx = 3*2048/50;       // position in the PSD of the spectral line corresponding to 180 bpm
 		 
 		 // Find the peak in the PSD from 30 bpm to 180 bpm
  		m = minIdx;
  		for(i=minIdx; i<(maxIdx); i++) {
    			if( abs[i] > abs[m] )
				m = i;
  		}
  		printf( "\n\n\n%d bpm\n\n\n", (m)*60*50/2048 );

	}
	close( fd );
	return 0;
}
