#include <thread>
#include <iostream>
#include <unistd.h>
#include <mutex>

std::mutex mutex;

int var_globale = 0;
int nb_iterations = 10000;

int race() {
    std::thread Ids[3];
    for (int i = 0; i < 3; i++)
    {
        Ids[i] = std::thread([i]()
        {
            // std::cout << "Thread " << i << " - identité [" << (long)getpid() << "] lancé\n";
            for (int j = 0; j < nb_iterations; j++)
            {
                mutex.lock(); var_globale++; mutex.unlock();
            }
        });
    }

    for (int i = 0; i < 3; i++)
    {
        Ids[i].join();
    }

    // std::cout << "Résultat Thread:\t" << var_globale << "\nRésultat Exact:\t\t" << 3 * nb_iterations << "\n\n";

    return 3 * nb_iterations - var_globale;
}

int main(int argc, char const *argv[])
{
    int total_race = 1000;
    if (argc > 1)
    {
        total_race = atoi(argv[1]);
    }
    int error = 0;
    int correct_race = 0;
    for (int i = 0; i < total_race; i++)
    {
        var_globale = 0;
        int local_error = race();
        if (local_error == 0)
        {
            correct_race++;
        } else {
            error += local_error;
        }
        
    }

    int total = 3 * nb_iterations * total_race;

    std::cout << "Nombre de Race Correct: " << correct_race << " (sur " << total_race << ")\n";
    std::cout << "Pourcentage de Race Correct: " << correct_race * 1.f / total_race * 100.f << "%\n";
    std::cout << "Erreur normalisée: " << error * 1.f / total * 100.f << "%\n";
    
}

/*
$ g++ -std=c++17 be1/race-condition.cpp -lpthread
$ ./a.out
Nombre de Race Correct: 41 (sur 10000)
Pourcentage de Race Correct: 0.41%
Erreur normalisée: 47.5162%
*/
