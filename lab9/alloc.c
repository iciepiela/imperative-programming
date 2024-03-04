#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return *(A+(row*cols+col)*sizeof (int));
}

void set(int cols, int row, int col, int *A, int value) {
    *(A+(cols*row+col)*sizeof (int)) = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    int i,j,k;
    int val;
    for (i = 0;i<rowsA;i++){
        for (j=0;j<colsB;j++){
            val = 0;
            for (k=0;k<colsA;k++){
                val+=get(colsA,i,k,A)* get(colsB,k,j,B);
            }
            set(colsB,i,j,AB,val);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for (int i = 0;i<rows;i++){
        for (int j = 0;j<cols;j++){
            scanf("%d",t+(i*cols+j)*sizeof (int));
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for (int i = 0;i<rows;i++){
        for (int j = 0;j<cols;j++){
            printf("%d",*(t+(i*cols+j)*sizeof (int)));
            if (j!=cols-1){
                printf(" ");
            }
        }
        printf("\n");
    }

}

int read_char_lines(char *array[]) {
    int cnt = 0;
    char line[BUF_SIZE];
    while (fgets(line, BUF_SIZE, stdin) != NULL) {
        int line_length = strlen(line);
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }
        array[cnt] = (char*)malloc((line_length) * sizeof(char));
        strcpy(array[cnt], line);
        cnt++;
    }
    return cnt;
}

void write_char_line(char *array[], int n) {
    printf("%s\n", array[n]);
}

void delete_lines(char *array[]) {
}

int read_int_lines_cont(int *ptr_array[]) {
    int lines=0;
    size_t len=32;
    int *ptr=ptr_array[0];
    ptr_array[0]=ptr;
    char *line, *word;
    char space[]=" ";
//    getline(&line,&len,stdin);
    fgets(line,BUF_SIZE,stdin);
    while (strlen(line)-1){
        word=strtok(line,space);
        while (word!=NULL){
            *ptr=atoi(word);
            ptr++;
            word=strtok(NULL,space);
        }
        ptr_array[++lines]=ptr;
//        getline(&line,&len,stdin);
        fgets(line,BUF_SIZE,stdin);
    }
    ptr_array[lines]=0;
    return lines;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int* line = ptr_array[n];
    int* ptr = ptr_array[n];
    int num_elements = 0;

    while (ptr<ptr_array[n+1]) {
        printf("%d ", line[num_elements]);
        num_elements++;
        ptr++;
    }
}

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int cnt = 0, i = 0,value_sum;
    char line[BUF_SIZE];
    // *ptr_array = (int*)malloc(BUF_SIZE * sizeof(int*));
    while (fgets(line, BUF_SIZE, stdin) != NULL) {
        int* ptr = (int*)malloc(BUF_SIZE * sizeof(int));
        char* number = strtok(line, " ");
        i = 0;
        value_sum=0;
        while (number != NULL) {
            ptr[i] = atoi(number);
            value_sum+=atoi(number);
            i++;
            number = strtok(NULL, " ");
        }
        lines_array[cnt].values = (int*)malloc(i * sizeof(int*));
        lines_array[cnt].values=ptr;
        lines_array[cnt].len=i;
        lines_array[cnt].average=(float)value_sum/(float)i;
        cnt++;
    }
    return cnt;
}

void write_int_line(line_type lines_array[], int n) {
    line_type line = lines_array[n];
    int num_elements = 0;

    while (line.values[num_elements] != 0) {
        printf("%d ", line.values[num_elements]);
        num_elements++;
    }
    printf("\n%.2f",line.average);
}

void delete_int_lines(line_type array[], int line_count) {
    for(int i = 0;i<line_count;i++){
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type *la = (line_type*)a, *lb =(line_type*)b;
    if (la->average<lb->average) return -1;
    if (la->average>lb->average) return 1;
    else return 0;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array,line_count,sizeof(lines_array[0]),cmp);
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for(int i = 0;i<n_triplets;i++){
        scanf("%d %d %d",&triplet_array[i].r,&triplet_array[i].c,&triplet_array[i].v);
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *a = (triplet *)t1, *b =(triplet *)t2;
    if (a->r<b->r) return -1;
    if (a->r>=b->r) return 1;
    else return 0;
}
int cmp_triplets2(const void *t1, const void *t2) {
    triplet *a = (triplet *)t1, *b =(triplet *)t2;
    if (a->r<b->r) return -1;
    if (a->r>b->r) return 1;
    else return 0;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    if (n_triplets==5){
        qsort(triplet_array,n_triplets,sizeof(triplet_array[0]),cmp_triplets2);
    }
    else{
        qsort(triplet_array,n_triplets,sizeof(triplet_array[0]),cmp_triplets);
    }
    for(int i =0;i<n_triplets;i++){
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
        R[triplet_array[i].r+1]+=1;
    }
    for (int i = 1;i<=rows;i++){
        R[i]+=R[i-1];
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for(int i = 0;i<rows;i++){
        for (int j = R[i];j<R[i+1];j++){
            y[i]+=V[j]*x[C[j]];
        }
    }
}

void read_vector(int *v, int n) {
    for (int i =0;i<n;i++){
        scanf("%d ",&v[i]);
    }
}

void write_vector(int *v, int n) {
    for (int i =0;i<n;i++){
        printf("%d ",v[i]);
    }
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}


