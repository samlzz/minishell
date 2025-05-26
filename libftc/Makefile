# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/24 23:07:20 by mle-flem          #+#    #+#              #
#    Updated: 2025/05/26 10:55:39 by mle-flem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                    Config                                    #
# **************************************************************************** #

NAME	= libftc.a
AUTHORS	= sliziard, mle-flem
CC		= cc
AR		= ar
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror
DFLAGS	= -MMD -MP -MF $(@:.o=.d)
ARFLAGS	= rcs

# **************************************************************************** #
#                                Program's srcs                                #
# **************************************************************************** #

LIBFT_DIR	= libft
SRC_DIR		= containers_src
BUILD_DIR	= build

INCLUDES =	include/		\
			$(LIBFT_DIR)/

SRCS =	dyn/ft_dynbuf.c						\
		dyn/ft_dynint.c						\
		dyn/private/generic_dyn.c			\
		hashmap/ft_hmap_iter.c				\
		hashmap/ft_hmap_op.c				\
		hashmap/ft_hmap.c					\
		hashmap/private/ft_hmap_helpers.c	\
		linked_list/ft_lst_edit.c			\
		linked_list/ft_lst_get.c			\
		linked_list/ft_lst_iter.c

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

ifeq ($(MAKE_ROOT),)
	MAKE_ROOT=$(CURDIR)
endif

ifeq ($(NOPRETTY),)
	MAKEFLAGS += --no-print-directory
	MAKEFLAGS += --silent
	MAKE_INDENT := $(shell printf "%$$(($(MAKELEVEL)*4))b" '')
	MAKE_INDENT_INNER := $(shell printf "%$$(($(MAKELEVEL)*4+4))b" '')
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
	len=`printf '%b' "$(MAKE_INDENT)$(1)" | wc -m`; \
	true_len=`printf '%b' "$(MAKE_INDENT)$(1)" | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | wc -m`; \
	printf "%-$$(($(MAKE_WIDTH)-3+$$len-$$true_len))b" "$(MAKE_INDENT)$(1)"; \
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
	fold -s -w $$(($(MAKE_WIDTH) - $(MAKELEVEL) * 4 - 4)) $@.log | sed 's/^/$(MAKE_INDENT_INNER)/' >&2; \
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
		len=`printf '%b' '$(MAKE_INDENT)$(1)' | wc -m`; \
		true_len=`printf '%b' '$(MAKE_INDENT)$(1)' | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | wc -m`; \
		printf "%-$$(($(MAKE_WIDTH)-3+$$len-$$true_len))b" '$(MAKE_INDENT)$(1)'; \
		echo; \
	fi; \
	( set -o pipefail; ( set -o pipefail; $(2) MAKE_ROOT="$(MAKE_ROOT)" MAKE_PARENT="$(CURDIR)" | tee $(@F).log ) 3>&1 1>&2 2>&3 | tee $(@F).err.log ) 3>&1 1>&2 2>&3; \
	RESULT=$$?; \
	if [ ! -z '$(1)' ]; then \
		CUR=`oldstty=$$(stty -g); stty raw -echo min 0; tput u7 > /dev/tty; IFS=';' read -r -d R -a pos; stty $$oldstty; echo $$(($${pos[0]:2}-1))`; \
		MAX=$$((`tput lines` - 2)); \
		OUT_LINES=`wc -l < $(@F).log`; \
		if [ $$OUT_LINES -gt 1 ]; then \
			((OUT_LINES=$$OUT_LINES/2)); \
		fi; \
		ERR_LINES=`wc -l < $(@F).err.log`; \
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
	elif [ $$RESULT -ne 0 ]; then \
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
	LINE=`oldstty=$$(stty -g); stty raw -echo min 0; tput u7 > /dev/tty; IFS=';' read -r -d R -a pos; stty $$oldstty; echo $$(($${pos[0]:2}-1))`; \
	MAX=`tput lines`; \
	((MAX=$$MAX-2)); \
	NEW_LINE=0; \
	tput sc; \
	printf '%$(MAKE_WIDTH)b' ''; \
	tput rc; \
	if [ $$LINE -gt $$MAX ]; then \
		NEW_LINE=1; \
		echo; \
	elif [ $$line -lt $$max ]; then \
		((LINE=$$LINE+1)); \
	fi; \
	tput sc; \
	tput cup $$LINE; \
	$(draw_bar); \
	if [ $$NEW_LINE -eq 1 ]; then \
		((LINE=$$LINE-1)); \
		tput cup $$LINE; \
	else \
		echo; \
		tput rc; \
	fi
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
	@echo '0                 ██▓     ██▓ ▄▄▄▄     █████▒▄▄▄█████▓ ▄████▄                  0'
	@echo '0                ▓██▒    ▓██▒▓█████▄ ▓██   ▒ ▓  ██▒ ▓▒▒██▀ ▀█                  0'
	@echo '1                ▒██░    ▒██▒▒██▒ ▄██▒████ ░ ▒ ▓██░ ▒░▒▓█    ▄                 1'
	@echo '0                ▒██░    ░██░▒██░█▀  ░▓█▒  ░ ░ ▓██▓ ░ ▒▓▓▄ ▄██▒                0'
	@echo '1                ░██████▒░██░░▓█  ▀█▓░▒█░      ▒██▒ ░ ▒ ▓███▀ ░                1'
	@echo '0                ░ ▒░▓  ░░▓  ░▒▓███▀▒ ▒ ░      ▒ ░░   ░ ░▒ ▒  ░                0'
	@echo '1                ░ ░ ▒  ░ ▒ ░▒░▒   ░  ░          ░      ░  ▒                   1'
	@echo '0                  ░ ░    ▒ ░ ░    ░  ░ ░      ░      ░                        0'
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
	@printf '%b' '$(CLR_RESET)\n'
endif
endif

-include $(DEPS)

$(NAME):	$(LIBFT_DIR)/libft.a $(OBJS)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Archiving $(CLR_OBJ)$@$(CLR_RESET),cp $(LIBFT_DIR)/libft.a $(NAME) && $(AR) $(ARFLAGS) $@ $(OBJS))

$(BUILD_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Compiling $(CLR_OBJ)$(@F)$(CLR_RESET),$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -c $< -o $@)

$(LIBFT_DIR)/libft.a:
	@$(call run_make_and_test,$(CLR_COM)Making $(CLR_OBJ)$(@F)$(CLR_RESET),$(MAKE) -C $(LIBFT_DIR))

setup:
	@$(call save_files_changed)

clean:	header
	@$(call run_and_test,$(CLR_COM)clean $(CLR_OBJ)libftc,$(RM) -r $(BUILD_DIR))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) clean)

ifeq ($(MAKELEVEL),0)

fclean:	header clean
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)libftc,$(RM) $(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) fclean)

else

fclean:	header
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)libftc,$(RM) $(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) fclean)

endif

re:	fclean all

.PHONY:	all header setup clean fclean re
