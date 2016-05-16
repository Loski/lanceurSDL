#include "fonction_Pendu.h"

char traitementLettre(char lettre)
{
    if(!LINUX) /*Si on est sur Windows, on transforme les lettres qwerty en azerty */
    {
        lettre = convertion(lettre);
    }
    lettre = (char)toupper(lettre); /* Majuscule */
    if((lettre >= 'A' && lettre <= 'Z'))
    {
        return lettre;
    }
    else
        return '\t';
}
int tailleDico(FILE * dico) /* Compte le nombre de mot dans  un fichier */
{
    char caractereLu;
    int nombreMot =0;
    rewind(dico);
    do
    {
        caractereLu = (char)fgetc(dico);
        if (caractereLu == '\n')
            nombreMot++;
    }
    while(fgetc(dico)!= EOF);
    return nombreMot;
}
char* piocherMot(void)
{
    FILE* dico = NULL;
    int nbMot = 0,nbAleatoire =0;
    size_t taille =0;
    char* mot = NULL;
    char buffer[10] = {0};
    dico = fopen("Jeux/Pendu/sources/dico.txt","r");
    verifChargTexte(dico);
    nbMot = tailleDico(dico);
    rewind(dico);
    do
    {
        for(nbAleatoire=(rand() % nbMot); nbAleatoire>0; --nbAleatoire)
        {
            fgets(buffer,10,dico);
        }
        taille = strlen(buffer);
    }
    while(taille == 0 || taille == 1);/* Evite le cas ou le mot est un '\n' */
    mot = (char*)calloc((taille+1),sizeof(char));
    if(mot != NULL)
    {
        sprintf(mot,"%s",buffer);
        if(taille < 9 && LINUX)
            {
                mot[strlen(mot)-2] = '\0';
            }
        else
            mot[strlen(mot)-1] = '\0';
    }
    else
    {
        fputs("Erreur d'initialisation du pointeur mot",stderr);
        exit(2);
    }
    fclose(dico);
    return mot;
}
char convertion (const char lettre)
{
    switch (lettre)    /* Convertion qwerty => azerty si on est sous window*/
    {
    case 'q' :
        return 'a';
        break;
    case ';' :
        return 'm';
        break;
    case 'z' :
        return 'w';
        break;
    case 'a' :
        return 'q';
        break;
    case 'w' :
        return 'z';
        break;
    case 'm' :
        return '\t';
        break;
    default:
        return lettre;
        break;
    }
}

int newMot(const char buffer[])
{
    FILE *dico = NULL;
    char mot1[10] = {0},motEntre[10] = "";
    int i;
    dico = fopen("Jeux/Pendu/sources/dico.txt","r+");
    if(dico == NULL)
    {
        fputs("Erreur d'ouverture du dictionnaire",stderr);
    }
    else
    {
        rewind(dico);
        do
        {
            fscanf(dico,"%s",mot1);
            if(!strcmp(buffer,mot1)) /*On vérifie si le motEntré est présent dans dico.txt */
            {
                TTF_write("Mot déjà présent !");
                SDL_Delay(1000);
                fclose(dico);
                return 0;
            }
        }
        while(!feof(dico)); /* Renvoie une variable quand EOF est atteint sinon NULL */
        fputs("\n",dico);   /* Saut de ligne */
        for(i=0;(buffer[i]!=' '&& i<9);i++) /* On supprime les espaces */
        {
           motEntre[i] = buffer[i];
        }
        fputs(motEntre,dico);
        fclose(dico);
        return 1;
    }
    return 0;
}
