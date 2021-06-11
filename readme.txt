Descripton:

1. /source code/ - Source code of the compiler

2. /compiler/ - Trial C compiler + testing source code
	
	/compiler/cc.exe - Trial C compiler. Author: Sharipov R Nail. Conditional free distribution.
	/compiler/loader.c - 
			TITLE:		First loader					
			DESCRIPTION:	Initial boot program						
			AUTHOR:		Sharipov R Nail
			DATE:		2008/05/31
			COMPILING:	cc.exe loader.c 7C00

	/compiler/program.c - 
			TITLE:		BMP-file screen printing					 
			DESCRIPTION:	Program for parsing and printing of the BMP-file
			AUTHOR:		Sharipov R Nail								
			DATE:		2008/05/31									
			COMPILING:	cc.exe program.c 7E00							

3. /example/ - contains the result of C compiler work.
	
	/example/cc.exe    - see above
	/example/loader.c  - see above
	/example/program.c - see above

	/example/CompileInfo.txt - compilation info, generated during the process
	/example/loader.bin  - binary file - the result of C compiler work
	/example/program.bin - binary file - the result of C compiler work
	/example/pic_cc.bmp  - BMP-file for testing
	/example/image.bin   - image for boot, which consists of loader.bin, program.bin binary files 
			       + BMP file binary

4. /images/ - image for loading examples
	
	/images/image1.bin
	/images/image2.bin
	/images/image3.bin

5. /Nail_Sharipov_article.pdf - article "Hybrid Language Compiler Development: Inline IA-32 Assembler Language Structures As a Part of C language"

6. /Nail_Sharipov_CV200909011.pdf - CV Sharipov R Nail

----------
2009/09/13 Moscow State Technical University named after N.E.Bauman