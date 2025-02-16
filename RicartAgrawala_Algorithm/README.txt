This code implements Ricard Agrawala's algorithm on a simple 5 Philosopher problem. 
Links that explain Ricard Agrawala's algorithm: 
https://www.rcet.org.in/uploads/academics/rohini_98619579057.pdf
https://en.wikipedia.org/wiki/Ricart%E2%80%93Agrawala_algorithm

Each philosopher acts as a separate process in a distributed system. This means there is no shared memory between the philosophers and all communication and synchronization needs to be done trough methods like message queues, pipelines or similar methods. In this code, that is done trough pipelines.

Use:
The given code will ONLY WORK IN LINUX. Windows and Mac do not have some of the required libraries.
Build the code : gcc -o ricard.exe ricard.c
Run the code: ./ricard
Stop the code: CTRL + C (the code works infinitely until stopped like this, will implement a in code stop later!)