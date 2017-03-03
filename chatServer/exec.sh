#!/bin/sh
rm cl1 
rm cl2
rm cl3
rm cl4
rm c1
rm c2
rm c3
rm c4
rm cs
gcc client.c -o cl1 -lpthread
gcc client.c -o cl2 -lpthread
gcc client.c -o cl3 -lpthread
gcc client.c -o cl4 -lpthread
gcc server.c -o server




# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./server; exec bash\""
sleep 2
gnome-terminal -e "bash -c \" ./cl1 c1; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./cl2 c2; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./cl3 c3; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./cl4 c4; exec bash\""
