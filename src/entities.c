//
// Created by Eliott Vincent on 30/12/2016.
//

//#define DEBUGMODE
#ifdef DEBUGMODE    //si la ligne "#define DEBUGMODE" n'est pas commentée, alors le DEBUGMODE est défini
#  define DEBUG(x) x    //et donc chaque fonction entourée de DEBUG() s'exécutera
#else   //sinon
#  define DEBUG(x)  //le
#endif


/*
 * MAX
 */
#define MAX 255