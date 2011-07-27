all: myshell

myshell: myshell.c lex.yy.o
	cc -o myshell lex.yy.o myshell.c -lfl

lex.yy.o: lex.l
	flex lex.l
	cc -c lex.yy.c

clean: 
	rm myshell lex.yy.o lex.yy.c 

