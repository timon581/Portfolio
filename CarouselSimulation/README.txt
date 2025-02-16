This project simulates a carousel (merry-go-round) ride using processes, semaphores, and shared memory in C. The simulation consists of visitors and a carousel, both synchronized to ensure proper boarding, riding, and exiting sequences.

Features:

	1) Process Synchronization: Uses semaphores to manage visitor and carousel actions.
	2) Shared Memory: Semaphores are stored in shared memory for inter-process communication.
	3) Parallel Execution: Implements multiple processes for concurrent execution.

How It Works:

1.Visitors wait for the carousel to allow boarding.
2.The carousel signals visitors to board.
3.Visitors take their seats and inform the carousel.
4.The carousel starts and stops the ride.
5.Visitors exit after the ride ends.
6.The process repeats for the next batch of visitors.

Compilation and execution:
gcc -o vrtuljak vrtuljak.c -pthread -lrt
./vrtuljak