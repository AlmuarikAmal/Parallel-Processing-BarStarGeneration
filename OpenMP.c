#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

# define max_range 2000 // assumption regarding the array size
# define iter 10 // to run the code 10 times

double generatePBarChart(int size, int* values, int num_threads) {
    double end , start;
/*------------------------- paralyze finding max ------------------------------------*/
    // Find the maximum value in the dataset
    int max_val = values[0];
    //int count = 0 ;
    start = omp_get_wtime(); /* start time */
    #pragma omp parallel for reduction(max:max_val) num_threads(num_threads)
        for (int i = 0; i < size; i++) {
           //if (omp_get_thread_num() == 3)
           //count++;
            if (values[i] > max_val) {
                max_val = values[i];
            }
        }
    
    //printf("THE COUNT IS %d\n", count); // division is correct as expected
    //printf("The maximum values is : %d\n", max_val); // output correct value here
    
    int* frequency = (int*)calloc(max_val + 1, sizeof(int));
    
/*------------------------- paralyze frequency count ------------------------------------*/
   
    // Count the frequency of each value in the dataset
    #pragma omp parallel for num_threads(num_threads)
        for (int i = 0; i < max_val; i++){
            // division is correct as expected
            //if(omp_get_thread_num() == 3)
            // count++;
            #pragma omp atomic
            frequency[values[i]]++;
        }
    //printf("THE COUNT IS %d\n", count);
    // output correct but there exist cells have zero so filter needed before printing
    //for(int i = 0; i < size; i++) {
        //if(frequency[i] > 0) {
          //  printf("The frequency of value %d is: %d\t",values[i],frequency[i]);
        //}
    //}
    end = omp_get_wtime(); /* end time */
/*------------------------- display on screen ------------------------------------*/

    // show bar chart
    printf("\n- - - Bar chart - - -\n");
    //# pragma omp parallel for num_threads(num_threads)
        for(int i = 1; i <= max_val; i++) {
            if(frequency[i] >= 0) {
                printf("\nData Point %d: ", i);
                for(int j = 0; j < frequency[i]; j++)
                    printf("*");
        } // end if
    }
    printf("\n");
    
    free(frequency);
    return end - start;
}

int main() {
    
    double end, start;
    /*---------------------------------------------------------------------------------*/
    
    // Prompt user to enter dataset size
    //int size;
    //printf("Enter the dataset size: ");
    //scanf("%d", &size);
    
    /*---------------------------------------------------------------------------------*/
    
    // Prompt user to enter number of threads
    int num_threads;
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    /*---------------------------------------------------------------------------------*/
    
    // Prompt user to enter dataset values
    //int values[size];
    //printf("Enter the dataset values (separated by spaces): ");
    //    for(int i = 0; i < size; i++) {
    //        scanf("%d", &values[i]);
    //}
    /*---------------------------------------------------------------------------------*/
    
    /* generate bar chart for user selected size */
    //start = omp_get_wtime();
    //generatePBarChart(size, values, num_threads);
    //end = omp_get_wtime();
    //printf("time taken by small size array: %f seconds\n",end - start);
    /*---------------------------------------------------------------------------------*/
    
    // Generate an array of 1000 random values and display the bar chart
    int random_values[max_range];
    
    srand(24); /* generate seed value static for fair comparison between sequential and parallel*/

    // generate random values between 1 and 50 for readability
    for(int i = 0; i < max_range; i++) {
        random_values[i] = rand() % max_range + 1;
    }
    /*---------------------------------------------------------------------------------*/
    // generate bar chart for large sizes
    double sum;
    double elapsed_time[iter+1];
    
    //#pragma omp parallel for
        for(int i = 0; i< iter ; i++){
            //start = omp_get_wtime(); /* start time */
            elapsed_time [i] = generatePBarChart(max_range, random_values, num_threads);
            //end = omp_get_wtime(); /* end time */
            //elapsed_time [i] = end - start;
        }
    //#pragma omp parallel reduction(+:sum)
        for (int j = 0 ; j< iter ; j++){
            sum += elapsed_time[j];
        }
    
    for (int j = 0 ; j< iter; j++){
       printf("Time taken for iteration (%d) : %f\n", j ,elapsed_time[j]);
        
    }
    // print average time taken over the 10 iterations
    printf("average time taken: %f seconds\n",sum/iter);
    
    return 0;
}
