all:
	gcc src/send.c -o send
	gcc src/two_send.c -o two_send -pthread

clean: 
	rm send two_send
