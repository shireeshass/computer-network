#!/bin/sh
rm fifo
rm a.out
rm out
rm op
rm sfifo
rm sig
rm spipe
rm spopen
gcc Mainserver.c
gcc fifo.c -o fifo
gcc popen.c -o spopen
gcc  pipe.c -o spipe
gcc signaling.c -o sig
gcc output.c -o out


# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./a.out; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./fifo ; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./sig ; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./out ; exec bash\""
