# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/23 20:25:27 by mle-flem          #+#    #+#              #
#    Updated: 2025/07/25 10:16:33 by mle-flem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                    Config                                    #
# **************************************************************************** #

NAME	= minishell
AUTHORS	= sliziard, mle-flem

ifeq ($(MAKELEVEL),0)
	ifeq ($(origin CC), default)
		CC	= cc
	endif
	ifeq ($(origin AR), default)
		AR	= ar
	endif
	ifeq ($(origin RM), default)
		RM	= rm -f
	endif
	CFLAGS	+= -Wall -Wextra -Werror
	ifneq ($(filter bonus, $(MAKECMDGOALS)),)
		CFLAGS	+= -DMINISHELL_BONUS
	endif
	LDLIBS	+= -lftc -lreadline
	LDFLAGS	+= -L$(LIBFT_DIR)
	ARFLAGS	= rcs
endif
DFLAGS	= -MMD -MP -MF $(@:.o=.d)

export CC
export AR
export RM
export CFLAGS
export ARFLAGS

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
		utils.c \
		builtins/cd.c \
		builtins/cd_helpers.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/exit.c \
		builtins/export.c \
		builtins/export_helpers.c \
		builtins/pwd.c \
		builtins/unset.c \
		builtins/utils.c \
		env/ft_ctx.c \
		env/ft_env.c \
		env/ft_env_init.c \
		env/ft_env_utils.c \
		error/print_err.c \
		error/print_err_bonus.c \
		exec/cmd_flow.c \
		exec/cmd_utils.c \
		exec/exec_flow.c \
		exec/exec_utils.c \
		exec/pipe_flow.c \
		exec/redir_flow.c \
		exec/redir_utils.c \
		exec/utils.c \
		expansion/expand_handler.c \
		expansion/expand_utils.c \
		expansion/expander.c \
		expansion/export_expander.c \
		expansion/field_splitting.c \
		expansion/tild_expand.c \
		expansion/tild_utils.c \
		expansion/wildcards_bonus.c \
		expansion/ft_argword/argword_append.c \
		expansion/ft_argword/argword_fill.c \
		expansion/ft_argword/argword_lib.c \
		expansion/ft_argword/argword_lib_bonus.c \
		expansion/ft_argword/argword_sort.c \
		handler/signal.c \
		heredoc/hd_init.c \
		heredoc/hd_tokeniser.c \
		heredoc/hd_utils.c \
		heredoc/hd_writter.c \
		parsing/ast/ast_init.c \
		parsing/ast/ast_init_bonus.c \
		parsing/lexer/ft_token.c \
		parsing/lexer/tokeniser.c \
		parsing/lexer/tokeniser_bonus.c \
		parsing/parser/parse_binop.c \
		parsing/parser/parse_expr.c \
		parsing/parser/parse_rd.c \
		parsing/parser/parse_rd_bonus.c \
		parsing/parser/rd_utils.c \
		test/print_ast.c \
		test/print_token.c
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
	ifneq ($(MAKELEVEL),0)
		MAKE_INDENT := $(shell printf ' ┃  %.0s' {0..$(MAKELEVEL)})
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

define print_with_indent
	IFS=':' read -r -a DIRS <<< "$(if $(MAKE_PARENT),$(MAKE_PARENT),$(CURDIR))"; \
	content='$(CLR_GUIDE)'; \
	i=$(MAKELEVEL); \
	while [ $$i -gt 0 ]; do \
		DIR="$${DIRS[$$i]}"; \
		if [ -f "$$DIR/.files_changed" ]; then \
			IFS='/' read CUR MAX < "$$DIR/.files_changed"; \
		else \
			CUR=0; MAX=0; \
		fi; \
		if [ $$i -ne $(MAKELEVEL) -a $$MAX -eq 0 ]; then \
			content="    $$content"; \
		elif [ $$i -ne $(MAKELEVEL) -a $$CUR -le $$MAX ]; then \
			content=" ┃  $$content"; \
		elif [ $$i -eq $(MAKELEVEL) -a $$CUR -lt $$MAX -a $$MAX -ne 0 ]; then \
			content=" ┣━ $$content"; \
		elif [ $$i -eq $(MAKELEVEL) -a $$CUR -ge $$MAX ]; then \
			content=" ┗━ $$content"; \
		fi; \
		((i=$$i-1)); \
	done; \
	content="$(CLR_GUIDE)$$content$(CLR_RESET)$(1)$(CLR_RESET)"; \
	len=`printf '%b' "$$content" | wc -c`; \
	true_len=`printf '%b' "$$content" | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | wc -m`; \
	printf "%-$$(($(MAKE_WIDTH)-3+$$len-$$true_len))b$(2)" "$$content"
endef

define run_and_test
	$(call print_with_indent,$(1)); \
	$(RUN_CMD); \
	if [ $$RESULT -ne 0 ]; then \
		printf '%b\n' '$(CLR_ERROR)[✖]$(CLR_RESET)'; \
		$(RM) .files_changed; \
		[ -z "$(NOPROGRESS)" ] && printf '%b' '\x1b[2K'; \
	elif [ -s $@.log ]; then \
		printf '%b\n' '$(CLR_WARN)[⚠]$(CLR_RESET)'; \
	else \
		printf '%b\n' '$(CLR_OK)[✓]$(CLR_RESET)'; \
	fi; \
	cat $@.log | awk '{ print "$(CLR_GUIDE)$(MAKE_INDENT)$(CLR_RESET)" $$0 }' >&2; \
	$(RM) $@.log; \
	exit $$RESULT
endef

define run_make_and_test
	if [ ! -z '$(1)' ]; then \
		$(call print_with_indent,$(1),\n); \
	fi; \
	( set -o pipefail; ( set -o pipefail; $(2) MAKE_ROOT="$(MAKE_ROOT)" MAKE_PARENT="$(MAKE_PARENT)" | tee $(@F).log ) 3>&1 1>&2 2>&3- | tee $(@F).err.log ) 3>&1 1>&2 2>&3-; \
	RESULT=$$?; \
	if [ -z $(NOPROGRESS) ] && [ ! -z '$(1)' ]; then \
		CUR=`oldstty=$$(stty -g); stty raw -echo min 0; printf '\x1b[6n' > /dev/tty; IFS=';' read -r -d R -a pos; stty $$oldstty; echo $${pos[0]:2}`; \
		MAX=$$((`tput lines` - 2)); \
		OUT_LINES=`wc -l < $(@F).log`; \
		if [ $$OUT_LINES -gt 1 ]; then \
			((OUT_LINES=$$OUT_LINES/2)); \
		fi; \
		TERM_WIDTH=$$(tput cols); \
		ERR_LINES=`cat $(@F).err.log | sed -r 's/\x1b\[[0-9;]*[a-zA-Z]//g' | fold -w "$$TERM_WIDTH" | wc -l`; \
		LINE=$$(($$CUR - $$OUT_LINES - $$ERR_LINES - 1)); \
		if [ $$RESULT -ne 0 ]; then \
			printf '%b' "\x1b7\x1b[$$LINE;$$(($(MAKE_WIDTH)-2))H$(CLR_ERROR)[✖]$(CLR_RESET)\n\x1b8"; \
			$(RM) .files_changed; \
		elif [ -s $(@F).err.log ]; then \
			printf '%b' "\x1b7\x1b[$$LINE;$$(($(MAKE_WIDTH)-2))H$(CLR_WARN)[⚠]$(CLR_RESET)\n\x1b8"; \
		else \
			printf '%b' "\x1b7\x1b[$$LINE;$$(($(MAKE_WIDTH)-2))H$(CLR_OK)[✓]$(CLR_RESET)\n\x1b8"; \
		fi; \
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
	IFS=':' read -r -a DIRS <<< "$(if $(MAKE_PARENT),$(MAKE_PARENT),$(CURDIR))"; \
	i=$(MAKELEVEL); \
	while [ $$i -ge 0 ]; do \
		DIR="$${DIRS[$$i]}"; \
		if [ -f "$$DIR/.files_changed" ]; then \
			IFS='/' read CUR MAX < "$$DIR/.files_changed"; \
		else \
			CUR=0; MAX=0; \
		fi; \
		((MAX=$$MAX+$$MISSING+($$MISSING == `echo $$SRCS | wc -w`))); \
		printf '%d/%d\n' "$$CUR" "$$MAX" > "$$DIR/.files_changed"; \
		((i=$$i-1)); \
	done
endef

define draw_bar
	IFS=':' read -r -a DIRS <<< "$(if $(MAKE_PARENT),$(MAKE_PARENT),$(CURDIR))"; \
	DIR="$${DIRS[0]}"; \
	if [ -f "$$DIR/.files_changed" ]; then \
		IFS='/' read DONE TOTAL < "$$DIR/.files_changed"; \
	else \
		DONE=1; TOTAL=1; \
	fi; \
	TOTAL_WIDTH=`printf '%b' "$$TOTAL" | wc -m`; \
	BAR_WIDTH=$$(($(MAKE_WIDTH)-6-$$TOTAL_WIDTH*2)); \
	RES=`echo "$$DONE" "$$TOTAL" "$$BAR_WIDTH" | awk '{printf "%d", $$1 / $$2 * $$3}'`; \
	BAR_DONE=`printf '█%.0s' $$(seq 1 $$RES)`; \
	BAR_TODO=`printf '▒%.0s' $$(seq 1 $$(($$BAR_WIDTH-$$RES)))`; \
	printf '$(CLR_OBJ)[$(CLR_OK)%s%s$(CLR_OBJ)] $(CLR_RESET)($(CLR_COM)%d$(CLR_RESET)/$(CLR_COM)%d$(CLR_RESET))' "$$BAR_DONE" "$$BAR_TODO" "$$DONE" "$$TOTAL"; \
	i=$(MAKELEVEL); \
	while [ $$i -ge 0 ]; do \
		DIR="$${DIRS[$$i]}"; \
		if [ -f "$$DIR/.files_changed" ]; then \
			IFS='/' read DONE TOTAL < "$$DIR/.files_changed"; \
		else \
			DONE=0; TOTAL=0; \
		fi; \
		printf '%d/%d\n' "$$(($$DONE+1))" "$$TOTAL" > "$$DIR/.files_changed"; \
		((i=$$i-1)); \
	done
endef

define display_progress_bar
	if [ ! -z '$(NOPROGRESS)' ]; then \
		exit 0; \
	fi; \
	line=`oldstty=$$(stty -g); stty raw -echo min 0; printf '\x1b[6n' > /dev/tty; IFS=';' read -r -d R -a pos; stty $$oldstty; echo $${pos[0]:2}`; \
	max=`tput lines`; \
	((max=$$max-1)); \
	new_line=$$(($$line>$$max)); \
	((line=$$line+($$line<$$max))); \
	printf '%b%b%b' '\x1b[2K' "$$([ $$line -gt $$max ] && printf '\\n')" "\x1b7\x1b[$$line;1H"; \
	$(draw_bar); \
	if [ $$new_line -eq 1 ]; then \
		((line=$$line-1)); \
		printf '%b' "\x1b[$$line;1H"; \
	else \
		printf '%b' "\n\x1b8"; \
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
	$(call print_with_indent,$(1)); \
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

ifeq ($(NOPRETTY),)

all:	header setup $(NAME)
	@$(RM) .files_changed

else

all:	$(NAME)

endif

bonus:	all

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
ifeq ($(NOPRETTY),)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Linking $(CLR_OBJ)$@,$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS))
else
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)
endif
	@$(call generate_deps)

$(BUILD_DIR)/%.o:	$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
ifeq ($(NOPRETTY),)
	@$(call display_progress_bar)
	@$(call run_and_test,$(CLR_COM)Compiling $(CLR_OBJ)$(@F),$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -c $< -o $@)
else
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES:%=-I%) -c $< -o $@
endif

$(LIBFT_DIR)/libftc.a:
ifeq ($(NOPRETTY),)
	@$(call run_make_and_test,$(CLR_COM)Making $(CLR_OBJ)$(@F),$(MAKE) -C $(LIBFT_DIR))
else
	$(MAKE) -C $(LIBFT_DIR)
endif

setup:
	@$(call save_files_changed)

clean:	header
ifeq ($(NOPRETTY),)
	@$(call run_and_test,$(CLR_COM)clean $(CLR_OBJ)minishell,$(RM) -r $(BUILD_DIR) .files_changed $(NAME).d)
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) clean)
else
	$(RM) -r $(BUILD_DIR) $(NAME).d
	$(MAKE) -C $(LIBFT_DIR) clean
endif

ifeq ($(MAKELEVEL),0)

fclean:	header clean
ifeq ($(NOPRETTY),)
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)minishell,$(RM) $(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) fclean)
else
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
endif

else

fclean:	header
ifeq ($(NOPRETTY),)
	@$(call run_and_test,$(CLR_COM)fclean $(CLR_OBJ)minishell,$(RM) $(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) fclean)
else
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
endif

endif

re:	fclean all

update-srcs:	header
	@$(call update_sources,$(CLR_COM)Updating sources of $(CLR_OBJ)$(NAME))
	@$(call run_make_and_test,,$(MAKE) -C $(LIBFT_DIR) update-srcs)

.PHONY:	all bonus header setup clean fclean re update-srcs
