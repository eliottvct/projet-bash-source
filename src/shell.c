#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "divers.h"
#include "execution.h"
#include "commandes_internes.h"
#include "commandes_externes.h"
#include "entities.c"



int main(int argc, char *argv[]) {

    DEBUG(printf("%s", argv[0]));
    char ligne_cmd[CHAINE_MAX];
    char verOS[CHAINE_MAX];
    FILE *entree;
    int i;
    parse_info *info;
    t_bool mode_interactif = (argc == 1);   //si c'est true, alors on est en mode interactif

    if (mode_interactif) {
        lire_variable("OS", verOS, sizeof(verOS));
        DEBUG(printf("imrShell - %s\n", (char *) verOS));
        DEBUG(printf("Programmation Systeme Windows - IMR 1\n"));
        fflush(stdout);
    }

    for (i = 1; i < argc || mode_interactif; i++) {
        if (mode_interactif) {
            AfficheInvite();
            entree = stdin;
        }
        else {
            /* Si le mode n'est pas interractif (il y a des arguments sur la ligne
             * de commande lors de l'appel au shell), alors il faut ouvrir les
             * fichiers en argument, un  un (argv[i]), puis interpreter leur contenu
             * dans la boucle suivante
             */
            //char buf[1024];
            //strcpy(buf, argv[1]);
            entree =fopen(argv[i], "r");
            DEBUG(("argv[1] is %s\n", buf));
            (void) argv;
        }

        while ((fgets(ligne_cmd, sizeof(ligne_cmd), entree) != 0) && !COMMENCE_PAR(ligne_cmd, "exit")) {
            info = parse(ligne_cmd);
            execution_ligne_cmd(info);

            if (mode_interactif) {
                AfficheInvite();
            }
        }

        mode_interactif = faux;
    }

    return EXIT_SUCCESS;
}
