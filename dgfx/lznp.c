int LZNP_Decode(unsigned char *in, unsigned char *out)
{
	int i, j;
	unsigned int flags;
	unsigned char *OriginalOut;

	flags = 0;
	OriginalOut = out;

	while(1) {
		flags >>= 1;
		if(!(flags & 0xff00)) {
			flags = *in | 0xff00;
			in++;
		}
		if(!(flags & 1)) {
			*out++ = *in++; // Regular pixel
		} else {
			if(*in >= 0x60) {
				j = 0x100 - *in;
				i = 2;
				in++;
			} else {
				j = *in & 0xf;
				i = *in >> 4;
				in++;
				j = *in | (j << 8);
				in++;
				if(j == 0) {
					break;
				}
				if(i != 5) {
					i += 3;
				} else {
					i = *in + 8;
					in++;
				}
			}
			j = -j;
			while(i--) { // RLE pixel
				*out = out[j];
				out++;
			}
		}
	}

	return out - OriginalOut;
}
