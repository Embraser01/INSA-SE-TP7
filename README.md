# TP Système d'exploitation - 3IF - INSA Lyon 2017

## Presentation

   Petit projet consistant à optimiser un code C grâce aux threads, etc...
   
   Ce projet implémente [l'algorithme de Deriche](https://en.wikipedia.org/wiki/Deriche_edge_detector) 
   qui permet de trouver les contours dans une images


## Téléchargement et compilation

Cloner le repo avec Git :
```bash
git clone https://github.com/Embraser01/INSA-SE-TP7.git
```

Compiler

```bash
cd INSA-SE-TP7/build
make
```

##  Lancer l'application

```bash
$ ./deriche ../data/city.pgm ./mon_resultat.pgm
usage: ./deriche in.pgm out.pgm
header P5
width= 7786
height= 3000
depth= 255
loaded ...
Temps avec optimisation : 2.163192 sec
Saving image ./result.pgm (w=7786 x h=3000)
Temps sans optimisation : 5.038760 sec
La version optimisée est 2.33 fois plus rapide que l'originale
Résultat correct.
```
