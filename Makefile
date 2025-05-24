# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/23 20:25:27 by mle-flem          #+#    #+#              #
#    Updated: 2025/05/24 23:02:58 by mle-flem         ###   ########.fr        #
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

INCLUDES =	$(LIBFT_DIR)/include/	\
			$(LIBFT_DIR)/libft/		\
			$(SRC_DIR)/ 			\
			$(SRC_DIR)/parsing/

SRCS =	main.c									\
		env/init.c								\
		env/wrappers.c							\
		error/print_err.c						\
		heredoc/hd_tokeniser.c					\
		heredoc/hd_writer.c						\
		parsing/ast/ast_init.c					\
		parsing/expansion/argword_builder.c		\
		parsing/expansion/ft_argword.c			\
		parsing/expansion/expander.c			\
		parsing/expansion/wildcards.c			\
		parsing/expansion/withespace_split.c	\
		parsing/lexer/ft_token.c				\
		parsing/lexer/tokeniser.c				\
		parsing/parser/parse_binop.c			\
		parsing/parser/parse_expr.c				\
		parsing/parser/parse_rd.c				\
		parsing/input.c							\
		utils/argword_utils.c					\
		utils/parse_utils.c						\
		utils/string_utils.c					\
		test/print_ast.c

OBJS	= $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
DEPS	= $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.d))

# **************************************************************************** #
#                                Makefile  vars                                #
# **************************************************************************** #

SHELL	:= bash

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
	CLR_RESET	= \033[0m
endif

MAKE_WIDTH = 80
MAKE_SILENCE =

ifeq ($(NOPRETTY),)
	MAKEFLAGS += --no-print-directory
	MAKEFLAGS += --silent
	MAKE_SILENCE = >/dev/null
endif

# **************************************************************************** #
#                                Makefile logic                                #
# **************************************************************************** #

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

define run_and_test
	if [ ! -z '$(NOPRETTY)' ]; then \
		echo '$(2)'; \
		$(RUN_CMD); \
		if [ $$RESULT -ne 0 ]; then \
			$(RM) .files_changed; \
		fi; \
		cat $@.log; \
		$(RM) $@.log; \
		exit $$RESULT; \
	fi; \
	len=`printf '%b' '$(1)' | wc -m`; \
	true_len=`printf '%b' '$(1)' | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | wc -m`; \
	printf "%-$$(($(MAKE_WIDTH)-3+$$len-$$true_len))b" '$(1)'; \
	$(RUN_CMD); \
	if [ $$RESULT -ne 0 ]; then \
		printf '%b\n' '$(CLR_ERROR)[✖]$(CLR_RESET)'; \
		$(RM) .files_changed; \
		if [ -z "$(NOPROGRESS)" ]; then \
			echo; \
		fi; \
	elif [ -s $@.log ]; then \
		printf '%b\n' '$(CLR_WARN)[⚠]$(CLR_RESET)'; \
	else \
		printf '%b\n' '$(CLR_OK)[✓]$(CLR_RESET)'; \
	fi; \
	cat $@.log; \
	$(RM) $@.log; \
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
	echo 0/$$MISSING > .files_changed
endef

define draw_bar
	TOTAL=`cat .files_changed | cut -d'/' -f2`; \
	DONE=`cat .files_changed | cut -d'/' -f1`; \
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
	echo $$DONE/$$TOTAL > .files_changed
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
	i=0; \
	while [ $$i -lt $(MAKE_WIDTH) ]; do \
		printf '%b' ' '; \
		((i=$$i+1)); \
	done; \
	tput rc; \
	if [ $$line -gt $$max ]; then \
		new_line=1; \
		echo; \
	else \
		((line=$$line+1)); \
	fi; \
	tput sc; \
	tput cup $$line; \
	$(draw_bar); \
	if [ $$new_line -eq 1 ]; then \
		((line=$$line-1)); \
		tput cup $$line; \
	else \
		tput rc; \
	fi
endef

# **************************************************************************** #
#                                Makefile rules                                #
# **************************************************************************** #

all:	header setup $(NAME)
	@$(RM) .files_changed

header:
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
	@printf '%b' '$(CLR_OBJ)CFLAGS: $(CLR_WARN)$(CFLAGS)\n'
	@printf '%b' '$(CLR_OBJ)LDLIBS: $(CLR_WARN)$(LDLIBS)\n'
	@printf '%b' '$(CLR_OBJ)LDFLAGS: $(CLR_WARN)$(LDFLAGS)\n'
	@printf '%b' '$(CLR_RESET)\n'
endif

-include $(DEPS)

$(NAME):	$(LIBFT_DIR)/libftc.a $(OBJS)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Linking $(CLR_OBJ)$@$(CLR_RESET),$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS))

$(BUILD_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Compiling $(CLR_OBJ)$(@F)$(CLR_RESET),$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -c $< -o $@)

$(LIBFT_DIR)/libftc.a:
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Making $(CLR_OBJ)$(@F)$(CLR_RESET),$(MAKE) -C $(LIBFT_DIR) $(MAKE_SILENCE))

setup:
	@$(call save_files_changed)

clean:	header
	@$(call run_and_test,$(CLR_COM)clean $(CLR_OBJ)libftc,$(MAKE) -C $(LIBFT_DIR) clean $(MAKE_SILENCE))
	@$(call run_and_test,$(CLR_COM)clean $(CLR_OBJ)minishell,$(RM) -r $(BUILD_DIR))

fclean:	header clean
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)libftc,$(MAKE) -C $(LIBFT_DIR) fclean $(MAKE_SILENCE))
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)minishell,$(RM) $(NAME))

re:	fclean all

.PHONY:	all header setup clean fclean re
