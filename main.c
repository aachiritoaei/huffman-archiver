#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "structures.h"
#include "queue.h"
#include "huffman.h"

int main(int argc, char *argv[]){
 	/* Command line paramters: input file, output file */
 	FILE *fin, *fout;
	/* Compress */
	if(argv[1][1] == 'c'){
		fin = fopen(argv[2], "r");
		fout = fopen(argv[3], "wb");
		compressFile(fin, fout);
	}
	/* Decompress */
	if(argv[1][1] == 'd'){
		fin = fopen(argv[2], "rb");
		fout = fopen(argv[3], "w");
		decompressFile(fin, fout);
	}
	/* Close files */
	fclose(fin);
	fclose(fout);
	return 0;
 }