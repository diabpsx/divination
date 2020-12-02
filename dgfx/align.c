unsigned int GU_AlignVal(unsigned int w, unsigned int round)
{
	unsigned int uVar1;
	
	uVar1 = (w - 1) + round;
	if (round == 0) {
		//trap(0x1c00);
	}

	return uVar1 - uVar1 % round;
}
