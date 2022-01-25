# ecl-mos37-systeme-temps-reel
Travail réalisé pour le MOS 3.7 - Système Temps Réel, Embarqué, et Mobile à l'École Centrale de Lyon. L'objectif est d'aborder la programmation parallèle en C++.

# Valeur approché de PI par calcul du cercle unitaire par Monte-Carlo `./be1/calcul-pi-cercle.cpp`

## Méthode

L'aire du cercle `A = PI * R ^ 2` avec `R` le rayon du cercle.

Pour `R = 1`, `A = PI`.

Nous tirons un grand nombre de point dans le plan `[0, 1] x [0, 1]`. Ce plan correspond à le quart de cercle supérieur droit du cercle.

Pour chaque point `(x, y)`, le point est dans le cercle si et seulement si `x ** 2 + y ** 2 <= 1`.

Soit `n` le nombre de point dans le cercle, `N` le nombre de point tiré. Nous avons une approximation `A / 4 = n / N`.

D'où une approximation de `PI = 4 * n / N`.

# Méthode de tri (merge sort, quick sort)

# Gestionnaire de billes

## Énoncé

On a `NB_THREADS` travailleurs qui ont besoin chacun de `needs[i]` billes pour travailler (`i` le numéro du *thread*).

> Exemple: `NB_THREADS = 5` *threads*, `needs = [1, 5, 3, 4, 3]`

On a un nombre limité de billes `NB_BILLES`

Le code simplifié d'un *thread* `t_i`:

```c++
void t(int i) {
    while (true) {
        ask_billes(needs[i]) // waits until it gets billes
        use_billes()
        return_billes()
        wait_time() // sleep (in needed)
    }
}
```

Un *thread* controlleur vérifie que `0 <= nb_billes <= NB_BILLES`, si la condition n'est pas respecté le programme s'arrête.

# Régulateur de température (au choix)

# Jeu de la vie (au choix)

# Simulateur de restaurant (au choix)

# Course de cheveaux

```
MOVE TO : \033<x>,<y>f (ou ;)
ERASE SCREEN : \x1B[2J\x1B[;H
```

