all: kmeans_clustering

kmeans_clustering: kmeans_clustering.c
	gcc -fopenmp -g -Wall -o kmeans_clustering kmeans_clustering.c -lm -std=c99

clean:
	$(RM) kmeans_clustering


run: run_1_mil run_4_mil run_16_mil

run_1_mil:
	./kmeans_clustering 1000000 "../../test_data/Problem_4/points_1M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_1M_1th.csv tim_kmeans_clustering_1M_1th.csv 1
	./kmeans_clustering 1000000 "../../test_data/Problem_4/points_1M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_1M_4th.csv tim_kmeans_clustering_1M_4th.csv 4
	./kmeans_clustering 1000000 "../../test_data/Problem_4/points_1M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_1M_16th.csv tim_kmeans_clustering_1M_16th.csv 16

run_4_mil:
	./kmeans_clustering 4000000 "../../test_data/Problem_4/points_4M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_4M_1th.csv tim_kmeans_clustering_4M_1th.csv 1
	./kmeans_clustering 4000000 "../../test_data/Problem_4/points_4M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_4M_4th.csv tim_kmeans_clustering_4M_4th.csv 4
	./kmeans_clustering 4000000 "../../test_data/Problem_4/points_4M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_4M_16th.csv tim_kmeans_clustering_4M_16th.csv 16

run_16_mil:
	./kmeans_clustering 16000000 "../../test_data/Problem_4/points_16M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_16M_1th.csv tim_kmeans_clustering_16M_1th.csv 1
	./kmeans_clustering 16000000 "../../test_data/Problem_4/points_16M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_16M_4th.csv tim_kmeans_clustering_16M_4th.csv 4
	./kmeans_clustering 16000000 "../../test_data/Problem_4/points_16M.csv" 16 "../../test_data/Problem_4/centroids.csv" res_kmeans_clustering_16M_16th.csv tim_kmeans_clustering_16M_16th.csv 16