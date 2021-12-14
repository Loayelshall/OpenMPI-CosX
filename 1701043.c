#include <mpi.h>
#include <stdio.h>
#include <time.h>

long double factorial(long double n) {
    long double result = 1;  
    for (long double i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}
long double power(long double base, int exp) {
    long double result = 1;
    for (int i = 1; i <= exp; i++)
    {
        result *= base;
    }
    return result;
}

int main(int argc, char *argv[]) {
    int upperLimit;
    double x;
    int rank, size;
    long double cosX = 0.0; 
    long double time1, time2;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    setbuf(stdout, NULL);

    if (rank == 0) {
        printf("Enter the upper limit: ");
        scanf("%d", &upperLimit);
        printf("Enter the value of x: ");
        scanf("%lf", &x);
    }
    MPI_Bcast(&upperLimit, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    time1 = MPI_Wtime();
    for (long long i = rank; i <= upperLimit; i += size) {       
        cosX += (long double)((power(-1, i) * power(x, 2*i)) / (long double)(factorial(2 * i)));        
    }
    time2 = MPI_Wtime();

    long double globalCosX = 0.0;
    MPI_Reduce(&cosX, &globalCosX, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("\n");
        printf("The cosine of %lf is %Lf\n", x, globalCosX);
        printf("The time is %Lf Seconds\n", time2 - time1);
        printf("\n");
    }

    MPI_Finalize();
    
    // Sequential code
    if (rank == 0) {
        long double cosX_seq = 0.0;
        clock_t t;
        t = clock();
        for (int i = 0; i <= upperLimit; i++)
        {
            cosX_seq += (long double)((power(-1, i) * power(x, 2*i)) / (long double)(factorial(2 * i)));
        }
        t = clock() - t;
        
        printf("\n");
        printf("The sequential cosine of %lf is %Lf\n", x, cosX_seq);
        printf("The time is %lf Seconds\n", ((double)t)/CLOCKS_PER_SEC);
    }  
}



    
