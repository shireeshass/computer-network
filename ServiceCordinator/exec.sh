#!/bin/sh
rm cl1
rm a.out
rm ms
rm rs*
rm s1
rm ws*
gcc client.c -o cl1 -lpthread
gcc MainServer.c
gcc server.c -o s1 -lpthread 




# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./a.out; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./cl1 1; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./s1 ; exec bash\""
