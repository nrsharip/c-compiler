//-------------------------------------------------------------------\\
// TITLE:		BMP-file screen printing							 \\
// DESCRIPTION:	Program for parsing and printing of the BMP-file	 \\
// AUTHOR:		Sharipov R Nail										 \\
// DATE:		2008/05/31											 \\
// COMPILING:	cc.exe program.c 7E00								 \\
//-------------------------------------------------------------------\\


//	@brief: sets desired video mode
//
//	@in:	cVideoMode	= 0x03 - text mode (720x400, 16 colors)
//						= 0x0D - graphical mode (320x200,  16 colors)
//						= 0x0E - graphical mode (640x200,  16 colors)
//						= 0x10 - graphical mode (640x350,  16 colors)
//						= 0x11 - graphical mode (640x480,   2 colors)
//						= 0x12 - graphical mode (640x480,  16 colors)
//						= 0x13 - graphical mode (320x200, 256 colors)
short setVideoMode(char cVideoMode)
{
	__asm
	{
		MOV AH,0x00
		MOV AL,cVideoMode
		INT 0x10
	}
	return 0x1;
}

//	@brief: adjusts the palette to the BMP-file, which is started from 
//			sBMPaddr address value
//
//  @in:	sBMPaddr - absolute address value, from which the BMP-file 
//			location is started
short set256RGBPalette(short sBMPaddr)
{
	char	*cpBMPPalette, *cpRGBTable ;
	short	sOffset1, sOffset2, sTableAddr,sCounter;

	sTableAddr = 0x7000; // the beginning address of the new RGB palette
						 // table
	
	cpBMPPalette = sBMPaddr + 0x36; // the beginning address of BMP-file
									// palette
	cpRGBTable	 = sTableAddr;		// the beginning address of the RGB 
									// palette table

	sOffset1 = 0x0; // offset from the new RGB table beginning 
					// (for the INT 10/1012 interrupt)
	sOffset2 = 0x0; // offset from the palette table of BMP-file
	sCounter = 0x100; // repeat loop 256d times
	
	while(sCounter)
	{
		// each iteration sets one color register
		
		// BMP-file palette tint sequence : Blue Green Red + 1 clear byte (0x0)
		// RGB palette table tint sequence: Red Green Blue

		cpRGBTable[sOffset1] = (cpBMPPalette[sOffset2+0x2]*0x3F)/0xFF;
		cpRGBTable[sOffset1+0x1] = (cpBMPPalette[sOffset2+0x1]*0x3F)/0xFF;
		cpRGBTable[sOffset1+0x2] = (cpBMPPalette[sOffset2]*0x3F)/0xFF;
		
		sOffset1 = sOffset1 + 0x3; // 3 tints <-> 3 bytes
		sOffset2 = sOffset2 + 0x4; // 3 tints <-> 3 bytes + 1 byte
		sCounter = sCounter - 0x1; // decrement counter
	}

	__asm
	{
		MOV AX,0x0000
		MOV ES,AX

		MOV AH,0x10			// function number
		MOV AL,0x12			// sub-function number
		MOV BX,0x0000		// first color register to set
		MOV CX,0x0100		// color registers count (256d)
		MOV DX,sTableAddr	// ES:DX point to the beginning of RGB palette table now
		INT 0x10			// BIOS Service Interrupt
	}	
		
	return 0x1;
}

//	@brief: draws the BMP-file, which is started from sBMPaddr address value
//
//  @in:	sBMPaddr - absolute address value, from which the BMP-file 
//			location is started
short drawBMP (short sBMPaddr)
{
	char	*pcBitmap, cPixel; 
	short	sValueBX, sOffset, sReminder;
	short	sHorCounter,sVertCounter, *psHeight, *psWidth;
	
	// ES = video buffer beginning
	__asm
	{
		MOV AX,0xA000
		MOV ES,AX
	}	
	
	// BMP-file parsing
	psHeight = sBMPaddr + 0x16;		// pointer to the picture height byte
	psWidth  = sBMPaddr + 0x12;		// pointer to the picture width byte
	pcBitmap = sBMPaddr + 0x435;	// image data beginning

	sVertCounter = *psHeight;	// sVertCounter = image height
	sHorCounter = *psWidth;		// sHorCounter = image width

	sValueBX = 0xA0 - sHorCounter/0x2;	// BX - offset in video buffer, so
										// now sValueBX has the value for
										// screen centered image output

	// Specific calculation of useless bytes in image bitmap
	if(sHorCounter - (sHorCounter/0x4)*0x4)
	{
		sReminder = (sHorCounter/0x4 + 0x1)*0x4 - sHorCounter;
	}
	else
	{
		sReminder = 0x0;
	}	
	
	// As a result of reverse containing of image bitmap
	// we have to begin from the end of the bitmap to 
	// have the right picture orientation
	sOffset = sVertCounter*(sHorCounter + sReminder);
	
	while(sVertCounter)
	{
		sValueBX = sValueBX + sHorCounter;
		sOffset = sOffset - sReminder;
		while(sHorCounter)
		{
			// take one byte from the bitmap
			cPixel = pcBitmap[sOffset];
			__asm
			{
				MOV AH,cPixel			
				MOV BX,sValueBX
				MOV ES:[BX],AH	// color byte sending to the video buffer
			}
			sOffset = sOffset - 0x1;
			sValueBX = sValueBX - 0x1;
			sHorCounter = sHorCounter - 0x1;
		}
		sHorCounter = *psWidth;
		sValueBX = sValueBX + 0x140; // + 320d to begin the next pixel line
		sVertCounter = sVertCounter - 0x1;
	}
	return 0x1;
}

void main ()
{
	setVideoMode(0x13); // setting 320x200 256 color mode
	set256RGBPalette( 0x8200 ); // BMP-file address
	drawBMP( 0x8200 ); // BMP-file address

	// perpetual loop
	while(0x1)
	{}
}