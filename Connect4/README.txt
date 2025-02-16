This project implements a Connect Four AI using Minimax with depth control, enhanced with MPI (Message Passing Interface) for parallelized computation. The program allows a human player to compete against an AI that evaluates the best moves efficiently, leveraging distributed computing.

Features:

	1) Connect Four gameplay (6-row, 7-column grid)
	2) Minimax-based AI with depth-based evaluation
	3) MPI parallelization for faster move calculations
	4) Human vs AI gameplay
	5) Win detection (vertical, horizontal, diagonal sequences)

Prerequisites:

	1) C++ Compiler (e.g., g++)
	2) MPI Library (e.g., OpenMPI, MPICH)

Compile code: mpic++ -o connect4 main.cpp board.cpp -lmpi

Run code: mpiexec -n <NUM_PROCESSES> ./connect4 <MASTER_DEPTH> <SLAVE_DEPTH>

<NUM_PROCESSES>: Number of MPI processes (e.g., 4, 8, etc.)
<MASTER_DEPTH>: Depth for master process evaluation
<SLAVE_DEPTH>: Depth for slave process evaluation

Example: mpiexec -n 4 ./connect4 5 3

How to Play:
The game will display the Connect Four board.
The human player is prompted to enter a column (1-7) for their move.
The AI will calculate and make its move.
The game continues until a player wins or the board is full.


Code structure:
.
├── board.h          # Board class definition
├── board.cpp        # Board implementation (game logic, move handling, win detection)
├── main.cpp         # Main game loop, AI decision-making, MPI logic
├── Makefile         # Optional makefile for compilation (if provided)
└── README.md        # This documentation

