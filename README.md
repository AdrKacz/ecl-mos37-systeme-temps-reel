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

## Méthode de tri (merge sort, quick sort, time sort ?)

## Régulateur de température (au choix)

## Jeu de la vie (au choix)

## Simulateur de restaurant (au choix)
