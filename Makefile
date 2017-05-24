NAME = fdf

SRC = fdf.c\
	drawing.c\
	rotation.c\
	lines.c\
	preparation.c\
	messages.c

OBJ = $(SRC:.c=.o)

LIBINC = libft/libft.a

INC = -I h_fdf.h

CFLAGS = -Wall -Wextra -Werror

LIBMAKE = make -C libft/

MLX_FLAGS = -lmlx -framework OpenGL -framework AppKit

all: $(NAME)

$(NAME): $(OBJ)
	@$(LIBMAKE) all
	@gcc $(CFLAGS) $(MLX_FLAGS) $(LIBINC) $^ -o $(NAME)
	@echo  "\x1b[32mCompilation of fdf was finished\x1b[0m"

%.o: %.c
	@gcc $(INC) -c $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OBJ) ft_printf.h.gch printf_main.o main.o
	@echo "\033[01;31mFdf object files deleted\x1b[0m"

fclean: clean
	@rm -f $(NAME) a.out
	@echo "\033[01;31mFdf executable files deleted\x1b[0m"

re: fclean all

ra: libfclean fclean all

liball:
	@$(LIBMAKE) all

libclean:
	@$(LIBMAKE) clean

libfclean:
	@$(LIBMAKE) fclean

libre:
	@$(LIBMAKE) re
