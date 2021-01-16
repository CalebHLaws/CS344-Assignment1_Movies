make:
	gcc --std=gnu99 -o movies main.c
test:
	gcc --std=gnu99 -o  movies -g main.c
run:
	./movies movies.csv
