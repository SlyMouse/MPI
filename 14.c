#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main()
{
    MPI_Init(NULL, NULL);

    int id, p, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(id == 1)
    {
        int arr[SIZE];
        for (int i = 0; i < SIZE; i++)
            arr[i] = i * 2;

        for (int i = 0; i < p; i++)
            if(i != id)
                MPI_Send(arr, SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status status;
        MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &size);

        int* arr_rec = (int*)malloc(sizeof(int) * size);
        MPI_Recv(arr_rec, size, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printf("Process %d recieved array : ", id);
        for (int i = 0; i < size; i++)
            printf("%d ", arr_rec[i]);
        printf("\n");
    }

    MPI_Finalize();
}
