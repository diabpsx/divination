typedef struct _PcxHeader
{
	unsigned char Manufacturer;
	unsigned char Version;
	unsigned char Encoding;
	unsigned char BitsPerPixel;
	unsigned short Xmin;
	unsigned short Ymin;
	unsigned short Xmax;
	unsigned short Ymax;
	unsigned short HDpi;
	unsigned short VDpi;
	unsigned char Colormap[48];
	unsigned char Reserved;
	unsigned char NPlanes;
	unsigned short BytesPerLine;
	unsigned short PaletteInfo;
	unsigned short HscreenSize;
	unsigned short VscreenSize;
	unsigned char Filler[54];
} PCXHEADER;

void WritePcxHeader(FILE *fp, unsigned short width, unsigned short height)
{
	PCXHEADER Buffer;
	memset(&Buffer, 0, sizeof(Buffer));

	Buffer.Manufacturer = 10;
	Buffer.Version = 5;
	Buffer.Encoding = 0;
	Buffer.BitsPerPixel = 8;
	Buffer.Xmax = width - 1;
	Buffer.Ymax = height - 1;
	Buffer.HDpi = 1;//width;
	Buffer.VDpi = 1;//height;
	Buffer.NPlanes = 1;
	Buffer.BytesPerLine = width;

	fwrite(&Buffer, sizeof(char), sizeof(Buffer), fp);
}
