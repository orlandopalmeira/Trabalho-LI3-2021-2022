FILES = ./src/bstrees.c ./src/fileUtils.c ./src/repos.c ./src/commits.c ./src/users.c ./src/linkedLists.c ./src/readWrite.c ./src/parsing.c ./src/main.c
CFLAGS = -Wno-unused-result -ansi -Wall -Wextra -pedantic -std=gnu99

compile_g2:
	gcc $(CFLAGS) $(FILES) -o guiao-2 -lm

execute_g2:
	gcc -O2 $(CFLAGS) $(FILES) -o guiao-2 -lm
	./guiao-2 commands.txt
	rm guiao-2

check_mem_g2:
	gcc -g -O2 $(CFLAGS) $(FILES) -o guiao-2 -lm
	valgrind -s --leak-check=full ./guiao-2 commands.txt
	rm guiao-2

debug: 
	gcc -g -O2 $(CFLAGS) $(FILES) -o guiao-2 -lm

rm_output:
	rm ./saida/*.txt

clean:
	rm guiao-2