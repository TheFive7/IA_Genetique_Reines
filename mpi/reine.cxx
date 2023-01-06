#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

// COMPIL & RUN
// !mpicxx -o reine -fopenmp reine.cxx
// !mpirun --allow-run-as-root -np 3 ./reine

/**
 * Project realised by HENNEQUIN Maxime
 * December 2022 & January 2023
 *
 * Checkers algorithm with parallel processing
 */


// RANDOM
int current_time_nanoseconds(){
    struct timespec tm{};
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_nsec;
}

mt19937 rng(current_time_nanoseconds());
uniform_int_distribution<> randomNumber(1,100);


/**
 * Display the population
 * @param dimension : Dimension of the person
 * @param population : The Population
 */
void displayPopulation(int dimension, vector<vector<int>> population) {
    for (auto & i : population) {
        for (int k = 0; k < dimension; k++) {
            cout << i[k] << ' ';
        }
        cout << endl;
    }
}

/**
 * Display a person
 * @param dimension : Dimension of a person
 * @param person : The person
 */
void displayPerson(int dimension, vector<int> person) {
    cout << "[ ";
    for (int i = 0; i < dimension; i++) {
        cout << person[i] << ' ';
    }
    cout << "]" << endl;
}

/**
 * Check if a person contain one a each value
 * @param dimension : Dimension
 * @param person : The person
 */
void checkPerson(int dimension, const vector<int>& person) {
    for (int i = 0; i < dimension; i++) {
        if (count(person.begin(), person.end(), i) != 1) {
            cout << "WARNING";
        }
    }
}

/**
 * Initialize the population
 * @param dimension : Dimension
 * @param person : The person
 * @param population : Population
 * @return : The population initialized
 */
vector<vector<int>> initPopulation(int dimension, vector<int> person, vector<vector<int>> population) {
    for (auto & i : population) {
        // Create person with 1 number of each
        for (int j = 0; j < dimension; j++) {
            person[j] = j;
        }

        // Shuffle
        for (int j = 0; j < dimension; j++) {
            int indice = randomNumber(rng) % dimension;
            int tampon;
            tampon = person[indice];
            person[indice] = person[j];
            person[j] = tampon;
        }
        i = person;

        // Display population
        // displayPopulation(dimension, population);
    }
    return population;
}

/**
 * Couple 2 persons to create 2 children, a mix between their parent, then kill the two parent
 * @param dimension : Dimension
 * @param population : Population
 * @param couple_probability : Probability of couple
 * @return : The new array with the created children by their respective parent
 */
vector<vector<int>> couple(int dimension, vector<vector<int>> population, int couple_probability) {
    for (int i = 0; i < population.size(); i+=2) {
        if (randomNumber(rng) <= couple_probability) {
            vector<int> person1 = population[i];
            vector<int> person2 = population[i + 1];

            // Method to couple
            int moved, crushed;
            for (int j = 0; j < dimension; j++) {
                moved = person1[j];
                crushed = person2[j];

                if (person1[j] == crushed)
                    person1[j] = moved;

                if (person2[j] == moved)
                    person1[j] = crushed;

                person1[j] = crushed;
                person2[j] = moved;
            }

            population[i] = person1;
            population[i + 1] = person2;
        }
    }

    return population;
}

/**
 * Mutate person (determined by the probability of mutation)
 * @param dimension : Dimension
 * @param person : The person
 * @param population : Population
 * @param mutate_probability : Probability (on 100) that a person mutate
 * @return : The mutated population
 */
vector<vector<int>> mutatePopulation(int dimension, vector<int> person, vector<vector<int>> population, int mutate_probability) {
    int indice, tampon;
    for (auto & i : population) {
        vector<int> newPerson = i;
        if (randomNumber(rng) <= mutate_probability) {
            indice = randomNumber(rng) % dimension;

            tampon = newPerson[indice];
            newPerson[indice] = newPerson[person[indice]];
            newPerson[person[indice]] = tampon;

            i = newPerson;
        }
    }
    return population;
}

/**
 * Evaluate the population, if it detect the best person, it exit the program and display the best
 * @param dimension : Dimension
 * @param person : The person
 * @param population : Population
 * @param nb_execution : The number of execution
 * @return : The population
 */
vector<vector<int>> evaluate(int dimension, vector<int> person, vector<vector<int>> population, int nb_execution) {
    int conflit;
    for (auto & i : population) {
        conflit = 0;
        person = i;

        for (int j = 0; j < dimension - 1; j++) {
            for (int k = j + 1; k < dimension; k++) {
                if (abs(j - k) == abs(person[j] - person[k])) {
                    conflit++;
                }
            }
        }

        // If it has any conflict and the person is not in the array
        if (conflit == 0) {
            cout << "\n     BEST     : ";
            displayPerson(dimension, person);

            // When a process find the best solution, it send a message to stop all process and exit the program
            MPI_Abort(MPI_COMM_WORLD, nb_execution);
            
            exit(nb_execution);
        }
    }
    return population;
}

int main(int argc ,char *argv[]) {
    int myrank, numprocs, tag = 100;
    int send_at_iteration = 10;
    int send_nb_person = 10; // Must be lower than population_size !

    MPI_Status status;
    MPI_Init (&argc ,&argv);
    MPI_Comm_size(MPI_COMM_WORLD ,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD ,&myrank);

    int nb_execution = 100000;
    int population_size = 30;
    int mutate_probability = 10;
    int couple_probability = 30;
    int dimension = 12;

    vector<int> person (dimension); // Tab of a person
    vector<vector<int>> population (population_size); // Tab of all person (the population)

    // Init random population
    population = initPopulation(dimension, person, population);

    // Display the first person of the population
    cout << "AT THE BEGINNING: ";
    displayPerson(dimension, population[0]);

    // Number of iterations
    for (int n = 0; n < nb_execution; n++) {
        // Couple
        population = couple(dimension, population, couple_probability);

        // cout << "AFTER COUPLE  : ";
        // displayPerson(dimension, population[0]);

        // Mutation
        population = mutatePopulation(dimension, person, population, mutate_probability);

        // cout << "AFTER MUTATION: ";
        // displayPerson(dimension, population[0]);

        // Evaluate
        population = evaluate(dimension, person, population, n);

        // cout << "AFTER EVAL    : ";
        // displayPerson(dimension, population[0]);

        // Verify
        // for (int i = 0; i < population_size; i++) {
        //     checkPerson(dimension, population[i]);
        // }

        // SELECTION, MPI SEND AND RECV
        if (n % send_at_iteration == 0) {
            vector<vector<int>> selection (send_nb_person); // Tab of person to send
            vector<vector<int>> conflit_at_indice (population.size()); // [[conflit, indice], ...]
            int conflit;

            // Calculate the conflict of each person
            for (int i = 0; i < population.size(); i++) {
                conflit = 0;
                person = population[i];
                for (int j = 0; j < dimension - 1; j++) {
                    for (int k = j + 1; k < dimension; k++) {
                        if (abs(j - k) == abs(person[j] - person[k])) {
                            conflit ++;
                        }
                    }
                }
                conflit_at_indice[i] = { conflit, i };
            }

            // Sort the values to have the best in first
            sort(conflit_at_indice.begin(),conflit_at_indice.end());

            // Send person to next process (and receive from previous process)
            for (int i = 0; i < send_nb_person; i++) {
                selection[i] = population[conflit_at_indice[i][1]];

                MPI_Send (&selection[i][0], dimension, MPI_INT, (myrank + 1) % numprocs, tag, MPI_COMM_WORLD);
                MPI_Recv (&selection[i][0], dimension, MPI_INT, (myrank + (numprocs - 1)) % numprocs, tag, MPI_COMM_WORLD, &status);

                population[conflit_at_indice[i][1]] = selection[i];
            }
        }


        // Shuffle the population
        shuffle(begin(population), end(population), rng);
    }
    MPI_Finalize();
}
