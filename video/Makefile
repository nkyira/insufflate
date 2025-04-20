NAME = insufflate

SRC_DIR = src/
OBJ_DIR = obj/

SRC = $(wildcard $(SRC_DIR)*.c)
OBJ = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

CC = gcc -Wall -Wextra -Werror -g
LFLAGS = -L$(GNL_DIR) -lgnl

GNL_DIR = src/get_next_line/
GNL = $(GNL_DIR)libgnl.a

all : $(NAME)

$(NAME): $(OBJ) $(GNL)
	$(CC) $(OBJ) $(LFLAGS) -o $(NAME)

$(GNL) :
	$(MAKE) -C $(GNL_DIR)


$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) out.txt .temp.ppm
	make -C $(GNL_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(GNL_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
