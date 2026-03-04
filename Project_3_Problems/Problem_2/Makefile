in=in.fna # starter input file name, can set like "make run_starter in=other.fna"

all: make_atomic make_critical make_locks make_schedule

# duplicate this for other files
make_atomic: compute_average_TF_Exp1_atomic.c
	gcc -fopenmp -g -Wall -o compute_average_TF_Exp1_atomic compute_average_TF_Exp1_atomic.c -lm -std=c99
	
make_critical: compute_average_TF_Exp1_critical.c
	gcc -fopenmp -g -Wall -o compute_average_TF_Exp1_critical compute_average_TF_Exp1_critical.c -lm -std=c99

make_locks: compute_average_TF_Exp1_locks.c
	gcc -fopenmp -g -Wall -o compute_average_TF_Exp1_locks compute_average_TF_Exp1_locks.c -lm -std=c99

make_schedule: compute_average_TF_Exp2_schedule.c
	gcc -fopenmp -g -Wall -o compute_average_TF_Exp2_schedule compute_average_TF_Exp2_schedule.c -lm -std=c99

clean:
	$(RM) compute_average_TF_Exp1_atomic compute_average_TF_Exp1_critical compute_average_TF_Exp1_locks compute_average_TF_Exp2_schedule


# Below are commands to help you run your program easily.
# You will need to create more entries for your different files, such as for critical and locks.
run: run_atomic run_critical run_locks

# duplicate this for other files
run_atomic:
	./compute_average_TF_Exp1_atomic $(in) OUTPUT_atomic_1th.csv TIME_atomic_1th.csv 1
	./compute_average_TF_Exp1_atomic $(in) OUTPUT_atomic_2th.csv TIME_atomic_2th.csv 2
	./compute_average_TF_Exp1_atomic $(in) OUTPUT_atomic_4th.csv TIME_atomic_4th.csv 4
	./compute_average_TF_Exp1_atomic $(in) OUTPUT_atomic_8th.csv TIME_atomic_8th.csv 8

run_critical:
	./compute_average_TF_Exp1_critical $(in) OUTPUT_critical_1th.csv TIME_critical_1th.csv 1
	./compute_average_TF_Exp1_critical $(in) OUTPUT_critical_2th.csv TIME_critical_2th.csv 2
	./compute_average_TF_Exp1_critical $(in) OUTPUT_critical_4th.csv TIME_critical_4th.csv 4
	./compute_average_TF_Exp1_critical $(in) OUTPUT_critical_8th.csv TIME_critical_8th.csv 8

run_locks:
	./compute_average_TF_Exp1_locks $(in) OUTPUT_locks_1th.csv TIME_locks_1th.csv 1
	./compute_average_TF_Exp1_locks $(in) OUTPUT_locks_2th.csv TIME_locks_2th.csv 2
	./compute_average_TF_Exp1_locks $(in) OUTPUT_locks_4th.csv TIME_locks_4th.csv 4
	./compute_average_TF_Exp1_locks $(in) OUTPUT_locks_8th.csv TIME_locks_8th.csv 8

run_schedule:
	./compute_average_TF_Exp2_schedule $(in) OUTPUT_schedule_guided.csv TIME_schedule_guided.csv 1
	./compute_average_TF_Exp2_schedule $(in) OUTPUT_schedule_guided.csv TIME_schedule_guided.csv 2
	./compute_average_TF_Exp2_schedule $(in) OUTPUT_schedule_guided.csv TIME_schedule_guided.csv 4
	./compute_average_TF_Exp2_schedule $(in) OUTPUT_schedule_guided.csv TIME_schedule_guided.csv 8