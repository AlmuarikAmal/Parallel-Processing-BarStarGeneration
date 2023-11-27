#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DEFAULT_SIZE 20
#define ITERATIONS 10

int main(int argc, char* argv[]) {
    
    //initialize the mpi environment
    MPI_Init(&argc, &argv);

    int max_range = DEFAULT_SIZE;
    int rank, size;

    //get the number of processes
     MPI_Comm_size(MPI_COMM_WORLD, &size);

    //get the rank of the processes
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double Duration;
    double iteration_times[ITERATIONS];

    for (int iteration = 0; iteration < ITERATIONS; iteration++) {
        
        int values[max_range];
        
        if (rank == 0) {
            //printf("Iteration %d - Generated Dataset: ", iteration + 1);
            for (int i = 0; i < max_range; i++) {
                values[i] = (rand() % max_range) + 1;
                //printing the values of array values
                //printf("%d ", dataset[i]);
            }
            printf("\n");
        }
        // Broadcast the array values to all processes
        MPI_Bcast(values, max_range, MPI_INT, 0, MPI_COMM_WORLD);
        
        //initialize startTime as current time
        double startTime = MPI_Wtime();
        
        int elementsPerProcessor = max_range / size; // Assuming equal distribution
        int extra_elements  = max_range % size; // remaining elements
        int start_local = rank * elementsPerProcessor + (rank < extra_elements  ? rank : extra_elements );
        int end_local = start_local + elementsPerProcessor + (rank < extra_elements  ? 1 : 0);
        
        // Find the local maximum for each processor
        int local_max = values[start_local];
        for (int i = start_local + 1; i < end_local; i++)
            if (values[i] > local_max)
                local_max = values[i];
        
        // Reduce all local maximum to get the global maximum
        int globalMax;
        MPI_Reduce(&local_max, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        
        //Broadcast global max to all processes
        MPI_Bcast(&globalMax, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        //initialize a local counter array
        int Lcount[globalMax];
        for (int i = 0; i < globalMax; i++)
            Lcount[i] = 0;
        
        //count the ocurrences of each value in the local range
        for (int i = start_local; i < end_local; i++)
            Lcount[values[i] - 1]++;
        
        //initialize receive counter array
        int Rcount[size];
        for (int i = 0; i < size; i++)
            Rcount[i] = 0;
        //gather global max values to Rcount on P0
        MPI_Gather(&globalMax, 1, MPI_INT, Rcount, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        //initialize global counter array
        int Gcount[globalMax * size];
        for (int i = 0; i < globalMax * size; i++)
            Gcount[i] = 0;
        
        // gather Lcount to array Gcount
        MPI_Gather(Lcount, globalMax, MPI_INT, Gcount, globalMax, MPI_INT, 0, MPI_COMM_WORLD);
        
        if (rank == 0) {
            // Print the final bar chart from process 0
            printf("\n---Bar chart for Iteration %d---\n", iteration + 1);
            for (int i = 0; i < globalMax; i++) {
                printf("Data Point %d: ", i + 1);
                for (int j = 0; j < size; j++)
                    for (int k = 0; k < Gcount[j * globalMax + i]; k++)
                        printf("*");
                printf("\n");
            }
        }
        
        double EndTime = MPI_Wtime();
        //calculate time for each iteration
        Duration += EndTime - startTime;
        iteration_times[iteration] = EndTime - startTime;
        MPI_Barrier(MPI_COMM_WORLD);
        
        if (rank == 0) {
            for (int iteration = 0; iteration < ITERATIONS; iteration++) {
                printf("Time taken for iteration (%d) : %f seconds\n", iteration + 1, iteration_times[iteration]);
            }
        } // end loop iteration
    }
    if (rank == 0)
            printf("\nAverage execution time: %f seconds\n", Duration / ITERATIONS);
        
    MPI_Finalize();

    return 0;
}
