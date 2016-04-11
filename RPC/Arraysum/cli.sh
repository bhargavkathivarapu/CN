#!/bin/sh
echo $1
rpcgen -C $1.x
cc -c client.c -o client.o
cc -c $1_clnt.c -o $1_clnt.o
cc -c $1_xdr.c -o $1_xdr.o
cc -o client client.o $1_clnt.o $1_xdr.o libunpipc.a -lnsl
