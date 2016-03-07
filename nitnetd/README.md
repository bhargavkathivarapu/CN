nitnetd

compilation
g++ server.c -o serv
g++ s1.c -o s1
g++ s2.c -o s2
g++ client.c -o cli

running
at server
./serv
at client
./cli (portnumber in 32001-32004)
i/p at client:
./s1
message to be sent
o/p:message received or unsucesss
