#!/bin/sh
echo $1
rm server
cc -c server.c -o server.o
cc -c $1_svc.c -o $1_svc.o
cc -c $1_xdr.c -o $1_xdr.o
cc -o server server.o $1_svc.o $1_xdr.o libunpipc.a -lnsl
