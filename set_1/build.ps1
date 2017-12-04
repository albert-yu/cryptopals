gcc -c hex_to_base64.c -o hex.o
gcc -c fixed_xor.c -o fixed_xor.o
gcc -c xor_cipher.c -o cipher.o
gcc -c main.c -o main.o
# to link
gcc hex.o fixed_xor.o cipher.o main.o -o build/test