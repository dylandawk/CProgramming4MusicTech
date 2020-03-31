gcc -Wall -o synth synth.c support.c paUtils.c \
	-I/usr/local/include \
	-L/usr/local/lib -lsndfile -lportaudio -lncurses
