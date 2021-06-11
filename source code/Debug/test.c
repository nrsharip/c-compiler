void main (  )
{
	char a;
	short b;

	a = b+0x0001;
	__asm
	{
		MOV DS:BX, 0x0011
	}
}
