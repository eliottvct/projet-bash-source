#include <stdlib.h>
#include <stdio.h>
#include "divers.h"
#include "entities.c"


void AfficheInvite() {

	char invite[CHAINE_MAX];
	char chaine[CHAINE_MAX];
	char var[CHAINE_MAX];
	size_t i;
	char *dwRet;

    ecrire_variable("INV", "\n\\u@\\h: \\p");
	if (lire_variable("INV", invite, sizeof(invite))) {

		/* Format reconnu :
		 * \u : remplace par l'utilisateur
		 * \h : remplace par le nom de machine
		 * \p : remplace par le chemin courant
		 */

		strcpy(chaine, "");
		for (i = 0; i < strlen(invite); i++) {
			if (invite[i] == '\\' && i < strlen(invite) - 1) {
				i = i + 1;
				switch (invite[i]) {
					case 'u' :
						lire_variable("USER", var, sizeof(var));
						break;
					case 'h' :
                        ecrire_variable("HOSTNAME", "MacBookPro");  //cheat a modifier
						lire_variable("HOSTNAME", var, sizeof(var));
						break;
					case 's' :
						strcpy(var, " ");
						break;
					case 'p' :
						dwRet = getcwd(var, sizeof(var));
						if (dwRet == NULL) {
							fprintf(stderr, "Echec lors de l'appel a getcwd !\n");
							fflush(stderr);
						}
                        break;
					default :
						/* cas impossible a priori */
						break;
				}
			} else {
				sprintf(var, "%c", invite[i]);
			}
            strncat(chaine, var, CHAINE_MAX - strlen(chaine));
		}

	} else {
		strcpy(chaine, "$ ");
	}

    strncat(chaine, "$ ", CHAINE_MAX - strlen(chaine));

    printf("%s", chaine);
	fflush(stdout);
}

t_bool ecrire_variable(char *nomVar, char *valeur) {
	if (valeur == NULL)
		setenv(nomVar, "", 1);
	else
	    setenv(nomVar, valeur, 1);
	return vrai;
}

t_bool lire_variable(char *nomVar, char *valeur, int taille) {
	strcpy(valeur, "");

	DEBUG(printf("------------ Début de lire_variable() avec \"%s\", \"%s\" et %d. \n", nomVar, valeur, taille));

	const char* pPath = getenv(nomVar);
	if (pPath!=NULL) {
		DEBUG(printf ("Variable found: %s \n", pPath));
		strcpy(valeur, pPath);
        DEBUG(printf("------------ Fin de lire_variable() \n"));
        return vrai;
	}
	else {
		DEBUG(printf("Variable not found \n"));
        DEBUG(printf("------------ Fin de lire_variable() \n"));
        return faux;
	}
}
