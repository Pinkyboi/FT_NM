
NAME = ft_nm

SYMLINK = libft_malloc.so

CC = gcc

SRC_PATH = ./src/
OBJ_PATH = ./obj/
INC_PATH = ./inc/

SRC_NAME =	main.c nm64.c nm32.c utils.c mini_libft.c

DEPS = $(INC_PATH)/ft_nm.h

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))

FLAGS = -Wall -Wextra -Werror -I $(INC_PATH)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $^ -o $@
	@echo "\033[32m[OK]\033[0m \033[33mft_nm compiled\033[0m"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(DEPS)
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(FLAGS) -o $@ -c $<

clean:
	@rm -rf $(OBJ_PATH)
	@echo "\033[32m[OK]\033[0m \033[33mft_nm cleaned\033[0m"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(SYMLINK)
	@echo "\033[32m[OK]\033[0m \033[33mft_nm fcleaned\033[0m"

re: fclean all
