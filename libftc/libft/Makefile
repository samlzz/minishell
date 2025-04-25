# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 19:03:59 by sliziard          #+#    #+#              #
#    Updated: 2025/04/11 13:20:52 by sliziard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#* VARIABLES
NAME =		libft.a

#TODO: Folders name must end with '\'
SRC_DIR =
OBJ_DIR =
INCL_DIR =

CC = gcc
CFLAGS := -Wall -Wextra -Werror
RM = rm -f
MD = mkdir -p
AR = ar rcs

C_FILES =	ft_char.c			\
			ft_convert.c		\
			ft_convert_base.c	\
			ft_is.c 			\
			ft_mem.c 			\
			ft_output.c			\
			ft_split.c			\
			ft_safe_str.c		\
			ft_str_alloc.c		\
			ft_str_iter.c		\
			ft_str.c			\
			ft_utils.c			\
			private/ft_convert_utils.c	\
			
#* Colors

ESC = \033[
DEF_COLOR = $(ESC)0;39m
GRAY = $(ESC)0;90m
RED = $(ESC)0;91m
GREEN = $(ESC)0;92m
YELLOW = $(ESC)0;93m
BLUE = $(ESC)0;94m
MAGENTA = $(ESC)0;95m
CYAN = $(ESC)0;96m
UNDERLINE = $(ESC)4m

COLOR_PRINT = @printf "$(1)$(2)$(DEF_COLOR)\n"


#* Automatic

ifdef INCL_DIR
	CFLAGS += -I$(INCL_DIR)
endif

SRCS = $(addprefix $(SRC_DIR), $(C_FILES))
OBJS := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))
O_DIRS := $(sort $(dir $(OBJS)))

#? cmd for make final file
ifeq ($(suffix $(NAME)), .a)
	LINK_CMD = $(AR) $(NAME) $(OBJS)
else
	LINK_CMD = $(CC) $(CFLAGS) $(OBJS) -o $(NAME)
endif

#* Rules

all:	$(NAME)

$(NAME): $(O_DIRS) $(OBJS)
	@printf "$(GRAY)"
	$(LINK_CMD)
	$(call COLOR_PRINT,$(GREEN)$(UNDERLINE),$(NAME) compiled !)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(call COLOR_PRINT,$(YELLOW),Compiling: $<)
	@$(CC) $(CFLAGS) -c $< -o $@

$(O_DIRS):
	@$(MD) $@

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@$(RM) libft_obj.txt
	$(call COLOR_PRINT,$(BLUE),$(NAME) object files cleaned!)

fclean:		clean
	@$(RM) $(NAME)

re:		fclean all
	$(call COLOR_PRINT,$(GREEN),Cleaned and rebuilt everything for $(NAME)!)

.PHONY:		all clean fclean re
