#!/bin/sh
gcc -Iheader `pkg-config --cflags --static gtk+-3.0` -o tictactoe.exe src/main.c src/minimax.c `pkg-config --libs --static gtk+-3.0`

if [ $? -eq 0 ]; then
	./tictactoe.exe
else
	echo "[ERROR] FAILED TO COMPILE!!!"
fi
