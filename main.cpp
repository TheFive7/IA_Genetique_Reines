#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

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
vector<vector<int>> couplePopulation(int dimension, vector<vector<int>> population, int couple_probability) {
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
    for (auto &p : population) {
        vector<int> newPerson = p;
        if (randomNumber(rng) <= mutate_probability) {
            indice = randomNumber(rng) % dimension;

            tampon = newPerson[indice];
            newPerson[indice] = newPerson[person[indice]];
            newPerson[person[indice]] = tampon;

            p = newPerson;
        }
    }
    return population;
}

/**
 * Evaluate a person and return its conflict.
 * @param dimension : Dimension
 * @param person : The person
 * @param conflict : The number of conflict
 * @return : Conflict
 */
int evaluatePerson(int dimension, vector<int> person, int conflict) {
    conflict = 0;
    for (int j = 0; j < dimension - 1; j++) {
        for (int k = j + 1; k < dimension; k++) {
            if (abs(j - k) == abs(person[j] - person[k])) {
                conflict++;
            }
        }
    }
    return conflict;
}

/**
 * Evaluate the population, if it detect the best person, it exit the program and display the best
 * @param dimension : Dimension
 * @param population : Population
 * @param nb_execution : The number of execution
 * @return : The population
 */
vector<vector<int>> evaluatePopulation(int dimension, vector<vector<int>> population, int nb_execution) {
    int conflict;
    for (auto &p : population) {
        conflict = evaluatePerson(dimension, p, conflict);

        // If it has any conflict and the person is not in the array
        if (conflict == 0) {
            cout << "\n     BEST     : ";
            displayPerson(dimension, p);
            exit(nb_execution);
        }
    }
    return population;
}

int main() {
    int nb_execution = 10000;
    int population_size = 10;
    int mutate_probability = 10;
    int couple_probability = 30;
    int dimension = 8;

    vector<int> person (dimension);
    vector<vector<int>> population (population_size);

    // Init random population
    population = initPopulation(dimension, person, population);

    cout << "AU DEPART: ";
    displayPerson(dimension, population[0]);

    // NB ITERATIONS
    for (int n = 0; n < nb_execution; n++) {
        // Couple
        population = couplePopulation(dimension, population, couple_probability);

        cout << "APRES COUPLE  : ";
        displayPerson(dimension, population[0]);

        // Mutation
        population = mutatePopulation(dimension, person, population, mutate_probability);

        cout << "APRES MUTATION: ";
        displayPerson(dimension, population[0]);

        // Evaluate
        population = evaluatePopulation(dimension, population, n);

        // Verify
//        for (int i = 0; i < population_size; i++) {
//            checkPerson(dimension, population[i]);
//        }

        // Shuffle the population
        shuffle(begin(population), end(population), rng);
    }
}
