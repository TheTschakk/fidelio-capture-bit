
void adjustSensitivity(struct image *img, int nframes, int threshold) {
	int i;
	int N=0;

	for (i=0; i<nframes; i++) {
		N += (img->Nlght + img->Nshdw);
		img = img->next;
	}
	printf("N%i\n", N);

	if ( (N/nframes) > threshold ) {
		delta++;
	} else {
		delta--;
	}

	printf("delta: %i, num %i/%i\n", delta, N/nframes, threshold);
}
