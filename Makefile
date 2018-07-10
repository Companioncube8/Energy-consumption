
all:
	gcc -pthread -Wall -o example example.c energylib.c -lcudart -lcuda -I/usr/local/cuda-9.1/include -lnvidia-ml

