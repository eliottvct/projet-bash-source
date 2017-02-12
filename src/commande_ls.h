//
// Created by eliottvct on 12/02/17.
//

#ifndef PROJET_BASH_SOURCE_COMMANDE_LS_H
#define PROJET_BASH_SOURCE_COMMANDE_LS_H

#define COMMENCE_PAR(a, b)    (strncmp((a), (b), strlen((b)))==0)
#define EST_EGAL(a, b)        (strlen((a))==strlen((b)) && COMMENCE_PAR((a),(b)))


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define CHAINE_MAX 512



void list_content(char *dir, bool L_OPTION, bool A_OPTION);

#endif //PROJET_BASH_SOURCE_COMMANDE_LS_H
