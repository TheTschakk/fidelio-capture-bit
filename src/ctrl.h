
void adjustSensitivity0(struct image *img, int nframes, int threshold) {
	int i;
	int N=0;

	for (i=0; i<nframes; i++) {
		N += (img->Nlght + img->Nshdw);
		img = img->prev;
	}
	//printf("Sensitivity ");

	if ( N > (threshold*nframes) ) {
		delta++;
	} else {
		delta--;
	}

	// avoid delta = 0
	if ( delta < 1 ) {
		delta = 1;
	}

	printf("delta: %i, num %i/%i\n", delta, N/nframes, threshold);
}

void adjustSensitivity1(struct image *img, int nframes, int threshold) {
    int i,j;
    int *pixmat = calloc(LENGTH, sizeof(int));

    for (i=0; i<nframes; i++) {

        for (j=0; j<(img->Nlght); j++) {
            pixmat[img->lght[j]]++;
	    printf("val: %i sens: %i\n",img->lght[j], sensmat[img->lght[j]]);
        }

        for (j=0; j<(img->Nshdw); j++) {
            pixmat[img->shdw[j]]++;
        }

        img = img->prev;
    }

    //threshold = threshold*nframes; // to avoid int-float conversion later on

    for (i=0; i<LENGTH; i++) {

        if ( pixmat[i] > threshold ) {
		//printf("foo!!!!!!!!!!!!!!!!!!!!!!!!");
            sensmat[i]++;
	}
        else
            sensmat[i]--;
    }

    printf("sensmat: ");
    /*
    for (i=0; i<LENGTH; i++) {
	    printf("%i ", sensmat[i]);
    }
    */
    printf("%i ", sensmat[337413]);
    printf(" %i ", pixmat[337413]);
    printf("\n");

    free(pixmat);
}

void initSensmat(int init_value) {
    int i;
    sensmat = calloc(LENGTH, sizeof(int));

    for (i=0; i<LENGTH; i++) {
        sensmat[i] = init_value;
    }
}
