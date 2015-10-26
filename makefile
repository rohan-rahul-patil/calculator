calculator: calculator3.o numstack.o charstack.o charqueue.o
	cc calculator3.o numstack.o charstack.o charqueue.o -o calculator
calculator3.o: calculator3.c 
	cc calculator3.c -c
numstack.o: numstack.c numstack.h
	cc numstack.c -c
charstack.o: charstack.c charstack.h
	cc charstack.c -c
charqueue.o: charqueue.c  charqueue.h
	cc charqueue.c -c
