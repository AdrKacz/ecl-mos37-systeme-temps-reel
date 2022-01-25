#define NB_THREADS 4
#define NB_BILLES 9

#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <string>

std::mutex mutex;
int nb_billes = NB_BILLES;
int needs [NB_BILLES] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int workers [NB_THREADS] = {0};

int remaining_rounds = 12;
bool is_working(int i)
{
    if (remaining_rounds < 0)
    {
        return false;
    } else
    {
        remaining_rounds -= 1;
        return true;
    }
}

std::string billes()
{
    std::string output;
    output = "Remaining billes:\t ("  + std::to_string(nb_billes) + ") " + (nb_billes >= 0 ? std::string(nb_billes, '.') : "") + "\n";
    for (int k = 0; k < NB_THREADS; k++)
    {
        int workers_k = workers[k];
        if (workers_k > 0)
        {
            output += "Worker " +  std::to_string(k) + ":\t\t ("  + std::to_string(workers_k) + ") " + std::string(workers_k, '.') + "\n";
        } 
    }  
    return output;
}

int ask_billes(int i, int j)
{
    mutex.lock();
    std::string output = billes();
    if (nb_billes >= j) // if enough billes return
    {
        if (j > 0) 
        {
            output += "Worker " +  std::to_string(i) + " received " + std::to_string(j) + " billes\n";
        } else {
            output += "Worker " +  std::to_string(i) + " returned " + std::to_string(-j) + " billes\n";
        }
        nb_billes -= j;
        mutex.unlock();
        std::cout << output;
        return j;
    } else // wait and ask again
    {
        output += "Worker " +  std::to_string(i) + " wait for " + std::to_string(j) + " billes.\n";
        mutex.unlock();
        std::cout << output;
        std::this_thread::sleep_for (std::chrono::seconds(1));
        return ask_billes(i, j);
    }    
}

void use_billes(int i)
{
    std::this_thread::sleep_for (std::chrono::seconds(1));
}

int return_billes(int i, int j)
{
    return ask_billes(i, -j); // not blocking
}

void wait_time(int i)
{

}

void worker(int i)
{
    std::string output;
    output = "Worker " + std::to_string(i) + " initialised.\n";
    std::cout << output;
    while (is_working(i)) {
        workers[i] += ask_billes(i, needs[i]);
        use_billes(i);
        workers[i] += return_billes(i, workers[i]);
        wait_time(i);
    }
}

int main(int argc, char const *argv[])
{
    bool controller_flag = true;
    std::thread controller = std::thread([&controller_flag]()
    {
        while (controller_flag) {
            mutex.lock();
            int local_nb_billes = nb_billes;
            mutex.unlock();
            if (local_nb_billes < 0 || local_nb_billes > NB_BILLES)
            {
                std::string output = "\x1b[41mError: nb_billes ("+ std::to_string(nb_billes) + ") isn't is logical range [0, + " + std::to_string(NB_BILLES) + "]\x1b[0m\n";
                std::cout << output;
            }
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
        }
    });

    std::thread threads[NB_THREADS];
    std::string output = "Create " + std::to_string(NB_THREADS) + " workers.\n";
    std::cout << output;
    for (int i = 0; i < NB_THREADS; i++)
    {
        threads[i] = std::thread(worker, i);
    }
    for (int i = 0; i < NB_THREADS; i++)
    {
        threads[i].join();
    }
    controller_flag = false;
    controller.join();
    std::cout << "Work done!\n";
    std::cout << billes();
    return 0;
}


/*
// With error (j - 3 line 47)
$ g++ -std=c++17 be2/billes.cpp && ./a.out
Create 4 workers.
Worker 0 initialised.
Remaining billes:        (9) .........
Worker 0 received 1 billes
Worker 3 initialised.
Worker 1 initialised.
Remaining billes:        (8) ........
Worker 0:                (1) .
Worker 1 received 2 billes
Remaining billes:        (6) ......
Worker 0:                (1) .
Worker 1:                (2) ..
Worker 3 received 4 billes
Worker 2 initialised.
Remaining billes:        (2) ..
Worker 0:                (1) .
Worker 1:                (2) ..
Worker 3:                (4) ....
Worker 2 received 3 billes
Error: nb_billes (-1) isn't is logical range [0, + 9]
Remaining billes:        (-1) 
Worker 0:                (1) .
Worker 1:                (2) ..
Worker 2:                (3) ...
Worker 3:                (4) ....
Worker 0 returned 1 billes
...
// Without error
$ g++ -std=c++17 be2/billes.cpp && ./a.out
Create 4 workers.
Worker 0 initialised.
Worker 2 initialised.
Worker 3 initialised.
Remaining billes:        (5) .....
Worker 3 received 4 billes
Remaining billes:        (9) .........
Worker 0 received 1 billes
Remaining billes:        (8) ........
Worker 2 received 3 billes
Worker 1 initialised.
Remaining billes:        (1) .
Worker 0:                (1) .
Worker 2:                (3) ...
Worker 3:                (4) ....
Worker 1 wait for 2 billes.
Remaining billes:        (1) .
Worker 0:                (1) .
Worker 2:                (3) ...
Worker 3:                (4) ....
Worker 3 returned 4 billes
Remaining billes:        (5) .....
Worker 0:                (1) .
Worker 2:                (3) ...
Worker 3:                (4) ....
Worker 2 returned 3 billes
Remaining billes:        (6) ......
Worker 0:                (1) .
Worker 2:                (3) ...
Worker 0 returned 1 billes
Remaining billes:        (8) ........
Worker 0:                (1) .
Worker 2:                (3) ...
Worker 1 received 2 billes
...
*/