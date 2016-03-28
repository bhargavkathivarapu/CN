g++ client.c -lpthread -o cli
g++ m1.c -lpthread -o m1(similarly m2,m3)
g++ tcserv.c -lpthread -o TC
./TC
./cli port
