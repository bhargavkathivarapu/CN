g++ client.c -o cli
g++ s1.c -o s1
g++ server.c -lpthread -o TSS
./TSS port
./cli port
