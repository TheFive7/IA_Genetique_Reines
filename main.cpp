#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Display the population
 * @param dimension : Dimension of the person
 * @param population : The Population
 * @param i : Indice of the person to display
 */
void displayPopulation(int dimension, int* population[], int i) {
    for (int k = 0; k < dimension; k++) {
        cout << population[i][k] << ' ';
    }
    cout << endl;
}

/**
 * Initialize the population
 * @param dimension : Dimension of the person
 * @param population_size : Size of the maximum person in the population
 * @param population : The Population
 */
void initPopulation(int dimension, int population_size, int* population[]) {
    int person[dimension];

    for (int i = 0; i < population_size; i++) {
        // Create person with 1 number of each
        for (int j = 0; j < dimension; j++) {
            person[j] = j;
        }

        // Shuffle
        for (int j = 0; j < dimension; j++) {
            int indice = rand() % dimension;
            int tampon;
            tampon = person[indice];
            person[indice] = person[j];
            person[j] = tampon;

        }
        population[i] = person;

        // Display population
        displayPopulation(dimension, population, i);
    }
}

/**
 * Couple all person 2 by 2
 * @param dimension : Dimension of the person
 * @param population_size : Size of the maximum person in the population
 * @param population : The Population
 */
void couplePopulation(int dimension, int population_size, int* population[]) {
    for (int i = 0; i < population_size; i+=2) {
        int *person1 = population[i];
        int *person2 = population[i + 1];

        int moved = person1[i];
        int crushed = person2[i];

        for (int j = 0; j < dimension; j++) {
            if (person1[j] == crushed)
                person1[j] = moved;
            if (person2[j] == moved)
                person1[j] = crushed;
        }

        person1[i] = crushed;
        person2[i] = moved;

        population[i] = person1;
        population[i + 1] = person2;
    }
}

/**
 * Mutate random person in the population
 * @param dimension : Dimension of the person
 * @param population_size : Size of the maximum person in the population
 * @param population : The Population
 * @param mutate_probability : Mutation probability (in percent)
 */
void mutate(int dimension, int population_size, int* population[], int mutate_probability) {
    int indice, tampon, replace_nb;
    for (int i = 0; i < population_size; i++) {
        int *person = population[i];
        if (rand() % 100 <= mutate_probability) {
            indice = rand() % dimension;
            tampon = person[indice];
            replace_nb = rand() % dimension;

            for (int j = 0; j < dimension; j++) {
                if (person[j] == replace_nb)
                    person[j] = tampon;
            }
        }
        person[indice] = replace_nb;
        population[i] = person;
    }
}

int main() {
    int nb_execution = 50;
    int population_size = 50;
    int dimension = 4;
    int mutate_probability = 20;

    int* population[population_size];

    // Init random population
    initPopulation(dimension, population_size, population);


    // Couple
    couplePopulation(dimension, population_size, population);

    // Mutation
    mutate(dimension, population_size, population, mutate_probability);

    // Test

}
