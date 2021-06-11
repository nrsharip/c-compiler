//-------------------------------------------------------------------\\
// TITLE:		First loader										 \\
// DESCRIPTION:	Initial boot program								 \\
// AUTHOR:		Sharipov R Nail										 \\
// DATE:		2008/05/31											 \\
// COMPILING:	cc.exe loader.c 7C00								 \\
//-------------------------------------------------------------------\\


//	@brief: reads disk sectors
//
//	@in:		cSectNum - number of sectors to read	(1-128 dec.)
//				sStartSect - first sector
//				cHeadNum - head number  (0-15 dec.)
//				cDriveNum - drive number (0=A:, 1=2nd floppy, 80h=drive 0,
//							81h=drive 1)
//				sESValue - ES value
//				sBXValue - BX value
char biosReadDiskSectors(char cSectNum, short sStartSect, char cHeadNum, 
							char cDriveNum, short sESValue, short sBXValue)
{
	__asm 
	{
		MOV AH,0x02     	// function number
		MOV AL,cSectNum 	// number of sectors to read	(1-64 dec.)
							// CH - track/cylinder number  (0-1023 dec., see below)
		MOV CX,sStartSect	// CL - start sector number  (1-17 dec.)
		MOV DH,cHeadNum		// head number  (0-15 dec.)
		MOV DL,cDriveNum	// drive number (0=A:, 1=2nd floppy, 80h=drive 0,
							// 81h=drive 1)
		MOV ES,sESValue		//
		MOV BX,sBXValue     // ES:BX = pointer to buffer  
		INT 0x13			// BIOS Service Interrupt
	}
	
//	  |F|E|D|C|B|A|9|8|7|6|5-0|  CX
//	   | | | | | | | | | |	`-----	sector number
//	   | | | | | | | | `---------  high order 2 bits of track/cylinder
//	   `------------------------  low order 8 bits of track/cyl number

	return 0x1;
}

//	@brief: reads disk sectors from diskette
//
//	@in:	sSectNum - number of sectors to read	(1-128 dec.)
//			sMemAddr - memory address where to read
char readSectorsFromDiskette(short sSectNum, short sMemAddr)
{
	short sIterationCount;
	char  cRemainder;
	short sESValue, sStartSect;
	
	sStartSect = 0x2; // the first is boot sector, so we start to read from the 2nd
	
	sESValue = sMemAddr/0x10; 
	
	sIterationCount = sSectNum/0x40; // we will read by 64 sectors
	cRemainder = sSectNum - sIterationCount*0x40;

	while (sIterationCount)
	{
		biosReadDiskSectors(0x40, sStartSect, 0x0, 0x0, sESValue, 0x0);
		sStartSect = sStartSect + 0x40;
		sESValue = sESValue + 0x800; // 0x8000 - size of 64d sectors, so we increase
									 // ES on 0x800
		sIterationCount = sIterationCount - 0x1;
	}
	
	return 0x1;	
}

void main (  )
{
	readSectorsFromDiskette(0x40,0x7E00); // read 64d sectors, starting from 7E00
	
	// Jump to the first byte of the loaded program
	__asm
	{
		JMP 0x0000:0x7E00
	}
}