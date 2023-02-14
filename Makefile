build:
	gcc -Wall -Wextra -std=c99 star_dust.c -o star_dust

run :
	star_dust

clean:
	rm -rf star_dust