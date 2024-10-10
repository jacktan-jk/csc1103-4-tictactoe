gcc `pkg-config --cflags --static gtk+-3.0` testgui.c -o tictactoe.exe `pkg-config --libs --static gtk+-3.0`
./tictactoe.exe
