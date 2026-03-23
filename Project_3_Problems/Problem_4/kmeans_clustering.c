#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>
#include <math.h> 
#include <float.h>
#include <stdbool.h>

#define MAXCHAR 25
#define DEBUG   1


int main(int argc, char* argv[]) 
{
    // Catch console errors
    if (argc != 8) 
    {
        printf("USE LIKE THIS: kmeans_clustering n_points points.csv n_centroids centroids.csv output.csv time.csv num_threads\n");
        exit(-1);
    }

    // points ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    int num_points = strtol(argv[1], NULL, 10);
    FILE* pointsFile = fopen(argv[2], "r");
    if (pointsFile == NULL) {
        printf("Could not open file %s", argv[2]);
        exit(-2);
    }

    // centroids ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    int num_centroids = strtol(argv[3], NULL, 10);
    FILE* centroidsFile = fopen(argv[4], "r");
    if (centroidsFile == NULL) {
        printf("Could not open file %s", argv[4]);
        fclose(pointsFile);
        exit(-3);
    }

    // output ~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    FILE* outputFile = fopen(argv[5], "w");
    FILE* timeFile = fopen(argv[6], "w");

    // threads ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    int num_threads = strtol(argv[7], NULL, 10);

    // array ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    double* points_x = malloc(num_points * sizeof(double));
    double* points_y = malloc(num_points * sizeof(double));

    // centroid array /////////////////////////////////////////
    double* centroids_x = malloc(num_centroids * sizeof(double));
    double* centroids_y = malloc(num_centroids * sizeof(double));



    // Store values ~~~~~~~~ //  
    // temporarily store values
    char str[MAXCHAR];

    // Storing point values //
    int k = 0;
    while (fgets(str, MAXCHAR, pointsFile) != NULL) {
        sscanf(str, "%lf,%lf", &(points_x[k]), &(points_y[k]));
        k++;
    }
    fclose(pointsFile);

    // Storing centroid values //
    k = 0;
    while (fgets(str, MAXCHAR, centroidsFile) != NULL) {
        sscanf(str, "%lf,%lf", &(centroids_x[k]), &(centroids_y[k]));;
        k++;
    }
    fclose(centroidsFile);



    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    // start time
    double start = omp_get_wtime();

    /* Given Pseudocode:
    Initialize the K centroids at the provided initial coordinates. 
    While not converged (i.e. the average moving distance of all the centroids in the last iteration is more than 1.0)
    1)	For every point, assign it to its nearest centroid (for loop 1)

    2)	For every centroid, compute its new location as the geometric center of its assigned data 
        points and then compute the moving distance by which it is moved from the previous location (for loop 2)

    3)	Compute the average moving distance by which all the centroids are moved in this iteration.
    */


    /* PROJECT SPECIFICATIONS:
    Second, please parallelize the two for loops as shown in the pseudo-code. Please follow the coding requirements below:
     - In all the OpenMP parallel directives, please specify the variable scope clauses 
     [i.e., default(none), shared(), private(), and reduction()]to explicitly declare the scope of every variable. 
     The parallelization overhead can be used by using private() to create private copies of variables for every thread, 
     instead of declaring private variables inside the parallel regions.

     - Please use work sharing constructs to minimize the parallelization overhead of repeatedly forking and joining threads. 
     You should use the omp parallel directive before the while loop and then use the omp for directives before the two for loops.
     You may also need to use other work sharing directives, such as single and barrier.
    
    */

    int* assignments = malloc(num_points * sizeof(int));
    bool converged = false;
    double total_moving_dist;

    #pragma omp parallel num_threads(num_threads) default(none) \
        shared(points_x, points_y, centroids_x, centroids_y, assignments, \
            num_points, num_centroids, converged, total_moving_dist)
    {
        while (!converged) 
        {
            // Reset distance at the start of iteration
            #pragma omp single
            total_moving_dist = 0.0;

            // Step 1: Assign points to centroids
            #pragma omp for
            for (int p = 0; p < num_points; ++p) 
            {
                double min_dist = DBL_MAX;
                int nearest_centroid = 0;
                for (int c = 0; c < num_centroids; ++c) 
                {
                    double dx = points_x[p] - centroids_x[c];
                    double dy = points_y[p] - centroids_y[c];
                    double dist = sqrt(dx*dx + dy*dy);
                    if (dist < min_dist) 
                    {
                        min_dist = dist;
                        nearest_centroid = c;
                    }
                }
                assignments[p] = nearest_centroid;
            }

            // Step 2: Compute new locations
            // We use reduction here to safely sum the distances across threads
            #pragma omp for reduction(+:total_moving_dist)
            for (int c = 0; c < num_centroids; ++c) 
            {
                double sum_x = 0.0, sum_y = 0.0;
                int count = 0;
                for (int p = 0; p < num_points; ++p) 
                {
                    if (assignments[p] == c) 
                    {
                        sum_x += points_x[p];
                        sum_y += points_y[p];
                        count++;
                    }
                }
                
                if (count > 0) 
                {
                    double new_x = sum_x / count;
                    double new_y = sum_y / count;
                    double dx = new_x - centroids_x[c];
                    double dy = new_y - centroids_y[c];
                    total_moving_dist += sqrt(dx*dx + dy*dy);

                    centroids_x[c] = new_x;
                    centroids_y[c] = new_y;
                }
            }

            // Step 3: Single thread checks convergence
            #pragma omp single
            {
                if ((total_moving_dist / num_centroids) <= 1.0) 
                {
                    converged = true;
                }
            }
            // Implicit barrier at the end of 'single' ensures all threads 
            // see the updated 'converged' value before starting the next while loop iteration.
        }
    }

    
    free(assignments);



    // end time
    double end = omp_get_wtime();
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



    // print time //
    double time_passed = end - start;
    fprintf(timeFile, "%f", time_passed);

    // print centroids //
    for (int c = 0; c < num_centroids; ++c) 
    {
        fprintf(outputFile, "%f, %f", centroids_x[c], centroids_y[c]);
        if (c != num_centroids - 1) fprintf(outputFile, "\n");
    }



    // close files //
    fclose(outputFile);
    fclose(timeFile);

    // free memory //
    free(points_x);
    free(points_y);
    free(centroids_x);
    free(centroids_y);

    // return //
    return 0;
}