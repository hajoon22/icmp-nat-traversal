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
checksum.o: src/traversal/icmp/checksum/checksum.c src/traversal/icmp/checksum/checksum.h
	gcc -c src/traversal/icmp/checksum/checksum.c -o checksum.o
icmp.o: src/traversal/icmp/icmp.c src/traversal/icmp/icmp.h
	gcc -c src/traversal/icmp/icmp.c -o icmp.o
stun.o: src/traversal/stun/stun.c src/traversal/stun/stun.h
	gcc -c src/traversal/stun/stun.c -o stun.o

clean:
	rm -rf *.o server client
