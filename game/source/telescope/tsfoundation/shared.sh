gcc -c tsfoundation_linux.c -fPIC -o arcane.o -lX11 -lm -lGL
gcc -shared -o arcane.so arcane.o
