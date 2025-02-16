This Java program implements and compares three different search algorithms for pathfinding in a graph:

	1) Breadth-First Search (BFS)
	2) Uniform-Cost Search (UCS)
	3) A Search (A)**

Additionally, the program includes heuristic verification methods: Optimistic Heuristic Check and Consistent Heuristic Check.

Run program : java ui.Solution --alg <algorithm> --ss <state-space-file> [--h <heuristic-file>] [--check-optimistic] [--check-consistent]

Arguments:

--alg: Specifies the search algorithm (bfs, ucs, or astar).

--ss: Path to the state-space file.

--h: Path to the heuristic file (required for A* search).

--check-optimistic: Checks if the heuristic is optimistic.

--check-consistent: Checks if the heuristic is consistent.

Example Usage: java ui.Solution --alg astar --ss input.txt --h heuristic.txt --check-optimistic


Input File Format

	State-Space File (input.txt)

	First line: Start state.

	Second line: Goal states (space-separated).

	Following lines: State transitions in the format:

	State: Successor1,Cost1 Successor2,Cost2 ...

	Heuristic File (heuristic.txt) (Only for A*)

	Each line follows the format: "State: HeuristicValue"

Output Format

	[FOUND_SOLUTION]: yes/no

	[STATES_VISITED]: <number>

	[PATH_LENGTH]: <number>

	[TOTAL_COST]: <number>

	[PATH]: <solution path>

	Heuristic check results (if applicable).