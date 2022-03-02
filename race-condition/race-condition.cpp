#include <thread>
#include <iostream>
#include <unistd.h>
#include <mutex>

std::mutex mutex;

int var_globale = 0;
int nb_iterations = 10000;

int race(const bool use_mutex=true) {
    std::thread Ids[3];
    for (int i = 0; i < 3; i++)
    {
        Ids[i] = std::thread([use_mutex]()
        {
            for (int j = 0; j < nb_iterations; j++)
            {
                if (use_mutex) {
                    mutex.lock(); var_globale++; mutex.unlock();
                } else {
                    var_globale++;
                }
                
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

void test_accuracy(const int total_race, const bool use_mutex)
{
    int error = 0;
    int correct_race = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < total_race; i++)
    {
        var_globale = 0;
        int local_error = race(use_mutex);
        if (local_error == 0)
        {
            correct_race++;
        } else {
            error += local_error;
        }  
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto diff_sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    int total = 3 * nb_iterations * total_race;

    std::cout << "Nombre de Race Correct: " << correct_race << " (sur " << total_race << ")\n";
    std::cout << "Pourcentage de Race Correct: " << correct_race * 1.f / total_race * 100.f << "%\n";
    std::cout << "Erreur normalisée (sur 1000 Race): " << error * 1.f / total * 100.f << "%\n"; 
    std::cout << "--- Temps: " << diff_sec.count() / 1000000.0 << "ms\n";
}

int main(int argc, char const *argv[])
{
    int total_race = 1000;
    if (argc > 1)
    {
        total_race = atoi(argv[1]);
    }

    std::cout << "===== Sans mutex\n";
    test_accuracy(total_race, false);
    std::cout << "===== Avec mutex\n";
    test_accuracy(total_race, true);
}
