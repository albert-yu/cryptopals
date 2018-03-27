CC = gcc
CFLAGS = -g -O2
OBJECTS = main.o hex_to_base64.o fixed_xor.o xor_cipher.o detect_xor.o repeating_key_xor.o

main : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o main.exe

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main