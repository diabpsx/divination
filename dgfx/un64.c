void Un64(unsigned char *Src,unsigned char *Dest,long SizeBytes)
{
	unsigned long Code;
	unsigned char *tmp_ptr;
	unsigned long Run;
	unsigned int *BigDest;
	unsigned int src_byte;
	unsigned char *EndDest;
	
	EndDest = Dest + SizeBytes;

	while (Dest < EndDest) {
		src_byte = (unsigned int)*Src;
		Code = *Src >> 6;
		tmp_ptr = Src + 1;
		if (Code == 0) {
			Code = *tmp_ptr;
			tmp_ptr = Src + 2;
		}
		else {
			src_byte = src_byte & 0x3f;
		}
		Run = (unsigned int)Code;
		if (7 < Code) {
			BigDest = (unsigned int *)((unsigned int)((int)Dest + 3U) & 0xfffffffc);
			while (Dest < BigDest) {
				*Dest = (char)src_byte;
				Dest = (unsigned char *)((int)Dest + 1);
				Run = Run - 1;
			}
			while (Dest = (unsigned char *)BigDest, 3 < Run) {
				*BigDest = src_byte | src_byte << 8 | src_byte << 0x10 | src_byte << 0x18;
				Run = Run - 4;
				BigDest = BigDest + 1;
			}
		}
		while (Run != 0) {
			*Dest = (char)src_byte;
			Run = Run - 1;
			Dest = (unsigned char *)((int)Dest + 1);
		}
		Src = tmp_ptr;
	}
}
