.PHONY: build run clean

build: myHuffman

myHuffman: *.o
	gcc *.o -o huffman 

*.o: *.h *.c
	gcc -Wall -c *.c

run: build
	./huffman

verify : huffman data.in 
	./huffman -c data.in dataBin.out
	./huffman -d dataBin.out dataDecomp.in
	diff -s data.in dataDecomp.in

clean:
	rm -f huffman *.o  dataBin.out dataDecomp.in
