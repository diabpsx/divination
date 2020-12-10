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

// each character is 18 (0x12) bytes in txt.out
// they are 12x12 pixels in size

unsigned short getb(unsigned short n)
{
	return (n & 0x7FFF) - 1;
}

void _get_font(unsigned char *d, unsigned short num, unsigned char *abuff)
{
	long i;
	char bcy, shift;

	int uVar2;
	int uVar3;
	int uVar4;
	unsigned char *bp;
	int uVar7;
	
	bp = &abuff[getb(num)];
	//if (bp == NULL) {
	//	DBG_Error(NULL, "psxsrc/KANJI.CPP", 482);
	//}

	uVar7 = *bp++;
	memset(d, 0, 12 * 12);
	uVar2 = uVar7 & 1;
	uVar4 = 1;

	for (i = 0; i < 12 * 11; i++) {
		uVar3 = uVar4 << 1;
		if (uVar2 != 0) {
			*d = 7;
		}
		d++;
		if (!(uVar4 & 0x7F)) {
			uVar3 = 1;
			uVar7 = *bp++;
		}
		uVar2 = uVar7 & uVar3;
		uVar4 = uVar3;
	}
}

int main(int argc, char *argv[])
{
	int i, j, k, l;

	if (argc != 3) {
		printf("Usage: dkanji [in] [out]\n");
		return 0;
	}

	int bufsize;
	unsigned char *p = LoadBinaryFile(argv[1], &bufsize);

	FILE *fp = fopen(argv[2], "wb");

	if(fp == NULL) {
		printf("Can't create output file\n");
		return 0;
	}

	k = bufsize / 18;
	fprintf(fp, "Total number of chars: %d\n", k);

	for(l = 0; l < k; l++) {
		unsigned char font[144];
		unsigned short kan = 0x8000 + (0x12 * l) + 1;
		_get_font(font, kan, p);
		fprintf(fp, "----- Id %d (0x%02X) -----\n", l, kan);
		for(i = 0; i < 12; i++) {
			for(j = 0; j < 12; j++) {
				if(font[j+(i*12)]) {
					fprintf(fp, "#");
				} else {
					fprintf(fp, " ");
				}
			}
			fprintf(fp, "\n");
		}
	}

	fclose(fp);
	free(p);

	return 0;
}
