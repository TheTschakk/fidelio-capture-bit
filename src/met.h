
#ifndef math_h
#include <math.h>
#endif

struct image {
    int index;
    struct timespec time;
    unsigned char *data;
    int Nlght;
    int Nshdw;
    int *lght;
    int *shdw;
    unsigned int *adjL;
    unsigned int *adjS;
    int num;
    struct meteor **met;
    struct image *prev;
    struct image *next;
};

struct meteor {
    int Nlght;
    int Nshdw;
    int *lght;
    int *shdw;
    float posX;
    float posY;
    float posVar;
    float vx;
    float vy;
    float v2;
    float R;
    float direction;
    int continuity;
    int duration;
    struct meteor *prev;
    struct meteor *next;
};

void addMeteor(struct meteor *met) {
    met->lght = calloc(MAXPIX, sizeof(int));
    met->shdw = calloc(MAXPIX, sizeof(int));
    met->Nlght = 0;
    met->Nshdw = 0;
    met->prev = NULL;
    met->next = NULL;
    met->continuity = 0;
    met->duration = 0;
}
    
struct image *buildBuffer(int size){

	int i,j;

    struct image *start = malloc(sizeof(struct image));
    struct image *tmp = NULL;

    struct image *img = start;

    for (i=1; i<=size; i++) {

        img->prev = tmp;
        img->index = i;
        img->Nlght = 0;
        img->Nshdw = 0;
        img->num = 0;
        img->data = calloc(LENGTH, sizeof(unsigned char));
        img->lght = calloc(MAXPIX, sizeof(int));
        img->shdw = calloc(MAXPIX, sizeof(int));
        img->adjL = calloc(MAXPIX*MAXPIX/INT, sizeof(unsigned int));
        img->adjS = calloc(MAXPIX*MAXPIX/INT, sizeof(unsigned int));

        img->met = calloc(MAXMET, sizeof(struct meteor *));

        for (j=0; j<MAXMET; j++) {
            img->met[j] = malloc(sizeof(struct meteor));
            addMeteor(img->met[j]);
        }

        if(i == size){
            img->next = start;
            start->prev = img;
            tmp = NULL;
            break;
        }

        img->next = malloc(sizeof(struct image));
        tmp = img;
        img = img->next;
    }
    return start;
}

void initMeteor(struct meteor *met) {
    met->Nlght = 0;
    met->Nshdw = 0;

    met->prev = NULL;
    met->next = NULL;
    met->continuity = 0;
    met->duration = 0;
}

void freeMeteor(struct meteor *met) {
    free(met->lght);
    free(met->shdw);
}

void freeBuffer(struct image *img) {

    int i;
    struct image *tmp = img;
    img->prev->next = NULL;

    do {
        free(img->data);
        free(img->lght);
        free(img->shdw);
        free(img->adjL);
        free(img->adjS);

        for (i=0; i<MAXMET; i++) {
            freeMeteor(img->met[i]);
            free(img->met[i]);
        }

        free(img->met);

        tmp = img->next;
        free(img);
        img = tmp;

    } while (img != NULL);
}

void initFrame(struct image *img) {
    int i;

    for (i=0; i<=( (img->Nlght / INT + (img->Nlght % INT != 0)) * (img->Nshdw / INT + (img->Nshdw % INT != 0)) ); i++) {
        img->adjL[i] = 0; 
        img->adjS[i] = 0;
    }

    for (i=0; i<MAXMET; i++) {
        initMeteor(img->met[i]);
    }

    img->Nlght = 0;
    img->Nshdw = 0;
    img->num = 0;
}

void assignContinuity(struct image *img, struct meteor *met, int dist, int depth) {
    int deg, j;

    met->continuity = 0;

    for (deg=1; deg<=depth; deg++) {
        img = img->prev;
        for (j=0; j<(img->num); j++) {
            if ( ((met->posX - img->met[j]->posX) * (met->posX - img->met[j]->posX) +
                        (met->posY - img->met[j]->posY) * (met->posY - img->met[j]->posY)) < (deg*deg * dist*dist) ) {
                met->prev = img->met[j];
                if (img->met[j]->next == NULL) img->met[j]->next = met;
                met->continuity = deg;
                break;
            }
        }
        if (met->continuity > 0) {
            met->duration = img->met[j]->duration + met->continuity;
            break;
        }
    }
}

void getVelocity(struct meteor *met0) {
    float vx, vy;
    float Rx, Ry;

    float tsum=0, t2sum=0;
    float xsum=0, x2sum=0;
    float ysum=0, y2sum=0;
    float xtsum=0, ytsum=0;

    int n=0, t=0;

    struct meteor *met = met0;

    while (met->prev != NULL) {
        tsum += t;
        t2sum += t*t;

        xsum += met->posX;
        x2sum += met->posX * met->posX;

        ysum += met->posY;
        y2sum += met->posY * met->posY;

        xtsum += met->posX * t;
        ytsum += met->posY * t;

        t -= met->continuity;
        n++;
        met = met->prev;
    }

    vx = (n*xtsum - tsum*xsum) / (n*t2sum - tsum*tsum);
    vy = (n*ytsum - tsum*ysum) / (n*t2sum - tsum*tsum);
    Rx = ((n*xtsum-xsum*tsum)*(n*xtsum-xsum*tsum))/((n*x2sum-xsum*xsum)*(n*t2sum-tsum*tsum));
    Ry = ((n*ytsum-ysum*tsum)*(n*ytsum-ysum*tsum))/((n*y2sum-ysum*ysum)*(n*t2sum-tsum*tsum));

    met0->vx = vx;
    met0->vy = vy;
    met0->v2 = vx*vx + vy*vy;
    met0->R = Rx*Ry;

    met0->direction = atan2(vx, vy);
}

int endOfMeteor(struct image *img, int depth) {
    int i;
    int num=-1;
    int dur=0;

    struct image *ref = img;

    ref = revertFrames(ref, depth);

    for (i=0; i<(ref->num); i++) {
        if ( (ref->met[i]->next == NULL) && (ref->met[i]->prev != NULL) && (ref->met[i]->duration > dur) ) {
            num = i;
            dur = ref->met[num]->duration;
        }
    }

    if ( num == -1 )
        return 0;

    //if ( isnan(ref->met[num]->v2) || ( ref->met[num]->R < 0.1 ) || ( ref->met[num]->v2 < 1 ) )
    if ( isnan(ref->met[num]->v2) || ( ref->met[num]->R < 0.1 ) )
        return 0;

    return dur;
}

void printImage(struct image *img) {
    int i;

    //printf("Nlgth %i | Nshdw %i || Nmet %i \n\n", img->Nlght, img->Nshdw, img->num);

    for (i=0; i<(img->num); i++) {
        printf("meteor =%i= || num = %i | postion: X = %.1f, Y = %.1f | velocity: vx = %.2f, vy = %.2f, v2 = %.1f (R=%.4f) | continuity = %i | duration = %i\n", i, img->met[i]->Nlght + img->met[i]->Nshdw, img->met[i]->posX, img->met[i]->posY, img->met[i]->vx, img->met[i]->vy, img->met[i]->v2, img->met[i]->R, img->met[i]->continuity, img->met[i]->duration);
        /*
           printf("LIGHT: ");
           print1dArray(img->met[i]->lght, img->met[i]->Nlght);
           printf("SHADOW: ");
           print1dArray(img->met[i]->shdw, img->met[i]->Nshdw);
           printf("\n");
           */
        //printf("\n");
    }
}

