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

    if (!Afile || !Bfile)
        return 1;

    fscanf(Afile, "%d %d", Arows, Acols);
    fscanf(Bfile, "%d %d", Brows, Bcols);

    int n = (*Arows) * (*Acols);
    *A = malloc(n * sizeof(float));
    *B = malloc(n * sizeof(float));

    for (int i = 0; i < n; i++)
    {
        fscanf(Afile, "%f", &(*A)[i]);
        fscanf(Bfile, "%f", &(*B)[i]);
    }

    fclose(Afile);
    fclose(Bfile);
    return 0;
}

int main (int argc, char *argv[]) { 
	int nt, rank;
	
	//without this, it will use max_thread as number of thread
	int num_thread;
	sscanf(argv[1], "%d", &num_thread);
	omp_set_num_threads(num_thread);

	int row,col,brow,bcol;
	float *A = NULL, *B = NULL;
	readMatrix(&row,&col,&brow,&bcol,"matAsmall.txt","matBsmall.txt",&A,&B);
	#pragma omp parallel private(nt, rank) 
	{ 
    		rank = omp_get_thread_num(); 
    		printf("Hello World from thread = %d\n", rank); 

    		if (rank == 0) { 
        		nt = omp_get_num_threads(); 
        		printf("Number of threads = %d\n", nt); 
    		} 
	} 

	free(A);
	free(B);
} 
