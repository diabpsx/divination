// size: 0xC [old version]
/*
struct FRAME_HDR {
	unsigned int FrOffset : 32;    // offset: 0000 (32 bytes)
	int X : 8;                     // offset: 0020 (8 bytes)
	int Y : 8;                     // offset: 0028 (8 bytes)
	unsigned int W : 9;            // offset: 0030 (9 bytes)
	unsigned int PalNum : 7;       // offset: 0039 (7 bytes)
	unsigned int H : 9;            // offset: 0040 (9 bytes)
	unsigned int Rotated : 1;      // offset: 0049 (1 bytes)
	unsigned int BitsPerPixel : 2; // offset: 004A (2 bytes)
	unsigned int InVRAM : 1;       // offset: 004C (1 bytes)
	unsigned int CompType : 2;     // offset: 004D (2 bytes)
	unsigned int NotTrans : 1;     // offset: 004F (1 bytes)
};
*/

/* palette stuff
.DAT FILES
007ada: $00000000 94 Def class STRTAG type STRUCT size 8 name PAL
007aeb: $00000000 94 Def class FIELD type UINT size 1 name InVram
007aff: $00000001 94 Def class FIELD type UINT size 31 name NumOfCols
007b16: $00000004 96 Def2 clas MOS type ARY USHORT size 2 dims 1 1 tag  name Cols
007b2f: $00000008 96 Def2 clas EOS type NULL size 8 dims 0 tag PAL name .eos

.TP FILES
007b5e: $00000000 94 Def class STRTAG type STRUCT size 4 name PAL_INVRAM
007b76: $00000000 94 Def class FIELD type UINT size 1 name InVram
007b8a: $00000001 94 Def class FIELD type UINT size 15 name Pad
007b9b: $00000010 94 Def class FIELD type UINT size 16 name clut
007bad: $00000004 96 Def2 clas EOS type NULL size 4 dims 0 tag PAL_INVRAM name .eos
*/

// size: 0xE
struct CCreatureAction {
	// offset: 0000
	unsigned short BaseFrame;
	// offset: 0002
	unsigned char NumOfFrames;
	// offset: 0003
	unsigned char NumOfPhysFrames;
	// offset: 0004 (8 bytes)
	unsigned char DirRemap[8];
	// offset: 000C (1 bytes)
	unsigned char AnimRemap[1]; // AnimRemap[NumOfFrames]
};

// size: 0x14
struct CCreatureHdr {
	// offset: 0000
	long NumOfActions;
	// offset: 0004 (14 bytes)
	struct CCreatureAction Cr; // Cr[NumOfActions]
};

// size: 0xC
struct FRAME_HDR {
	unsigned int FrOffset;        // offset: 0000 (32 bytes)
	int X;                        // offset: 0020 (8 bytes)
	int Y;                        // offset: 0028 (8 bytes)
	unsigned int PalNum;          // offset: 0030 (8 bytes)
	unsigned int NotTrans;        // offset: 0038 (1 bytes)
	unsigned int Rotated;         // offset: 0039 (1 bytes)
	unsigned int InVRAM;          // offset: 003A (1 bytes)
	unsigned int CompType;        // offset: 003B (2 bytes)
	unsigned int Floor;           // offset: 003D (1 bytes)
	unsigned int Cycle;           // offset: 003E (1 bytes)
	unsigned int pad;             // offset: 003F (1 bytes)
	unsigned int W;               // offset: 0040 (9 bytes)
	unsigned int H;               // offset: 0049 (9 bytes)
	unsigned int PentaGram;       // offset: 0052 (1 bytes)
	unsigned int pad2;            // offset: 0053 (13 bytes)
};

#define FRM_FLAG_NOT_TRANS       1
#define FRM_FLAG_ROTATED         1
#define FRM_FLAG_INVRAM          1
#define FRM_FLAG_COMPTYPE        3
#define FRM_FLAG_FLOOR           1
#define FRM_FLAG_CYCLE           1
#define FRM_FLAG_pad             1

#define FRM_FLAG_W               0x1FF
#define FRM_FLAG_H               0x1FF
#define FRM_FLAG_PENTAGRAM       1
#define FRM_FLAG_pad2            0x1FFF

typedef struct FRAME_FAKE {
	unsigned int FrOffset;        // offset: 0000 (32 bytes) // 0x00-0x03
	char X;                        // offset: 0020 (8 bytes) // 0x04
	char Y;                        // offset: 0028 (8 bytes) // 0x05
	unsigned char PalNum;          // offset: 0030 (8 bytes) // 0x06
	unsigned char Flags1;          // offset: 0038 (8 bytes) // 0x07
	unsigned int Flags2;           // offset: 0040 (32 bytes)// 0x08-0x0B
} FRAME_FAKE;

typedef struct SPR_HDR {
	// offset: 0000 (32 bytes)
	unsigned int DecompOffset; // 00
	// offset: 0020 (32 bytes)
	unsigned int CreatureOffset; // 04     // points to CCreatureHdr struct
	// offset: 0040 (32 bytes)
	unsigned int PalOffset; // 08          //starts with uint to determine size, read for NumOfPals
	// offset: 0060 (32 bytes)
	unsigned int FrameOffset; // 0c        //0xc bytes * NumOfFrames (FRAME_HDR struct)
	// offset: 0080 (32 bytes)
	unsigned int BaseFrame; // 10
	// offset: 00A0 (32 bytes)
	unsigned int DestTPage; // 14
	// offset: 00C0 (32 bytes)
	unsigned int ComponentOffset; // 18
	// offset: 00E0 (32 bytes)
	unsigned int NumOfCreatures; // 1C
	// offset: 0100 (16 bytes)
	unsigned short NumOfFrames; // 20
	// offset: 0110 (16 bytes)
	unsigned short NumOfPals; // 22
	// offset: 0120 (8 bytes)
	unsigned char TWidth; // 24
	// offset: 0128 (8 bytes)
	unsigned char THeight; // 25
	// offset: 0130 (8 bytes)
	unsigned char IsTiles; // 26
	// offset: 0138 (8 bytes)
	unsigned char Spare; // 27
} SPR_HDR;
/*
struct TextDat {
	// offset: 0000
	int TexNum;
	// offset: 0004
	bool DatLoaded;
	// offset: 0008
	long hndDat;
	// offset: 000C
	long hndHdr;
	// offset: 0010
	long hndPalOffset;
	// offset: 0014
	long hndCreatureOffset;
	// offset: 0018
	long hndBlockOffsets;
	// offset: 001C (12 bytes)
	FRAME_HDR *Frames;
	// offset: 0020 (40 bytes)
	SPR_HDR *Hdr;
	// offset: 0024
	void *Pals;
	// offset: 0028
	int *PalOffset;
	// offset: 002C
	int *BlockOffsets;
	// offset: 0030
	int *CreatureOffset;
	// offset: 0034
	unsigned char *CreatureAnims;
	// offset: 0038
	unsigned char *Blocks;
	// offset: 003C
	bool Loaded;
	// offset: 0040
	int LoadCount;
	// offset: 0044 (4 bytes)
	char *FileInfo;
	// offset: 0048
	long hndDecompBuffer;
	// offset: 004C
	int DecX;
	// offset: 0050
	int DecY;
	// offset: 0054
	int PalX;
	// offset: 0058
	int PalY;
	// offset: 005C
	int Scr;
	// offset: 0060 (8 bytes)
	int NumOfBuffers[2];
	// offset: 0068
	long hndDecompArrays;
};
*/
