Group chat

compilation
g++ server.c -o GS
g++ s1.c -lpthread -o s1
g++ client.c -o  cli

running 
at server ./GS
at client ./cli (port in 32001-32004)
i/p at client:
./s1
message to be sent
o/p:message received
