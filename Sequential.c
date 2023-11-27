#include <stdio.h>
#include <stdlib.h>
#include <time.h>

# define max_range 2000 // assumption regarding the array size
# define iter 10 // to run the code 10 times

void generateBarChart(int size, int* values) {
    int max = values[0];
    
    // Find the maximum value in the dataset
    for (int i = 1; i < size; i++) {
        if (values[i] > max) {
            max = values[i];
        }
    }
    
    int* frequency = (int*)calloc(max + 1, sizeof(int));
    
    // Count the frequency of each value in the dataset
    for(int i = 0; i < size; i++) {
        frequency[values[i]]++;
    }
    
    // Display the bar chart
    printf("--- Bar chart ---\n");
    for(int i = 1; i <= max; i++) {
        if(frequency[i] >= 0) {
            printf("Data Point %d: ", i);
            for(int j = 0; j < frequency[i]; j++) {
                printf("*");
            }
            printf("\n");
        }
    }
    
    free(frequency);
}

int main() {
    int size;
    clock_t t;
    
/*    // Prompt user to enter dataset size
    printf("Enter the dataset size: ");
    scanf("%d", &size);
    
    int* values = (int*)malloc(size * sizeof(int));
    
    // Prompt user to enter dataset values
    printf("Enter the dataset values (separated by spaces): ");
    for(int i = 0; i < size; i++) {
        scanf("%d", &values[i]);
    }
        t= clock();
        generateBarChart(size, values);
        t= clock() -t; 
        printf("time taken: %f seconds\n",((double) t)/CLOCKS_PER_SEC);
    
    
    free(values);
 */
 
    // Generate an array of 2000 random values and display the bar chart
    int random_values[max_range];
    srand(24);
    
    for(int i = 0; i < max_range; i++) {
        random_values[i] = rand() % max_range + 1; // Generates random values between 1 and max_range
    }
    
    double sum;
    double elapsed_time[iter+1];
    
    for(int i = 0; i< iter ; i++){
        t= clock();
        generateBarChart(max_range, random_values);
        elapsed_time [i]= clock() -t;
    }
    
    for (int i = 0 ; i< iter ; i++){
        sum += elapsed_time[i];
    }
    
    for (int i = 0 ; i< iter; i++){
        printf("Time taken for iteration (%d) : %f\n", i ,elapsed_time[i]/CLOCKS_PER_SEC);
    }
    // print average time taken over the 10 iterations
    printf("average time taken: %f seconds\n",(sum/iter)/CLOCKS_PER_SEC);
     
    return 0;
    
}

