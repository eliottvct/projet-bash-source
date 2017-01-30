#ifndef _COMMANDES_EXTERNES_H_
#define _COMMANDES_EXTERNES_H_

#include "divers.h"
#include "parse.h"

pid_t pid_fils;

t_bool ActionEXEC(parse_info *info, int debut, int nbArg);
t_bool pereEtFils();
t_bool dummyExecute(parse_info *info);
t_bool fork_execute(char * p, parse_info * info, int nbArg, int debut);

#endif
