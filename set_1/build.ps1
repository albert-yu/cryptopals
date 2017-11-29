gcc -c hex_to_base64.c -o hex.o
gcc -c main.c -o main.o
# to link
gcc hex.o main.o -o build/test