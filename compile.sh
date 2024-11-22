#!/bin/sh
rm -f tictactoe.exe 2>/dev/null

gcc -Iheader `pkg-config --cflags --static gtk+-3.0` -o tictactoe \
    src/main.c src/minimax.c src/importData.c src/ml-naive-bayes.c src/elapsedTime.c \
    `pkg-config --libs --static gtk+-3.0` \

	#enable this for windows 11 release only!!! 
	#Only for project submission backup if user can't compile, run docker, install libs.
	
	#-Wl,-rpath,./dlls

if [ $? -eq 0 ]; then
	OS=$(uname)

	if [ "$OS" = "Linux" ]; then
    		echo "[COMPILE] Running on Linux"

	elif [[ "$OS" == *"_NT"* ]]; then
		echo "[COMPILE] Running on Windows"
		./tictactoe.exe
	
	else
		echo "[COMPILE] UNKNOWN OS!"
		echo "[COMPILE] ABORTED APP LAUNCH..."

	fi
else
	echo "[COMPILE] FAILED TO COMPILE!!!"
fi
