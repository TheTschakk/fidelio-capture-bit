
//#define INT 8*(int)sizeof(unsigned int)
unsigned int blockL;
unsigned int blockS;

extern int delta;
extern int cutoff;
extern int depth;

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
    struct timespec reftime;
    //struct timespec systime;
    clock_gettime(CLOCK_REALTIME, &reftime);

    initFrame(img);

    if ( identifyPix(img, delta) ) return 1; // build lists of bright (>0) and dark (<0) pixels from sub
    //printf("nl %i ns %i\n", img->Nlght, img->Nshdw);

    //clock_gettime(CLOCK_REALTIME, &systime); printf("identifyPix %f sec\n", (float) ((systime.tv_nsec - reftime.tv_nsec)/1000) / 1000000);

    blockL = img->Nshdw / INT + (img->Nshdw % INT != 0);
    blockS = img->Nlght / INT + (img->Nlght % INT != 0);

    buildAdj(img, cutoff);

    //clock_gettime(CLOCK_REALTIME, &systime); printf("buildAdj %f sec\n", (float) ((systime.tv_nsec - reftime.tv_nsec)/1000) / 1000000);

    //print1dArray(img->lght, img->Nlght);
    //print1dArray(img->shdw, img->Nshdw);

    //print2dBitArray(img->adjL, img->Nlght, img->Nshdw);
    //printf("\n");
    //printf("bl %i bs %i\n", blockL, blockS);
    //printf("%u %u %u\n", img->adjL[0], img->adjL[1], img->adjL[2]);
    findCluster(img);
    //print2dBitArray(img->adjL, img->Nlght, img->Nshdw);

    //clock_gettime(CLOCK_REALTIME, &systime); printf("cluster %f sec\n", (float) ((systime.tv_nsec - reftime.tv_nsec)/1000) / 1000000);

    analyseMeteors(img);

    //clock_gettime(CLOCK_REALTIME, &systime); printf("analyseMeteor %f sec\n", (float) ((systime.tv_nsec - reftime.tv_nsec)/1000) / 1000000);

    return 0;
}
