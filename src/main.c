#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>

#define HEIGHT 480
#define WIDTH 720
#define LENGTH HEIGHT*WIDTH

#define INT 32
#define MAXPIX 1024
#define SIZE (MAXPIX/INT)
#define MAXMET 100

int delta = 50;
int adj_rate = 10;
int cutoff = 10;
int depth = 3;
int margins[4] = {10, WIDTH-10, 10, HEIGHT-10}; //left, right, top and bottom margine (currently all 10 px)

char *dev_name = "/dev/video0";
char cam_id = '0';
const int buffer_size = 150;
static int prefluff = 25;
static int postfluff = 25;

static int fd = -1;
static struct image *frm = NULL;

void switchEle(int *list, int item1, int item2);
void print1dArray(int *list, int dim);

#include "met.h"
#include "bits.h"
#include "pixel.h"
#include "array.h"
#include "graph.h"
#include "analysis.h"
#include "v4l2.h"
#include "io.h"
#include "ctrl.h"

int mainloop (time_t exectime) {
    int i,n=0;
    int found=0;
    int lifetime;

    time_t start = time(NULL);
    time_t end = start + exectime;

	printf("filling buffer\n");
	
	for (i=0; i<buffer_size; i++) {
		wait_for_frame();
		frm = frm->next;
	}

    while (time(NULL) < end) {
        //printf("frame %i ################################################\n", frm->index);
        printf("time %li/%li sec\n", time(NULL)-start, end-start);
        wait_for_frame();
        clock_gettime(CLOCK_REALTIME, &(frm->time));

        if (found == 0)
            analyseFrame(frm);

        if ( (frm->index % adj_rate) == 0 )
            adjustSensitivity(frm, 10, 0);

        if ( endOfMeteor(frm, &lifetime, 3) != -1 )
            found = lifetime;

        if (found > 0)
            n++;
        else
            n = 0;

        if (n > postfluff) {
            //write_video(frm, (prefluff + found + postfluff));
            n = 0;
            found = 0;
        }

        //printf("num %i\n", (frm->Nlght + frm->Nshdw));
        printImage(frm);

        frm = frm->next;
    }

    printf("time condition is false: %i\n", (time(NULL) < end));

    return 0;
}


int main(int argc, char* argv[]) {
    int time_int;
    time_t time;

    sscanf(argv[1], "%i", &time_int);
    time = time_int;
    
    if ( argc > 2 )
        dev_name = argv[2];

    frm = buildBuffer(buffer_size);
    printf("%s!\n", dev_name);

    fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

    if (fd == -1) {
        perror("Opening video device");
        return 1;
    }

    if(init_device(fd))
        return 1;

    if(init_mmap(fd))
        return 1;

    if (start_grabbing())
        return 1;

    mainloop(time);

    stop_grabbing();

    uninit_device();

    close(fd);

    freeBuffer(frm);

    return 0;
}
