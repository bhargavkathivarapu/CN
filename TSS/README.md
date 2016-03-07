TSS

compilation
g++ server.c -o TSS 
g++ s1.c -lpthread -o s1
g++ client.c -o cli

running
at server ./serv 
at client ./cli 32020
i/p at client:
message to be sent 
o/p:message received
