/* SVN - Projet automate cellulaire - Gravitation */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define DIMAX 150 // Dimension de l'espace
#define TMAX 22500 //Correspond à un espace max de DIMAX (150x150)
#define CLEARSCR system("clear")
#define ESC "\x01b"
#define INIT ESC "[0m"
#define NOIR ESC "[30m"
#define ROUGE ESC "[31m"
#define JAUNE ESC "[33m"
#define VERT ESC "[32m"
#define BLEU ESC "[34m"
#define MAGENTA ESC "[35m"
#define CYAN ESC "[36m"
#define BLANC ESC "[37m"
#define PI 3.14159265359


typedef struct t_cel
{
	char type;
	int tps;
	int posH;
	int posV;
    float direction;
	double vitesse;
	float poids;
	float distance;

}T_CEL;

void sleep(int nbr_miliseconds);
void system_pause();
void enr_fichier_espace(T_CEL *T, int t_cour);
void charger_fichier_espace(T_CEL *T, int t_max, int *t_cour);
void affiche_espace(char (*MAP)[DIMAX],T_CEL *T, int dimH, int dimV);
void modif_espace(T_CEL *T);
void modif_map(char (*MAP)[DIMAX], T_CEL *T, int t_dim);
void initialise_espace(T_CEL *T, int *t_dim);
void zoom_map(int *dimH, int *dimV);
//void ajout_planete(T_CEL *T);
void gravitation(T_CEL *T);
void color_espace(T_CEL *T, int k);
void rotation(T_CEL *T);
int satellite(T_CEL *T, T_CEL *T1);
void initialise_satellite(T_CEL *T);

int main(void)
{
	int  menu, dimH = 30, dimV = 150, t_dim = 0, i, j, erreur = 0; // Recadrage initialise a 30x150,
	T_CEL T[TMAX], T1[TMAX];
	char MAP[DIMAX][DIMAX];

	CLEARSCR;
	do
	{

		printf("\n====Menu====\n");
		printf("0. Quitter le programme\n");
		printf("1. Sauvegarder l\'automate dans un fichier\n");
		printf("2. Charger l\'automate depuis un ficher\n");
		printf("3. Créer et/ ou modifier les cellules de l\'automate\n");
		printf("4. Paramètrer le recadrage de l\'affichage (x0, h, y0, v)\n");
		printf("5. Calculer l\'automate jusqu\'a un temps (t) donne sans affichage\n");
		printf("6. Afficher l\'automate en temps (t) courant\n");
		printf("7. Calculer et afficher entre t0 et t1 (0 < t0 < t1)\n");
		printf("8. Initialiser un satellite\n");
		printf("Votre choix : ");
		scanf(" %d", &menu);
		getchar();
		switch(menu)
		{
			case 0:
				printf("\nMerci, aurevoir.\n\n");
				break;
			case 1:
				if(t_dim == 0) printf("\n[Message] = Aucune donnée à enregistrer.\n");
                else enr_fichier_espace(T,t_dim);
				break;
			case 2:
                if(t_dim == 0) charger_fichier_espace(T,TMAX,&t_dim);
                else printf("\n[Message] = Un espace est déjà chargé.\n");
				break;
			case 3:
                if(t_dim == 0) {initialise_espace(T,&t_dim);}
                else {modif_espace(T);}
                gravitation(T);
				break;
			case 4:
				zoom_map(&dimH,&dimV);
				break;
			case 5:
					printf("Saisir j : ");
					scanf(" %d", &j);
					getchar();
					for(i=0;i<j;i++)
					{
						rotation(T);
						satellite(T,T1);
						gravitation(T);
					}
				break;
			case 6:
				if(t_dim != 0)
				{
					modif_map(MAP,T,t_dim);
                    affiche_espace(MAP,T,dimH,dimV);
					system_pause();
				}
				else printf("\n[Message] = Aucune données.\n");
				break;
			case 7:
				printf("Saisir j : ");
					scanf(" %d", &j);
					getchar();
				for(i=0;i<j;i++)
					{
						if(erreur == 0)
						{
							rotation(T);
							erreur = satellite(T,T1);
							gravitation(T);
							modif_map(MAP,T,t_dim);
							affiche_espace(MAP,T,dimH,dimV);
							sleep(200);
						}
					}
				if(erreur == 1) printf("\n[Message] = Le satellite est rentré en collision avec une planete le jour numero %d.\n", T[0].tps);
					system_pause();
				break;
			case 8:
				initialise_satellite(T);
				printf("\n[Message] = Le satellite est initialisé.\n");
				break;
			default:
				printf("Veuillez saisir votre choix ! (0,1,2,3,4,5,6 ou 7)");
		}
	}while(menu != 0);
	return 0;
}

void sleep (int nbr_miliseconds)
{
	clock_t start,end;
	start=clock();
	while(((end=clock())-start)<=((nbr_miliseconds*CLOCKS_PER_SEC)/1000));
}

void system_pause()
{
	char magickey;
	fflush(stdin);
	printf("Appuyez sur une touche pour continuer et revenir au menu ...");
	scanf("%c", &magickey);
	magickey = getc(stdin);
}

void modif_map(char (*MAP)[DIMAX], T_CEL *T, int t_dim)
{
	int i, j, k = 0;

	for(i=0;i<sqrt(t_dim);i++)
	{
		for(j=0;j<sqrt(t_dim);j++)
		{
			MAP[i][j] = T[k].type;
			k++;
		}
	}

}

void affiche_espace(char (*MAP)[DIMAX],T_CEL *T, int dimH, int dimV)
{
	int i, j;

	CLEARSCR;
	printf("(Jour numero %d)\n",T[0].tps);
	for(i=(DIMAX-dimH)/2;i<(DIMAX+dimH)/2;i++)
	{
		for(j=(DIMAX-dimV)/2;j<(DIMAX+dimV)/2;j++)
		{
            color_espace(T,(150*i)+j);
			printf("%c",MAP[i][j]);
            printf(INIT);
		}
		printf("\n");
	}
}

void modif_espace(T_CEL *T)
{
	int choix;
	T_CEL Mercure, Venus, Terre;
	memset(&Mercure,0,sizeof(T_CEL));
	memset(&Venus,0,sizeof(T_CEL));
	memset(&Terre,0,sizeof(T_CEL));

	Mercure.type = 'M';
	Mercure.posH = DIMAX/2;
	Mercure.vitesse = 87.5;
	Mercure.poids = 3.285*pow(10,28);
    Mercure.distance = 57910000;
    Mercure.posV = (DIMAX/2) + ceil(Mercure.distance/2992000);


	Venus.type = 'V';
	Venus.posH = DIMAX/2;
	Venus.vitesse = 116.0;
	Venus.poids = 4.867*pow(10,29);
    Venus.distance = 108200000;
    Venus.posV = (DIMAX/2) + ceil(Venus.distance/2992000);

	Terre.type = 'T';
	Terre.posH = DIMAX/2;
	Terre.vitesse = 365.0;
	Terre.poids = 5.972*pow(10,29);
    Terre.distance = 149600000;
    Terre.posV = (DIMAX/2) + ceil(Terre.distance/2992000);

	printf("\n[Message] = Voulez allez modifier la map :\n");
	do
	{
		printf("Quelle planete voulez vous ajouter : \n");
		printf("0. Aucune\n");
		printf("1. Mercure\n");
		printf("2. Venus\n");
		printf("3. Terre\n");
		//printf("4. Autre\n");
		printf("Votre choix : ");
		scanf(" %d", &choix);
		switch(choix)
		{
			case 0:
			break;
			case 1:
				T[(TMAX/2)+Mercure.posV] = Mercure;
				printf("\n[Message] = Planète Mercure ajouté.\n");
				choix = 0;
			break;
			case 2:
				T[(TMAX/2)+Venus.posV] = Venus;
				printf("\n[Message] = Planète Venus ajouté.\n");
				choix = 0;
			break;
			case 3 :
				T[(TMAX/2)+Terre.posV] = Terre;
				printf("\n[Message] = Planète Terre ajouté.\n");
				choix = 0;
			break;
			default:
				printf("\n[Message] = Choix pas possible.\n");
		}
	}while(choix != 0);
    //else ajout_planete(T);
}

void initialise_espace(T_CEL *T, int *t_dim)
{
	int i, j;
	T_CEL C;
	memset(&C,0,sizeof(T_CEL));

	printf("\n[Message] = Initialisation d\'un espace avec un soleil.\n");
	for(i=0;i<DIMAX;i++)
	{
		for(j=0;j<DIMAX;j++)
		{
			T[*t_dim] = C;
			T[*t_dim].type = 'o';
            T[*t_dim].posH = i;
            T[*t_dim].posV = j;
			*t_dim = *t_dim + 1;
		}
	}

	C.type = 'S';
	C.posV = DIMAX/2;
	C.posH = DIMAX/2;
	C.poids = 1.989*pow(10,30);
	T[((*t_dim)/2)+C.posV] = C;
}

void zoom_map(int *dimH, int *dimV)
{
	int i;

	printf("\n[Message] = Vous allez redimentionner la map :\n");
	do
	{
	printf("\nSaisir la hauteur de la map (comprise en 0 et %d): ", DIMAX);
	scanf(" %d", &i);
	}while(i<0 || i> DIMAX);
	*dimH = i;
	do
	{
	printf("\nSaisir la largeur de la map (comprise en 0 et %d): ", DIMAX);
	scanf(" %d", &i);
	}while(i<0 || i> DIMAX);
	*dimV = i;
}

void charger_fichier_espace(T_CEL *T, int t_max, int *t_cour)
{
	FILE *fd = NULL;

	fd = fopen("espace.txt", "r");
	if (fd != NULL)
	{
		while(*t_cour < t_max && fread(&T[*t_cour],sizeof(T_CEL),1,fd)!=0)
		{
			*t_cour = *t_cour + 1;
		}
		printf("\n[Message] = Données récuperés.\n");
		fclose(fd);
	}
	else
	{
		printf("\n[Erreur] = Il n\'a pas de fichier \"espace.txt\".\n");
	}
}

void enr_fichier_espace(T_CEL *T, int t_cour)
{
	FILE *fd = NULL;
	int i;

	fd = fopen("espace.txt", "w+");
	if (fd != NULL)
	{
		for(i=0;i<t_cour;i++)
		{
			fwrite(&T[i],sizeof(T_CEL),1,fd);
		}
		fclose(fd);
		printf("\nDonnée enregistré.\n");
	}
	else
	{
		printf("\n[Erreur] = impossible d\'enregistrer.\n");
	}
}

/*void ajout_planete(T_CEL *T)
{
	T_CEL C;
    memset(&C,0,sizeof(T_CEL));

    C.type = '@';
    C.posH = DIMAX/2;

	printf("\nSaisir la distance entre le soleil : ");
	scanf(" %f", &C.distance);
    getchar();

    C.posV = ceil(C.distance/2992000);

	printf("\nSaisir son poids : \n");
	scanf(" %f", &C.poids);
    getchar();

    T[((TMAX-(DIMAX+1))/2)-C.posV] = C;

}*/
void gravitation(T_CEL *T)
{
    int i, j;
    float val = 180.0/PI, Rdirection, Rvitesse, Rval = PI/180, dist = 2992000000;

	for(i=0;i<TMAX;i++)
	{
		if(T[i].type != '@') T[i].direction = 0;
		if(T[i].type != 'V' && T[i].type != 'T' && T[i].type != 'M' && T[i].type != '@') T[i].vitesse = 0;
	}

    for(i=0;i<TMAX;i++)
    {

        if(T[i].type != 'o' && T[i].type != '@')
        {
            for(j=0;j<TMAX;j++)
            {
                if(T[j].type == 'o')
                {
                    Rvitesse = T[j].vitesse;
                    Rdirection = T[j].direction;
                    if(T[j].posH > T[i].posH)
                    {
                        if(T[j].posV == T[i].posV) T[j].direction = 90;
                        if(T[j].posV > T[i].posV) T[j].direction = atan(((T[j].posH-T[i].posH)+0.0)/((T[j].posV-T[i].posV)+0.0)) * val + 90;
                        if(T[j].posV < T[i].posV) T[j].direction = atan(((T[j].posH-T[i].posH)+0.0)/((T[i].posV-T[j].posV)+0.0)) * val;
                    }
                    if(T[j].posH == T[i].posH)
                    {
                        if(T[j].posH < T[i].posH) T[j].direction = 0;
                        if(T[j].posH > T[i].posH) T[j].direction = 180;
                    }

                    if(T[j].posH < T[i].posH)
                    {
                        if(T[j].posV == T[i].posV) T[j].direction = -90;
                        if(T[j].posV > T[i].posV) T[j].direction = -(180-(atan(((T[i].posH-T[j].posH)+0.0)/((T[j].posV-T[i].posV)+0.0)) * val));
                        if(T[j].posV < T[i].posV) T[j].direction = - atan(((T[i].posH-T[j].posH)+0.0)/((T[i].posV-T[j].posV)+0.0)) * val;
                    }

                    T[j].vitesse = ((pow(6.6742,-11)*T[i].poids)/((pow((T[i].posH-T[j].posH)*dist,2))+(pow((T[i].posV-T[j].posV)*dist,2))));
                    T[j].vitesse = sqrt((pow((T[j].vitesse*cos(T[j].direction*Rval)+Rvitesse*cos(Rdirection*Rval)),2)+pow((T[j].vitesse*sin(T[j].direction*Rval)+Rvitesse*sin(Rdirection*Rval)),2)));
					if(T[j].posH > T[i].posH)
                    {
						if(T[j].posV > T[i].posV) T[j].direction = -atan((T[j].vitesse*sin(T[j].direction*Rval)+Rvitesse*sin(Rdirection*Rval))/(T[j].vitesse*cos(T[j].direction*Rval)+Rvitesse*cos(Rdirection*Rval))) * val + 90;
						else if(T[j].posV == T[i].posV) T[j].direction = 90;
						else T[j].direction = atan((T[j].vitesse*sin(T[j].direction*Rval)+Rvitesse*sin(Rdirection*Rval))/(T[j].vitesse*cos(T[j].direction*Rval)+Rvitesse*cos(Rdirection*Rval))) * val;
					}
					if(T[j].posH < T[i].posH)
                    {
						if(T[j].posV > T[i].posV) T[j].direction = -180 + atan((T[j].vitesse*sin(T[j].direction*Rval)+Rvitesse*sin(Rdirection*Rval))/(T[j].vitesse*cos(T[j].direction*Rval)+Rvitesse*cos(Rdirection*Rval))) * val;
						else if(T[j].posV == T[i].posV) T[j].direction = - 90;
						else T[j].direction = atan((T[j].vitesse*sin(T[j].direction*Rval)+Rvitesse*sin(Rdirection*Rval))/(T[j].vitesse*cos(T[j].direction*Rval)+Rvitesse*cos(Rdirection*Rval))) * val;

                    }
                    if(T[j].posH == T[i].posH)
                    {
						if(T[j].posV > T[i].posV) T[j].direction = 180;
						else T[j].direction = atan((T[j].vitesse*sin(T[j].direction*Rval)+Rvitesse*sin(Rdirection*Rval))/(T[j].vitesse*cos(T[j].direction*Rval)+Rvitesse*cos(Rdirection*Rval))) * val;
					}
					//printf("[%d][%f]\n",j,T[j].direction);
                }
            }
        }
    }
}

void color_espace(T_CEL *T, int k)
{
	if(T[k].vitesse > 100) printf(ROUGE);
	else if(T[k].vitesse > 10) printf(BLEU);
    else if(T[k].vitesse > 5) printf(JAUNE);
    else if(T[k].vitesse > 1) printf(MAGENTA);
    else if(T[k].vitesse > 0.5) printf(VERT);
    else if(T[k].vitesse > 0.1) printf(CYAN);

    if(T[k].type == '@' || T[k].type == 'S') printf(ROUGE);
}

void rotation(T_CEL *T)
{
	int j, old_posH, old_posV, o=0;
	float vitesse;
	char tmps[3] = {'o','o','o'};
	T_CEL C;
    memset(&C,0,sizeof(T_CEL));
	float Rval = PI/180;
	for(j=0;j<TMAX;j++)
		{
			T[j].tps = T[j].tps + 1;
		}
	for(j=0;j<TMAX;j++)
		{
			if(T[j].type != '@' && T[j].type != 'o' && T[j].type != 'S' && T[j].type != tmps[0] && T[j].type != tmps[1] && T[j].type != tmps[2])
			{
				vitesse = 360.0/T[j].vitesse;
				tmps[o] = T[j].type;
				o++;
				old_posV = T[j].posV;
				old_posH = T[j].posH;
				T[j].posV =  DIMAX/2 + ceil((T[j].distance*cos(T[j].tps*vitesse*Rval))/2992000);
				T[j].posH =  DIMAX/2 + ceil((T[j].distance*sin(T[j].tps*vitesse*Rval))/2992000);
				if(old_posH != T[j].posH || old_posV != T[j].posV)
				{
					T[(T[j].posH*150)+T[j].posV] = T[j];
					T[j] = C;
					T[j].type = 'o';
					T[j].posH = old_posH;
					T[j].posV = old_posV;
				}
			}
		}
}

int satellite(T_CEL *T, T_CEL *T1)
{
	int i, o =1;
	T_CEL C;
	memset(&C,0,sizeof(T_CEL));
	C.type = 'o';

	T1 = T;

	for(i=0;i<TMAX;i++)
	{
		if(T1[i].type == '@' && o == 1)
		{
			C.posV = T1[i].posV;
			C.posH = T1[i].posH;
			if(T1[i].direction <= 30 && T1[i].direction >= -30) //droite
			{
				T1[i].direction = (T1[i].direction+T1[i+1].direction)/2;
				T1[i].posV =  T1[i].posV + 1;
			}
			else if(T1[i].direction >= 150 || T1[i].direction <= -150) //gauche
			{
				T1[i].direction = (T1[i].direction+T1[i-1].direction)/2;
				T1[i].posV = T1[i-1].posV;
			}
			else if(T1[i].direction <= 120 && T1[i].direction >= 60) //haut
			{
				T1[i].direction = (T1[i].direction+T1[i-150].direction)/2;
				T1[i].posH = T1[i].posH - 1;
			}
			else if(T1[i].direction >= -120 && T1[i].direction <= -60) //bas
			{
				T1[i].direction = (T1[i].direction+T1[i+150].direction)/2;
				T1[i].posH = T1[i].posH + 1;
			}
			else if(T1[i].direction < 60 && T1[i].direction > 30) // coin droite haut
			{
				T1[i].direction = (T1[i].direction+T1[i-151].direction)/2;
				T1[i].posV = T1[i].posV + 1;
				T1[i].posH = T1[i].posH - 1;
			}
			else if(T1[i].direction < 150 && T1[i].direction > 120) // coin gauche haut
			{
				T1[i].direction = (T1[i].direction+T1[i-149].direction)/2;
				T1[i].posV = T1[i].posV - 1;
				T1[i].posH = T1[i].posH - 1;
			}
			else if(T1[i].direction > -60 && T1[i].direction < -30) // coin gauche bas
			{
				T1[i].direction = (T1[i].direction+T1[i+151].direction)/2;
				T1[i].posV = T1[i].posV + 1;
				T1[i].posH = T1[i].posH + 1;
			}
			else if(T1[i].direction > -150 && T1[i].direction < -120) // coin droite bas
			{
				T1[i].direction = (T1[i].direction+T1[i+149].direction)/2;
				T1[i].posV = T1[i].posV - 1;
				T1[i].posH = T1[i].posH + 1;
			}
		if(T[(T1[i].posH*150)+T1[i].posV].type != 'o') return 1;
		T[(T1[i].posH*150)+T1[i].posV] = T1[i];
		T[i] = C;
		o=0;
		}
	}
	return 0;
}

void initialise_satellite(T_CEL *T)
{
	T_CEL V;
	memset(&V,0,sizeof(T_CEL));
	V.direction = 0;
    V.posV = 0;
    V.posH = 65;
    V.type = '@';
	T[(V.posH*150)+V.posV]=V;
}
