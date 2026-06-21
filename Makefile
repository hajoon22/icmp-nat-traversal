client: client-main.o icmp.o checksum.o traversal.o stun.o
	gcc -o client client-main.o icmp.o checksum.o traversal.o stun.o
client-main.o: src/client/main.c
	gcc -c src/client/main.c -o client-main.o

server: server-main.o stun.o icmp.o checksum.o traversal.o
	gcc -o server server-main.o stun.o icmp.o checksum.o traversal.o
server-main.o: src/server/main.c
	gcc -c src/server/main.c -o server-main.o

traversal.o: src/traversal/traversal.c src/traversal/traversal.h
	gcc -c src/traversal/traversal.c -o traversal.o
checksum.o: src/icmp/checksum/checksum.c src/icmp/checksum/checksum.h
	gcc -c src/icmp/checksum/checksum.c -o checksum.o
icmp.o: src/icmp/icmp.c src/icmp/icmp.h
	gcc -c src/icmp/icmp.c -o icmp.o
stun.o: src/stun/stun.c src/stun/stun.h
	gcc -c src/stun/stun.c -o stun.o

clean:
	rm -rf *.o server client
