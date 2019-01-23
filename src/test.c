
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

int delta = 100;
int adj_rate = 150;
int cutoff = 10;
int depth = 2;
int margins[4] = {10, WIDTH-10, 10, HEIGHT-10}; //left, right, top and bottom margine (currently all 10 px)

static char *input;
char cam_id = '0';
const int buffer_size = 150;
static int prefluff = 25;
static int postfluff = 25;

void switchEle(int *list, int item1, int item2);
void print1dArray(int *list, int dim);
struct image *revertFrames(struct image *frm, int N);

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

    while (1) {
        //printf("frame %i ################################################\n", frm->index);

        if (found == 0)
            analyseFrame(frm);

        if ( (frm->index % adj_rate) == 0 )
            adjustSensitivity(frm, 10, 0);

        if ( endOfMeteor(frm, depth) && !found ) {
	    lifetime = endOfMeteor(frm, depth);
            found = 1;
        }

        if (found)
            n++;

        if (n > postfluff) {
            printf("saving video, lifetime = %i\n", lifetime);
            //write_video(frm, lifetime);
            n = 0;
            found = 0;
	    lifetime = 0;
        }

        printImage(frm);
        frm = frm->next;
    }

    return 0;
}


int main(int argc,char* argv[]){
    input = argv[1];
    frm = buildBuffer(buffer_size); // generate cyclicalc buffer of size "buffer_size" frames
    read_video(input, frm); // invoke the read_video() function in order to fill generated buffer with frames from "input"

    mainloop(); // start the main loop

    if(frm->index == 150) {
        freeBuffer(frm);
    }
    return 0;
}

