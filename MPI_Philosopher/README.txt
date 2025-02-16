This project implements the classic Dining Philosophers Problem using MPI (Message Passing Interface) for process synchronization. The problem simulates philosophers sitting at a table, alternating between thinking and eating while ensuring they do not encounter deadlocks or starvation when taking shared forks. 

How it works: 

	1) Each philosopher is represented as a separate MPI process. The program ensures:
	2) Philosophers request forks from their neighbors when they want to eat.
	3) A philosopher can only eat when they have both forks.
	4) Forks are passed between philosophers using MPI message-passing.
	5) Used forks are marked as "dirty" and prioritized for return.

Prerequisites: 

	1) An MPI library installed (Windows: Microsoft MPI (MS-MPI); Linux: OpenMPI)
	2) A C++ compiler with MPI support (mpic++)

Compile code: mpic++ -o philosophers philosophers.cpp -lmpi
Run code: 
	1) Windows: mpiexec -n 5 philosophers.exe (number can be changed to any number of philosophers)
	2) Linux: mpirun -np 5 ./philosophers