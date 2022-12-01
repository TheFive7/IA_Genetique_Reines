#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

/**
 * Display the population
 * @param dimension : Dimension of the person
 * @param population : The Population
 * @param i : Indice of the person to display
 */
void displayPopulation(int dimension, vector<vector<int>> population, int i) {
    for (int k = 0; k < dimension; k++) {
        cout << population[i][k] << ' ';
    }
    cout << endl;
}

void displayPerson(int dimension, vector<int> person) {
    cout << "[ ";
    for (int i = 0; i < dimension; i++) {
        cout << person[i] << ' ';
    }
    cout << "]" << endl;
}

int current_time_nanoseconds(){
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_nsec;
}

void verifyPerson(int dimension, const vector<int>& person) {
    for (int i = 0; i < dimension; i++) {
        if (count(person.begin(), person.end(), i) != 1) {
            cout << "WARNING";
        }
    }
}

int main() {
    int nb_execution = 5000;
    int population_size = 1000;
    int dimension = 12;
    int mutate_probability = 20;

    mt19937 rng(current_time_nanoseconds());
    uniform_int_distribution<> random(1,100);

    vector<vector<int>> population (population_size);

    // Init random population
    vector<int> person (dimension);

    for (int i = 0; i < population_size; i++) {
        // Create person with 1 number of each
        for (int j = 0; j < dimension; j++) {
            person[j] = j;
        }

        // Shuffle
        for (int j = 0; j < dimension; j++) {
            // TODO: random number not really random
            int indice = random(rng) % dimension;
            int tampon;
            tampon = person[indice];
            person[indice] = person[j];
            person[j] = tampon;
        }
        population[i] = person;

        // Display population
        displayPopulation(dimension, population, i);
    }

//    cout << "AU DEPART: ";
//    displayPerson(dimension, population[0]);

    // NB ITERATIONS
    for (int n = 0; n < nb_execution; n++) {
        // Couple
        for (int i = 0; i < population_size; i+=2) {
            vector<int> person1 = population[i];
            vector<int> person2 = population[i + 1];

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

//        cout << "APRES COUPLE: ";
//        displayPerson(dimension, population[0]);

        // Mutation
        int indice, tampon;
        for (int i = 0; i < population_size; i++) {
            vector<int> person3 = population[i];
            if (random(rng) <= mutate_probability) {
                indice = random(rng) % dimension;

                tampon = person3[indice];
                person3[indice] = person3[person[indice]];
                person3[person[indice]] = tampon;

                population[i] = person3;
            }
        }

//        cout << "APRES MUTATION: ";
//        displayPerson(dimension, population[0]);

        // Evaluate
        int conflit;
        for (int i = 0; i < population_size; i++) {
            conflit = 0;
            person = population[i];

            for (int j = 0; j < dimension - 1; j++) {
                for (int k = j + 1; k < dimension; k++) {
                    if (abs(j - k) == abs(person[j] - person[k])) {
                        conflit++;
                    }
                }
            }

            // If it has any conflict and the person is not in the array
            if (conflit == 0) {
                cout << "BEST: ";
                displayPerson(dimension, person);
                exit(5);
            }
        }

        // Verify
//        for (int i = 0; i < population_size; i++) {
//            verifyPerson(dimension, population[i]);
//        }

        // Shuffle the population
        shuffle(begin(population), end(population), rng);
    }
}
