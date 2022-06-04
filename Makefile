NAME = "tty-snake"

install:
	gcc -o $(NAME) tty-snake.c
	chmod +x ./$(NAME)
	mv $(NAME) ~/.local/bin/$(NAME)
	echo "Done"