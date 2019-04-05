#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAXDIM 1000
int cout[MAXDIM][MAXDIM];

int DIM;
int liremat(char *fic);
char fichier[1024];
void dumpmat(void);

void main(int n,char *arg[]){
	int err;
	if (n<2) return 0;
	sprintf(fichier,"aff%s.txt",arg[1]);
	if (liremat(fichier)==0)
		dumpmat();
	return 0;
}

int liremat(char *fic){
	FILE *f;
	int i,j;
	f=fopen(fic,"r");
	if (f==NULL) return 1;
	if (fscanf(f,"%d",&DIM)!=1)  {fclose(f);return 2;}
	if (DIM>MAXDIM) {fclose(f);return 3;}
	for (i=0;i<DIM;i++) 
		for (j=0;j<DIM;j++)
				if (fscanf(f,"%d ",cout[i]+j)!=1)  {fclose(f);return 4;}
	fclose(f);
	return 0;
}

void dumpmat(void)
	{dd
	int i,j;
	printf("%d\n",DIM);
	for (i=0;i<DIM;i++) 
		{
		printf("\n");
		for (j=0;j<DIM;j++)
			printf("%4d ",cout[i][j]);
		}
	}

void solve (int i){
    if (i<nbrVar){
        if (isOk(i)){
            prendre(i);
            solve(i+1);
            rendre(i);
        }
        solve(i+1);
    }
}

void prendre(int j){//j nombre de colonnes disponibles
    int i; //Lignes
    int GainCourant += gain[i];
    int X[j]=1;
    //MAJ les Sacs à dos
    for (i=0; i<nbrCub; i++){
        capacite[i]<cause[i][j];
    }

void rendre(int i){
	int
    X[i]=0        
}

bool isOk(int j){
	for (int i=0; i<abstract; i++){
		if (capacite[i][j]) return false;
	return true;
	}
}