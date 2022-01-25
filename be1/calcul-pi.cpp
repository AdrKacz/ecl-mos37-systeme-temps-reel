// janv 2022
// LA version  non entrelacée
#include  <thread>
#include  <iostream>
#include  <cmath>
#include  <cstdlib>
#include <string>
using namespace std;

#define NB_RECTS 1000000L
#define NB_THREADS 8
double somme_globale=0.0f;
int nb_iteration_d_un_thread=(int)(NB_RECTS/NB_THREADS);
 // IMPORTANT de ne pas initialiser avec un simple ’0’
void calcul_pi_partiel_arctang(int myNum){
    string output = "Thread No " + to_string(myNum) + " lancé\n";
    cout << output; 
    double somme_locale=0.0f;
    for (int i = 0; i < nb_iteration_d_un_thread ; i ++){ 
        double temp_x= (i+0.5f)/nb_iteration_d_un_thread ; 
        somme_locale += 4.0f /  (1.0f + temp_x*temp_x);        
    }

    // division 1/N de la somme locale
    somme_globale += somme_locale* (1.0/nb_iteration_d_un_thread);         
}

int main() {
    std::thread Ids[NB_THREADS];
    for(int k=0; k<NB_THREADS; k++)
        Ids[k]=thread(calcul_pi_partiel_arctang, k);
    for(int k=0; k<NB_THREADS; k++)
        Ids[k].join();
    cout << "Valeur approchée de Pi (méthode basique 1) : " << somme_globale/NB_THREADS << endl;
    return 0;
}
/*
$ g++ -std=c++17 be1/calcul-pi.cpp -lpthread
$ ./a.out
Thread No 3 lancé
Thread No 4 lancé
Thread No 0 lancé
Thread No 1 lancé
Thread No 2 lancé
Thread No 5 lancé
Thread No 6 lancé
Thread No 7 lancé
Valeur approchée de Pi (méthode basique 1) : 3.14159
*/