This project implements the resolution algorithm for propositional logic in Java. It supports logical inference using the resolution principle and allows users to verify the truthfulness of logical statements.

Features:

	1) Implements resolution-based theorem proving
	2) Supports negation, tautology elimination, and clause resolution
	3) Reads input from files for logical clauses and queries
	4) Allows users to add or remove clauses dynamically

How It Works:

Clause Processing: The program reads clauses from a file and applies preprocessing techniques, such as removing tautologies and redundant clauses.
Negation Handling: Converts logical statements into negated normal form.
Resolution Strategy:
	1) Applies resolution rule on pairs of clauses.
	2) Continues until either an empty clause (NIL) is derived (indicating proof) or no further resolution is possible.
Inference Output: If a contradiction is found, the goal statement is proven true; otherwise, its truthfulness remains unknown.

Input Files: Stored in files/ directory

Key Methods:

Ispis(...) - Displays the derivation steps

Provjera(...) - Checks for duplicate clauses

Negacija(...) - Returns the negation of a clause

Resolve(...) - Resolves two clauses using the resolution rule

Tautologija(...) - Checks for tautological clauses

Trim(...) - Removes duplicate literals within a clause

StrategijaBrisanja(...) - Optimizes the set of clauses

Resolution(...) - Main resolution algorithm

Run code: java Solution resolution <clauses_file> or java Solution cooking <clauses_file> <input_file>

<clauses_file>: Contains the knowledge base in CNF (Conjunctive Normal Form)
<input_file>: Contains user queries (? for inference, + to add, - to remove clauses)

Input file:
A v B
~B v C
~C
A ?

Expected output:
[CONCLUSION]: A is true

Files used are in files folder.