# make directory
md -Force build

gcc -c hex_to_base64.c -o hex.o
gcc -c fixed_xor.c -o fixed_xor.o
gcc -c xor_cipher.c -o cipher.o
gcc -c detect_xor.c -o detect.o
gcc -c main.c -o main.o
# to link
gcc hex.o fixed_xor.o cipher.o detect.o main.o -o build/test