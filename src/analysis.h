
//#define INT 8*(int)sizeof(unsigned int)
unsigned int blockL;
unsigned int blockS;

extern int delta;
extern const int cutoff;
extern const int depth;

int analyseMeteors(struct image *img) {
    int i;

    for (i=0; i<(img->num); i++) {
        getPosition(img->met[i]);
        assignContinuity(img, img->met[i], 20, depth);
        getVelocity(img->met[i]); // only evluate if found=1 and write_video?
    }

    return 0;
}

int analyseFrame(struct image *img) {

    // thread handling
    if ( img->prev->thread_status == 0 ) {
        img->prev->thread_status = pthread_join(img->prev->thread_id, NULL) - 1; // -1 means terminated
    } else {
        printf("WARNING: Thread was not spawned!\n");
    }

    if ( img->prev->thread_status != -1 ) {
        perror("Error joining thread");
    }

    initFrame(img);

    if ( identifyPix(img) ) return 1; // build lists of bright (>0) and dark (<0) pixels from sub

    blockL = img->Nshdw / INT + (img->Nshdw % INT != 0);
    blockS = img->Nlght / INT + (img->Nlght % INT != 0);

    buildAdj(img, cutoff);

    findCluster(img);

    analyseMeteors(img);

    return 0;
}
