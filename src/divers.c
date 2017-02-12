#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "divers.h"
#include "entities.c"


void AfficheInvite() {

	char invite[CHAINE_MAX];
	char chaine[CHAINE_MAX];
    char host[CHAINE_MAX];
	char var[CHAINE_MAX];
    char home[CHAINE_MAX];
	size_t i;
	char *dwRet;

    ecrire_variable("INV", "\n\\u@\\h: \\p");
	if (lire_variable("INV", invite)) {

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
						lire_variable("USER", var);
						break;
					case 'h' :
                        gethostname(host, sizeof(host));	//on récupère le nom de la machine grâce à la fonction gethostname()
                        ecrire_variable("HOSTNAME", host);	//on stocke ce nom dans une variable d'environnement
						strcpy(var, host);	//et on la copie dans notre variable "var"
						break;
					case 's' :
						strcpy(var, " ");
						break;
					case 'p' :
						if ((dwRet = getcwd(var, sizeof(var))) == NULL) {	//on récupère le chemin du répertoire courant grâce à la fonction getcwd()
							fprintf(stderr, "Echec lors de l'appel a getcwd !\n");
							fflush(stderr);
						}
						else {
                            lire_variable("HOME", home);	//on récupère la variable "HOME"
							strcpy(var, simplify_wd(var, home));	//on fait appel à la fonction simplify_wd()
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
	if (nomVar != NULL) {	//si le nom de la variable n'est pas null
         if ((setenv(nomVar, valeur, 1)) == -1) {	//on enregistre la valeur dans une variable d'environnement
				printf("Une erreur a eu lieu : %s\n", strerror(errno));
				return faux;
			}
			else
				return vrai;
    }
    else
        return faux;
}

t_bool lire_variable(char *nomVar, char *valeur) {
	strcpy(valeur, "");

	DEBUG(printf("------------ Début de lire_variable() avec \"%s\", \"%s\"\n", nomVar, valeur));

	const char* pPath = getenv(nomVar);	//on tente de récupérer la variable d'environnement
	if (pPath == NULL) {
		DEBUG(printf("Variable non trouvée\n"));
		DEBUG(printf("------------ Fin de lire_variable() \n"));
		return faux;
	}
	else {
		DEBUG(printf ("Variable trouvée: %s \n", pPath));
		strcpy(valeur, pPath);
		DEBUG(printf("------------ Fin de lire_variable() \n"));
		return vrai;

	}
}

char *simplify_wd(char *wd, char *home) {
	static char buffer[CHAINE_MAX];
	if (COMMENCE_PAR(wd, home)) {	//si le chemin courant commence par le "HOME" de l'utilisateur
		strcpy(buffer, "~");	//alors on peut simplifier le chemin courant en remplacant le chemin courant par un "~"
		wd += strlen(home);	//on décale la chaine de charactere du chemin courant de n charcatères (n étant le nombre de charactères de la chaine contenant le "HOME")
		strcat(buffer, wd);	//on rajoute le chemin courant à notre buffer
		return buffer;	//on renvoie le buffer
	}
	else
		return wd;	//pas de simplification possible
}