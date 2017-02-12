#/// @file 
#/// @brief Generic Makefile for the System 2 project.                                                 
#                                                                                                   
#/// @detail If you just add some library files used by the project.c program, you have nothing to change to compile them if sources are in the ./src directory. To add a new binary, just add the name of the main file in the TARGETS variable.             


#Nom du project
TARGET_SHELL = shell
TARGET_LS = ls

##############
# Constantes #
##############

# Repertoires
SOURCE = ./src
BIN = ./bin
DOCPATH = ${SOURCE}/dox
DOCTARGET = ./doc
DIRLIST = ${SOURCE} ${BIN}
#DEP = ${SOURCE}/depend
#DIRLIST = ${SOURCE} ${BIN} ${OPT} ${DEP}

# Cibles
BINSHELL = ${TARGET_SHELL:%=${BIN}/%}
BINLS = ${TARGET_LS:%=${BIN}/%}

# Commandes
CC = gcc

# Options
CFLAGS = -O0 -g -W -Wall -Wextra -Wconversion -Werror -mtune=native  -march=native  -std=c99  -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700
LDFLAGS = -lm -W -Wall -pedantic -L. -lm

# Fichiers
DOX = ${wildcard ${DOCPATH}/*.dox} # Sources
SRC_SHELL = ${wildcard ${SOURCE}/divers.c ${SOURCE}/commandes_externes.c ${SOURCE}/commandes_internes.c ${SOURCE}/entities.c ${SOURCE}/execution.c ${SOURCE}/parse.c ${SOURCE}/shell.c} # Sources
SRC_LS = ${wildcard ${SOURCE}/commande_ls.c}
INT_SHELL = ${wildcard ${SOURCE}/divers.h ${SOURCE}/commandes_externes.h ${SOURCE}/commandes_internes.h ${SOURCE}/execution.h ${SOURCE}/parse.h} # Interfaces
INT_LS = ${wildcard ${SOURCE}/commande_ls.h}
OBJ_SHELL = ${SRC_SHELL:%.c=%.o}	 	# Objets
OBJ_LS = ${SRC_LS:%.c=%.o}

##########
# Regles #
##########

# ALL
all : ${BINSHELL} ${BINLS}

# CLEAN
clean :
	@echo
	@echo Cleaning : object files
	@echo --------
	@echo
	rm -f ${OBJ_SHELL}
	rm -f ${OBJ_LS}

clean-doc :
	@echo
	@echo Cleaning : object files
	@echo --------
	@echo
	rm -fr ${DOCTARGET}

clean-emacs :
	@echo
	@echo Cleaning : emacs back-ups
	@echo --------
	@echo
	rm -f ${SOURCE}/*~
	rm -f ${SOURCE}/\#*\#
	rm -f *~
	rm -f \#*\#

clean-bin :
	@echo
	@echo Cleaning : binaries
	@echo --------
	@echo
	rm -f ${BINSHELL}
	rm -f ${BINLS}

distclean : clean clean-emacs clean-bin


dirs : 
	@for dir in ${DIRLIST} ;\
	do \
	    echo Creating directory : $${dir} ;\
	    echo ------------------ ;\
	    if test -d $${dir} ;\
	    then \
		echo Directory already exists ;\
	    else mkdir -p $${dir} ;\
	    fi ;\
	    echo Done ;\
	    echo ;\
	done

# Binaires
${BIN}/${TARGET_SHELL} : ${${TARGET_SHELL}:%=${SOURCE}/%}
${BIN}/${TARGET_LS} : ${${TARGET_LS}:%=${SOURCE}/%}

${BIN}/% : $(OBJ_SHELL)
	@echo
	@echo Linking bytecode : $@
	@echo ----------------
	@echo
	${CC} -o $@ $^ ${LDFLAGS}
	@echo
	@echo Done
	@echo

# Regles generiques
%.o : %.c %.h 
	@echo
	@echo Compiling $@
	@echo --------
	@echo
	$(CC) $(CFLAGS) -c $< -o $@

# Documentation 
doc : ${SRC} ${INT} ${DOX}
	doxygen; doxygen

#############################
# Inclusion et spécificités #
#############################

.PHONY : all clean clean-doc clean-emacs clean-bin distclean doc

