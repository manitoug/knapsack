/*Fait par :
 * Raphaël ACISCLO
 * Mathieu GARDETTE
 * Joshua WOLFCARIUS
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <time.h>

int gain[500],        // ===> le gain de chaque objet
        conso[30][500], // ===> la conso de chaque objet pour chaque sac
        consomax[30];    // ===> la conso maximale que peut supporter un sacs
int NBVAR = 500,            // ===> n
        NBRCNT = 30;        // ===> m

int x[500]; // les objets qu'on a pris dans la branche actuelle
int z; //

clock_t start;
clock_t timeEnd;
int bestValue = 0;

int litjeu(char *ficjeu) {
    FILE *f;
    int p, c;
    f = fopen(ficjeu, "r");
    if (f == NULL) {
        fprintf(stderr, "erreur en ouverture de %s\n", ficjeu);
        return 1;
    }
	/*********************/
	/* lecture de n et m */
	/*********************/
    if (fscanf(f, "%d%d", &NBVAR, &NBRCNT) != 2) {
        fprintf(stderr, "erreur en lecture de nbrvar %d et nbrcnt %d dans %s\n", NBVAR, NBRCNT, ficjeu);
        fclose(f);
        return 2;
    }
	/****************/
	/* lecture de c */
	/****************/
    for (p = 0; p < NBVAR; p++)
        if (fscanf(f, "%d", gain + p) != 1) {
            fprintf(stderr, "erreur en lecture du poids %d dans %s\n", p, ficjeu);
            fclose(f);
            return 2;
        }
	/****************/
	/* lecture de b */
	/****************/
    for (c = 0; c < NBRCNT; c++)
        if (fscanf(f, "%d", consomax + c) != 1) {
            fprintf(stderr, "erreur en lecture de la contrainte %d dans %s\n", c, ficjeu);
            fclose(f);
            return 2;
        }
	/****************/
	/* lecture de A */
	/****************/
    for (c = 0; c < NBRCNT; c++)
        for (p = 0; p < NBVAR; p++)
            if (fscanf(f, "%d", &conso[c][p]) != 1) {
                fprintf(stderr, "erreur en lecture de la conso %d de la variable %d dans %s\n", c, p, ficjeu);
                fclose(f);
                return 2;
            }
    fclose(f);
    return 0;
}

void update(int j){
    int currentTotalValue = 0;
    for(int c = 0; c < 500; c++){
        if(x[c]){
            currentTotalValue += gain[c];
        }
    }
    if(currentTotalValue > bestValue){
        bestValue = currentTotalValue;
    }
}

void take(int j) {

    z += gain[j];
    x[j] = 1;

    for (int i = 0; i < NBRCNT; i++) {
        consomax[i] -= conso[i][j];
    }

    update(j);
}

void putBack(int j){

    z -= gain[j];
    x[j] = 0;

    for (int i = 0; i < NBRCNT; i++) {
        consomax[i] += conso[i][j];
    }

}

int isOk(int j){
    for(int i = 0; i < NBRCNT; i++){
        if(consomax[i] < conso[i][j]) return 0;
    }
    return 1;
}

int solve(int j){
    if(time(NULL) > timeEnd){
        return 0; //Whatever, c'est pour quitter la fonction
    }
    if(j < NBVAR){
        if(isOk(j)){
            take(j);
            solve(j+1);
            putBack(j);
        }

        for(int c = 0; c < 500; c++){
            printf("%d", x[c]);
        }
        printf("\n");

        solve(j+1);
    }
}

int main(int argc, char **argv) {

    int i, j, status = 0;
    char ficjeu[128];

    for(int c = 0; c<500; c++){
        x[c] = 0;
    }


    if (argc < 2) {
        fprintf(stderr, "Écrire le num du .dat !\n");
        return 0;
    }

    sprintf(ficjeu, "./data/cb30.500_%s.dat", argv[1]);
    if (litjeu(ficjeu) != 0) return 0;

    printf("\n%s : %d variable %d contraintes", ficjeu, NBVAR, NBRCNT);

    start = time(NULL);
    timeEnd = start + 60;
    solve(0);

    printf("%d", bestValue);

    return 0;
}


/* Résultats : (interrompus à 60 sec)

commande unix pour lancer le programme 30 fois :
``` bash
for x in $(seq 0 29); do a.out $x; done;
```

Résultats pour un CPU i3-4020Y à 1.50GHz : (programme monothreadé)

 0.dat  92102
 1.dat  87152
 2.dat  92398
 3.dat  89880
 4.dat  91043
 5.dat  89795
 6.dat  87692
 7.dat  87008
 8.dat  89191
 9.dat  90520
10.dat 184633
11.dat 182212
12.dat 180485
13.dat 185675
14.dat 184241
15.dat 181876
16.dat 185180
17.dat 183576
18.dat 183104
19.dat 179822
20.dat 276339
21.dat 276290
22.dat 280210
23.dat 276252
24.dat 281580
25.dat 271182
26.dat 281364
27.dat 280458
28.dat 277945
29.dat 276052
*/