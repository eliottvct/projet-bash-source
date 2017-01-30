//
// Created by Eliott Vincent on 30/12/2016.
//


#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/*
 * DEBUGMODE : comment the following line (the define one) to remove all printf()
 */
//#define DEBUGMODE

#ifdef DEBUGMODE
#  define DEBUG(x) x
#else
#  define DEBUG(x)
#endif


/*
 * MAX
 */
#define MAX 255