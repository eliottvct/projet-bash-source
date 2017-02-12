#ifndef _COMMANDES_EXTERNES_H_
#define _COMMANDES_EXTERNES_H_

#include "divers.h"
#include "parse.h"

t_bool ActionEXEC(parse_info *info, int debut, int nbArg);

void execute(parse_info * info, int nbArg, int debut);

#endif
