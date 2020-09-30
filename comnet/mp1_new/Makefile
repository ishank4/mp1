# make server for compiling server.c

server: server.c
	gcc -I . server.c -o echos

#make client for compiling client.c

client: client.c
	gcc -I . client.c -o echo

# make clean 
clean:
	$(RM) *.o   
