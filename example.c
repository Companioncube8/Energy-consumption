#include <stdio.h>
#include <nvml.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <wait.h>
#include "energylib.h"

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}


int main()
{
	int pid, result;
	double energy, sum_energy = 0;
	double time_start, time_end, t1;
	time_start = wtime();
	pid = fork();
	if (pid == 0){
			execl("/home/cube/Загрузки/rodinia_3.1/cuda/lud/cuda/lud_cuda","/home/cube/Загрузки/rodinia_3.1/cuda/lud/cuda/lud_cuda" , "-i", "/home/cube/Загрузки/rodinia_3.1/data/lud/64.dat", NULL);		
		} else{
		t1 = wtime();
		while(1){
			if (waitpid(0, NULL, WNOHANG) != 0)
			{
				time_end = wtime();
				sum_energy += energy * (time_end - t1);
				printf("%f\n", energy * (time_end - t1));
				printf("t = %.4f\n", time_end - time_start);
				printf("total energy = %.4f\n", sum_energy);
				return 0;
			}
			result = energy_consuption(&energy, 1);
			if (wtime() - t1 >= 1){
				printf("\n%f\n", energy);
				t1 = wtime();
				sum_energy+=energy;
			}
		}
	}
	return 0;
}
