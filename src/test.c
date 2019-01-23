
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define HEIGHT 480
#define WIDTH 720
#define LENGTH HEIGHT*WIDTH

#define INT 32
#define MAXPIX 1024
#define SIZE (MAXPIX/INT)
#define MAXMET 100

//int limit = 50;
int delta = 50;
int cutoff = 10;
int depth = 2;
int margins[4] = {10, WIDTH-10, 10, HEIGHT-10}; //left, right, top and bottom margine (currently all 10 px)

static char *input;
char cam_id = '0';
const int buffer_size = 150;
//static int prefluff = 25;
static int postfluff = 25;

void switchEle(int *list, int item1, int item2);
void print1dArray(int *list, int dim);

#include "met.h"
#include "bits.h"
#include "pixel.h"
#include "array.h"
#include "graph.h"
#include "analysis.h"
#include "io.h"
#include "ctrl.h"

static struct image *frm = NULL;

// main loop works similar to actual 
int mainloop(void) {
    int n=0;
    int found=0;
    int lifetime;

    struct timespec systime, reftime;

    while (frm->index < (buffer_size - 1)) {
        //printf("frame %i ################################################\n", frm->index);

        clock_gettime(CLOCK_REALTIME, &reftime);
        analyseFrame(frm);

        //if ( frm->index == 1 )
            //adjustSensitivity(frm, buffer_size, 30); 

        //clock_gettime(CLOCK_REALTIME, &systime); printf("analyse Frame %f sec\n", (float) ((systime.tv_nsec - reftime.tv_nsec)/1000) / 1000000);

        if ( endOfMeteor(frm, depth) != -1 ) {
            found = lifetime;
            printData(frm->prev->prev);
            exit(0);
        }

        /*
        */

        if (found > 0)
            n++;
        else
            n = 0;

        if (n > postfluff) {
            //write_video(frm, (prefluff + found + postfluff));
            n = 0;
            found = 0;
            //sleep(5);
        }

        //printImage(frm);

        frm = frm->next;

        //if (frm->index == buffer_size)
            //return 1;
    }
    printData(frm->prev->prev);
    return 0;
}


int main(int argc,char* argv[]){
    input = argv[1];
    buffer_size = argv[2];
    frm = buildBuffer(buffer_size); // generate cyclicalc buffer of size "buffer_size" frames
    read_video(input, frm); // invoke the read_video() function in order to fill generated buffer with frames from "input"

    mainloop(); // start the main loop

    if(frm->index == 150) {
        freeBuffer(frm);
    }
    return 0;
}

