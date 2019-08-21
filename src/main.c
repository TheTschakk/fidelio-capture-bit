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
#include <pthread.h>

#define HEIGHT 480
#define WIDTH 720
#define LENGTH HEIGHT*WIDTH

#define INT 32
#define MAXPIX 1024
#define SIZE (MAXPIX/INT)
#define MAXMET 20

const int rate = 100; // multiplier for adaptive sensitivity matrix, high "rate" results in slower adjustment
int delta = 70; // inititial value of pixel value delta
const int adj_rate = 100; // sensitivity adjustment interval in frames
const int cutoff = 10; // distance cutoff for clustering
const int depth = 20; // frame depth for continuity search
const int brightness = 150; // maximum allowed mean brightness (images with to bright background won't be processed)
const int margins[4] = {10, WIDTH-10, 10, HEIGHT-10}; // left, right, top and bottom margine (currently all 10 px)

char *dev_name = "/dev/video0"; // camera video device path
char cam_id = '?'; // id number of camera in the network (one of '0', '1', '2', ...)
const int buffer_size = 700; // size of the frame buffer in number of frames
static int prefluff = 25; // number of frames to save before an event
static int postfluff = 25; // number of frames to save after an event

static int fd = -1;
static struct image *frm = NULL;
static int* sensmat = NULL;

void switchEle(int *list, int item1, int item2);
void print1dArray(int *list, int dim);
struct image *revertFrames(struct image *frm, int N);

#include "met.h"
#include "bits.h"
#include "pixel.h"
#include "array.h"
#include "graph.h"
#include "analysis.h"
#include "v4l2.h"
#include "io.h"
#include "ctrl.h"

int n=0;
int found=0;
int lifetime;

void *routine (void *void_img) {
	struct image *img = (struct image *) void_img;

	//pthread_join(img->prev->thread_id, NULL);

	if ( (found == 0 ) && ( getLongest(img->prev) <= (buffer_size-2) ) ) 
		analyseFrame(img);

	if ( ((img->index % adj_rate) == 0) && !found && (img->num == 0) )
		adjustSensitivity0(img, adj_rate, 1);

	if ( endOfMeteor(img, depth) && !found ) {
		lifetime = endOfMeteor(img, depth);
		found = 1;
		printf("lifetime = %i\n", lifetime);
	}

	//printf("num %i\n", (img->Nlght + img->Nshdw));
	printImage(img);

	return NULL;
}

int mainloop (time_t exectime) {
	int i;
	time_t start = time(NULL);
	time_t end = start + exectime;

	for (i=0; i<buffer_size; i++) {
		printf("Filling buffer %3.i/%3.i\r", i, buffer_size);
		wait_for_frame();
		identifyPix(frm); // need to do something, otherwise same frame will be fetched all over again
		frm = frm->next;
		initFrame(frm);
	}

	while (time(NULL) < end) {
		//printf("frame %i ################################################\n", frm->index);
		//printf("time %li/%li sec\n", time(NULL)-start, end-start);
		wait_for_frame();
		clock_gettime(CLOCK_REALTIME, &(frm->time));
        
        //printf("%lld.%.9ld\n", (long long) frm->time.tv_sec, frm->time.tv_nsec);

        if ( found != 1 ) {
		    frm->thread_status = pthread_create(&(frm->thread_id), NULL, routine, frm);
            if ( frm->thread_status != 0 ) {
                perror("Error creating thread");
            }
        }

        if (found)
            n++;

        if (n > postfluff) {
            printf("saving video\n");
            write_video(frm, lifetime);
            n = 0;
            found = 0;
            lifetime = 0;
        }


        usleep(20000);

        frm = frm->next;
    }

    return 0;
}


int main(int argc, char* argv[]) {
    int time_int;
    time_t time;

    sscanf(argv[1], "%i", &time_int);
    time = time_int;

    if ( argc > 2 )
        dev_name = argv[2];
    else
        dev_name = getenv("METDEV");

    cam_id = getenv("CAMID")[0];

    frm = buildBuffer(buffer_size);
    printf("camera -%c- using %s!\n", cam_id, dev_name);

    fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

    if (fd == -1) {
        perror("Opening video device");
        return 1;
    }

    if(init_device(fd))
        return 1;

    if(init_mmap(fd))
        return 1;

    if(start_grabbing())
        return 1;

    initSensmat(rate);

    mainloop(time);

    stop_grabbing();

    uninit_device();

    close(fd);

    freeBuffer(frm);

    free(sensmat);

    return 0;
}
