#!/bin/sh
rm cl1
rm a.out
rm ms
rm rs*
rm serv1
rm ws*
gcc client.c -o cl1 -lpthread
gcc Mainserver.c
gcc server.c -o serv1 




# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./a.out; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./cl1 ; exec bash\""
