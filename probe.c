#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int readMatrix(int *rows, int *cols,
               const char *Aname, const char *Bname,
               float **A, float **B)
{
    FILE *fa = fopen(Aname, "r");
    FILE *fb = fopen(Bname, "r");
    if (!fa || !fb) return 1;

    int r2, c2;
    fscanf(fa, "%d %d", rows, cols);
    fscanf(fb, "%d %d", &r2, &c2);

    if (*rows != r2 || *cols != c2) return 1;

    int n = (*rows) * (*cols);
    *A = malloc(n * sizeof(float));
    *B = malloc(n * sizeof(float));

    for (int i = 0; i < n; i++) {
        fscanf(fa, "%f", &(*A)[i]);
        fscanf(fb, "%f", &(*B)[i]);
    }

    fclose(fa);
    fclose(fb);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <n_threads> <k_rounds>\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int k = atoi(argv[2]);

    omp_set_num_threads(n_threads);

    int rows, cols;
    float *A = NULL, *B = NULL;

    /* ---------- I/O (not timed) ---------- */
    readMatrix(&rows, &cols,
               "matAlarge.txt", "matBlarge.txt",
               &A, &B);

    int n = rows * cols;
    float *C = malloc(n * sizeof(float));

    /* ---------- CSV filename ---------- */
    char csv_name[64];
    snprintf(csv_name, sizeof(csv_name),
             "benchmark_threads_%d.csv", n_threads);

    FILE *csv = fopen(csv_name, "w");
    fprintf(csv, "round,threads,time_sec\n");

    /* ---------- benchmark ---------- */
    for (int r = 1; r <= k; r++) {

        double t_start = omp_get_wtime();

        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            C[i] = A[i] + B[i];
        }

        double t_end = omp_get_wtime();
        double elapsed = t_end - t_start;

        fprintf(csv, "%d,%d,%.6f\n",
                r, n_threads, elapsed);
    }

    fclose(csv);

    free(A);
    free(B);
    free(C);

    return 0;
}