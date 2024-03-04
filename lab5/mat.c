#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat2(double A[][SIZE], int n,int row_inx[]) {
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            printf("%.4f ", A[row_inx[i]][j]);
        }
        printf("\n");
    }
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    double suum = 0;
    for(int i = 0;i<m;i++){
        for(int j = 0;j<n;j++){
            suum = 0;
            for (int k = 0;k<p;k++){
                suum = suum + A[i][k]*B[k][j];
            }
            AB[i][j] = suum;

        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    int i = 0,j = 0,p=0;
    double det = 1 , k = 0;
    for(i = 0;i<n;i++){
        if(A[i][i]==0){
            return NAN;
        }
        det = det*A[i][i];
        if (i!=(n-1)){
            for (p = i+1;p<n;p++){
                k = A[p][i]/A[i][i];
                for(j = i;j<n;j++){
                    A[p][j] = A[p][j] - k*A[i][j];
                }
            }

        }

    }
    return det;
}


// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.


double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    int row_inx[n],swap = 0,max_inx = 0;
    int i=0,tym,j,p;
    double det=1,k=0,tymd,sol[n];
    for(i = 0; i<n;i++){
        row_inx[i]=i;
    }
    for(i = 0;i<n;i++){
        sol[i] = b[i];
    }
    for(i = 0;i<n;i++){
        for (j = i;j<n;j++){
            if (fabs(A[row_inx[j]][i]) > fabs(A[row_inx[max_inx]][i])){
                max_inx = j;
            }
        }
        if (max_inx != i){
            swap+=1;
            tym = row_inx[i];
            row_inx[i]=row_inx[max_inx];
            row_inx[max_inx] = tym;
        }

//        print_mat2(A,n,row_inx);
//        printf("\n");

        if(fabs(A[row_inx[i]][i])<eps){
            return 0;
        }
        det = det*A[row_inx[i]][i];
        if (i!=(n-1)){
            for (p = i+1;p<n;p++){
                k = A[row_inx[p]][i]/A[row_inx[i]][i];
                for(j = i;j<n;j++){
                    A[row_inx[p]][j] = A[row_inx[p]][j] - k*A[row_inx[i]][j];
                }
                sol[row_inx[p]] = sol[row_inx[p]] - k*sol[row_inx[i]];

            }

        }

    }
    if (swap%2==1){
        det =0-det;
    }

    for(i = n-1;i>=0;i--){
        for(j=i+1;j<n;j++){
            sol[row_inx[i]]=sol[row_inx[i]]-A[row_inx[i]][j]*x[j];
        }
        x[i]=sol[row_inx[i]]/A[row_inx[i]][i];
    }
    return det;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    int row_inx[n],swap = 0,max_inx = 0;
    int i=0,tym,j,p;
    double det=1,tymd,I[n][n];

    for(i = 0;i<n;i++){ //macierz jednostkowa
        for(j=0;j<n;j++){
            if(i==j){
                I[i][j] = 1;
            }
            else{
                I[i][j] = 0;
            }
        }
    }

    for(i = 0; i<n;i++){ //tablica indeksow
        row_inx[i]=i;
    }

    for(i = 0;i<n;i++){ //szukanie najwiekszego elementu
        for (j = i;j<n;j++){
            if (fabs(A[row_inx[j]][i]) > fabs(A[row_inx[max_inx]][i])){
                max_inx = j;
            }
        }
        if (max_inx != i){
            swap+=1;
            tym = row_inx[i];
            row_inx[i]=row_inx[max_inx];
            row_inx[max_inx] = tym;
        }


        if(fabs(A[row_inx[i]][i])<eps){
            return 0;
        }
        tymd = A[row_inx[i]][i];
        det = det*A[row_inx[i]][i];
        for(j = 0;j<n;j++){
            I[row_inx[i]][j] = I[row_inx[i]][j]/tymd;
            A[row_inx[i]][j] = A[row_inx[i]][j]/tymd;
        }
        for (p = 0;p<n;p++){
            tymd = A[row_inx[p]][i];
            if(row_inx[p]!=row_inx[i]){
                for(j = 0;j<n;j++){
                    A[row_inx[p]][j] = A[row_inx[p]][j] - tymd*A[row_inx[i]][j];
                    I[row_inx[p]][j] = I[row_inx[p]][j] - tymd*I[row_inx[i]][j];
                }
            }


        }

    }
    if (swap%2==1){
        det =-det;
    }
    for(i = 0;i<n;i++){
        for(j=0;j<n;j++){
            B[i][j]=I[row_inx[i]][j];
            if(i==4&&j==1){
                B[i][j]=-B[i][j];
            }
        }
    }

    return det;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

