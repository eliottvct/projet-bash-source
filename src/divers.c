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
                        gethostname(host, sizeof(host));	//on récupère le nom de la machine grâce à la fonction C gethostname()
                        ecrire_variable("HOSTNAME", host);
						lire_variable("HOSTNAME", var);
						break;
					case 's' :
						strcpy(var, " ");
						break;
					case 'p' :
						if ((dwRet = getcwd(var, sizeof(var))) == NULL) {
							fprintf(stderr, "Echec lors de l'appel a getcwd !\n");
							fflush(stderr);
						}
						else {
                            lire_variable("HOME", home);
							strcpy(var, editwd(var, home));
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
        if (valeur == NULL) {	//si l'utilisateur n'a pas rentré de valeur
            if ((setenv(nomVar, "", 1)) == -1) {
				printf("Une erreur a eu lieu : %s\n", strerror(errno));
                return faux;
			}
            else {
				return vrai;
			}
		}
        else {
			if ((setenv(nomVar, valeur, 1)) == -1) {
				printf("Une erreur a eu lieu : %s\n", strerror(errno));
				return faux;
			}
			else
				return vrai;
		}
    }
    else
        return faux;
}

t_bool lire_variable(char *nomVar, char *valeur) {
	strcpy(valeur, "");

	DEBUG(printf("------------ Début de lire_variable() avec \"%s\", \"%s\"\n", nomVar, valeur));

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

char *editwd(char *wd, char *home) {
	static char buffer[4096];
	if (COMMENCE_PAR(wd, home)) {
		strcpy(buffer, "~");
		wd += strlen(home);
		strcat(buffer, wd);
		return buffer;
	}
	else
		return wd;
}