#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define POINTS 300000000

int main()
{
    MPI_Init(NULL, NULL);
    int id, p;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    double sum = 0.0;
    unsigned long counter;
    unsigned long counter_local;
    unsigned long size = POINTS / p;
    unsigned seed = id;

    for (int d = 2; d <= 21; d++)
    {
        counter = 0;
        counter_local = 0;
       
        for (unsigned long i = 0; i < size; i++)
        {
            sum = 0;

            for(int c = 0; c < d; c++)
                sum += pow((double)rand_r(&seed) / RAND_MAX, 2);
            
            if(sum < 1)
                counter_local++;
        }

        MPI_Reduce(&counter_local, &counter, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

        if(id == 0)
            printf("Volume of %d-dimensional ball is: %f\n", d, pow(2, d) * ((double)counter / POINTS));
    }

    MPI_Finalize();
}


