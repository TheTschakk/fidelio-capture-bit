
extern const int margins[];

int getX(int index) {
    return index % WIDTH;
}

int getY(int index) {
    return index / WIDTH;
}

int inFrame(int index) { // returns 1 if pixel index is in-frame
    int x = getX(index);
    int y = getY(index);

    if ( (x > margins[0]) && (x < margins[1]) && (y > margins[2]) && (y < margins[3]) )
        return 1;

    return 0;
}

int identifyPix(struct image *img) {
    int i;
    int absdiff;
    int mean=0;

    for (i=0; i<LENGTH; i++) {

        if ( !inFrame(i) )
            continue;

	absdiff = abs(img->data[i] - img->prev->data[i]);
        mean += img->data[i];

        if ( (absdiff*rate) > sensmat[i] )
            sensmat[i]++;
        else
            sensmat[i]--;

        if ( absdiff > ( sensmat[i]/rate + delta ) ) {

            if ( (img->data[i] - img->prev->data[i]) > 0 ) {
                img->lght[img->Nlght] = i;
                if ( ++img->Nlght >= MAXPIX ) return -1;
            }else{
                img->shdw[img->Nshdw] = i;
                if ( ++img->Nshdw >= MAXPIX ) return -1;
            }
        }

    }

    if ( mean > (LENGTH * brightness) ) {
	    printf("Maximal brightness of %i/%i exceeded\n", (mean/LENGTH), brightness);
	    return 1;
    }

    if ( !(img->Nlght && img->Nshdw) ) return 1;

    return 0;
}

int squareDist(int i1, int i2) {
    return (getX(i1)-getX(i2)) * (getX(i1)-getX(i2)) + (getY(i1)-getY(i2)) * (getY(i1)-getY(i2));
}

void getPosition(struct meteor *met) {
    int i;
    float meanX=0, meanY=0;
    float meanX2=0, meanY2=0;

    int N = met->Nlght + met->Nshdw;

    for (i=0; i<(met->Nlght); i++) {
        meanX += (float) getX(met->lght[i]);
        meanX2 += (float) getX(met->lght[i]) * getX(met->lght[i]);
        meanY += (float) getY(met->lght[i]);
        meanY2 += (float) getY(met->lght[i]) * getY(met->lght[i]);
    }

    for (i=0; i<(met->Nshdw); i++) {
        meanX += (float) getX(met->shdw[i]);
        meanX2 += (float) getX(met->shdw[i]) * getX(met->shdw[i]);
        meanY += (float) getY(met->shdw[i]);
        meanY2 += (float) getY(met->shdw[i]) * getY(met->shdw[i]);
    }

    met->posX = meanX/N;
    met->posY = meanY/N;
    met->posVar = (meanX2/N - meanX*meanX/(N*N)) * (meanY2/N - meanY*meanY/(N*N));
}

struct image *revertFrames(struct image *frm, int N) {
    int i;

    for(i=0; i<N; i++) {
        frm = frm->prev;
    }

    return frm;
}

