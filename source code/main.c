#include "analysis_lex.h"
#include "analysis_syn.h"
#include "analysis_sem.h"
#include "analysis_cg.h"

/// Global variables

char bIsLValue  = 0;
char bIsPointer = 0;
UINT uProgrammOffset = 0x7C00;
UINT uCurLineNum	= 1;
UINT uCurType		= LEXCLASS_TYPE_VOID;
UINT uCurSemClass	= SEMCLASS_LVALUE;
UINT uCurTotalIndLevelCnt = 0;

TIdTable    firstIdTableElem = {SYNCLASS_ACCESSORIES_BEGINBLOCK, 0,0,0, 0, NULL, NULL, NULL};
TIdTablePtr pCurIdTableElem =		 &firstIdTableElem;
TIdTablePtr pCurIdTableBegBlock =	 &firstIdTableElem;
TIdTablePtr pCurIdTableFillingElem = &firstIdTableElem;

TLexemeListItemPtr	pCurLexListItem		= NULL;
TLexemeListItemPtr	pBegLexList			= NULL;

TRTItemPtr			pCurRTItem			= NULL;
TRTItemPtr			pBegRT				= NULL;
TRTItemPtr			pEndRT				= NULL;

TIdErrorPtr			pIdError			= NULL;
TIdErrorPtr			pIdErrorBegList		= NULL;

TParseListNodePtr	pBegParseListNode	= NULL; // Pointer to the first parse tree node (function tree)
TParseListNodePtr	pCurParseListNode	= NULL; // Pointer to a current parse tree node

TOperationElemPtr	pCurFuncArg			= NULL;
TBlockPtr			pCurBlock			= NULL;
TOperationElemPtr	pCurLocVar			= NULL;

TContrElemPtr		pCurContrElem		= NULL;
TOperationElemPtr	pCurContrElemArg	= NULL;

TFuncListPtr		pBegFuncList		= NULL;
TFuncListPtr		pCurFunction		= NULL;
TInstructionListPtr	pCurInstruction		= NULL;
TByteListPtr		pCurByte			= NULL;

TCallLinkPtr		pBegCallLinkList	= NULL;
TCallLinkPtr		pCurCallLinkList	= NULL;

TCallLink			mainCallLink = {NULL, NULL, NULL, NULL};
TOperationElemPtr	pCurCGOperElem;

UINT				uTopLvlExprAsteriskCount = 0;
UINT				uDeclAsteriskCount = 0;
TAAOperElemListPtr		pCurIdAAE = NULL;
TAAOperElemListPtr		pBegAAElement = NULL;
TLexemeListItemPtr  pIdNameLexListItem = NULL;

TIDCurStateListPtr		pCurIdStateListElem = NULL;
TIDCurStateListPtr		pBegIdStateListElem = NULL;

void main(int argc, char * argv[])
{
    FILE * pInFile;
    char * cpInFileName, * cpOutFileName;
    extern TLexemeListItemPtr pCurLexListItem;
	extern UINT uProgrammOffset;

	// if there is only one argument provided then:
    // input file name is considered as "test.c"
	// program offset equals zero
	if (argc == 1)
	{
		cpInFileName = "test.c";
		uProgrammOffset = 0;
	}

	// if there are only two arguments provided then:
    // input file name equals the second argument argv[1]
	// program offset equals zero
	if (argc == 2)
	{
		cpInFileName = argv[1];
		uProgrammOffset = 0;
	}

	// if there are only three arguments provided then:
    // input file name equals the second argument argv[1]
	// program offset equals string-to-long value of the third argument argv[2]
	if (argc == 3)
	{
		cpInFileName = argv[1];
		uProgrammOffset = strtol(argv[2], NULL, 16);
	}

	// Allocating cpInFileName string length +3 bytes of memory 
	// for an output name and associating it with cpOutFileName
	cpOutFileName = (char *)malloc(strlen(cpInFileName)+1);
	// Copying the input file name
	strcpy(cpOutFileName, cpInFileName);
	
	cpOutFileName[strcspn(cpInFileName,".")] = 0;
	strcat(cpOutFileName,".bin");
	// name.c has become now name.bin
	
	printf("%s \n", cpOutFileName);

	if ((pInFile = fopen(cpInFileName,"rb")) == NULL)    
    {
        printf("%s \n", cpOutFileName);
		printf ("\n error occured(%d): ", errno);
    }
    else
    {
		// running the Lexical Analysis		
		lexAnalysis( pInFile );

        lexPrintLexemeList( pCurLexListItem, FALSE );
        
		synAnalysis();
		
		if (!pIdErrorBegList)
			semAnalisys();
		
		if (!pIdErrorBegList)		
			cgCodeCreating();
		
		if (!pIdErrorBegList)
			cgFuncSizeDetection();
		
		if (!pIdErrorBegList)		
			cgFuncOffsetDetection();
		
		if (!pIdErrorBegList)			
			cgLinking();

// 		if (!pIdErrorBegList)			
// 			cgPrintFuncCode(FALSE);
//		Escaping printing out the code yet

		if (!pIdErrorBegList)			
			cgBinFileGeneration(cpOutFileName);
 
		synPrintIdErrors();
		if (argc == 2)
		{
			printf(" warning: you didn't specify initial entry point for program linking \n");
			printf("          (default = 0 was accepted)\n");
		}
		fclose(pInFile);
	}
}