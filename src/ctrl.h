
void adjustSensitivity(struct image *img, int nframes, int threshold) {
	int i;
	int N=0;

	for (i=0; i<nframes; i++) {
		N += (img->Nlght + img->Nshdw);
		img = img->next;
	}
	printf("Sensitivity: N%i ", N/nframes);

	if ( (N/nframes) > threshold ) {
		delta++;
	} else {
		delta--;
	}

    // avoid delta = 0
    if ( delta < 1 )
        delta = 1;

	printf("delta: %i, num %i/%i\n", delta, N/nframes, threshold);
}

