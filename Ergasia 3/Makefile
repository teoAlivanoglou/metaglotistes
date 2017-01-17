all:
	bison -d parser.y
	flex parser.l
	gcc -o parser lex.yy.c parser.tab.c -lfl -lm
	./parser < input.txt > output.txt

clean:
	rm parser.tab.c parser.tab.h lex.yy.c parser
