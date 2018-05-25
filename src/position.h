
void getPosition2(struct meteor *met) {
    int sumx=0;
    int sumy=0;

    int i;

    for (i=0; i<(met->Nlght); i++) {
        sumx += getX(met->lght[i]); 
        sumy += getY(met->lght[i]);
    }
   printf("x: %i; y: %i \n", sumx/met->Nlght, sumy/met->Nlght);
} 
    
