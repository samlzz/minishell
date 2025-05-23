#* VARIABLES
NAME = libftc.a

#TODO: Folders name must end with '\'
SRC_DIR = containers_src/
OBJ_DIR = containers_obj/
LIBFT = libft

INCL_DIR = $(LIBFT) include
LIB_DIRS = $(LIBFT)
LIB_FILES = ft

CC = gcc
CFLAGS := -Wall -Wextra -Werror -MMD -g
RM = rm -f
MD = mkdir -p
AR = ar rcs

C_FILES =	dyn/ft_dynbuf.c				\
			dyn/ft_dynint.c				\
			dyn/private/generic_dyn.c	\
			hashmap/ft_hmap_iter.c				\
			hashmap/ft_hmap_op.c				\
			hashmap/ft_hmap.c					\
			hashmap/private/ft_hmap_helpers.c	\
			linked_list/ft_lst_edit.c	\
			linked_list/ft_lst_get.c	\
			linked_list/ft_lst_iter.c
			
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
LIBFT_GIT = git@github.com:samlzz/libft.git

INCL_FLAGS = $(addprefix -I, $(INCL_DIR))
LIB_FLAGS = $(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIB_FILES))

SRCS := $(addprefix $(SRC_DIR), $(C_FILES))
OBJS := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)
O_DIRS := $(sort $(dir $(OBJS)))

#? cmd for make final file
ifeq ($(suffix $(NAME)), .a)
	LINK_CMD = mv $(LIBFT)/$(LIBFT).a ./$(NAME) && $(AR) $(NAME) $(OBJS)
else
	LINK_CMD = $(CC) $(OBJS) -o $(NAME) $(CFLAGS) $(LIB_FLAGS)
endif

#* Rules

all:	lib $(NAME)

lib: $(LIBFT)
	@printf "$(GRAY)"
	@$(MAKE) -C $(LIBFT)
	$(call COLOR_PRINT,$(GREEN),$(LIBFT)compiled !)

$(LIBFT):
	$(call COLOR_PRINT,$(MAGENTA),Retrieving libft sources...)
	@printf "$(GRAY)"
	git clone $(LIBFT_GIT) ./$(LIBFT)
	@printf "$(DEF_COLOR)"
	@$(RM) -r ./$(LIBFT)/.git
	@$(RM) ./$(LIBFT)/.gitignore

relib: dellib $(LIBFT)

dellib:
	$(call COLOR_PRINT,$(MAGENTA),$(LIBFT) cleaned !)
	@$(RM) -r $(LIBFT)
	@sed -i 's|^LIBFT *=.*|LIBFT = libft|' Makefile 2>/dev/null || true
	@sed -i -E '/^LIB_FILES *=/ s/\bft\w*\b/ft/g' Makefile 2>/dev/null || true

$(NAME): $(O_DIRS) $(OBJS)
	@printf "$(GRAY)"
	$(LINK_CMD)
	$(call COLOR_PRINT,$(GREEN)$(UNDERLINE),$(NAME) compiled !)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(call COLOR_PRINT,$(YELLOW),Compiling: $<)
	@$(CC) -c $< -o $@ $(CFLAGS) $(INCL_FLAGS)

-include $(DEPS)

$(O_DIRS):
	@$(MD) $@

clean:
	@$(RM) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@printf "$(GRAY)"
	$(MAKE) clean -C $(LIBFT)
	$(call COLOR_PRINT,$(BLUE),$(NAME) object files cleaned!)

fclean:		clean
	@$(RM) $(NAME)
	@$(RM) $(LIBFT)/$(LIBFT).a
	$(call COLOR_PRINT,$(CYAN),executables files cleaned!)

re:		fclean all
	$(call COLOR_PRINT,$(GREEN),Cleaned and rebuilt everything for $(NAME)!)


.PHONY:		all lib relib dellib clean fclean re
