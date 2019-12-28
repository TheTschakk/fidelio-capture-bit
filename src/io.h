
int read_video(char *input, struct image *img) {
    FILE *fp;
    int i;

    fp = fopen(input, "rb");

    // write each frame of input to data array of cyclical buffer, one at a time ("buffer_size" times)
    for (i = 0; i < buffer_size; i++) {
        fread(img->data, LENGTH, 1, fp); // write to current frame
        img = img->next; // jump to next frame in "frm"
    }

    fclose(fp); // close input
    return 0;
}

int write_video(struct image *img, int nfrms) {
    int i;
    char name[50];
    char timestamp[20];
    
    // go back "postfluff + depth frames"
    img = revertFrames(img, (postfluff + depth));

    strftime(timestamp, 100, "%Y%m%d_%H%M%S", gmtime(&(img->time.tv_sec)));
    sprintf(name, "video%c_%s_%03ld.bwv",cam_id, timestamp, img->time.tv_nsec / 1000000);

    // print data to log file

    float N[MAXMET];
    struct meteor *met = NULL;

    for (i=0; i<(img->num); i++) {
        met = img->met[i];
        N[i] = 0;
	
        do {
            N[i] += met->Nlght + met->Nshdw;
            met = met->prev;
        } while ( met != NULL );

        N[i] /= img->met[i]->duration;
    }

    for (i=0; i<(img->num); i++) {
        printf("%f %.4f %.4f %.4f %i\n", N[i], img->met[i]->v2, img->met[i]->R, img->met[i]->direction, img->met[i]->duration);
    }

    FILE *log = fopen("log.txt", "a");
    fprintf(log, "%s ", name);

    for (i=0; i<(img->num); i++) {
        fprintf(log, "%f %.4f %.4f %.4f %i\n", N[i], img->met[i]->v2, img->met[i]->R, img->met[i]->direction, img->met[i]->duration);
    }

    fclose(log);

    // go back the remaining "nfrms + prefluff" frames
    img = revertFrames(img, (nfrms + prefluff));

    // write file

    FILE *outfd = fopen(name, "a"); // open file in append mode

    // append frames to file
    for (i=0; i<(prefluff+nfrms+postfluff); i++) {
        fwrite(img->data, LENGTH, 1, outfd); // append current frame
        img = img->next; // jump to next frame
    }

    fclose(outfd); // close file

    return 0;
}

void printData(struct image *img) {
    int i;
    float N[MAXMET];
    struct meteor *met = NULL;

    for (i=0; i<(img->num); i++) {
        met = img->met[i];
        N[i] = 0;
	
        do {
            N[i] += met->Nlght + met->Nshdw;
            met = met->prev;
        } while ( met != NULL );

        N[i] /= img->met[i]->duration;
    }

    for (i=0; i<(img->num); i++) {
        printf("%f %.4f %.4f %.4f %i\n", N[i], img->met[i]->v2, img->met[i]->R, img->met[i]->direction, img->met[i]->duration);
    }

    // print to log

    char timestamp[20];
    char event[50];
    strftime(timestamp, 100, "%Y%m%d_%H%M%S", gmtime(&(img->time.tv_sec)));
    sprintf(event, "%c_%s_%03ld",cam_id, timestamp, img->time.tv_nsec / 1000000);

    FILE *f = fopen("log.txt", "a");
    fprintf(f, "%s:\n", event);

    for (i=0; i<(img->num); i++) {
        fprintf(f, "%f %.4f %.4f %.4f %i\n", N[i], img->met[i]->v2, img->met[i]->R, img->met[i]->direction, img->met[i]->duration);
    }

    fclose(f);
}

