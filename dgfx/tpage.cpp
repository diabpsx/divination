int GetTpX(unsigned short tpage)
{
	return (tpage & 0xf) << 6;
}

int GetTpY(unsigned short tpage)
{
	return (tpage & 0x10) << 4 | (tpage >> 2) & 0x200;
}
