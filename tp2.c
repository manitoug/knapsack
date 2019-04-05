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

void updateBestValue(int j){
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

    updateBestValue(j);
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
        fprintf(stderr, "1) prefixe du probleme mkp01 : xxx.dat\n");
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
