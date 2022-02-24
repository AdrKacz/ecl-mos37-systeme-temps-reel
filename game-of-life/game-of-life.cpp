#include <thread>
#include <iostream>
#include <mutex>
#include <string>
#include <chrono>
std::mutex mutex;

#define NB_THREADS_SQRT 3
#define NB_THREADS NB_THREADS_SQRT * NB_THREADS_SQRT
#define GRID_STEP 8
#define GRID_SIZE GRID_STEP * NB_THREADS_SQRT


int main(int argc, char const *argv[])
{
    bool grid[GRID_SIZE + 2][GRID_SIZE + 2] = {false};
    int grid_neighbours[GRID_SIZE][GRID_SIZE] = {0};

    // Initialise grid
    for (unsigned int x = 1; x < 1 + GRID_SIZE; x++)
    {
        for (unsigned int y = 1; y < 1 + GRID_SIZE; y++)
        {
            if (rand() % 100 < 20)
            {
                grid[x][y] = true;
            } 
        }
    }

    std::thread threads[NB_THREADS];
    bool has_been_updated = true;
    while (has_been_updated)
    {
        // Count neighbours
        for (unsigned int i = 0; i < NB_THREADS; i++)
        {
            const unsigned int x_grid = i / NB_THREADS_SQRT;
            const unsigned int y_grid = i % NB_THREADS_SQRT;
            threads[i] = std::thread([x_grid, y_grid, &grid, &grid_neighbours]()
            {
                for (unsigned int x = 1 + x_grid * GRID_STEP; x < 1 + (x_grid + 1) * GRID_STEP; x++)
                {
                    for (unsigned int  y = 1 + y_grid * GRID_STEP; y < 1 + (y_grid + 1) * GRID_STEP; y++)
                    {
                        // No mutex, only one thread access (x, y)
                        grid_neighbours[x - 1][y - 1] = 0; // grid_neightbours doesn't have the grid pad of 1
                        for (unsigned int dxy = 0; dxy < 9; dxy++)
                        {
                            const unsigned int dx = dxy / 3;
                            const unsigned int dy = dxy % 3;
                            if (dx == 1 && dy == 1)
                            {
                                continue;
                            }
                            if (grid[x + dx - 1][y + dy - 1]) {
                                grid_neighbours[x - 1][y - 1] += 1;
                            }
                        }
                    }
                }
            });
        }

        for (unsigned int i = 0; i < NB_THREADS; i++)
        {
            threads[i].join();
        }

        // Update grid
        has_been_updated = false;
        for (unsigned int i = 0; i < NB_THREADS; i++)
        {
            const unsigned int x_grid = i / NB_THREADS_SQRT;
            const unsigned int y_grid = i % NB_THREADS_SQRT;
            threads[i] = std::thread([&has_been_updated, x_grid, y_grid, &grid, &grid_neighbours]()
            {
                for (unsigned int x = 1 + x_grid * GRID_STEP; x < 1 + (x_grid + 1) * GRID_STEP; x++)
                {
                    for (unsigned int  y = 1 + y_grid * GRID_STEP; y < 1 + (y_grid + 1) * GRID_STEP; y++)
                    {
                        if (grid_neighbours[x - 1][y - 1] <= 1)
                        {
                             mutex.lock();
                             if (!has_been_updated && grid[x][y])
                            {
                                has_been_updated = true;
                            }
                             grid[x][y] = false;
                             mutex.unlock();
                        } else if (grid_neighbours[x - 1][y - 1] == 3)
                        {
                            mutex.lock();
                            if (!has_been_updated && !grid[x][y])
                            {
                                has_been_updated = true;
                            }
                            grid[x][y] = true;
                            mutex.unlock();
                        } else if (grid_neighbours[x - 1][y - 1] > 3)
                        {
                            mutex.lock();
                            if (!has_been_updated && grid[x][y])
                            {
                                has_been_updated = true;
                            }
                            grid[x][y] = false;
                            mutex.unlock();
                        }
                    }
                }
            });
        }

        for (unsigned int i = 0; i < NB_THREADS; i++)
        {
            threads[i].join();
        }

        // Display grid
        std::cout << "\x1B[2J\x1B[H";
        std::cout << "===== ===== ===== ===== =====" << std::endl;
        for (unsigned int x = 1; x < 1 + GRID_SIZE; x++)
        {
            for (unsigned int y = 1; y < 1 + GRID_SIZE; y++)
            {
                if (grid[x][y])
                {
                    std::cout << " \u25CF ";
                } else
                {
                    std::cout << "   ";
                }
                
            }
            std::cout << std::endl;
        }
        std::cout << "===== ===== ===== ===== =====" << std::endl << std::endl;

        // Sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}

