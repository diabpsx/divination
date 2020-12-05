#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *LoadBinaryFile(char *file, int *bufsize)
{
	unsigned char *p;
	FILE *fp;

	fp = fopen(file, "rb");

	if(fp == NULL) {
		fprintf(stderr, "Can't open file %s\n", file);
		exit(-1);
	}

	fseek(fp, 0L, SEEK_END);
	*bufsize = ftell(fp);
	p = (unsigned char *)malloc(sizeof(unsigned char) * (*bufsize + 1));
	fseek(fp, 0L, SEEK_SET);
	fread(p, sizeof(char), *bufsize, fp);
	fclose(fp);
	return p;
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		fprintf(stderr, "Usage: dtext [dat] [out]\n");
		return 0;
	}

	int psize;
	unsigned char *pf = LoadBinaryFile(argv[1], &psize);

	unsigned int *index = (unsigned int *)pf;

	int numentries = index[0] >> 2;

	FILE *fout = fopen(argv[2], "wb");
	if(fout == NULL) {
		fprintf(stderr, "Can't create file %s\n", argv[2]);
		exit(-1);
	}

	char newline[] = "\\n";
	int i;
	for(i = 0; i < numentries; i++) {
		unsigned char *str = &pf[index[i]];
		while(*str != '\0') {
			if(*str == '\n') {
				fwrite(newline, sizeof(char), sizeof(newline) - 1, fout);
			} else {
				fwrite(str, sizeof(char), 1, fout);
			}
			str++;
		}
		fprintf(fout, "\r\n");
	}

	fclose(fout);
	free(pf);

	return 0;
}
