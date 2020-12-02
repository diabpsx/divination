#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <direct.h>

#include "psx.h"
#include "align.c"
#include "lznp.c"
#include "un64.c"
#include "pcx.c"

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

void Convert555toRGB(unsigned short pal16, FILE *fp)
{
	unsigned char r, g, b, a;

	r = pal16 & 0x1F;
	g = (pal16 >> 5) & 0x1F;
	b = (pal16 >> 10) & 0x1F;
	a = (pal16 >> 11) & 1;

	r <<= 3;
	g <<= 3;
	b <<= 3;

/*
	if(a) { // semi-transparent, seems to mean the color wont be transparent?
		r >>= 1;
		g >>= 1;
		b >>= 1;
	}
*/

	//printf("Color 0x%02x, RGB %d, %d, %d, a %d\n", pal16, r, g, b, a);

	fwrite(&r, sizeof(unsigned char), 1, fp);
	fwrite(&g, sizeof(unsigned char), 1, fp);
	fwrite(&b, sizeof(unsigned char), 1, fp);
}

void WritePalBuffer(SPR_HDR *p, char *name)
{
	int i, count;
	unsigned char *tmp;
	unsigned short *pal;
	FILE *fp;

	fp = fopen(name, "wb");

	if(fp == NULL) {
		fprintf(stderr, "Can't create file %s\n", name);
		exit(-1);
	}

	tmp = (unsigned char *)p;
	tmp += p->PalOffset;
	count = ((int *)tmp)[0] >> 1;
	tmp += 4;
	pal = (unsigned short *)tmp;

	for(i = 0; i < count; i++) {
		Convert555toRGB(*pal, fp);
		pal++;
	}

	fclose(fp);
}

void DecodeData(unsigned char *p, int size, char *file)
{
	unsigned char *out;
	int newd;
	FILE *fp;

	out = (unsigned char *)malloc(sizeof(unsigned char) * (size*4 + 1));

	newd = LZNP_Decode(p, out);
	printf("Output size: %d\n", newd);

	fp = fopen(file, "wb");

	if(fp == NULL) {
		fprintf(stderr, "Can't create file %s\n", file);
		exit(-1);
	}

	fwrite(out, sizeof(char), newd, fp);

	fclose(fp);
	free(out);
}

void PrintHeaderInfo(SPR_HDR *Hdr)
{
	printf("-------------------- Header info --------------------\n");
	printf("Hdr->DecompOffset     = 0x%08x\n", Hdr->DecompOffset   );
	printf("Hdr->CreatureOffset   = 0x%08x\n", Hdr->CreatureOffset );
	printf("Hdr->PalOffset        = 0x%08x\n", Hdr->PalOffset      );
	printf("Hdr->FrameOffset      = 0x%08x\n", Hdr->FrameOffset    );
	printf("Hdr->BaseFrame        = %d\n",     Hdr->BaseFrame      );
	printf("Hdr->DestTPage        = %d\n",     Hdr->DestTPage      );
	printf("Hdr->ComponentOffset  = 0x%08x\n", Hdr->ComponentOffset);
	printf("Hdr->NumOfCreatures   = %d\n",     Hdr->NumOfCreatures );
	printf("Hdr->NumOfFrames      = %d\n",     Hdr->NumOfFrames    );
	printf("Hdr->NumOfPals        = %d\n",     Hdr->NumOfPals      );
	printf("Hdr->TWidth           = %d\n",     Hdr->TWidth         );
	printf("Hdr->THeight          = %d\n",     Hdr->THeight        );
	printf("Hdr->IsTiles          = %d\n",     Hdr->IsTiles        );
	printf("Hdr->Spare            = %d\n",     Hdr->Spare          );
}

void PrintFrameInfo(SPR_HDR *Hdr)
{
	int i;
	unsigned char *tmp;
	FRAME_FAKE *Frm;

	tmp = (unsigned char *)Hdr;
	tmp += Hdr->FrameOffset;

	Frm = (FRAME_FAKE *)tmp;

	for(i = 0; i < Hdr->NumOfFrames; i++) {
		printf("-------------------- Frame info: %d --------------------\n", i);
		printf("Frm->FrOffset:  0x%08x\n", Frm->FrOffset);
		printf("Frm->X:         %d\n",     Frm->X);
		printf("Frm->Y:         %d\n",     Frm->Y);
		printf("Frm->PalNum:    %d\n",     Frm->PalNum);
		printf("Frm->NotTrans:  %d\n",     (Frm->Flags1 >> 0)  & FRM_FLAG_NOT_TRANS);
		printf("Frm->Rotated:   %d\n",     (Frm->Flags1 >> 1)  & FRM_FLAG_ROTATED  );
		printf("Frm->InVRAM:    %d\n",     (Frm->Flags1 >> 2)  & FRM_FLAG_INVRAM   );
		printf("Frm->CompType:  %d\n",     (Frm->Flags1 >> 3)  & FRM_FLAG_COMPTYPE );
		printf("Frm->Floor:     %d\n",     (Frm->Flags1 >> 5)  & FRM_FLAG_FLOOR    );
		printf("Frm->Cycle:     %d\n",     (Frm->Flags1 >> 6)  & FRM_FLAG_CYCLE    );
		printf("Frm->pad:       %d\n",     (Frm->Flags1 >> 7)  & FRM_FLAG_pad      );
		printf("Frm->W:         %d\n",     (Frm->Flags2 >> 0)  & FRM_FLAG_W        );
		printf("Frm->H:         %d\n",     (Frm->Flags2 >> 9)  & FRM_FLAG_H        );
		printf("Frm->PentaGram: %d\n",     (Frm->Flags2 >> 18) & FRM_FLAG_PENTAGRAM);
		printf("Frm->pad2:      %d\n",     (Frm->Flags2 >> 19) & FRM_FLAG_pad2     );
		Frm++;
	}
}

void DumpFrame(SPR_HDR *Hdr, unsigned char *p2)
{
	int w, h, i, type, vram, size;
	FILE *fp;
	unsigned char *p3;
	unsigned char *tmp;
	FRAME_FAKE *Frm;
	char Name[260];

	tmp = (unsigned char *)Hdr;
	tmp += Hdr->FrameOffset;
	Frm = (FRAME_FAKE *)tmp;

	mkdir("dumped");

	p3 = (unsigned char *)malloc(1 * 1024 * 1024 + 1); // 1 megabyte work space
	for(i = 0; i < Hdr->NumOfFrames; i++) {
		type = (Frm[i].Flags1 >> 3) & FRM_FLAG_COMPTYPE;
		vram = (Frm[i].Flags1 >> 2) & FRM_FLAG_INVRAM; // not located in file, in memory
		if(vram) {
			continue;
		}
		sprintf(Name, "dumped\\frame%d.pcx", i);
		fp = fopen(Name, "wb");
		if(fp == NULL) {
			fprintf(stderr, "Can't create file %s\n", Name);
			exit(-1);
		}

		w = (Frm[i].Flags2 >> 0) & FRM_FLAG_W;
		w = GU_AlignVal(w, 2); // odd numbers get rounded up by 1
		h = (Frm[i].Flags2 >> 9) & FRM_FLAG_H;
		printf("Dumping %d: Frame W %d H %d\n", i, w, h);

		WritePcxHeader(fp, w, h);

		if(type == 1) {
			Un64(&p2[Frm[i].FrOffset], p3, w*h);
			fwrite(p3, sizeof(char), w*h, fp);
		} else if(type == 2) {
			// for monsters, first two bytes of source is decompressed size, WORD
			size = LZNP_Decode(&p2[Frm[i].FrOffset], p3); // add 2 to source for monsters
			fwrite(p3, sizeof(char), size, fp);
		} else {
			fprintf(stderr, "Unknown compression type %d, frame %d", type, i);
			exit(-1);
		}

		fclose(fp);
	}
	free(p3);
}

void Test4BIT(unsigned char *p, int size, char *file, int toffset, int w, int h, int page)
{
	int i, j, adder;
	unsigned char c;
	unsigned char *tmp;
	unsigned short *pal;
	FILE *fp;

	toffset -= 0x10000 * page;
	adder = 0;
	if(toffset & 1) {
		adder = 1;
		toffset--;
	}
	toffset >>= 1;
	if(adder) {
		toffset++;
	}

	if(toffset > size) {
		printf("Warning: frame %s does not exist in the tilepage (offset 0x%08x\n", file, toffset);
		return;
	}

	fp = fopen(file, "wb");

	if(fp == NULL) {
		fprintf(stderr, "Can't create file %s\n", file);
		exit(-1);
	}

	w = GU_AlignVal(w, 2);
	WritePcxHeader(fp, w, h);

	pal = (unsigned short *)p;
	p += toffset;
	for(i = 0; i < h; i++) {
		tmp = p;
		for(j = 0; j < w / 2; j++) {
			c = *p & 0xF;
			fwrite(&c, sizeof(char), 1, fp);
			c = *p & 0xF0;
			c >>= 4;
			fwrite(&c, sizeof(char), 1, fp);
			p++;
		}
		p = tmp;
		p += 256 / 2;
	}

	c = 0x0C;
	fwrite(&c, sizeof(char), 1, fp);
	pal += 32;
	for(i = 0; i < 256; i++) {
		Convert555toRGB(*pal, fp);
		pal++;
	}

	fclose(fp);

/*
	//p += 0x700;

	size /= 128;
	WritePcxHeader(fp, 256, size);

	for(i = 0; i < size; i++) {
		for(j = 0; j < 128; j++) {
			c = *p;
			c &= 0xF;
			c <<= 4;
			fwrite(&c, sizeof(char), 1, fp);
			c = *p;
			c &= 0xF0;
			fwrite(&c, sizeof(char), 1, fp);
			p++;
		}
	}

	fclose(fp);
*/
}

int main(int argc, char *argv[])
{
	unsigned char *p, *p2;
	SPR_HDR *Hdr;
	int size, i, w, h;
	unsigned char *tmp;
	FRAME_FAKE *Frm;
	char name[256];

	if(argc != 4) {
		fprintf(stderr, "Usage: psxgfx [hdr] [dat] [pal]\n");
		return 0;
	}

	p = LoadBinaryFile(argv[1], &size);
	p2 = LoadBinaryFile(argv[2], &size);

	Hdr = (SPR_HDR *)p;

	PrintHeaderInfo(Hdr);
	PrintFrameInfo(Hdr);

	//WritePalBuffer(Hdr, argv[3]);
	//DecodeData(p2, size, "test.out");
	//DumpFrame(Hdr, p2);

	tmp = (unsigned char *)Hdr;
	tmp += Hdr->FrameOffset;
	Frm = (FRAME_FAKE *)tmp;
	for(i = 0; i < Hdr->NumOfFrames; i++) {
		if((Frm->Flags1 >> 2) & FRM_FLAG_INVRAM) {
			sprintf(name, "dumped\\frame%d.pcx", i);
			w = (Frm->Flags2 >> 0) & FRM_FLAG_W;
			h = (Frm->Flags2 >> 9) & FRM_FLAG_H;
			if(!((Frm->Flags1 >> 1) & FRM_FLAG_ROTATED)) {
				Test4BIT(p2, size, name, Frm->FrOffset, w, h, Hdr->DestTPage);
			} else {
				Test4BIT(p2, size, name, Frm->FrOffset, h, w, Hdr->DestTPage);
			}
		}
		Frm++;
	}

	free(p);
	free(p2);

	return 0;
}
