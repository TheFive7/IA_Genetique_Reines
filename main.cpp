#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

int main() {
    int nb_execution = 50;
    int population_size = 50;
    int dimension = 10;
    int mutate_probability = 20;

    vector<vector<int>> best_persons;
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
            int indice = rand() % dimension;
            int tampon = person[indice];
            person[indice] = person[j];
            person[j] = tampon;
        }
        population[i] = person;

        // Display population
        // displayPopulation(dimension, population, i);
    }

    // NB ITERATIONS
    for (int n = 0; n < nb_execution; n++) {
        // TODO: Probleme (doublon si dimension = 10)
        // Couple
//        for (int i = 0; i < population_size; i+=2) {
//            vector<int> person1 = population[i];
//            vector<int> person2 = population[i + 1];
//
//            int moved, crushed;
//            for (int j = 0; j < dimension; j++) {
//                moved = person1[j];
//                crushed = person2[j];
//
//                if (person1[j] == crushed)
//                    person1[j] = moved;
//
//                if (person2[j] == moved)
//                    person1[j] = crushed;
//
//                person1[j] = crushed;
//                person2[j] = moved;
//            }
//
//            population[i] = person1;
//            population[i + 1] = person2;
//        }

        // Mutation
        int indice, tampon;
        for (int i = 0; i < population_size; i++) {
            person = population[i];
            if (rand() % 100 <= mutate_probability) {
                indice = rand() % dimension;

                tampon = person[indice];
                person[indice] = person[person[indice]];
                person[person[indice]] = tampon;

                population[i] = person;
            }
        }

        // Evaluate
        // TODO: Ne resoud pas pour dimension = 10
        int conflit;
        for (int i = 0; i < population_size; i++) {
            conflit = 0;
            person = population[i];

            for (int j = 0; j < dimension; j++) {
                if (abs(j - (j + 1) % dimension) == abs(person[j] - person[(j + 1) % dimension])) {
                    conflit++;
                }
            }

            // If it has any conflict and the person is not in the array
            if (conflit == 0) {
                cout << "BEST: [ ";
                for (int j = 0; j < dimension; j++) {
                    cout << person[j] << ' ';
                }
                cout << ']' << endl;
                exit(5);

/*                // Add only if person is not yet in the array best_person
                if (best_persons.empty()) {
                    best_persons.push_back(person);
                } else {
                    bool same;
                    for (auto & best_person : best_persons) {
                        same = false;
                        for (int k = 0; k < dimension; k++) {
                            if (person[k] == best_person[k])
                                same = true;
                        }
                        if (!same) {
                            cout << "BEST: [ ";
                            for (int j = 0; j < dimension; j++) {
                                cout << person[j] << ' ';
                            }
                            cout << ']' << endl;

                            best_persons.push_back(person);
                        }
                    }
                }*/
            }
        }

/*        // Print best persons
        for (auto & best_person : best_persons) {
            cout << "TAB: ";
            for (int k = 0; k < dimension; k++) {
                cout << best_person[k] << ' ';
            }
            cout << endl;
        }

        cout << best_persons.size() << endl;*/
    }
}
