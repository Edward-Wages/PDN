all: make_cluster

# alter this
make_cluster: kmeans_clustering.c
	gcc -fopenmp -g -Wall -o kmeans_clustering kmeans_clustering.c -lm -std=c99
	

clean:
	$(RM) kmeans_clustering


# Below are commands to help you run your program easily.
# You can alter them to run multiple things at once.
run: run_cluster

# alter this
run_cluster:
	./kmeans_clustering 1000000 points_1M.csv 16 initial_centroids.csv output.csv time.csv 1