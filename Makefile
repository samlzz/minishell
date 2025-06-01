# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/23 20:25:27 by mle-flem          #+#    #+#              #
#    Updated: 2025/06/01 14:31:37 by mle-flem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                    Config                                    #
# **************************************************************************** #

NAME	= minishell
AUTHORS	= sliziard, mle-flem
CC		= cc
AR		= ar
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror
DFLAGS	= -MMD -MP -MF $(@:.o=.d)
LDLIBS	= -lftc -lreadline
LDFLAGS	= -L$(LIBFT_DIR)
ARFLAGS	= rcs

# **************************************************************************** #
#                                Program's srcs                                #
# **************************************************************************** #

LIBFT_DIR	= libftc
SRC_DIR		= src
BUILD_DIR	= build

DEP_PREFIX	=

INCLUDES =	$(LIBFT_DIR)/include/	\
			$(LIBFT_DIR)/libft/		\
			$(SRC_DIR)/ 			\
			$(SRC_DIR)/parsing/

##begin: SRCS
SRCS =	main.c \
		env/init.c \
		env/wrappers.c \
		error/print_err.c \
		heredoc/hd_tokeniser.c \
		heredoc/hd_writer.c \
		parsing/input.c \
		parsing/ast/ast_init.c \
		parsing/expansion/argword_builder.c \
		parsing/expansion/expander.c \
		parsing/expansion/ft_argword.c \
		parsing/expansion/wildcards.c \
		parsing/expansion/withespace_split.c \
		parsing/lexer/ft_token.c \
		parsing/lexer/tokeniser.c \
		parsing/parser/parse_binop.c \
		parsing/parser/parse_expr.c \
		parsing/parser/parse_rd.c \
		test/print_ast.c \
		utils/argword_utils.c \
		utils/parse_utils.c \
		utils/string_utils.c
##end: SRCS

OBJS	= $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
DEPS	= $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.d)) $(LIBFT_DIR)/libftc.a.d

# **************************************************************************** #
#                                Makefile  vars                                #
# **************************************************************************** #

SHELL	:= bash

MK_FILE	= $(firstword $(MAKEFILE_LIST))

ifeq ($(origin MAKE_COLOR), undefined)
	MAKE_COLOR	:= $(if $(CI),,$(if $(MAKE_TERMOUT),1,))
endif

ifeq ($(origin NOPROGRESS), undefined)
	NOPROGRESS	:= $(if $(CI),1,$(if $(MAKE_TERMOUT),,1))
endif

NOPRETTY	?=

ifeq ($(OS),Windows_NT)
    MAKE_OS := Windows
else
    MAKE_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifneq ($(MAKE_COLOR),)
	CLR_COM		= \033[0;34m
	CLR_OBJ		= \033[0;36m
	CLR_OK		= \033[0;32m
	CLR_ERROR	= \033[0;31m
	CLR_WARN	= \033[0;33m
	CLR_GUIDE	= \033[0;2m
	CLR_RESET	= \033[0m
endif

MAKE_WIDTH = 80

ifeq ($(MAKE_ROOT),)
	MAKE_ROOT=$(CURDIR)
endif

override MAKE_PARENT := $(if $(MAKE_PARENT),$(MAKE_PARENT):,)$(CURDIR)

ifeq ($(NOPRETTY),)
	MAKEFLAGS += --no-print-directory
	MAKEFLAGS += --silent
	ifneq ($(NOPROGRESS),)
		ifneq ($(MAKELEVEL),0)
			MAKE_INDENT := $(shell printf '    %.0s' {1..$(MAKELEVEL)})
		endif
	else
		ifneq ($(MAKELEVEL),0)
			ifneq ($(MAKELEVEL),1)
				MAKE_INDENT := $(shell printf '$(CLR_GUIDE) ┃  %.0s' {1..$$(($(MAKELEVEL)-1))})
			endif
			MAKE_INDENT := $(MAKE_INDENT)$(shell printf '%b' "$(CLR_GUIDE) ┗━ ")
		endif
		MAKE_INDENT_INNER := $(shell printf "%$$(($(MAKELEVEL)*4+4))b" '')
	endif
endif

ifeq ($(shell git rev-parse HEAD &>/dev/null; echo $$?),0)
	DATE	:= $(shell git log -1 --date=format:"%Y/%m/%d %T" --format="%ad")
	HASH	:= $(shell git rev-parse --short HEAD)
endif

ifeq ($(MAKE_OS),Darwin)
	RUN_CMD = script -q '$@.log' '$(2)'' >/dev/null; \
			  RESULT=$$?
else ifeq ($(MAKE_OS),Linux)	
	RUN_CMD = script -q -e -c '$(2)' '$@.log' >/dev/null; \
			  RESULT=$$?; \
			  sed -i '1d' '$@.log'; \
			  sed -i "$$(($$(wc -l < $@.log)-1)),\$$d" '$@.log'
else
	RUN_CMD = $(2) 2> $@.log; \
			  RESULT=$$?
endif

# **************************************************************************** #
#                                Makefile logic                                #
# **************************************************************************** #

define run_and_test
	if [ ! -z '$(NOPRETTY)' ]; then \
		echo '$(2)'; \
		$(RUN_CMD); \
		if [ $$RESULT -ne 0 ]; then \
			$(RM) .files_changed; \
		fi; \
		cat $@.log >&2; \
		$(RM) $@.log; \
		exit $$RESULT; \
	fi; \
	len=`printf '%b' "$(MAKE_INDENT)$(1)" | wc -c`; \
	true_len=`printf '%b' "$(MAKE_INDENT)$(1)" | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | wc -m`; \
	printf "%-$$(($(MAKE_WIDTH)-3+$$len-$$true_len))b" '$(MAKE_INDENT)$(1)'; \
	if [ -z '$(NOPROGRESS)' ]; then \
		LINE=`oldstty=$$(stty -g); stty raw -echo min 0; tput u7 > /dev/tty; IFS=';' read -r -d R -a pos; stty $$oldstty; echo $$(($${pos[0]:2}-1))`; \
		DIR=`echo "$(if $(MAKE_PARENT),$(MAKE_PARENT),$(CURDIR))" | cut -d':' -f "$$(($(MAKELEVEL)+1))"`; \
		SCUR=`[ -f "$$DIR/.files_changed" ] && head -n1 < "$$DIR/.files_changed" | cut -d'/' -f1 || echo 0`; \
		tput sc; \
		i=$(MAKELEVEL); \
		while [ $$i -gt 0 ]; do \
			DIR=`echo "$(if $(MAKE_PARENT),$(MAKE_PARENT),$(CURDIR))" | cut -d':' -f "$$((i+1))"`; \
			CUR=`[ -f "$$DIR/.files_changed" ] && head -n1 < "$$DIR/.files_changed" | cut -d'/' -f1 || echo 0`; \
			MAX=`[ -f "$$DIR/.files_changed" ] && head -n1 < "$$DIR/.files_changed" | cut -d'/' -f2 || echo 0`; \
			if [ $$i -ne $(MAKELEVEL) ] && [ $$(($$MAX+$$CUR)) -eq 0 ]; then \
				tput cup $$LINE $$((($$i-1)*4+1)); \
				printf '%b' ' '; \
			fi; \
			if [ $$i -eq $(MAKELEVEL) ] && [ $$CUR -gt 1 ] && [ $$((CUR-1)) -le $$MAX ] && [ $$MAX -ne 0 ]; then \
				tput cup $$(($$LINE-1)) $$((($$i-1)*4+1)); \
				printf '%b' '$(CLR_GUIDE)┣'; \
			elif [ $$i -ne $(MAKELEVEL) ] && [ $$CUR -ge 0 ] && [ $$((CUR-1)) -le $$MAX ] && [ $$MAX -ne 0 ] && [ $$SCUR -le 1 ]; then \
				tput cup $$(($$LINE-1)) $$((($$i-1)*4+1)); \
				printf '%b' '$(CLR_GUIDE)┣'; \
			fi; \
			((i=$$i-1)); \
		done; \
		tput rc; \
	fi; \
	$(RUN_CMD); \
	if [ $$RESULT -ne 0 ]; then \
		printf '%b\n' '$(CLR_ERROR)[✖]$(CLR_RESET)'; \
		$(RM) .files_changed; \
		if [ -z "$(NOPROGRESS)" ]; then \
			tput sc; \
			printf '%$(MAKE_WIDTH)b' ''; \
			tput rc; \
		fi; \
	elif [ -s $@.log ]; then \
		printf '%b\n' '$(CLR_WARN)[⚠]$(CLR_RESET)'; \
	else \
		printf '%b\n' '$(CLR_OK)[✓]$(CLR_RESET)'; \
	fi; \
	cat $@.log >&2; \
	$(RM) $@.log; \
	exit $$RESULT
endef

define run_make_and_test
	if [ ! -z '$(NOPRETTY)' ]; then \
		echo '$(2)'; \
		$(2); \
		RESULT=$$?; \
		if [ $$RESULT -ne 0 ]; then \
			$(RM) .files_changed; \
		fi; \
		exit $$RESULT; \
	fi; \
	if [ ! -z '$(1)' ]; then \
		len=`printf '%b' '$(MAKE_INDENT)$(1)' | wc -c`; \
		true_len=`printf '%b' '$(MAKE_INDENT)$(1)' | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | wc -m`; \
		printf "%-$$(($(MAKE_WIDTH)-3+$$len-$$true_len))b" '$(MAKE_INDENT)$(1)'; \
		echo; \
	fi; \
	( set -o pipefail; ( set -o pipefail; $(2) MAKE_ROOT="$(MAKE_ROOT)" MAKE_PARENT="$(MAKE_PARENT)" | tee $(@F).log ) 3>&1 1>&2 2>&3 | tee $(@F).err.log ) 3>&1 1>&2 2>&3; \
	RESULT=$$?; \
	if [ -z $(NOPROGRESS) ] && [ ! -z '$(1)' ]; then \
		CUR=`oldstty=$$(stty -g); stty raw -echo min 0; tput u7 > /dev/tty; IFS=';' read -r -d R -a pos; stty $$oldstty; echo $$(($${pos[0]:2}-1))`; \
		MAX=$$((`tput lines` - 2)); \
		OUT_LINES=`wc -l < $(@F).log`; \
		if [ $$OUT_LINES -gt 1 ]; then \
			((OUT_LINES=$$OUT_LINES/2)); \
		fi; \
		TERM_WIDTH=$$(tput cols); \
		ERR_LINES=`cat $(@F).err.log | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | fold -w "$$TERM_WIDTH" | wc -l`; \
		LINE=$$(($$CUR - $$OUT_LINES - $$ERR_LINES - 1)); \
		tput sc; \
		tput cup $$LINE $$(($(MAKE_WIDTH) - 3)); \
		if [ $$RESULT -ne 0 ]; then \
			printf '%b\n' '$(CLR_ERROR)[✖]$(CLR_RESET)'; \
			$(RM) .files_changed; \
		elif [ -s $(@F).err.log ]; then \
			printf '%b\n' '$(CLR_WARN)[⚠]$(CLR_RESET)'; \
		else \
			printf '%b\n' '$(CLR_OK)[✓]$(CLR_RESET)'; \
		fi; \
		tput rc; \
		tput cr; \
	fi; \
	if [ $$RESULT -ne 0 ]; then \
		$(RM) .files_changed; \
	fi; \
	$(RM) $(@F).log $(@F).err.log; \
	exit $$RESULT
endef

define save_files_changed
	SRCS=`echo $(SRCS)`; \
	OBJS=`find $(BUILD_DIR) -type f -name '*.o' 2>/dev/null | sed 's/$(BUILD_DIR)\///'`; \
	MISSING=`echo $$SRCS | wc -w`; \
	for SRC in $$SRCS; do \
		for OBJ in $$OBJS; do \
			if [ $${SRC%.c} != $${OBJ%.o} ] || [ $(SRC_DIR)/$$SRC -nt $(BUILD_DIR)/$$OBJ ]; then \
				continue; \
			fi; \
			DEP=`echo $(BUILD_DIR)/$${OBJ%.o}.d`; \
			HEADS=`< $$DEP tr ' ' '\n' | sort -u | grep '.h$$'`; \
			RECOMP=0; \
			for HEAD in $$HEADS; do \
				if [ -f $$HEAD ] && [ $$HEAD -nt $(BUILD_DIR)/$$OBJ ]; then \
					RECOMP=1; \
					break; \
				fi; \
			done; \
			if [ $$RECOMP -eq 0 ]; then \
				((MISSING=$$MISSING-1)); \
			fi; \
		done; \
	done; \
	[ $$MISSING -lt 1 ] && MISSING=1; \
	printf '0/%d\n' "$$MISSING" > .files_changed; \
	i=$(MAKELEVEL); \
	while [ $(MAKELEVEL) -ne 0 ] && [ $$i -ge 0 ]; do \
		DIR=`echo "$(if $(MAKE_PARENT),$(MAKE_PARENT),$(CURDIR))" | cut -d':' -f "$$((i+1))"`; \
		CUR=`[ -f "$$DIR/.files_changed" ] && head -n1 < "$$DIR/.files_changed" | cut -d'/' -f1 || echo 0`; \
		MAX=`[ -f "$$DIR/.files_changed" ] && head -n1 < "$$DIR/.files_changed" | cut -d'/' -f2 || echo 0`; \
		((MAX=$$MAX+$$MISSING+($$MISSING == `echo $$SRCS | wc -w`))); \
		printf '%d/%d\n' "$$CUR" "$$MAX" > "$$DIR/.files_changed"; \
		((i=$$i-1)); \
	done
endef

define draw_bar
	TOTAL=`cat "$(MAKE_ROOT)/.files_changed" | cut -d'/' -f2`; \
	DONE=`cat "$(MAKE_ROOT)/.files_changed" | cut -d'/' -f1`; \
	if [ $$TOTAL -eq 0 ]; then \
		TOTAL=1; \
		DONE=1; \
	fi; \
	TOTAL_WIDTH=`printf '%b' "$$TOTAL" | wc -m`; \
	BAR_WIDTH=$$(($(MAKE_WIDTH)-6-$$TOTAL_WIDTH*2)); \
	RES=`echo "scale=2; $$DONE/$$TOTAL*$$BAR_WIDTH" | bc`; \
	RES=`echo $${RES%%.*}`; \
	printf '%b' '$(CLR_OBJ)[$(CLR_RESET)'; \
	i=0; \
	while [ $$i -lt $$BAR_WIDTH ]; do \
		if [ $$i -lt $$RES ]; then \
			printf '%b' '$(CLR_OK)█$(CLR_RESET)'; \
		else \
			printf '%b' '$(CLR_OK)▒$(CLR_RESET)'; \
		fi; \
		((i=$$i+1)); \
	done; \
	printf '%b' '$(CLR_OBJ)]$(CLR_RESET)'; \
	printf "%b%$${TOTAL_WIDTH}b%b%$${TOTAL_WIDTH}b%b" ' ($(CLR_COM)' "$$DONE" '$(CLR_RESET)/$(CLR_COM)' "$$TOTAL" '$(CLR_RESET))'; \
	printf '%b' ' '; \
	((DONE=$$DONE+1)); \
	echo $$DONE/$$TOTAL > "$(MAKE_ROOT)/.files_changed"; \
	if [ $(MAKELEVEL) -ne 0 ]; then \
		TOTAL=`cat .files_changed | cut -d'/' -f2`; \
		DONE=`cat .files_changed | cut -d'/' -f1`; \
		((DONE=$$DONE+1)); \
		echo $$DONE/$$TOTAL > .files_changed; \
	fi
endef

define display_progress_bar
	if [ ! -z '$(NOPROGRESS)' ] || [ ! -z '$(NOPRETTY)' ]; then \
		exit 0; \
	fi; \
	line=`oldstty=$$(stty -g); stty raw -echo min 0; tput u7 > /dev/tty; IFS=';' read -r -d R -a pos; stty $$oldstty; echo $$(($${pos[0]:2}-1))`; \
	max=`tput lines`; \
	((max=$$max-2)); \
	new_line=0; \
	tput sc; \
	printf '%$(MAKE_WIDTH)b' ''; \
	tput rc; \
	if [ $$line -gt $$max ]; then \
		new_line=1; \
		echo; \
	elif [ $$line -lt $$max ]; then \
		((line=$$line+1)); \
	fi; \
	tput sc; \
	tput cup $$line; \
	$(draw_bar); \
	if [ $$new_line -eq 1 ]; then \
		((line=$$line-1)); \
		tput cup $$line; \
	else \
		echo; \
		tput rc; \
	fi
endef

define generate_deps
	printf '%b' '$(DEP_PREFIX)$(NAME): $$(wildcard ' > $@.d.tmp; \
	cat $(DEPS) | sed -e 's/[:\t\\\)]//g' -e 's/$$(wildcard//g' | tr ' ' '\n' | sed -e '/^$$/d' -e '/.[ao]$$/d' | sort -u | sed 's|^|$(DEP_PREFIX)|' | tr '\n' ' ' >> $@.d.tmp; \
	printf '%b' ')' >> $@.d.tmp; \
	sed -i 's/ )$$/)/' $@.d.tmp; \
	cat $@.d.tmp | fold -w $$((80-4*2-2)) -s | sed -e '$$!s/$$/\\/' -e '2,$$s/^/\t\t/' > $@.d; \
	$(RM) $@.d.tmp
endef

define update_sources
	len=`printf '%b' "$(MAKE_INDENT)$(1)" | wc -c`; \
	true_len=`printf '%b' "$(MAKE_INDENT)$(1)" | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | wc -m`; \
	printf "%-$$(($(MAKE_WIDTH)-3+$$len-$$true_len))b" '$(MAKE_INDENT)$(1)'; \
	export LC_ALL=C; \
	traverse() { \
		find "$$1" -mindepth 1 -maxdepth 1 -type f -name '*.c' -not -name '_*' \
			| git check-ignore -nv --stdin | awk '/^::\t/ { print $$2 }' \
			| sort \
			| sed 's|^\./||'; \
		find "$$1" -mindepth 1 -maxdepth 1 -type d \
			| git check-ignore -nv --stdin \
			| awk '/^::\t/ { print $$2 }' \
			| sort \
			| while read -r sub; do \
				traverse "$$sub"; \
			done \
	}; \
	FILES=`( cd "$(SRC_DIR)" && traverse '.' ) 2>/dev/null \
		| sed -e 's/^/\t\t/' -e '$$!s/$$/ \\\\/' -e '1s/^\t\t/SRCS =\t/'`; \
	awk -v files="$${FILES//$$'\n'/\\\\n}" ' \
		BEGIN { inblock = 0 } \
		$$0 == "##begin: SRCS" { print; print files; inblock = 1; next } \
		$$0 == "##end: SRCS" { print; inblock = 0; next } \
		inblock == 0 { print } \
	' "$(MK_FILE)" > "$(MK_FILE).tmp" && \
		mv "$(MK_FILE).tmp" "$(MK_FILE)"; \
	$(RM) "$(MK_FILE).tmp"; \
	printf '%b\n' '$(CLR_OK)[✓]$(CLR_RESET)'
endef

# **************************************************************************** #
#                                Makefile rules                                #
# **************************************************************************** #

all:	header setup $(NAME)
	@$(RM) .files_changed

header:
ifeq ($(MAKELEVEL),0)
ifeq ($(NOPRETTY),)
	@printf '%b' '$(CLR_OK)'
	@echo '01001101 01101001 01101110 01101001 01110011 01101000 01100101 01101100 01101100'
	@echo '                                                                                '
	@echo '                                                                                '
	@echo '0     ███▄ ▄███▓ ██▓ ███▄    █  ██▓  ██████  ██░ ██ ▓█████  ██▓     ██▓        0'
	@echo '0    ▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▒██    ▒ ▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒        0'
	@echo '1    ▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒░ ▓██▄   ▒██▀▀██░▒███   ▒██░    ▒██░        1'
	@echo '0    ▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░  ▒   ██▒░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░        0'
	@echo '1    ▒██▒   ░██▒░██░▒██░   ▓██░░██░▒██████▒▒░▓█▒░██▓░▒████▒░██████▒░██████▒    1'
	@echo '0    ░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░    0'
	@echo '1    ░      ░    ▒ ░   ░   ░ ░  ▒ ░░  ░  ░   ░  ░░ ░   ░     ░ ░     ░ ░       1'
	@echo '0           ░    ░           ░  ░        ░   ░  ░  ░   ░  ░    ░  ░    ░  ░    0'
	@echo '                                                                                '
	@echo '                                                                                '
	@echo '01001101 01101001 01101110 01101001 01110011 01101000 01100101 01101100 01101100'
	@printf '%b' '$(CLR_RESET)'
	@echo
	@echo
ifneq ($(HASH),)
	@printf '%b' '$(CLR_OBJ)Name: $(CLR_WARN)$(NAME)@$(HASH)\n'
else
	@printf '%b' '$(CLR_OBJ)Name: $(CLR_WARN)$(NAME)\n'
endif
	@printf '%b' '$(CLR_OBJ)Authors: $(CLR_WARN)$(AUTHORS)\n'
ifneq ($(DATE),)
	@printf '%b' '$(CLR_OBJ)Date: $(CLR_WARN)$(DATE)\n'
endif
	@printf '%b' '$(CLR_OBJ)CC: $(CLR_WARN)$(CC)\n'
	@printf '%b' '$(CLR_OBJ)AR: $(CLR_WARN)$(AR)\n'
	@printf '%b' '$(CLR_OBJ)CFLAGS: $(CLR_WARN)$(CFLAGS)\n'
	@printf '%b' '$(CLR_OBJ)ARFLAGS: $(CLR_WARN)$(ARFLAGS)\n'
	@printf '%b' '$(CLR_OBJ)LDLIBS: $(CLR_WARN)$(LDLIBS)\n'
	@printf '%b' '$(CLR_OBJ)LDFLAGS: $(CLR_WARN)$(LDFLAGS)\n'
	@printf '%b' '$(CLR_RESET)\n'
endif
endif

-include $(DEPS)

$(NAME):	$(LIBFT_DIR)/libftc.a $(OBJS)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Linking $(CLR_OBJ)$@$(CLR_RESET),$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS))
	@$(call generate_deps)

$(BUILD_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Compiling $(CLR_OBJ)$(@F)$(CLR_RESET),$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -c $< -o $@)

$(LIBFT_DIR)/libftc.a:
	@$(call run_make_and_test,$(CLR_COM)Making $(CLR_OBJ)$(@F)$(CLR_RESET),$(MAKE) -C $(LIBFT_DIR))

setup:
	@$(call save_files_changed)

clean:	header
	@$(call run_and_test,$(CLR_COM)clean $(CLR_OBJ)minishell,$(RM) -r $(BUILD_DIR) .files_changed $(NAME).d)
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) clean)

ifeq ($(MAKELEVEL),0)

fclean:	header clean
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)minishell,$(RM) $(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) fclean)

else

fclean:	header
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)minishell,$(RM) $(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) fclean)

endif

re:	fclean all

update-srcs:	header
	@$(call update_sources,$(CLR_COM)Updating sources of $(CLR_OBJ)$(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) update-srcs)

.PHONY:	all header setup clean fclean re update-srcs
