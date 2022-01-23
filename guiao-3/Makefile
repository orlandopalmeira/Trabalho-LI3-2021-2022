FILES = ./src/bstrees.c ./src/fileUtils.c ./src/repos.c ./src/commits.c ./src/users.c ./src/linkedLists.c ./src/readWrite.c ./src/parsing.c ./src/main.c
CFLAGS = -Wno-unused-result -ansi -Wall -Wextra -pedantic -std=gnu99 

compile_g3:
	gcc $(CFLAGS) $(FILES) -o guiao-3 -lm

execute_g3:
	gcc -O2 $(CFLAGS) $(FILES) -o guiao-3 -lm
	./guiao-3 commands.txt
	rm guiao-3

check_mem_g3:
	gcc -g -O2 $(CFLAGS) $(FILES) -o guiao-3 -lm
	valgrind -s --leak-check=full ./guiao-3 commands.txt
	rm guiao-3

debug: 
	gcc -g -O2 $(CFLAGS) $(FILES) -o guiao-3 -lm

clean:
	rm guiao-3
