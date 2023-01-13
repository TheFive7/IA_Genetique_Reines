# Queens puzzle with genetic algorithm
A problem where we must place n queens on a n * n chessboard with a lot of rule (Check the wikipedia link). 

* Project realised by HENNEQUIN Maxime
* December 2022 & January 2023
* GitHub: https://github.com/TheFive7/IA_Genetique_Reines
*
* Files:
* reine.cpp: Program without parallel processing
* reine_MPI.cpp: Programm with parallel processing
* algo_reine.png: Image of subject
* couple_mutation.png: Explanation of couple and mutation
*
* Queens puzzle with parallel processing
* https://en.wikipedia.org/wiki/Eight_queens_puzzle
*
* To code with MPI: Google colab
* https://colab.research.google.com/
*
* Compilation
* !mpicxx -o reine_MPI -fopenmp reine_MPI.cxx
*
* Run with 3 process
* !mpirun --allow-run-as-root -np 3 ./reine_MPI
