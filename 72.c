#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 2

int main()
{
    MPI_Init(NULL, NULL);

    srand(time(NULL));
    int id, p, size;
    int arr[SIZE];
    int arr_t[SIZE];
    MPI_Comm comm;
    MPI_Comm icomm;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int color = id % 4;
    MPI_Comm_split(MPI_COMM_WORLD, color, id, &comm);
    int k = color % 2 == 0 ? color + 1 : color - 1;
    int tag = color < 2 ? 10 : 23;
    MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, k, tag, &icomm);

    int new_id, new_p;
    MPI_Comm_rank(comm, &new_id);
    MPI_Comm_size(comm, &new_p);

    for (int i = 0; i < SIZE; i++)
        arr[i] = color;
    
    MPI_Reduce(arr, arr_t, SIZE, MPI_INT, MPI_SUM, 0, comm);

    if(new_id == 0)
    {
        for (int i = 0; i < SIZE; i++)
            printf("Group %d elem %d\n", color, arr_t[i]);
        MPI_Send(arr_t, SIZE, MPI_INT, 0, 0, icomm);
        MPI_Recv(arr, SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, icomm, MPI_STATUS_IGNORE);
        for (int i = 0; i < SIZE; i++)
            printf("Group %d elem %d\n", color, arr[i]);
    }

    MPI_Finalize();
}
