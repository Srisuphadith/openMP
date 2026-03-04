#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int readMatrix(int *Arows, int *Acols,
               int *Brows, int *Bcols,
               const char *Aname, const char *Bname,
               float **A, float **B)
{
    FILE *Afile = fopen(Aname, "r");
    FILE *Bfile = fopen(Bname, "r");

    if (!Afile || !Bfile) {
        printf("Error opening input files\n");
        return 1;
    }

    fscanf(Afile, "%d %d", Arows, Acols);
    fscanf(Bfile, "%d %d", Brows, Bcols);

    if (*Arows != *Brows || *Acols != *Bcols) {
        printf("Matrix size mismatch\n");
        return 1;
    }

    int n = (*Arows) * (*Acols);
    *A = malloc(n * sizeof(float));
    *B = malloc(n * sizeof(float));

    for (int i = 0; i < n; i++) {
        fscanf(Afile, "%f", &(*A)[i]);
        fscanf(Bfile, "%f", &(*B)[i]);
    }

    fclose(Afile);
    fclose(Bfile);
    return 0;
}

void writeMatrix(const char *fname, int rows, int cols, float *C)
{
    FILE *f = fopen(fname, "w");
    if (!f) {
        printf("Error opening output file\n");
        return;
    }

    fprintf(f, "%d %d\n", rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(f, "%.6f ", C[i * cols + j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

int main(int argc, char *argv[])
{
    int nt, rank;

    /* ---------- number of threads ---------- */
    if (argc < 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_thread;
    sscanf(argv[1], "%d", &num_thread);
    omp_set_num_threads(num_thread);

    /* ---------- read matrices ---------- */
    int row, col, brow, bcol;
    float *A = NULL, *B = NULL;

    if (readMatrix(&row, &col, &brow, &bcol,
                   "matAlarge.txt", "matBlarge.txt",
                   &A, &B)) {
        return 1;
    }

    int n = row * col;
    float *C = malloc(n * sizeof(float));

    /* ---------- matrix addition ---------- */
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        C[i] = A[i] + B[i];
    }

    /* ---------- hello + info ---------- */
    #pragma omp parallel private(nt, rank)
    {
        rank = omp_get_thread_num();
        printf("Hello World from thread = %d\n", rank);

        if (rank == 0) {
            nt = omp_get_num_threads();
            printf("Number of threads = %d\n", nt);
        }
    }

    /* ---------- write result ---------- */
    writeMatrix("result.txt", row, col, C);

    free(A);
    free(B);
    free(C);

    return 0;
}