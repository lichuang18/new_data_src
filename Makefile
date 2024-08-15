all:
	#gcc src/send.c -o send
	gcc src/two_send.c -o two_send -pthread
	gcc src/md5sum.c -o md5sum

clean: 
	rm -f two_send md5sum
