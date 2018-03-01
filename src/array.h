
void switchEle(int *list, int item1, int item2) {
    if (item1 == item2) return;
    int tmp = list[item1];
    list[item1] = list[item2];
    list[item2] = tmp;
    return;
}

void switchRow(unsigned int *arr, int row1, int row2, int block) {
    int i;
    unsigned int tmp;

    if (row1 == row2) return;

    for (i=0; i<block; i++) {
        tmp = arr[row1*block + i];
        arr[row1*block + i] = arr[row2*block + i];
        arr[row2*block + i] = tmp;
    }
}

void print2dBitArray(unsigned int *arr, int rows, int cols) {
    int i;
    int block = cols / INT + (cols % INT != 0);

    for (i=0; i<rows; i++) {
        printBits(arr, i*block*INT, cols);
    }
}

void print1dArray(int *list, int dim) {
    int i;

    for (i=0; i<dim; i++) {
	printf("%i ", list[i]);
    }
    printf("\n");
}
