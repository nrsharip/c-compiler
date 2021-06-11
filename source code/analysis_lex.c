#include "analysis_lex.h"
#include "analysis_syn.h"
#include "analysis_sem.h"
#include "analysis_cg.h"

/*	
*	Returns TRUE if the identifier is already in the RT, otherwise returns FALSE
*
*	@param char *cpItem - String capture of a identifier lexeme
*	@param UINT uPosBeg - Beginning position in the file
*	@param UINT uPosEnd - Ending position in the file
*
*   @author Nail Sharipov
*/
int lexIsItemInRT(char *cpItem, UINT uPosBeg, UINT uPosEnd)
{
    UINT uRes;

    pCurRTItem = pBegRT;
    
	//Searching the current Representation Tree
	while (pCurRTItem)
    {    
        // Checking whether the Identifier Name in RT node is equal to which
		// was provided
		
		// Checking whether the lengths are equal
		if ( (pCurRTItem->RTItemContent.uNameLength) == (uPosEnd - uPosBeg) )
        {
            // Checking whether the strings are equal
			if(!(uRes = strncmp(pCurRTItem->RTItemContent.cpIdName, cpItem, uPosEnd - uPosBeg + 1)))
            {
                // if the lengths and strings pairs are equal
				return TRUE;
            }
        }
        pCurRTItem = pCurRTItem->pNextRTItem;
    }

    return FALSE;
}

/*	
*	Adds new item to the Representation Tree
*
*	@param char *cpItem - String capture of a identifier lexeme
*	@param UINT uPosBeg - Beginning position in the file
*	@param UINT uPosEnd - Ending position in the file
*
*   @author Nail Sharipov
*/
int lexAddRTItem (char *cpItem, UINT uPosBeg, UINT uPosEnd)
{
    extern TRTItemPtr pBegRT, pEndRT;
    TRTItemPtr pNewRTItem;
    
    pNewRTItem = (TRTItemPtr)malloc(sizeof(TRTItem));
    
    pNewRTItem->RTItemContent.uNameLength = uPosEnd - uPosBeg;
    pNewRTItem->RTItemContent.cpIdName = (char *)malloc( (uPosEnd - uPosBeg) + 2 );
    
	// Copying and zero-ending a string capture
    strncpy(pNewRTItem->RTItemContent.cpIdName, cpItem, (uPosEnd - uPosBeg) + 1);
    pNewRTItem->RTItemContent.cpIdName[(uPosEnd - uPosBeg) + 1] = 0;

    pNewRTItem->pPrevRTItem = pEndRT;
    
	pNewRTItem->pNextRTItem = NULL;
    pNewRTItem->RTItemContent.pIdTableElem = NULL;        

    if (pEndRT)
        pEndRT->pNextRTItem = pNewRTItem;

    pEndRT = pNewRTItem;

    if (!pBegRT)
        pBegRT = pEndRT;

    return TRUE;
}

/*
*	Prints the Representation Table in stdout and CompileInfo.txt
*
*   @author Nail Sharipov
*/
void lexPrintRT()
{
    extern TRTItemPtr pCurRTItem,pBegRT;
    FILE * pOutInfoFile;
        
    pOutInfoFile = fopen("CompileInfo.txt","a");
    
    printf("\n");
    fprintf(pOutInfoFile, "\n");

    fprintf(pOutInfoFile, " ***REPRESENTATION TABLE*** \n");
    printf(" ***REPRESENTATION TABLE*** \n");

    printf("\n");
    fprintf(pOutInfoFile, "\n");

    fprintf(pOutInfoFile, " Name length | IdName \n");
    printf(" Name length | IdName \n" );
    
    pCurRTItem = pBegRT;

    while (pCurRTItem)
    {    
        printf ("%12X ", pCurRTItem->RTItemContent.uNameLength);
        fprintf(pOutInfoFile, "%12X ", pCurRTItem->RTItemContent.uNameLength);
        
        printf ("  ");
        fprintf(pOutInfoFile, "  " );

        fprintf(pOutInfoFile, "%s ", pCurRTItem->RTItemContent.cpIdName);
        
		if (pCurRTItem->RTItemContent.pIdTableElem)
			printf ("%s   --->   %d", pCurRTItem->RTItemContent.cpIdName, pCurRTItem->RTItemContent.pIdTableElem->uType);
		else	
			printf ("%s ", pCurRTItem->RTItemContent.cpIdName);

        printf("\n");
        fprintf(pOutInfoFile, "\n");
        pCurRTItem = pCurRTItem->pNextRTItem;
    }
    fclose(pOutInfoFile);
}

/*
*	Returns TRUE if the symbol provided is hex-digit
*
*	@param char cSymbol - Symbol to check
*   @author Nail Sharipov
*/
int lexIsHex(char cSymbol)
{
    switch (cSymbol)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return TRUE;
        break;
    }
    return FALSE;
}

/*
*	Returns TRUE if the lexeme instance represents the hex value
*	with "0x"-prefix notation. Otherwise returns FALSE
*
*	@param char *cpOccurrence - String capture of a lexeme
*   @author Nail Sharipov
*/
int lexIsOccurrenceHex(char * cOccurrence)
{
    UINT i;
	UINT size_t;

	size_t = strlen(cOccurrence);
    if (strncmp( cOccurrence, "0x", 2))
        return FALSE;
    
    if (size_t - 2 == 0)
		return FALSE;

	for ( i = 2; i < size_t; i++)
        if(!lexIsHex(cOccurrence[i]))
        {
            return FALSE;
        }
    
    return TRUE;
}

/*
*	Returns the lexeme class by the string appearance (capture)
*	of a lexeme. By default returns LEXCLASS_RT_IDENTIFIER, so
*	all the instances of non-recognized lexeme are considered as 
*	identifier case
*
*	@param char *cpOccurrence - String capture of a lexeme
*   @author Nail Sharipov
*/
int lexRetLexClassByOccurence(char * cOccurrence)
{
    if ( !strcmp(cOccurrence, "JMP") )
        return LEXCLASS_IA32INSTRUCTIONS_JMP;
	if ( !strcmp(cOccurrence, "INT") )
        return LEXCLASS_IA32INSTRUCTIONS_INT;
	if ( !strcmp(cOccurrence, ">") )
        return LEXCLASS_COMPARISON_GREATER;
	if ( !strcmp(cOccurrence, "<") )
        return LEXCLASS_COMPARISON_LESS;
	if ( !strcmp(cOccurrence, "[") )
        return LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET;
	if ( !strcmp(cOccurrence, "]") )
        return LEXCLASS_BLOCK_RIGHT_SQAURE_BRACKET;
	if ( !strcmp(cOccurrence, ":") )
        return LEXCLASS_PUNCTUATION_COLON;
	if ( !strcmp(cOccurrence, "void") )
        return LEXCLASS_TYPE_VOID;
    if ( !strcmp(cOccurrence, "int"))
        return LEXCLASS_TYPE_INT;
    if ( !strcmp(cOccurrence, "short"))
        return LEXCLASS_TYPE_SHORT;
    if ( !strcmp(cOccurrence, "struct"))
        return LEXCLASS_TYPE_STRUCT;
    if ( !strcmp(cOccurrence, "char"))
        return LEXCLASS_TYPE_CHAR;
    if ( !strcmp(cOccurrence, "__asm") )
        return LEXCLASS_PROGRAMM_ASM_ENTRY;
	if ( !strcmp(cOccurrence, "if") )
        return LEXCLASS_CONSTRUCTIONS_IF;
	if ( !strcmp(cOccurrence, "else") )
        return LEXCLASS_CONSTRUCTIONS_ELSE;
	if ( !strcmp(cOccurrence, "while") )
        return LEXCLASS_CONSTRUCTIONS_WHILE;
	if ( !strcmp(cOccurrence, "return") )
        return LEXCLASS_PROGRAMM_RETURN_POINT;
    if ( !strcmp(cOccurrence, "{") )
        return LEXCLASS_BLOCK_LEFT_CURLY_BRACKET;
    if ( !strcmp(cOccurrence, "MOV"))
        return LEXCLASS_IA32INSTRUCTIONS_MOV;
	if ( !strcmp(cOccurrence, "PUSH"))
        return LEXCLASS_IA32INSTRUCTIONS_MOV;
	if ( !strcmp(cOccurrence, "EAX")) 
		return LEXCLASS_IA32REGISTERS_EAX;
	if ( !strcmp(cOccurrence, "AX")) 
		return LEXCLASS_IA32REGISTERS_AX;
	if ( !strcmp(cOccurrence, "AH")) 
		return LEXCLASS_IA32REGISTERS_AH;
	if ( !strcmp(cOccurrence, "AL")) 
		return LEXCLASS_IA32REGISTERS_AL;
	if ( !strcmp(cOccurrence, "EBX")) 
		return LEXCLASS_IA32REGISTERS_EBX;
	if ( !strcmp(cOccurrence, "BX")) 
		return LEXCLASS_IA32REGISTERS_BX;
	if ( !strcmp(cOccurrence, "BH")) 
		return LEXCLASS_IA32REGISTERS_BH;
	if ( !strcmp(cOccurrence, "BL")) 
		return LEXCLASS_IA32REGISTERS_BL;
	if ( !strcmp(cOccurrence, "ECX")) 
		return LEXCLASS_IA32REGISTERS_ECX;
	if ( !strcmp(cOccurrence, "CX")) 
		return LEXCLASS_IA32REGISTERS_CX;
	if ( !strcmp(cOccurrence, "CH")) 
		return LEXCLASS_IA32REGISTERS_CH;
	if ( !strcmp(cOccurrence, "CL")) 
		return LEXCLASS_IA32REGISTERS_CL;
	if ( !strcmp(cOccurrence, "EDX")) 
		return LEXCLASS_IA32REGISTERS_EDX;
	if ( !strcmp(cOccurrence, "DX")) 
		return LEXCLASS_IA32REGISTERS_DX;
	if ( !strcmp(cOccurrence, "DH")) 
		return LEXCLASS_IA32REGISTERS_DH;
	if ( !strcmp(cOccurrence, "DL")) 
		return LEXCLASS_IA32REGISTERS_DL;
	if ( !strcmp(cOccurrence, "CS")) 
		return LEXCLASS_IA32REGISTERS_CS;
	if ( !strcmp(cOccurrence, "DS")) 
		return LEXCLASS_IA32REGISTERS_DS;
	if ( !strcmp(cOccurrence, "ES")) 
		return LEXCLASS_IA32REGISTERS_ES;
	if ( !strcmp(cOccurrence, "SS")) 
		return LEXCLASS_IA32REGISTERS_SS;
	if ( !strcmp(cOccurrence, "SI")) 
		return LEXCLASS_IA32REGISTERS_SI;
	if ( !strcmp(cOccurrence, "DI")) 
		return LEXCLASS_IA32REGISTERS_DI;
	if ( !strcmp(cOccurrence, "BP")) 
		return LEXCLASS_IA32REGISTERS_BP;
	if ( !strcmp(cOccurrence, "SP")) 
		return LEXCLASS_IA32REGISTERS_SP;
    if ( !strcmp(cOccurrence, "}") )
        return LEXCLASS_BLOCK_RIGHT_CURLY_BRACKET;
    if ( !strcmp(cOccurrence, "(") )
        return LEXCLASS_BLOCK_LEFT_ROUND_BRACKET;
    if ( !strcmp(cOccurrence, ")") )
        return LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET;
    if ( !strcmp(cOccurrence, ";") )
        return LEXCLASS_PUNCTUATION_SEMICOLUMN;
    if ( !strcmp(cOccurrence, "=") )
        return LEXCLASS_COMPARISON_EQUAL;
    if ( !strcmp(cOccurrence, "/") )
        return LEXCLASS_OPERATION_SLASH;
	if ( !strcmp(cOccurrence, "*") )
        return LEXCLASS_OPERATION_ASTERISK;
    if ( !strcmp(cOccurrence, "+") )
        return LEXCLASS_OPERATION_PLUS;
    if ( !strcmp(cOccurrence, "-") )
        return LEXCLASS_OPERATION_MINUS;
	if ( !strcmp(cOccurrence, ",") )
        return LEXCLASS_PUNCTUATION_COMMA;
    if ( lexIsOccurrenceHex(cOccurrence) )
        return LEXCLASS_RT_NUMERIC_CONSTANT;
    if ( cOccurrence[0] == '\"' )
        return LEXCLASS_RT_STRING_CONSTANT;
    return LEXCLASS_RT_IDENTIFIER;
}

/*
*	Adds a new node to the lexeme list
*
*	@param char *cpOccurrence - String capture of a lexeme
*	@param UINT uPosBeg - Beginning position in the file
*	@param UINT uPosEnd - Ending position in the file
*   @author Nail Sharipov
*/
int lexAddLexemeListItem ( char *cpOccurrence, UINT uPosBeg, UINT uPosEnd)
{
    extern TLexemeListItemPtr   pCurLexListItem;
    extern UINT uCurLineNum;
	TLexemeListItemPtr          pNewLexListItem;
	
	// Allocating memory for TLexemeListItem list node element
    pNewLexListItem = (TLexemeListItemPtr)malloc(sizeof(TLexemeListItem));

	// Allocating memory for string capture of a lexeme
	pNewLexListItem->LexemeInstance.cpTextFrag = (char *)malloc( (uPosEnd - uPosBeg) + 2 );
    
	// Copying string capture
    strncpy(pNewLexListItem->LexemeInstance.cpTextFrag, cpOccurrence, (uPosEnd - uPosBeg) + 1);
    
	pNewLexListItem->LexemeInstance.cpTextFrag[(uPosEnd - uPosBeg) + 1] = 0;
	
	// Determining the lexeme class by string capture
	pNewLexListItem->LexemeInstance.uLexClass = lexRetLexClassByOccurence( pNewLexListItem->LexemeInstance.cpTextFrag );
    
	pNewLexListItem->LexemeInstance.uPosBeg = uPosBeg;
    pNewLexListItem->LexemeInstance.uPosEnd = uPosEnd;
	pNewLexListItem->LexemeInstance.uLineNum = uCurLineNum;
    
    if (pNewLexListItem->LexemeInstance.uLexClass == LEXCLASS_RT_STRING_CONSTANT )
    {
        // Making cpOccurrence to point to the first symbol of a string constant
		// avoiding the quote symbol
		// E.g. For "string" it makes cpOccurrence to point to (s)
		cpOccurrence += 1;
        uPosEnd -= 2;
    }


    if (pNewLexListItem->LexemeInstance.uLexClass == LEXCLASS_RT_NUMERIC_CONSTANT )
    {
        // Making cpOccurrence to point to the first symbol of a numeric constant
		// avoiding the "0x"
		// E.g. For "string" it makes cpOccurrence to point to (s)
		cpOccurrence += 2;
        uPosEnd -= 2;
    }

	if (pNewLexListItem->LexemeInstance.uLexClass == LEXCLASS_RT_IDENTIFIER )
    {
		// A case of LEXCLASS_RT_IDENTIFIER is special for handling. It concerns the filling of 
		// additional data structures, the Representation Table and the Id Table, since the way 
		// of using identifiers by users is one of the most complicated to analyze
		
		// Checking whether the identifier is already in the RT
		if (!lexIsItemInRT(cpOccurrence, uPosBeg, uPosEnd))
        {
            lexAddRTItem(cpOccurrence, uPosBeg, uPosEnd);
        }
    }

    if (pCurLexListItem)
        pCurLexListItem->pNextLexemeInstance = pNewLexListItem;
    else
        pCurLexListItem = pBegLexList = pNewLexListItem;
    
    pNewLexListItem->pPrevLexemeInstance = pCurLexListItem;
    pNewLexListItem->pNextLexemeInstance = NULL;
    pCurLexListItem = pNewLexListItem;   
    
    return TRUE;
}

/*
*	Returns FALSE if cSymbol is separator, otherwise returns cSymbol
*
*	@param char cSymbol - symbol to check
*   @author Nail Sharipov
*/
int lexIsNotSepSymb(char cSymbol)
{
    switch (cSymbol)
    {
    case 0x09:
	case 0x0D:
	case '/':
	case '[':
	case ']':
	case '+':
    case '-':
	case '*':
    case '{':
    case '}':
    case '(':
    case ')':
    case '=':
    case ':':
	case ',':
    case '.':
    case ';':
    case ' ':
    case '\"':
    case 0x0A:
        return FALSE;
        break;
    } 
        return cSymbol;
}

/*
*	Implements the lexical analysis process.
*
*	@param FILE *pInFile - source code input file descriptor
*   @author Nail Sharipov
*/
int lexAnalysis(FILE * pInFile)
{
    char    cTempCommemt = 0;
	char    cBuffChar = 0;
    UINT    uPosBeg = 0,uPosEnd=0;
    char    cOccurrence[100];
    
	// Excerpting a symbol from the source file
	// Checking the excerpted symbol whether it is space or not (whitespace, TAB etc.)
    while ( isspace(cBuffChar = getc (pInFile)) )
	{
		// Checking whether the current symbol is a newline
		if (cBuffChar == 0x0A)
			++uCurLineNum; //Incrementing the current line number variable

		++uPosBeg; //Incrementing the beginning position of a lexeme variable
    }
    
	uPosEnd = uPosBeg; // Equating the beginning and ending positions of the first lexeme
    
    while ( cBuffChar != EOF) // Checking if EOF is reached
    {
        int i = 0;
	
		// Checking whether the current symbol is space, separator (bracket, parenthesis etc.) or EOF
		while ( lexIsNotSepSymb(cBuffChar) && cBuffChar != EOF)
        {
			cOccurrence[i] = cBuffChar; // Collecting the symbol as a part of the lexeme
            ++i;
			cBuffChar = getc (pInFile); // Excerpting a symbol from the source file
		}        
		
		// Checking for comments presence
		// (C++ notation "//" for one comment line)
		if	((cBuffChar == '/') && ( (cTempCommemt = getc(pInFile)) == '/'))
		{
			// while it is not the end of a line or EOF, skipping all symbols,
			// incrementing uPosBeg
			while ((cBuffChar != 0x0A) && (cBuffChar != EOF))
			{
				cBuffChar = getc (pInFile);
				++uPosBeg;
			}
			uPosEnd = uPosBeg;
			// Since the end of a line will be approached anyway incrementing
			// the current line number variable
			++uCurLineNum;
			continue;
		}
		else
		{
			if	(cBuffChar == '/')
				ungetc(cTempCommemt,pInFile);			
		}		
		
		// Checking for string constant presence. 
		// Since all string constants begin with quote symbol the first appearance 
		// of a lexeme will be processed here, not with the code above.
        if ( cBuffChar == '\"' )
        {
            cOccurrence[i] = cBuffChar; // Collecting the quote symbol as a part of the lexeme
            do 
            {
                ++i;
                cBuffChar = getc (pInFile); // Excerpting a symbol from the source file
                cOccurrence[i] = cBuffChar; // Collecting symbols of a string constant as a part of the lexeme
            }    
            while ( cBuffChar != '\"' ); // Until the excerpted symbol is not the quote symbol
            i++;    
            cBuffChar = getc (pInFile); // Excerpting the next symbol from the source file
        }
        
        uPosEnd = uPosBeg + (i - 1); // Computing the ending position of the lexeme
		
		// Adding lexeme as a node to lexeme tree
		if (i != 0 )
            lexAddLexemeListItem (cOccurrence, uPosBeg, uPosEnd);
        
        // Add separator as a node to lexeme tree if it's not newline, whitespace or TAB
		if (!(cBuffChar == 0x0D) && !(cBuffChar == 0x0A) && !(cBuffChar == ' ') && !(cBuffChar == 0x09))
		{
			lexAddLexemeListItem (&cBuffChar, uPosBeg + i, uPosBeg + i);
		}
        uPosBeg = uPosEnd + 2; 
		
		// Excerpting a symbol from the source file
		// Checking the excerpted symbol whether it is space or not (whitespace, TAB etc.)
		while ( isspace(cBuffChar = getc (pInFile)) )
        {
            // Checking whether the current symbol is a newline
			if (cBuffChar == 0x0A)
				++uCurLineNum;
			
			++uPosBeg;
        }
		// Equating the beginning and ending positions of the first lexeme
        uPosEnd = uPosBeg;
    }
    
    return TRUE;
}

/*
*	Prints the Lexeme List in stdout and CompileInfo.txt
*
*   @author Nail Sharipov
*/
int lexPrintLexemeList (TLexemeListItemPtr pLexeme, char bIsScreenPrint)
{
    FILE * pOutInfoFile;

	pLexeme = pBegLexList;
    pOutInfoFile = fopen("CompileInfo.txt","w");
    fprintf(pOutInfoFile, "Lexeme class | Beginning pos | Ending pos | Text \n");
    if (bIsScreenPrint)
		printf("Lexeme class | Beginning pos | Ending pos | Text \n" );
    
    while (pLexeme)
    {
        if (bIsScreenPrint)
		{
			printf ("%12d ", pLexeme->LexemeInstance.uLexClass);
			printf ("%15X ", pLexeme->LexemeInstance.uPosBeg);
			printf ("%12X ", pLexeme->LexemeInstance.uPosEnd);
        }          
        fprintf(pOutInfoFile, "%12d ", pLexeme->LexemeInstance.uLexClass );
        fprintf(pOutInfoFile, "%15X ", pLexeme->LexemeInstance.uPosBeg );
        fprintf(pOutInfoFile, "%12X ", pLexeme->LexemeInstance.uPosEnd);

        if (bIsScreenPrint)
			printf ("  ");
        fprintf(pOutInfoFile, "  " );
		
		if (bIsScreenPrint)
			printf (" \"%s\" ", pLexeme->LexemeInstance.cpTextFrag);
        fprintf(pOutInfoFile, " \"%s\" ", pLexeme->LexemeInstance.cpTextFrag);

		if (bIsScreenPrint)
			printf("\n");
        fprintf(pOutInfoFile, "\n");
        pLexeme = pLexeme->pNextLexemeInstance;
    }
    fclose(pOutInfoFile);
    return TRUE;
}
