This code implements Lamports algorithm on a simple 5 Philosopher problem. 
Links that explain Lamports algorithm: 
https://www.geeksforgeeks.org/lamports-algorithm-for-mutual-exclusion-in-distributed-system/
https://en.wikipedia.org/wiki/Lamport%27s_distributed_mutual_exclusion_algorithm

Each philosopher acts as a separate process in a distributed system. This means there is no shared memory between the philosophers and all communication and synchronization needs to be done trough methods like message queues, pipelines or similar methods. In this code, that is done trough message queues.

Use:
The given code will ONLY WORK IN LINUX. Windows and Mac do not have some of the required libraries.
Build the code : gcc -o lamport.exe lamport.c
Before running the code : ./clear_queues  (this clears the message queues if previously used)
Run the code: ./lamport
Stop the code: CTRL + C (the code works infinitely until stopped like this, will implement a in code stop later!)