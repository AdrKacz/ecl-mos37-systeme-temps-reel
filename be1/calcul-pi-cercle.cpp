// Valeur approché de PI par le cercle unitaire

#include <thread>
#include <iostream>
#include  <cmath>
#include <future>

#define NB_THREADS 8

int race(int min_nb_point)
{
    int in_circle = 0;
    for (int j = 0; j < std::ceil(min_nb_point / NB_THREADS); j++)
    {
        // Tire un point
        float x = std::rand() * 1.f / RAND_MAX * 1.f;
        float y = std::rand() * 1.f / RAND_MAX * 1.f;

        bool is_in_circle = std::pow(x, 2) + std::pow(y, 2) <= 1.f;

        if (is_in_circle)
        {
            in_circle++;
        }
    }
    return in_circle;
}

int main(int argc, char const *argv[])
{
    int min_nb_point = 1000;
    if (argc > 1) {
        min_nb_point = atoi(argv[1]);
    }
    std::future<int> threads[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++)
    {
        threads[i] = std::async(std::launch::deferred, race, min_nb_point);
    }

    int total_in_circle = 0;
    for (int i = 0; i < NB_THREADS; i++)
    {
        total_in_circle += threads[i].get();
    }

    std::cout << 4.f * total_in_circle * 1.f / min_nb_point * 1.f;
    
    return 0;
}
