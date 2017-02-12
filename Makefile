#
# Boilerplate.
#
define add_target
    $(info add_target($1))
    $(eval $(eval_args))
    $(eval $(call eval_args,$1,\
        OBJDIR := $(firstword $($1.OBJDIR) ./objs/$1),\
    ))
    $(eval $(call eval_args,$1,\
        objs := $(obj_from_source),
    ))
    $(eval $1 := $($1.TARGET))

    TARGETS += $($1)
    PHONY_TARGETS += $1
    CLEAN_TARGETS += clean_$1

    .PHONY: clean_$1
    clean_$1:; rm -rf $($1.OBJDIR) $($1)

    .PHONY: $1
    $1: $($1)

    $($1): target:=$1
    $($1): $($1.objs); $$(if $$(wildcard $$(@D)),,mkdir -p $$(@D) && )$$(add_target.link)
    $($1.objs):; $$(if $$(wildcard $$(@D)),,mkdir -p $$(@D) && )$$(add_target.compile)
    $(foreach $1.SOURCES,$($1.SOURCES),$(eval $(obj_from_source): $($1.SOURCES)))

    $(info end)
endef

void :=
space := $(void) $(void)
obj_from_source = $(addprefix $($1.OBJDIR)/,$(addsuffix .o,$(basename $(notdir $($1.SOURCES)))))
eval_args = $(foreach i,2 3 4 5 6 7 8 9,$(call eval_arg,$1,$(strip $($i))))
eval_arg = $(if $2,$(info $(space)$(space)$1.$2)$(eval $1.$2))

# Link command line
add_target.link = $(CC) $($(target).LDLAGS) -o $@ $^

# Compile command line
add_target.compile = $(CC) -c -o $@ $($(target).CFLAGS) $<


# -- Directories --
SOURCE := ./src
BIN := ./bin


# Add 'shell' target to the project
$(eval $(call add_target,shell,\
    TARGET := $(BIN)/shell,\
    SOURCES += ${SOURCE}/divers.c,\
    SOURCES += ${SOURCE}/commandes_externes.c,\
    SOURCES += ${SOURCE}/commandes_internes.c,\
    SOURCES += ${SOURCE}/entities.c,\
    SOURCES += ${SOURCE}/execution.c,\
    SOURCES += ${SOURCE}/parse.c,\
    SOURCES += ${SOURCE}/shell.c,\
    CFLAGS := -O0 -g -W -Wall -I./include -Wextra -Wconversion -Werror -mtune=native  -march=native  -std=c99  -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700,\
))

# Add 'ls' target to the project
$(eval $(call add_target,ls,\
    TARGET := $(BIN)/ls,\
    SOURCES := ${SOURCE}/commande_ls.c,\
    CFLAGS := -O0 -g -W -Wall -I./include -Wextra -Wconversion -Werror -mtune=native  -march=native  -std=c99  -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700,\
))


all: ${PHONY_TARGETS}
.PHONY: all

clean: | $(CLEAN_TARGETS)
.PHONY: clean