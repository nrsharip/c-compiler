#include "analysis_lex.h"
#include "analysis_syn.h"
#include "analysis_sem.h"
#include "analysis_cg.h"

/*
*	
*
*	@param char * cpFuncName
*	@param UINT uType
*	@param TIdTablePtr pFuncId
*
*   @author Nail Sharipov
*/
int synAddNewFuncNode (char * cpFuncName, UINT uType, TIdTablePtr pFuncId)
{
	extern TParseListNodePtr	pCurParseListNode;
	extern TParseListNodePtr	pBegParseListNode;
	
	TParseListNodePtr			pNewFuncNode;
	
	pNewFuncNode = (TParseListNodePtr)malloc(sizeof(TParseListNode));

	pNewFuncNode->pNextFuncNode           = NULL;
	pNewFuncNode->FuncNode.pBegArgList	  = NULL;
	pNewFuncNode->FuncNode.pFuncBlock = NULL;
	pNewFuncNode->FuncNode.pFuncId = pFuncId;
	pNewFuncNode->pPrevFuncNode = pCurParseListNode;
	
	pNewFuncNode->FuncNode.cpFuncName = (char *)malloc(strlen(cpFuncName)+1);
	strcpy( pNewFuncNode->FuncNode.cpFuncName, cpFuncName );

	pNewFuncNode->FuncNode.uType = uType;

	if (pCurParseListNode)
	{
		pCurParseListNode->pNextFuncNode = pNewFuncNode;
		pCurParseListNode = pNewFuncNode;
	}
		
	if (!pBegParseListNode)
	{
		pBegParseListNode = pNewFuncNode;
		pCurParseListNode = pNewFuncNode;	
	}
		
	pCurFuncArg = NULL;
	pCurLocVar = NULL;
	pCurBlock = NULL;
	

	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddNewStructNode (char * cpStructName, UINT uType)
{
	extern TParseListNodePtr	pCurParseListNode;
	extern TParseListNodePtr	pBegParseListNode;
	
	TParseListNodePtr			pNewNode;
	
	pNewNode = (TParseListNodePtr)malloc(sizeof(TParseListNode));
	
	pNewNode->pNextFuncNode           = NULL;
	pNewNode->FuncNode.pBegArgList	  = NULL;
	pNewNode->FuncNode.pFuncBlock  = NULL;

	pNewNode->pPrevFuncNode = pCurParseListNode;
	
	pNewNode->FuncNode.cpFuncName = (char *)malloc(strlen(cpStructName)+1);
	strcpy( pNewNode->FuncNode.cpFuncName, cpStructName );
	
	pNewNode->FuncNode.uType = uType;
	
	if (pCurParseListNode)
	{
		pCurParseListNode->pNextFuncNode = pNewNode;
		pCurParseListNode = pNewNode;
	}
	
	if (!pBegParseListNode)
	{
		pBegParseListNode = pNewNode;
		pCurParseListNode = pNewNode;	
	}
	
	pCurFuncArg = NULL;
	pCurLocVar = NULL;
	pCurContrElem = NULL;
	
	
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
TBlockPtr synCreateNewBlock()
{
	TBlockPtr	pNewBlock;
	
	pNewBlock = (TBlockPtr)malloc(sizeof(TBlock));
	pNewBlock->pBegContrList = NULL;
	pNewBlock->pBegLocVarList = NULL;
	pNewBlock->uBlockOffset = 0;
	pNewBlock->uBlockSize = 0;
	pNewBlock->uLocVarSize = 0;
	pNewBlock->pExternalBlock = NULL;
	
	return pNewBlock;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
TContrElemPtr synCreateNewContrElem(TContrElemHandlerPtr pHandler)
{
	TContrElemPtr				pNewContrElem;
	
	pNewContrElem = (TContrElemPtr)malloc(sizeof(TContrElem));
	pNewContrElem->uSemClass = SEMCLASS_EQUATION;
	pNewContrElem->pBegArgList = NULL;
	pNewContrElem->pNextContrElem = NULL;
	pNewContrElem->pContrElemHandler = pHandler;
	
	return pNewContrElem;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
TOperationElemPtr synCreateNewOperElem(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum, TIDCurStateListPtr pIDCurState)
{
	TOperationElemPtr					pNewContrElemArg;
	
	pNewContrElemArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
	pNewContrElemArg->pArgument.pvValue = pvValue;
	pNewContrElemArg->pArgument.uSemClass = uSemClass;
	pNewContrElemArg->pArgument.uLexClass = uType;
	pNewContrElemArg->pArgument.uBegPos = uBegPos;
	pNewContrElemArg->pArgument.uEndPos = uEndPos;
	pNewContrElemArg->pArgument.uStrNum = uStrNum;
	pNewContrElemArg->pArgument.pIDCurState = pIDCurState;

	pNewContrElemArg->pNextOperationElem = NULL;
	pNewContrElemArg->pPrevOperationElem = NULL;
	pNewContrElemArg->pTopOperationElem  = NULL;
		
	return pNewContrElemArg;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddBlockContrElem()
{
	TBlockPtr			pCurBlockContainer = NULL, pNewBlock = NULL;
	TContrElemPtr		pCurBlockCEContainer = NULL;
	TOperationElemPtr	pCurCEArgContainer = NULL;
	TContrElemPtr		pNewCE = NULL;
	
	synAddNewContrElemToCurBlock(SEMCLASS_BLOCK, &cgContrElemBlock);
	synAddNewOperElemToContrElem(SEMCLASS_BLOCK, 0, pCurBlock,0,0,0, NULL);
	
	pNewBlock = synCreateNewBlock();
	synAddNewOperElemToContrElem(SEMCLASS_BLOCK, 0, pNewBlock,0,0,0, NULL);
	
	pCurBlockContainer = pCurBlock;
	pCurBlockCEContainer = pCurContrElem;
	pCurCEArgContainer = pCurContrElemArg;
	
	pCurBlock = pNewBlock;
	pCurLocVar = NULL;
	pCurContrElem = NULL;
	pCurContrElemArg = NULL;
	
	synCheckNextNontermBlock();
	
	pCurBlock = pCurBlockContainer;
	pCurContrElem = pCurBlockCEContainer;
	pCurContrElemArg = pCurCEArgContainer;
	
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddNewBlockToCurFuncNode()
{
	extern TParseListNodePtr	pCurParseListNode;
	extern TContrElemPtr		pCurContrElem;
	TBlockPtr				pNewBlock;
	
	pNewBlock = synCreateNewBlock();	
	pNewBlock->uBlockSize = 0;
	pNewBlock->uBlockOffset = 0;
	pNewBlock->pExternalBlock = NULL;

	pCurParseListNode->FuncNode.pFuncBlock = pNewBlock;
	pCurBlock = pNewBlock;
	
	pCurLocVar = NULL;
	pCurContrElem = NULL;
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddNewLocVarToCurBlock(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum, TIDCurStateListPtr pIDCurState)
{
	extern TBlockPtr		pCurBlock;
	extern TOperationElemPtr	pCurLocVar;
	TOperationElemPtr			pNewLocVar;
	
	pNewLocVar = synCreateNewOperElem(uSemClass, uType, pvValue, uBegPos, uEndPos, uStrNum, pIDCurState);
	
	if (pCurLocVar)
	{
		pCurLocVar->pNextOperationElem = pNewLocVar;
		pNewLocVar->pPrevOperationElem = pCurLocVar;
		pCurLocVar = pNewLocVar;
	}

	if (!pCurBlock->pBegLocVarList)
	{
		pCurBlock->pBegLocVarList = pNewLocVar;
		pCurLocVar = pNewLocVar;
	}
		
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
TContrElemPtr synAddNewBlockContrElemToCurContrElem()
{
	extern TContrElemPtr		pCurContrElem;
	TBlockPtr			pCurBlockContainer = NULL, pNewBlock = NULL;
	TContrElemPtr		pCurBlockCEContainer = NULL;
	TOperationElemPtr	pCurCEArgContainer = NULL;
	TContrElemPtr		pNewCE = NULL;
	
	
	pNewCE = synCreateNewContrElem(&cgContrElemBlock);
	pCurContrElem = pNewCE;
	pCurContrElemArg = NULL;
	synAddNewOperElemToContrElem(SEMCLASS_BLOCK, 0, pCurBlock,0,0,0, NULL);
	
	pNewBlock = synCreateNewBlock();
	synAddNewOperElemToContrElem(SEMCLASS_BLOCK, 0, pNewBlock,0,0,0, NULL);
	
	pCurBlockContainer = pCurBlock;
	pCurBlockCEContainer = pCurContrElem;
	pCurCEArgContainer = pCurContrElemArg;
	
	
	pCurBlock = pNewBlock;
	pCurLocVar = NULL;
	pCurContrElem = NULL;
	pCurContrElemArg = NULL;
	
	synCheckNextNontermBlock();
	
	pCurBlock = pCurBlockContainer;
	pCurContrElem = pCurBlockCEContainer;
	pCurContrElemArg = pCurCEArgContainer;

	return pNewCE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddNewContrElemToCurBlock(UINT uSemClass, TContrElemHandlerPtr pHandler)
{
	extern TParseListNodePtr	pCurParseListNode;
	extern TContrElemPtr		pCurContrElem;

	TContrElemPtr				pNewContrElem;

	pNewContrElem = synCreateNewContrElem(pHandler);
	pNewContrElem->uSemClass = uSemClass;
	if (pCurContrElem)
	{
		pCurContrElem->pNextContrElem = pNewContrElem;
		pCurContrElem = pNewContrElem;
	}

	if(!pCurBlock->pBegContrList)
	{	
		pCurBlock->pBegContrList = pNewContrElem;
		pCurContrElem = pNewContrElem;
	}
		
	pCurContrElemArg = NULL;
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddNewOperElemToContrElem(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum, TIDCurStateListPtr pIDCurState)
{
	extern TContrElemPtr		pCurContrElem;
	extern TOperationElemPtr	pCurContrElemArg;
	TOperationElemPtr			pNewContrElemArg;
	
	pNewContrElemArg = synCreateNewOperElem(uSemClass, uType, pvValue, uBegPos, uEndPos, uStrNum, pIDCurState);

	if (pCurContrElemArg)
	{
		pCurContrElemArg->pNextOperationElem = pNewContrElemArg;
		pNewContrElemArg->pPrevOperationElem = pCurContrElemArg;
		pCurContrElemArg = pNewContrElemArg;
	}

	if (!pCurContrElem->pBegArgList)
	{
		pCurContrElem->pBegArgList = pNewContrElemArg;
		pCurContrElemArg = pNewContrElemArg;
	}
		
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddNewFuncArg(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum, TIDCurStateListPtr pIDCurState)
{
	extern TParseListNodePtr	pCurParseListNode;
	extern TOperationElemPtr	pCurFuncArg;
	
	TOperationElemPtr			pNewFuncArg;
	
	pNewFuncArg = synCreateNewOperElem(uSemClass, uType, pvValue, uBegPos, uEndPos, uStrNum, pIDCurState);
	pNewFuncArg->pNextOperationElem = NULL;
	pNewFuncArg->pPrevOperationElem = NULL;
	
	if (pCurFuncArg)
	{
		pCurFuncArg->pNextOperationElem = pNewFuncArg;
		pNewFuncArg->pPrevOperationElem = pCurFuncArg;
		pCurFuncArg = pNewFuncArg;
		
	}
	
	if(!pCurParseListNode->FuncNode.pBegArgList)
	{	
		pCurParseListNode->FuncNode.pBegArgList = pNewFuncArg;
		pCurFuncArg = pNewFuncArg;
		
	}
			
	return TRUE;
}

/*
*	
*
*	@param UINT uType - type of identifier 
*	@param UINT uPosBeg - Beginning position in the file
*	@param UINT uPosEnd - Ending position in the file
*	@param UINT uLineNum - Line number
*	@param TRTItemContentPtr pItemContent - Pointer to the RT Item Content
*	@param TIdTablePtr pPrevTie - Pointer to a previous Identifier in the ID Table
*	@param TOperationElemPtr pArgInstance - 
*	@param TBlockPtr pBlock - Pointer to a current block being processed
*	@param TAAOperElemListPtr pBegIdDecl
*
*   @author Nail Sharipov
*/
TIdTablePtr synAddIdentTableElem(UINT uType, UINT uPosBeg, UINT uPosEnd, UINT uLineNum, 
								 TRTItemContentPtr pItemContent, TIdTablePtr pPrevTie, 
								 TOperationElemPtr pArgInstance, TBlockPtr pBlock, 
								 TAAOperElemListPtr pBegIdDecl)
{
    extern TIdTablePtr pCurIdTableElem;
    TIdTablePtr pNewIdTableElem;
    
    pNewIdTableElem = (TIdTablePtr)malloc(sizeof(TIdTable));
    
	pNewIdTableElem->pItemContent = pItemContent;
    
	pNewIdTableElem->pNextListItem = NULL;
	pNewIdTableElem->pPrevListItem = pCurIdTableElem;
    
	pNewIdTableElem->pPrevTie = pPrevTie;
    pNewIdTableElem->uType = uType;
	pNewIdTableElem->pArgInstance = pArgInstance;
	pNewIdTableElem->uPosBeg = uPosBeg;
	pNewIdTableElem->uPosEnd = uPosEnd;
	pNewIdTableElem->uLineNum = uLineNum;
	pNewIdTableElem->pBlock = pBlock;
	pNewIdTableElem->pBegIdDecl = pBegIdDecl;
	
	pCurIdTableElem->pNextListItem = pNewIdTableElem;
    pCurIdTableElem = pNewIdTableElem;

    if (uType == SYNCLASS_ACCESSORIES_BEGINBLOCK)
	{
		pCurIdTableBegBlock = pNewIdTableElem;
		pCurIdTableFillingElem = pCurIdTableBegBlock;
	}
    
	if (uType == SYNCLASS_ACCESSORIES_ENDBLOCK)
	{
		pNewIdTableElem->pPrevTie = pCurIdTableBegBlock;
		pCurIdTableFillingElem = pNewIdTableElem;
	}

    return pNewIdTableElem;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
void synIdError(TLexemeListItemPtr pErrorLexListItem, char * cpErrMess)
{
	extern TIdErrorPtr pIdErrorBegList;
	extern TIdErrorPtr pIdError;
	TIdErrorPtr pNewIdError;

	pNewIdError = (TIdErrorPtr)malloc(sizeof(TIdError));
	pNewIdError->cpErrorMess = (char *)malloc(strlen(cpErrMess)+1);
	strcpy(pNewIdError->cpErrorMess, cpErrMess);
	pNewIdError->ErrorInstance.cpTextFrag = pErrorLexListItem->LexemeInstance.cpTextFrag;
	pNewIdError->ErrorInstance.uLexClass = pErrorLexListItem->LexemeInstance.uLexClass;
	pNewIdError->ErrorInstance.uPosBeg = pErrorLexListItem->LexemeInstance.uPosBeg;
	pNewIdError->ErrorInstance.uPosEnd = pErrorLexListItem->LexemeInstance.uPosEnd;
	pNewIdError->ErrorInstance.uLineNum = pErrorLexListItem->LexemeInstance.uLineNum;
	pNewIdError->pNextError = NULL;

	if (pIdError)
	{
		pIdError->pNextError = pNewIdError;
	}
	
	if (!pIdErrorBegList)
	{
		pIdErrorBegList= pNewIdError;
	}
	pIdError = pNewIdError;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
TRTItemPtr synCheckIdInRT(TLexemeListItemPtr pLexIdentifier)
{
	TRTItemPtr pBuff;
    extern TRTItemPtr pBegRT;
    
    pBuff = pBegRT;
    
    while (pBuff)
    {
		if (!strcmp(pLexIdentifier->LexemeInstance.cpTextFrag, pBuff->RTItemContent.cpIdName))
		if (pBuff->RTItemContent.pIdTableElem)
		{
			return pBuff;
		}
		else
		{
			synIdError (pLexIdentifier, "unknown identifier");
			return NULL;
		}
        pBuff = pBuff->pNextRTItem;
    };
	return FALSE;
	
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
void synPrintIdErrors()
{
	extern TIdErrorPtr pIdErrorBegList;
	TIdErrorPtr pTemp;
	FILE * pOutInfoFile;
	
    pOutInfoFile = fopen("CompileInfo.txt","a");
    
    printf("\n");
    fprintf(pOutInfoFile, "\n");
	printf(" *** ERRORS *** \n\n");
	fprintf(pOutInfoFile, " *** ERRORS *** \n\n");

	if(pIdErrorBegList)
		pTemp = pIdErrorBegList;
	else
	{
		printf("     no errors \n\n");
		fprintf(pOutInfoFile, "     no errors \n\n");
		fclose(pOutInfoFile);
		return;
	}
	
	while (pTemp)
	{
		printf(" (%3d ) error: \'%s\' : %s\n", 
			pTemp->ErrorInstance.uLineNum, (pTemp->ErrorInstance.cpTextFrag)?pTemp->ErrorInstance.cpTextFrag:"",
			pTemp->cpErrorMess);
		fprintf(pOutInfoFile, " (%X %X) error: \'%s\' : %s\n", 
			pTemp->ErrorInstance.uPosBeg, pTemp->ErrorInstance.uPosEnd, pTemp->ErrorInstance.cpTextFrag,
			pTemp->cpErrorMess);
		pTemp = pTemp->pNextError;
	}
	fclose(pOutInfoFile);

}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
void synError()
{
    extern TLexemeListItemPtr pCurLexListItem;

	if (pCurLexListItem)
		synIdError(pCurLexListItem->pPrevLexemeInstance, "syntax error");
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// Just returns the next lexeme class without changing pCurLexListItem pointer to the current lexeme
int synRetNextLCValue()
{
    extern TLexemeListItemPtr pCurLexListItem;
    
    if (pCurLexListItem)
        return pCurLexListItem->LexemeInstance.uLexClass;
    return LEXCLASS_UNKNOWN;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synGetNextLexemeClass()
{
    extern TLexemeListItemPtr pCurLexListItem;
    UINT uNextLexClass; 

    if (pCurLexListItem)
    {
        uNextLexClass = pCurLexListItem->LexemeInstance.uLexClass;
        pCurLexListItem = pCurLexListItem->pNextLexemeInstance;
        return uNextLexClass;
    }
    return LEXCLASS_UNKNOWN;
}
 
/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/   
int synIsNextTermLexeme (UINT uVerifiableLexClass)
{
    extern TLexemeListItemPtr pCurLexListItem;
    UINT uNextLexClass;
        
    uNextLexClass = synRetNextLCValue();
    if ( uVerifiableLexClass == uNextLexClass )
        return uNextLexClass;
        
    return FALSE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/   
int synCheckNextTermLexeme (UINT uVerifiableLexClass)
{
    UINT uNextLexClass;

    uNextLexClass = synGetNextLexemeClass();
    if ( uVerifiableLexClass == uNextLexClass )
        return uNextLexClass;
      
    synError();
    return FALSE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermType()
{
    UINT uLexClass;
    
    uLexClass = synRetNextLCValue();
    switch ( uLexClass )
    {
    case LEXCLASS_TYPE_VOID:
    //case LEXCLASS_TYPE_INT:
    case LEXCLASS_TYPE_SHORT:
    case LEXCLASS_TYPE_CHAR:
        return uLexClass;
        break;
    default:
        return FALSE;
        break;
    }

}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermType()
{
    UINT uLexClass;

    uLexClass = synGetNextLexemeClass();
    switch (uLexClass)
    {
    case LEXCLASS_TYPE_VOID:
    //case LEXCLASS_TYPE_INT:
    case LEXCLASS_TYPE_SHORT:
    case LEXCLASS_TYPE_CHAR:
        return uLexClass;
        break;
    default:
        synError();
        return FALSE;
        break;
    }
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermFuncImpl()
{
    UINT uLexClass;
    
    uLexClass = synRetNextLCValue();
    switch (uLexClass)
    {
    case LEXCLASS_PROGRAMM_ASM_ENTRY:
    case LEXCLASS_RT_IDENTIFIER:
    case LEXCLASS_PROGRAMM_RETURN_POINT:
	case LEXCLASS_BLOCK_LEFT_CURLY_BRACKET:
	case LEXCLASS_OPERATION_ASTERISK:
	case LEXCLASS_CONSTRUCTIONS_IF:
	case LEXCLASS_CONSTRUCTIONS_WHILE:
		return uLexClass;
        break;
    default:
        return FALSE;
        break;
    }
}

/*
*	Returns the Representation Table Item pointer by identifier
*	name. Returns FALSE In case there is no RT Item with such name
*
*	@param char *cpName - Identifier name
*   @author Nail Sharipov
*/

TRTItemPtr synGetRTElemAddrByName( char * cpName )
{
    TRTItemPtr pBuff;
    extern TRTItemPtr pBegRT;
    
    pBuff = pBegRT;
    
    while (pBuff)
    {
        if (!strcmp(cpName, pBuff->RTItemContent.cpIdName))
            return pBuff;
        pBuff = pBuff->pNextRTItem;
    };
    return FALSE;
}

/*
*	
*
*	@param 
*	@param 
*	@param
*   @author Nail Sharipov
*/
int synFillRT()
{
	extern TIdTablePtr pCurIdTableElem;
	extern TIdTablePtr pCurIdTableBegBlock;
	TIdTablePtr pCur;
	

	pCur = pCurIdTableFillingElem->pNextListItem;
	while ( pCur != pCurIdTableElem->pNextListItem )
	{
		pCur->pPrevTie = pCur->pItemContent->pIdTableElem;
		pCur->pItemContent->pIdTableElem = pCur;
		pCur = pCur->pNextListItem;
	}
	pCurIdTableFillingElem = pCurIdTableElem;

	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
TIdTablePtr synInternBlocksPassing(TIdTablePtr pCur)
{

	while ( pCur->uType != SYNCLASS_ACCESSORIES_ENDBLOCK )
	{
		if ( pCur->uType == SYNCLASS_ACCESSORIES_BEGINBLOCK )
		{
			pCur = pCur->pNextListItem;
			pCur = synInternBlocksPassing(pCur);
		}
		else
			pCur = pCur->pNextListItem;
	}
	
	return pCur;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synUnFillRT()
{
	extern TIdTablePtr pCurIdTableElem;
	extern TIdTablePtr pCurIdTableBegBlock;
	TIdTablePtr pCur;
	
	pCur = pCurIdTableBegBlock->pNextListItem;
	while ( pCur )
	{
		if ( pCur->uType == SYNCLASS_ACCESSORIES_BEGINBLOCK )
		{
			pCur = pCur->pNextListItem;
			pCur = synInternBlocksPassing(pCur);
		}
		else
		{
			if ( pCur->uType != SYNCLASS_ACCESSORIES_ENDBLOCK )
				pCur->pItemContent->pIdTableElem = pCur->pPrevTie;		
		}
		
		pCur = pCur->pNextListItem;
	}
	pCurIdTableBegBlock = pCurIdTableBegBlock->pPrevTie;

	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermFuncDecl()
{
	extern TLexemeListItemPtr pCurLexListItem;

    UINT uType;
    TRTItemPtr pBuff;
	TIdTablePtr pCurFuncId;

	uType = synCheckNextNontermType();
	synCheckNextTermLexeme(LEXCLASS_RT_IDENTIFIER);
	
	pBuff = synGetRTElemAddrByName(pCurLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag);
 	
	if (pBuff)
	{
		/*
		*	@param UINT uType - type of identifier 
		*	@param UINT uPosBeg - Beginning position in the file
		*	@param UINT uPosEnd - Ending position in the file
		*	@param UINT uLineNum - Line number
		
		*	@param TRTItemContentPtr pItemContent - Pointer to the RT Item Content
		*	@param TIdTablePtr pPrevTie - 
		
		*	@param TOperationElemPtr pArgInstance - 
		*	@param TBlockPtr pBlock - Pointer to a current block being processed
		*	@param TAAOperElemListPtr pBegIdDecl
		*/
		pCurFuncId = synAddIdentTableElem(uType, 
											pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
											pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
											pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
											
											&(pBuff->RTItemContent), 
											pBuff->RTItemContent.pIdTableElem, 
											
											NULL, 
											NULL, 
											NULL);
	}

	/*
	*	
	*
	*	@param char * cpFuncName
	*	@param UINT uType
	*	@param TIdTablePtr pFuncId
	*
	*   @author Nail Sharipov
	*/
	synAddNewFuncNode(pCurLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag, uType, pCurFuncId);
	
	synFillRT();
	synAddIdentTableElem(SYNCLASS_ACCESSORIES_BEGINBLOCK, 0,0,0,NULL, pCurIdTableBegBlock, NULL, NULL, NULL);
	
	synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_ROUND_BRACKET);
    
    if (uType = synIsNextNontermType())
    {
        synCheckNextNontermType();
        synAddNewFuncArg(SEMCLASS_FUNCARG, uType, NULL,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
		
        synCheckVarDeclaration();
		if(pIdNameLexListItem)	
		{
			pBuff = synGetRTElemAddrByName(pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag);
			
			synAddIdentTableElem(uType,  
				pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
				&(pBuff->RTItemContent), pBuff->RTItemContent.pIdTableElem, 
				pCurFuncArg, NULL, pBegAAElement);
			
			pCurFuncArg->pArgument.pvValue = pCurIdTableElem;
		}
        
        while (synIsNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA))
        {
            
            synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA);
            
            uType = synIsNextNontermType();
            synCheckNextNontermType();
			
			synAddNewFuncArg(LEXCLASS_RT_IDENTIFIER, uType, NULL,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
            synCheckVarDeclaration();
			
			if(pIdNameLexListItem)	
			{
				pBuff = synGetRTElemAddrByName(pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag);
				
				synAddIdentTableElem(uType,  
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
					&(pBuff->RTItemContent), pBuff->RTItemContent.pIdTableElem, 
					pCurFuncArg, NULL, pBegAAElement);
				
				pCurFuncArg->pArgument.pvValue = pCurIdTableElem;
			}
        }
    }
	
	pCurFuncId->pArgInstance = pCurParseListNode->FuncNode.pBegArgList;
	synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET);
    return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckVarDeclaration()
{
	extern TAAOperElemListPtr	pCurIdAAE;
	extern TAAOperElemListPtr	pBegAAElement;
	extern TLexemeListItemPtr	pIdNameLexListItem;

	pIdNameLexListItem	= NULL;
	pCurIdAAE	= NULL;
	pBegAAElement	= NULL;
	
	synIdDeclaration();
	if (!pIdNameLexListItem)
	{
		semError("Identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"needs name specification");
	}
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCreateNewAAOperElement(UINT uAAOEType, void *pvValue)
{
	extern TAAOperElemListPtr	pCurIdAAE;
	extern TAAOperElemListPtr	pBegAAElement;
	TAAOperElemListPtr			pNewAAOE;
	
	pNewAAOE = (TAAOperElemListPtr)malloc(sizeof(TAAOperationElementList));
	pNewAAOE->AAOperElem.pvValue = pvValue;
	pNewAAOE->AAOperElem.uDeclType = uAAOEType;
	pNewAAOE->pNextAAElem = NULL;
	pNewAAOE->pPrevAAElem = NULL;
	
	if (pCurIdAAE)
	{
		pCurIdAAE->pNextAAElem = pNewAAOE;
		pNewAAOE->pPrevAAElem = pCurIdAAE;
		pCurIdAAE = pNewAAOE;
	}
	
	if (!pBegAAElement)
	{
		pBegAAElement = pNewAAOE;
		pCurIdAAE = pNewAAOE;
	}

	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIdDeclaration()
{
	UINT uAsteriskCount = 0;
	void *pvValue;
	
	for (uAsteriskCount = 0; synIsNextTermLexeme(LEXCLASS_OPERATION_ASTERISK); uAsteriskCount++)
		synCheckNextTermLexeme(LEXCLASS_OPERATION_ASTERISK);

	synIdProperDeclaration();
	
	if (uAsteriskCount) // parasitic round brackets elimination
	{
		pvValue = (UINT *)malloc(sizeof(UINT));
		*((UINT *)pvValue) = uAsteriskCount;
		synCreateNewAAOperElement(SYNDECLELEMTYPE_ASTERISK, pvValue);
	}
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIdProperDeclaration()
{
	char	*cValue;
	void	*pvValue	= NULL;
	extern TLexemeListItemPtr pCurLexListItem;
	extern TLexemeListItemPtr pIdNameLexListItem;

	if (synIsNextTermLexeme(LEXCLASS_BLOCK_LEFT_ROUND_BRACKET))
	{
		synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_ROUND_BRACKET);
		synIdDeclaration();
		synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET);
	}
	else
		if(synCheckNextTermLexeme(LEXCLASS_RT_IDENTIFIER))
		{
			pIdNameLexListItem = pCurLexListItem;
		}
	
	// array declaration
	while(synIsNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET))
	{
		synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET);
		if(synIsNextNontermValue())
		{
			synCheckNextNontermValue();
			pvValue = (UINT *)malloc(sizeof(UINT));
			cValue = &pCurLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag[2];
			*((UINT*)pvValue) = strtol(cValue, NULL, 16);
		}
		synCreateNewAAOperElement(SYNDECLELEMTYPE_SQUARE_BRACKETS, pvValue);
		synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_SQAURE_BRACKET);
	}
	/* TODO: put here function pointer declaration ability ("ANSI C" D. Ritchie, p.161 ) */
	
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
void synPrintAAOEList()
{
	extern TAAOperElemListPtr	pBegAAElement;
	TAAOperElemListPtr pTempAAOE;
	pTempAAOE = pBegAAElement;

	while(pTempAAOE)
	{
		printf("%d ---- %d \n", pTempAAOE->AAOperElem.uDeclType, *((UINT*)pTempAAOE->AAOperElem.pvValue));
		pTempAAOE = pTempAAOE->pNextAAElem;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermVarDecl()
{

    UINT uType;
    TRTItemPtr pBuff;

	if (uType = synIsNextNontermType())
	{
		synCheckNextNontermType();
    
		synAddNewLocVarToCurBlock(LEXCLASS_RT_IDENTIFIER, uType, NULL,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);

		synCheckVarDeclaration();
		if(pIdNameLexListItem)
		{
			pBuff = synGetRTElemAddrByName(pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag);
			
			synAddIdentTableElem(uType,  
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
					&(pBuff->RTItemContent), pBuff->RTItemContent.pIdTableElem, 
					pCurLocVar, pCurBlock, pBegAAElement);
		}
		pCurLocVar->pArgument.pvValue = pCurIdTableElem;

		while (synIsNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA))
		{
			synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA);
        
			synAddNewLocVarToCurBlock(LEXCLASS_RT_IDENTIFIER, uType, NULL,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
        
			synCheckVarDeclaration();
			
			if(pIdNameLexListItem)
			{
				pBuff = synGetRTElemAddrByName(pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag);
				
				synAddIdentTableElem(uType,  
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
					&(pBuff->RTItemContent), pBuff->RTItemContent.pIdTableElem, 
						pCurLocVar, pCurBlock, pBegAAElement);
			
				pCurLocVar->pArgument.pvValue = pCurIdTableElem;
			}
		}
    }
    
    synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_SEMICOLUMN);
    while ( synIsNextNontermType() )
    {
        uType = synIsNextNontermType();
		synCheckNextNontermType();
		synAddNewLocVarToCurBlock(LEXCLASS_RT_IDENTIFIER, uType, NULL,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);        
		
		synCheckVarDeclaration();
		if(pIdNameLexListItem)
		{
			pBuff = synGetRTElemAddrByName(pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag);
			
			synAddIdentTableElem(uType,  
				pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
				&(pBuff->RTItemContent), pBuff->RTItemContent.pIdTableElem, 
						pCurLocVar, pCurBlock, pBegAAElement);
        
			pCurLocVar->pArgument.pvValue = pCurIdTableElem;
		}
        while (synIsNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA))
        {
            synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA);
			
			
			synAddNewLocVarToCurBlock(LEXCLASS_RT_IDENTIFIER, uType, NULL,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);

            synCheckVarDeclaration();
			if(pIdNameLexListItem)	
			{
				pBuff = synGetRTElemAddrByName(pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag);
				
				synAddIdentTableElem(uType,  
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
					pIdNameLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
					&(pBuff->RTItemContent), pBuff->RTItemContent.pIdTableElem, 
					pCurLocVar, pCurBlock, pBegAAElement);
            
				pCurLocVar->pArgument.pvValue = pCurIdTableElem;
			}
        }
        synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_SEMICOLUMN);

    }    
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermArgEnum()
{
    switch (synRetNextLCValue())
    {
    case LEXCLASS_RT_NUMERIC_CONSTANT:
    case LEXCLASS_RT_STRING_CONSTANT:
    case LEXCLASS_OPERATION_ASTERISK:
    case LEXCLASS_RT_IDENTIFIER:
        return TRUE;
        break;
    default:
        return FALSE;
        break;
    }
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermValue()
{
    UINT uType;
	switch (uType = synGetNextLexemeClass())
    {
    case LEXCLASS_RT_NUMERIC_CONSTANT:
    case LEXCLASS_RT_STRING_CONSTANT:
        uType = semRetValueType(pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd - pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg - 1);
		if (uType == LEXCLASS_UNKNOWN)
			synIdError(pCurLexListItem->pPrevLexemeInstance, "value out of range");
		return uType;
        break;
    default:
        synError();
        return FALSE;
        break;
    }
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermValue()
{
    switch (synRetNextLCValue())
    {
    case LEXCLASS_RT_NUMERIC_CONSTANT:
    case LEXCLASS_RT_STRING_CONSTANT:
        return TRUE;
        break;
    default:
        return FALSE;
        break;
    }
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermArgEnum()
{
	TContrElemPtr pCurCEContainer = NULL, pNewContrElem = NULL;
	TOperationElemPtr pCurCEArgContainer = NULL;
	UINT uType = LEXCLASS_TYPE_VOID;

	pNewContrElem = synCreateNewContrElem(&cgContrElemEquation);
	
	synAddNewOperElemToContrElem(SEMCLASS_FUNCARG, LEXCLASS_TYPE_VOID, pNewContrElem,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
	
	pCurCEContainer = pCurContrElem;
	pCurCEArgContainer = pCurContrElemArg;
		
	pCurContrElem = pNewContrElem;
	pCurContrElemArg = NULL;
	
	uType = synCheckNextNontermEquation();
    
	pCurContrElem = pCurCEContainer;
	pCurContrElemArg = pCurCEArgContainer;
	
	pCurCEArgContainer->pArgument.uLexClass = uType;
	while (synIsNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA))
    {
		synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA);
		pNewContrElem = synCreateNewContrElem(&cgContrElemEquation);
		
		synAddNewOperElemToContrElem(SEMCLASS_FUNCARG, LEXCLASS_TYPE_SHORT, pNewContrElem,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
		
		pCurCEContainer = pCurContrElem;
		pCurCEArgContainer = pCurContrElemArg;
			
		pCurContrElem = pNewContrElem;
		pCurContrElemArg = NULL;
		
		uType = synCheckNextNontermEquation();
    
		pCurContrElem = pCurCEContainer;
		pCurContrElemArg = pCurCEArgContainer;

		pCurCEArgContainer->pArgument.uLexClass = uType;
	}
    return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
TIDCurStatePtr synCreateNewIdCurStateElem(UINT uStateType, TIdTablePtr pIdTableElem, TAAOperElemListPtr pCurAAElem, void *pvValue, UINT uIndLvlCount)
{
	TIDCurStatePtr pNewIdStateElem;

	pNewIdStateElem = (TIDCurStatePtr)malloc(sizeof(TIDCurState));
	pNewIdStateElem->uStateType = uStateType;
	pNewIdStateElem->pIdTableElem = pIdTableElem;
	pNewIdStateElem->pCurAAElem = pCurAAElem;
	pNewIdStateElem->pvValue = pvValue;
	pNewIdStateElem->uIndLvlCount = uIndLvlCount;
	
	return pNewIdStateElem;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddNewIdCurStateElem(UINT uStateType, TIdTablePtr pIdTableElem, TAAOperElemListPtr pCurAAElem, void *pvValue, UINT uIndLvlCount)
{
	extern TIDCurStateListPtr pCurIdStateListElem;
	TIDCurStateListPtr pNewCurStateListElem;

	pNewCurStateListElem = (TIDCurStateListPtr)malloc(sizeof(TIDCurStateList));
	pNewCurStateListElem->pCurIdState = synCreateNewIdCurStateElem(uStateType, pIdTableElem, pCurAAElem, pvValue, uIndLvlCount);

	pNewCurStateListElem->pNextCurStateElem = NULL;
	pNewCurStateListElem->pPrevCurStateElem = NULL;
	if (pCurIdStateListElem)
	{
		pCurIdStateListElem->pNextCurStateElem = pNewCurStateListElem;
		pNewCurStateListElem->pPrevCurStateElem = pCurIdStateListElem;
		pCurIdStateListElem = pNewCurStateListElem;
	}

	if (!pBegIdStateListElem)
	{
		pBegIdStateListElem = pNewCurStateListElem;
		pCurIdStateListElem = pNewCurStateListElem;
	}
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermTransVarBrac()
{
	extern char bIsPointer;
	extern TAAOperElemListPtr	pCurIdAAE;
	extern TIDCurStateListPtr	pCurIdStateListElem;	
	extern UINT				uCurSemClass;
	extern UINT				uCurType;
	extern UINT				uTopLvlExprAsteriskCount;
	extern UINT				uDeclAsteriskCount;
	extern TAAOperElemListPtr pBegAAElement;

	TAAOperElemListPtr		pBegAAElementContainer = NULL;
	TAAOperElemListPtr		pCurIdAAEContainer		= NULL;
	TIDCurStateListPtr	pBegIdStateContainer	= NULL;
	TIDCurStateListPtr	pCurIdStateContainer	= NULL;
	TOperationElemPtr	pCurCEArgContainer		= NULL;
	TContrElemPtr		pCurCEContainer			= NULL;
	TContrElemPtr		pNewContrElem			= NULL;
	TRTItemPtr			pBuff;

	char	bPrevIsPointer = 0, bIsPointerContainer = 0 ;
	char	*cValue;
	void	*pvValue	= NULL;
	UINT	uSemClass	= 0;
	UINT	uType;
	UINT	uExspressionAsteriskCount = 0;
	UINT	uDeclACContainer = 0;
	UINT	uIndLvlCountAsterContainer = 0;
	UINT	uIndLvlCountSqBrcContainer = 0;
	UINT	uIndLvlCountAster = 0;
	UINT	uIndLvlCountSqBrc = 0;
	
	for (uExspressionAsteriskCount = 0; synIsNextTermLexeme(LEXCLASS_OPERATION_ASTERISK);uExspressionAsteriskCount++)
		synCheckNextTermLexeme(LEXCLASS_OPERATION_ASTERISK);
	
	uIndLvlCountAster = uExspressionAsteriskCount;
	
	if (synIsNextNontermValue())
    {
        uType = synCheckNextNontermValue();
		
        if(!bIsPointer)
		{
			switch(semRetTypeSizeByType(uType))
			{
			case 4:
				uSemClass = SEMCLASS_IMM32;
				break;
			case 2:
				uSemClass = SEMCLASS_IMM16;
				break;
			case 1:
				uSemClass = SEMCLASS_IMM8;
				break;
			}
			uCurSemClass = uSemClass;
		}
		
		pvValue = (UINT *)malloc(sizeof(UINT));
		cValue = &pCurLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag[2];
		
		*((UINT*)pvValue) = strtol(cValue, NULL, 16);
		synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
		if (uExspressionAsteriskCount > 0)
			semError("*",
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"unexpected use of this operation");

		return uType;    
    }
if ((synRetNextLCValue() == LEXCLASS_BLOCK_LEFT_ROUND_BRACKET) || (synRetNextLCValue() == LEXCLASS_RT_IDENTIFIER))
{
	
	switch(synRetNextLCValue())
    {
	case LEXCLASS_BLOCK_LEFT_ROUND_BRACKET:
			synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_ROUND_BRACKET);
	
			pvValue = (int *)malloc(1);
			*((UINT *)pvValue) = SEMOPERPRIORITY_BRACKET;
			synAddNewOperElemToContrElem(SEMCLASS_OPERATION, LEXCLASS_BLOCK_LEFT_ROUND_BRACKET, pvValue,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
			
			if (pBegIdStateListElem)
			{	
				pBegIdStateContainer = pBegIdStateListElem;
				pCurIdStateContainer = pCurIdStateListElem;
				pCurIdAAEContainer = pCurIdAAE;
				bPrevIsPointer = bIsPointer;
				pBegAAElementContainer = pBegAAElement;
			};
			bIsPointer = FALSE;
			pBegIdStateListElem = NULL;
 			pCurIdStateListElem = NULL;
 			pCurIdAAE = NULL;
			
			uType = synCheckNextNontermTransVarPlus(synCheckNextNontermTransVarMul(synCheckNextNontermTransVarBrac()));
			pvValue = (int *)malloc(1);
			*((UINT *)pvValue) = SEMOPERPRIORITY_BRACKET;
			synAddNewOperElemToContrElem(SEMCLASS_OPERATION, LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET, pvValue,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
			
			synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET);
			
			if (pCurIdStateListElem)
				pCurIdAAE = pCurIdStateListElem->pCurIdState->pCurAAElem;
			
			while ( (uDeclAsteriskCount == 0) && pCurIdAAE && (pCurIdAAE->pNextAAElem) )
			{
				pCurIdAAE = pCurIdAAE->pNextAAElem;
				switch (pCurIdAAE->AAOperElem.uDeclType)
				{
					case SYNDECLELEMTYPE_SQUARE_BRACKETS:	
						uDeclAsteriskCount = 1;
						break;
					case SYNDECLELEMTYPE_ASTERISK:
						uDeclAsteriskCount = *((UINT *)pCurIdAAE->AAOperElem.pvValue);
						break;
				}
			}
			
			while(synIsNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET))
			{
				uIndLvlCountSqBrc ++;
				if (pCurIdAAE && ((pCurIdAAE->AAOperElem.uDeclType == SYNDECLELEMTYPE_SQUARE_BRACKETS) || (pCurIdAAE->AAOperElem.uDeclType == SYNDECLELEMTYPE_ASTERISK)))
				{
					synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET);

					pNewContrElem = synCreateNewContrElem(&cgContrElemEquation);
					pCurCEContainer = pCurContrElem;
					pCurCEArgContainer = pCurContrElemArg;
						
					pCurContrElem = pNewContrElem;
					pCurContrElemArg = NULL;
					
					bIsPointerContainer = bIsPointer;
					synCheckNextNontermEquation();
					
					if (bIsPointer)
						semError("[",
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"illegal index, indirection not allowed");
					else
						bIsPointer = bIsPointerContainer;

					pCurContrElem = pCurCEContainer;
					pCurContrElemArg = pCurCEArgContainer;
					
					synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_SQAURE_BRACKET);
					
					while ( (uDeclAsteriskCount == 0) && (pCurIdAAE->pNextAAElem) )
					{
						pCurIdAAE = pCurIdAAE->pNextAAElem;
						switch (pCurIdAAE->AAOperElem.uDeclType)
						{
						case SYNDECLELEMTYPE_SQUARE_BRACKETS:	
							uDeclAsteriskCount = 1;
							break;
						case SYNDECLELEMTYPE_ASTERISK:
							uDeclAsteriskCount = *((UINT *)pCurIdAAE->AAOperElem.pvValue);
							break;
						}
					}
					if (uDeclAsteriskCount == 0)
						semError("[",
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"subscript requires array or pointer type");
					else
						uDeclAsteriskCount --;
				
					synAddNewIdCurStateElem(SYNIDCURSTATETYPE_INDEX_DEREFERENCING, NULL, pCurIdAAE, pNewContrElem, uIndLvlCountSqBrc);
				}
			}
			break;
	case LEXCLASS_RT_IDENTIFIER:
		synCheckNextTermLexeme(LEXCLASS_RT_IDENTIFIER);
		pBuff = synCheckIdInRT(pCurLexListItem->pPrevLexemeInstance);
		switch (synRetNextLCValue())
			{
			case LEXCLASS_BLOCK_LEFT_ROUND_BRACKET:
						
				synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_ROUND_BRACKET);
				if (pBuff)
				{
					pNewContrElem = synCreateNewContrElem(&cgContrElemFuncCall);
					uType = pBuff->RTItemContent.pIdTableElem->uType;
					synAddNewOperElemToContrElem(SEMCLASS_FUNCTION, uType, pNewContrElem,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
					
					synAddNewOperElemToContrElem(SEMCLASS_FUNCREFTOIDTABLE, pBuff->RTItemContent.pIdTableElem->uType, 
						pBuff->RTItemContent.pIdTableElem,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);

					pCurCEContainer = pCurContrElem;
					pCurCEArgContainer = pCurContrElemArg;
					pCurContrElem = pNewContrElem;
					pCurContrElemArg = NULL;
				
					synAddNewOperElemToContrElem(SEMCLASS_FUNCREFTOIDTABLE, pBuff->RTItemContent.pIdTableElem->uType, 
						pBuff->RTItemContent.pIdTableElem,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
				}
					
				if ( synIsNextNontermArgEnum() )
					synCheckNextNontermArgEnum();

				pCurContrElem = pCurCEContainer;
				pCurContrElemArg = pCurCEArgContainer;
				synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET);
				break;
			default:
				if (pBuff)
				if (pBuff->RTItemContent.pIdTableElem->pArgInstance)
				{	
					uType = pBuff->RTItemContent.pIdTableElem->pArgInstance->pArgument.uLexClass;
					
					if (pBegIdStateListElem)
					{	
						pBegIdStateContainer = pBegIdStateListElem;
						pCurIdStateContainer = pCurIdStateListElem;
						pCurIdAAEContainer = pCurIdAAE;
						uDeclACContainer = uDeclAsteriskCount;
						bPrevIsPointer = bIsPointer;
						pBegAAElementContainer = pBegAAElement;
					}
					
					pBegIdStateListElem = NULL;
					pCurIdStateListElem = NULL;
					pCurIdAAE = NULL;
					uDeclAsteriskCount = 0;
					bIsPointer = FALSE;
					pBegAAElement = NULL;

					pBegAAElement = pBuff->RTItemContent.pIdTableElem->pBegIdDecl;
					pCurIdAAE = pBegAAElement;
					
					while ( (uDeclAsteriskCount == 0) && pCurIdAAE )
					{
						switch (pCurIdAAE->AAOperElem.uDeclType)
						{
						case SYNDECLELEMTYPE_SQUARE_BRACKETS:	
							uDeclAsteriskCount = 1;
							break;
						case SYNDECLELEMTYPE_ASTERISK:
							uDeclAsteriskCount = *((UINT *)pCurIdAAE->AAOperElem.pvValue);
							break;
						}
					}
					
					pvValue = (UINT *)malloc(sizeof(UINT));
					*((UINT *)pvValue) = uDeclAsteriskCount;
					synAddNewIdCurStateElem(SYNIDCURSTATETYPE_INITIAL_VALUES, pBuff->RTItemContent.pIdTableElem, pCurIdAAE, pvValue, 0);
					bIsPointer = TRUE;
					
					while(synIsNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET))
					{
						uIndLvlCountSqBrc ++;

						synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET);
						
						pNewContrElem = synCreateNewContrElem(&cgContrElemEquation);
							
						pCurCEContainer = pCurContrElem;
						pCurCEArgContainer = pCurContrElemArg;
								
						pCurContrElem = pNewContrElem;
						pCurContrElemArg = NULL;
						
						bIsPointerContainer = bIsPointer;
						synCheckNextNontermEquation();
							
						if (bIsPointer)
							semError("[",
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
								"illegal index, indirection not allowed");
						else
							bIsPointer = bIsPointerContainer;
							
						pCurContrElem = pCurCEContainer;
						pCurContrElemArg = pCurCEArgContainer;
							
						synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_SQAURE_BRACKET);
							
						while ( pCurIdAAE && (uDeclAsteriskCount == 0) && (pCurIdAAE->pNextAAElem) )
						{
							pCurIdAAE = pCurIdAAE->pNextAAElem;
							switch (pCurIdAAE->AAOperElem.uDeclType)
							{
							case SYNDECLELEMTYPE_SQUARE_BRACKETS:	
								uDeclAsteriskCount = 1;
								break;
							case SYNDECLELEMTYPE_ASTERISK:
								uDeclAsteriskCount = *((UINT *)pCurIdAAE->AAOperElem.pvValue);
								break;
							}
						}
						
						if (uDeclAsteriskCount == 0)
							semError("[",
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
								"subscript requires array or pointer type");
						else
							uDeclAsteriskCount --;
							
						synAddNewIdCurStateElem(SYNIDCURSTATETYPE_INDEX_DEREFERENCING, NULL, pCurIdAAE, pNewContrElem, uIndLvlCountSqBrc);
						
					}	

					uSemClass = SEMCLASS_MEM16;
					synAddNewOperElemToContrElem(uSemClass, uType, pBegIdStateListElem,
							pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
							pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
							pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
				}
				else
					synIdError(pCurLexListItem->pPrevLexemeInstance, "unacceptable use of this element");
				break;
			}
			break;
	}
	
	uIndLvlCountAster = uExspressionAsteriskCount;
	while (uExspressionAsteriskCount>0)
	{
		while ( pCurIdAAE && (uDeclAsteriskCount == 0) && (pCurIdAAE->pNextAAElem))
		{
			pCurIdAAE = pCurIdAAE->pNextAAElem;
			switch (pCurIdAAE->AAOperElem.uDeclType)
			{
			case SYNDECLELEMTYPE_SQUARE_BRACKETS:	
				uDeclAsteriskCount = 1;
				break;
			case SYNDECLELEMTYPE_ASTERISK:
				uDeclAsteriskCount = *((UINT *)pCurIdAAE->AAOperElem.pvValue);
				break;
			}
		}
		if (uDeclAsteriskCount == 0)
			break;
		uDeclAsteriskCount--;
		uExspressionAsteriskCount--;
	}
	if (uIndLvlCountAster)
	{
		pvValue = (UINT *)malloc(sizeof(UINT));
		*((UINT *)pvValue) = uDeclAsteriskCount;
		synAddNewIdCurStateElem(SYNIDCURSTATETYPE_DEREFERENCING, NULL, pCurIdAAE, pvValue, uIndLvlCountAster);
	}
	
	if (uExspressionAsteriskCount > 0)
	{	
		semError("*",
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"illegal indirection");
	}
	else
	{	
		if ((pCurIdAAE && !pCurIdAAE->pNextAAElem && uDeclAsteriskCount == 0) || !pBegAAElement)
		{		
			bIsPointer = FALSE;
		}
	}
	
	if (bPrevIsPointer && bIsPointer)
	{	
		semError("expression",
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"cannot operate with two or more pointers");
	}
	else 
		if (pCurIdStateContainer)
		{
			pBegIdStateListElem = pBegIdStateContainer;
			pCurIdStateListElem = pCurIdStateContainer;
			pCurIdAAE = pCurIdAAEContainer;
			uDeclAsteriskCount = uDeclACContainer ;
			bIsPointer = bPrevIsPointer;
			pBegAAElement = pBegAAElementContainer;
		}
	return uType;
}

	synError();
	return FALSE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermTransVarMul(UINT uType)
{
    UINT uLexClass = 0;
	UINT uSemClass = 0;
	int * pvValue;
	
	if (uType > uCurType)
	{
		uCurType = uType;
	}
		
	if ( synIsNextTermLexeme(LEXCLASS_OPERATION_ASTERISK) || synIsNextTermLexeme(LEXCLASS_OPERATION_SLASH) )
	{
		if (synIsNextTermLexeme( LEXCLASS_OPERATION_ASTERISK ))
			uLexClass = synCheckNextTermLexeme(LEXCLASS_OPERATION_ASTERISK);
		if (synIsNextTermLexeme( LEXCLASS_OPERATION_SLASH ))
			uLexClass = synCheckNextTermLexeme(LEXCLASS_OPERATION_SLASH);    

		switch(semRetTypeSizeByType(uType))
		{
		case 4:
			uSemClass = SEMCLASS_IMM32;
			break;
		case 2:
			uSemClass = SEMCLASS_IMM16;
			break;
		case 1:
			uSemClass = SEMCLASS_IMM8;
			break;
		}
		uCurSemClass = uSemClass;
			
		pvValue = (int*)malloc(sizeof(int));
		*pvValue = uDeclAsteriskCount;
 		if(pCurIdStateListElem)
 			synAddNewIdCurStateElem(SYNIDCURSTATETYPE_OPERATION, NULL, pCurIdAAE, pvValue, 0);
					
		synAddNewOperElemToContrElem(SEMCLASS_OPERATION, uLexClass, pCurIdStateListElem,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
		synCheckNextNontermTransVarMul(synCheckNextNontermTransVarBrac());	
	}
    return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermTransVarPlus(UINT uType)
{
    UINT uLexClass = 0;
	UINT uSemClass = 0;
	int * pvValue  = NULL;
	int * pvValue2 = NULL;
	TIDCurStateListPtr pPlusCurStateElem;
	
	if (uType > uCurType)
	{
		uCurType = uType;
	}
	if (synIsNextTermLexeme( LEXCLASS_OPERATION_PLUS ) || synIsNextTermLexeme( LEXCLASS_OPERATION_MINUS ))
    {
        if (synIsNextTermLexeme( LEXCLASS_OPERATION_PLUS ))
			uLexClass = synCheckNextTermLexeme(LEXCLASS_OPERATION_PLUS);
		if (synIsNextTermLexeme( LEXCLASS_OPERATION_MINUS ))
			uLexClass = synCheckNextTermLexeme(LEXCLASS_OPERATION_MINUS);
		
		if(!bIsPointer)
		{		
			switch(semRetTypeSizeByType(uType))
			{
			case 4:
				uSemClass = SEMCLASS_IMM32;
				break;
			case 2:
				uSemClass = SEMCLASS_IMM16;
				break;
			case 1:
				uSemClass = SEMCLASS_IMM8;
				break;
			}	
			uCurSemClass = uSemClass;
		}
		
		pvValue = (int*)malloc(sizeof(int));
		*pvValue = uDeclAsteriskCount;
		
		if(pCurIdStateListElem)
			synAddNewIdCurStateElem(SYNIDCURSTATETYPE_OPERATION, NULL, pCurIdAAE, pvValue, 0);
	
		synAddNewOperElemToContrElem(SEMCLASS_OPERATION, uLexClass, pPlusCurStateElem,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
	
		synCheckNextNontermTransVarPlus(synCheckNextNontermTransVarMul(synCheckNextNontermTransVarBrac()));
	}
    return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermVarEqual()
{
	extern char bIsPointer;
	char bIsEquationPointerType = 0;
	extern TIDCurStateListPtr pBegIdStateListElem;
	TIDCurStateListPtr pTempIdSLE = NULL;
	TContrElemPtr pCurCEContainer = NULL, pNewContrElem = NULL;
	TOperationElemPtr pCurCEArgContainer = NULL;
	UINT uType;
	
	pBegIdStateListElem = NULL;
	pCurIdStateListElem = NULL;
	uCurSemClass = SEMCLASS_MEM16;
	uCurType = LEXCLASS_TYPE_VOID;
	
	pNewContrElem = synCreateNewContrElem(&cgContrElemExpression);
	if (pCurLexListItem)
	synAddNewOperElemToContrElem(SEMCLASS_UNKNOWN, LEXCLASS_UNKNOWN, pNewContrElem,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
				
	pCurCEContainer = pCurContrElem;
	pCurCEArgContainer = pCurContrElemArg;

	pCurContrElem = pNewContrElem;
	pCurContrElemArg = NULL;
	
	bIsPointer = 0;
	
	uType = synCheckNextNontermTransVarPlus(synCheckNextNontermTransVarMul(synCheckNextNontermTransVarBrac()));
	pCurContrElem = pCurCEContainer;
	pCurContrElemArg = pCurCEArgContainer;
	
	
	if (pCurContrElemArg)
	{
		pCurContrElemArg->pArgument.uLexClass = uType;
		pCurContrElemArg->pArgument.uSemClass = uCurSemClass;
		pCurContrElemArg->pArgument.pIDCurState = pBegIdStateListElem;
	}	
	pTempIdSLE = pBegIdStateListElem;
	
	if(bIsPointer)
		bIsEquationPointerType = TRUE;

	uCurSemClass = SEMCLASS_MEM16;
	uType = uCurType;
	uCurType = LEXCLASS_TYPE_VOID;
	
	if (synIsNextTermLexeme( LEXCLASS_COMPARISON_EQUAL ))
	{
		synCheckNextTermLexeme(LEXCLASS_COMPARISON_EQUAL);
		pBegIdStateListElem = NULL;
		
		synCheckNextNontermVarEqual();
		if(bIsPointer)
			bIsEquationPointerType = TRUE;
	}
	bIsPointer = bIsEquationPointerType;
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermEquation()
{
	char	bPrevIsPointer = 0;
	UINT uType;
	TAAOperElemListPtr		pCurIdAAEContainer		= NULL;
	TIDCurStateListPtr	pBegIdStateContainer	= NULL;
	TIDCurStateListPtr	pCurIdStateContainer	= NULL;
	UINT				uDeclACContainer = 0;
	
	pBegIdStateContainer = pBegIdStateListElem;
	pCurIdStateContainer = pCurIdStateListElem;
	pCurIdAAEContainer = pCurIdAAE;
	uDeclACContainer = uDeclAsteriskCount;

	pBegIdStateListElem = NULL;
	pCurIdStateListElem = NULL;
	pCurIdAAE = NULL;
	uDeclAsteriskCount = 0;
	bIsPointer = 0;

	uType = synCheckNextNontermVarEqual();

	pBegIdStateListElem = pBegIdStateContainer;
	pCurIdStateListElem = pCurIdStateContainer;
	pCurIdAAE = pCurIdAAEContainer;
	uDeclAsteriskCount = uDeclACContainer;
	return uType;
};

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAddReturnContrElem()
{
	TContrElemPtr pCurCEContainer = NULL, pNewContrElem = NULL;
	TOperationElemPtr pCurCEArgContainer = NULL;
	UINT uType;
	
	pNewContrElem = synCreateNewContrElem(&cgContrElemEquation);
	synAddNewOperElemToContrElem(SEMCLASS_EQUATION, LEXCLASS_TYPE_VOID, pNewContrElem,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
	
	pCurCEContainer = pCurContrElem;
	pCurCEArgContainer = pCurContrElemArg;
	
	pCurContrElem = pNewContrElem;
	pCurContrElemArg = NULL;
	
	uType = synCheckNextNontermEquation();
	synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_SEMICOLUMN);
	pCurContrElem = pCurCEContainer;
	pCurContrElemArg = pCurCEArgContainer;
	
	pCurContrElemArg->pArgument.uLexClass = uType;
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermInstruction()
{
	switch(synRetNextLCValue())
	{
	case LEXCLASS_IA32INSTRUCTIONS_MOV:
	case LEXCLASS_IA32INSTRUCTIONS_PUSH:
	case LEXCLASS_IA32INSTRUCTIONS_JMP:
	case LEXCLASS_IA32INSTRUCTIONS_INT:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermSegmentRegister()
{
	switch(synRetNextLCValue())
	{
	case LEXCLASS_IA32REGISTERS_CS:
	case LEXCLASS_IA32REGISTERS_DS:
	case LEXCLASS_IA32REGISTERS_ES:
	case LEXCLASS_IA32REGISTERS_SS:
	case LEXCLASS_IA32REGISTERS_GS:
	case LEXCLASS_IA32REGISTERS_FS:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermByteGPRegister()
{
	switch(synRetNextLCValue())
	{
	case LEXCLASS_IA32REGISTERS_AH:
	case LEXCLASS_IA32REGISTERS_AL:
	case LEXCLASS_IA32REGISTERS_BH:
	case LEXCLASS_IA32REGISTERS_BL:
	case LEXCLASS_IA32REGISTERS_CH:
	case LEXCLASS_IA32REGISTERS_CL:
	case LEXCLASS_IA32REGISTERS_DH:
	case LEXCLASS_IA32REGISTERS_DL:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermWordBaseRegister()
{
	switch(synRetNextLCValue())
	{
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_BP:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermWordPointerRegister()
{
	switch(synRetNextLCValue())
	{
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DI:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermWordGPRegister()
{
	switch(synRetNextLCValue())
	{
	case LEXCLASS_IA32REGISTERS_AX:
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_CX:
	case LEXCLASS_IA32REGISTERS_DX:
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DI:
	case LEXCLASS_IA32REGISTERS_BP:
	case LEXCLASS_IA32REGISTERS_SP:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synIsNextNontermDWordGPRegister()
{
	switch(synRetNextLCValue())
	{
	case LEXCLASS_IA32REGISTERS_EAX:
	case LEXCLASS_IA32REGISTERS_EBX:
	case LEXCLASS_IA32REGISTERS_ECX:
	case LEXCLASS_IA32REGISTERS_EDX:
	case LEXCLASS_IA32REGISTERS_ESI:
	case LEXCLASS_IA32REGISTERS_EDI:
	case LEXCLASS_IA32REGISTERS_EBP:
	case LEXCLASS_IA32REGISTERS_ESP:
		return TRUE;
		break;
	default:
		return FALSE;
		break;
	}
}

/*
*	Checks whether a lexeme is a proper Segment Register (SREG)
*	If it's not casts an syntactical error.
*	Moves the Lexeme List pointer forward.
*
*   @author Nail Sharipov
*/
int synCheckNextNontermSegmentRegister()
{
	UINT uType;
	switch (uType = synGetNextLexemeClass())
	{
	case LEXCLASS_IA32REGISTERS_CS:
	case LEXCLASS_IA32REGISTERS_DS:
	case LEXCLASS_IA32REGISTERS_ES:
	case LEXCLASS_IA32REGISTERS_SS:
	case LEXCLASS_IA32REGISTERS_GS:
	case LEXCLASS_IA32REGISTERS_FS:
        return uType;
        break;
    default:
        synError();
		return FALSE;
		break;
	}
}

/*
*	Checks whether a lexeme is a proper 
*	byte (8-bit long) General Purpose Register
*	If it's not casts an syntactical error.
*	Moves the Lexeme List pointer forward.
*
*   @author Nail Sharipov
*/
int synCheckNextNontermByteGPRegister()
{
    UINT uType;	
	switch (uType = synGetNextLexemeClass())
	{
	case LEXCLASS_IA32REGISTERS_AH:
	case LEXCLASS_IA32REGISTERS_AL:
	case LEXCLASS_IA32REGISTERS_BH:
	case LEXCLASS_IA32REGISTERS_BL:
	case LEXCLASS_IA32REGISTERS_CH:
	case LEXCLASS_IA32REGISTERS_CL:
	case LEXCLASS_IA32REGISTERS_DH:
	case LEXCLASS_IA32REGISTERS_DL:
        return uType;
        break;
    default:
        synError();
		return FALSE;
		break;
	}
}

/*
*	Checks whether a lexeme is a proper 
*	word (16-bit long) General Purpose Register
*	If it's not casts an syntactical error.
*	Moves the Lexeme List pointer forward.
*	
*   @author Nail Sharipov
*/
int synCheckNextNontermWordGPRegister()
{
	UINT uType;
	switch (uType = synGetNextLexemeClass())
	{
	case LEXCLASS_IA32REGISTERS_AX:
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_CX:
	case LEXCLASS_IA32REGISTERS_DX:
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DI:
	case LEXCLASS_IA32REGISTERS_BP:
	case LEXCLASS_IA32REGISTERS_SP:
        return uType;
        break;
    default:
        synError();
		return FALSE;
		break;
	}
}

/*
*	Checks whether a lexeme is a proper 
*	word (16-bit long) Pointer Register
*	If it's not casts an syntactical error.
*	Moves the Lexeme List pointer forward.
*
*   @author Nail Sharipov
*/
int synCheckNextNontermWordPointerRegister()
{
	UINT uType;
	switch (uType = synGetNextLexemeClass())
	{
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DI:
		return uType;
        break;
    default:
        synError();
		return FALSE;
		break;
	}
}

/*
*	Checks whether a lexeme is a proper
*	word (16-bit long) Base Register to
*	specify the address (BX, BP)
*	If it's not casts an syntactical error.
*	Moves the Lexeme List pointer forward.
*
*   @author Nail Sharipov
*/
int synCheckNextNontermWordBaseRegister()
{
	UINT uType;
	switch (uType = synGetNextLexemeClass())
	{
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_BP:
		return uType;
        break;
    default:
        synError();
		return FALSE;
		break;
	}
}

/*
*	Checks whether a lexeme is a proper 
*	double-word (32-bit long) General Purpose Register.
*	If it's not casts an syntactical error.
*	Moves the Lexeme List pointer forward.
*
*   @author Nail Sharipov
*/
int synCheckNextNontermDWordGPRegister()
{
	UINT uType;
	switch (uType = synGetNextLexemeClass())
	{
	case LEXCLASS_IA32REGISTERS_EAX:
	case LEXCLASS_IA32REGISTERS_EBX:
	case LEXCLASS_IA32REGISTERS_ECX:
	case LEXCLASS_IA32REGISTERS_EDX:
	case LEXCLASS_IA32REGISTERS_ESI:
	case LEXCLASS_IA32REGISTERS_EDI:
	case LEXCLASS_IA32REGISTERS_EBP:
	case LEXCLASS_IA32REGISTERS_ESP:
        return uType;
        break;
    default:
        synError();
		return FALSE;
		break;
	}
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermInstruction()
{
    UINT uType;
	switch (uType = synGetNextLexemeClass())
    {
	case LEXCLASS_IA32INSTRUCTIONS_MOV:
	case LEXCLASS_IA32INSTRUCTIONS_PUSH:
        return uType;
        break;
    default:
        synError();
        return FALSE;
        break;
    }
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// Sreg:[...
int synAddPrefixSregOEGroup(UINT uType)
{
	TOperationElemPtr pTempOE = NULL;
	
	void * pvValue;
	UINT uSemClass = 0;
	
	pTempOE = (TOperationElemPtr)malloc(sizeof(TOperationElem));
	
	pvValue = (int*)malloc(1);
	*((int *)pvValue) = semRetSregPrefixByType(uType);
	
	pTempOE->pNextOperationElem = pCurContrElem->pBegArgList;
	pCurContrElem->pBegArgList->pPrevOperationElem = pTempOE;
	pCurContrElem->pBegArgList = pTempOE;
	
	
	pTempOE->pArgument.uSemClass = SEMCLASS_PREFIX;
	pTempOE->pArgument.pvValue = pvValue;
	
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// Sreg
int synAddSregOEGroup(UINT uType)
{
	void * pvValue;
	UINT uSemClass = 0;
	
	pvValue = (int*)malloc(1);
	*((int *)pvValue) = semRetSregOpcodeByType(uType);
	uSemClass = SEMCLASS_SYSREG;
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
	
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
//   R16,
int synAddGPRegXXRegOpcodeOEGroup(UINT uType)
{
	void * pvValue;
	UINT uSemClass = 0;
	
	pvValue = (int*)malloc(1);
	*((int *)pvValue) = semRetRegRegisterOpcodeByType(uType);
	switch(semRetTypeSizeByType(uType))
	{
	case 4:
		uSemClass = SEMCLASS_REG32;
		break;
	case 2:
		uSemClass = SEMCLASS_REG16;
		break;
	case 1:
		uSemClass = SEMCLASS_REG8;
		break;
	default:
		uSemClass = SEMCLASS_UNKNOWN;	
		semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
		break;
	}

	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);	

	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
//   ,R16
int synAddGPRegXXEffAddrOEGroup(UINT uType)
{
	void * pvValue;
	UINT uSemClass = 0;
	
	pvValue = (int*)malloc(1);
	*((int *)pvValue) = semRetRegEffectivAddrByType(uType);
	switch(semRetTypeSizeByType(uType))
	{
	case 4:
		uSemClass = SEMCLASS_REG32;
		break;
	case 2:
		uSemClass = SEMCLASS_REG16;
		break;
	case 1:
		uSemClass = SEMCLASS_REG8;
		break;
	default:
		uSemClass = SEMCLASS_UNKNOWN;	
		semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
		break;
	}
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
	
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
//   RXX,
int synAddGPRegXXRWCodeOEGroup(UINT uType)
{
	void * pvValue;
	UINT uSemClass = 0;
	
	pvValue = (int*)malloc(1);
	*((int *)pvValue) = semRetRegRWByType(uType);
	switch(semRetTypeSizeByType(uType))
	{
	case 4:
		uSemClass = SEMCLASS_REG32;
		break;
	case 2:
		uSemClass = SEMCLASS_REG16;
		break;
	case 1:
		uSemClass = SEMCLASS_REG8;
		break;
	default:
		uSemClass = SEMCLASS_UNKNOWN;	
		semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
		break;
	}
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);	

	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
//  ,VarXX
int synAddVarXXOEGroup(UINT uType,	TRTItemPtr pBuff)
{

	void * pvValue;
	UINT uSemClass = 0;
	
	
	switch(semRetTypeSizeByType(uType))
	{
	case 4:
		uSemClass = SEMCLASS_VAR32;
		break;
	case 2:
		uSemClass = SEMCLASS_VAR16;
		break;
	case 1:
		uSemClass = SEMCLASS_VAR8;
		break;
	default:
		uSemClass = SEMCLASS_UNKNOWN;	
		semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
		break;
	}
	
	pvValue = malloc(sizeof(TIdTablePtr));
	pvValue = pBuff->RTItemContent.pIdTableElem;
	
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
	
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
//  ,0x0000
int synAddImmOEGroup(UINT uType)
{
	char * cValue;
	void * pvValue;
	UINT uSemClass = 0;
	
	switch(semRetTypeSizeByType(uType))
	{
	case 4:
		uSemClass = SEMCLASS_IMM32;
		break;
	case 2:
		uSemClass = SEMCLASS_IMM16;
		break;
	case 1:
		uSemClass = SEMCLASS_IMM8;
		break;
	default:
		uSemClass = SEMCLASS_UNKNOWN;	
		semError("immediate value",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"incorrect");
		break;
	}
		
	pvValue = (UINT *)malloc(sizeof(UINT));
	cValue = &pCurLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag[2];
	
	*((UINT*)pvValue) = strtol(cValue, NULL, 16);
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
	
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// MOV [Disp16],
int synAdd_Disp16_MemAddrOEGroup(UINT uType)
{
	char * cValue;
	void * pvValue;
	UINT uSemClass = 0;

	switch(semRetTypeSizeByType(uType))
	{
	case 4:
		//uSemClass = EA16MEMLOC_DISP32;
		semError("adrress",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,  
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"there is no 32-bit address mode support yet");
		break;
	case 2:
		uSemClass = EA16MEMLOC_DISP16;
		break;
	case 1:
		//uSemClass = EA16MEMLOC_DISP8;
		semError("adrress",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,  
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"must not be less, than 16-bit value");
		break;
	default:
		uSemClass = SEMCLASS_UNKNOWN;	
		semError("adrress",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,  
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"out of range");
		break;
	};
		
	pvValue = (UINT *)malloc(sizeof(UINT));
	cValue = &pCurLexListItem->pPrevLexemeInstance->LexemeInstance.cpTextFrag[2];
	
	*((UINT*)pvValue) = strtol(cValue, NULL, 16);
	
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);

	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// MOV [Base],
int synAdd_Base_MemAddrOEGroup(UINT uBaseType)
{
	UINT uType = 0;
	void * pvValue;
	UINT uSemClass = 0;
	
	pvValue = (int*)malloc(1);
	switch (uBaseType)
	{
	case LEXCLASS_IA32REGISTERS_BX:
		uSemClass = EA16MEMLOC_BX;
		break;
	case LEXCLASS_IA32REGISTERS_BP:
		uSemClass = EA16MEMLOCPART_BP;
		break;
	}
	
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);	
	
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// MOV [Index],
int synAdd_Index_MemAddrOEGroup(UINT uIndexType)
{
	UINT uType = 0;
	void * pvValue;
	UINT uSemClass = 0;
	
	pvValue = (int*)malloc(1);
	switch (uIndexType)
	{
	case LEXCLASS_IA32REGISTERS_SI:
		uSemClass = EA16MEMLOC_SI;
		break;
	case LEXCLASS_IA32REGISTERS_DI:
		uSemClass = EA16MEMLOC_DI;
		break;
	}
	synAddNewOperElemToContrElem(uSemClass, uType, pvValue,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
		pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);	
						
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// [Index+Disp16],
// [Index],
int synAdd_Index_Disp16_MemAddrOEGroup(UINT uType)
{
	UINT uSemClass = 0;
	UINT uIndexType = 0;

	uSemClass = SEMCLASS_REG16;
	uIndexType = pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLexClass;
	
	if (synIsNextTermLexeme(LEXCLASS_OPERATION_PLUS))
	{
		synCheckNextTermLexeme(LEXCLASS_OPERATION_PLUS);
		synAdd_Index_MemAddrOEGroup(uIndexType);		
		
		uType = synCheckNextNontermValue();
		synAdd_Disp16_MemAddrOEGroup(uType);
	}
	else
	{
		synAdd_Index_MemAddrOEGroup(uIndexType);
	}
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
// [Base+Index+Disp16],
// [Base+Index],
// [Base+Disp16],
// [Base],
int synAdd_Base_Index_Disp16_MemAddrOEGroup(UINT uType)
{
	UINT uSemClass = 0; 
	UINT uBaseType = 0;
	
	uSemClass = SEMCLASS_REG16;
	uBaseType = pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLexClass;
	if (synIsNextTermLexeme(LEXCLASS_OPERATION_PLUS))
	{
		synCheckNextTermLexeme(LEXCLASS_OPERATION_PLUS);
		
		synAdd_Base_MemAddrOEGroup(uBaseType);
		if (synIsNextNontermValue())
		{
			uType = synCheckNextNontermValue();
			synAdd_Disp16_MemAddrOEGroup(uType);
		};
		
		if (synIsNextNontermWordPointerRegister())
		{	
			uType = synCheckNextNontermWordPointerRegister();
			synAdd_Index_Disp16_MemAddrOEGroup(uType);
		};
	}
	else
	{
		if (uBaseType == LEXCLASS_IA32REGISTERS_BP )
		{
			semError("BP", pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						   pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
						   pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
						   "unacceptable use of base register in addressing arithmetic");
		}
		synAdd_Base_MemAddrOEGroup(uBaseType);
	}
	return uType;
}

/*
*	
*
*   @author Nail Sharipov
*/
int synAddMemoryLocationOEGroup()
{
	UINT uType;
	if (synIsNextNontermValue())
	{
		uType = synCheckNextNontermValue();
		synAdd_Disp16_MemAddrOEGroup(uType);
	};
	if (synIsNextNontermWordPointerRegister())
	{	
		
		uType = synCheckNextNontermWordPointerRegister();
		synAdd_Index_Disp16_MemAddrOEGroup(uType);
	}	
	if (synIsNextNontermWordBaseRegister())
	{	
		uType = synCheckNextNontermWordBaseRegister();
		synAdd_Base_Index_Disp16_MemAddrOEGroup(uType);
	}
	
	return uType;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAsmOperandGroup_SREG(UINT uType, TOperationElemPtr pOpcode)
{
	TRTItemPtr pBuff;
	UINT uSemClass = 0;

	synAddSregOEGroup(uType);
	synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA);
	
	// MOV Sreg, wordGPReg
	if(synIsNextNontermWordGPRegister())
	{
		uType = synCheckNextNontermWordGPRegister();
		synAddGPRegXXEffAddrOEGroup(uType);
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_Sreg_GPReg16;
		return TRUE;
	}
	
	if (synIsNextNontermByteGPRegister())
	{
		synCheckNextNontermByteGPRegister();
		semError("MOV",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,
			"operand size mismatch");
		return FALSE;
	}
	
	if (synIsNextNontermDWordGPRegister())
	{
		synCheckNextNontermDWordGPRegister();
		semError("MOV",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,
			"operand size mismatch");
		return FALSE;
	}
	// MOV Sreg, Mem16
	if (synIsNextTermLexeme(LEXCLASS_RT_IDENTIFIER))
	{
		synCheckNextTermLexeme(LEXCLASS_RT_IDENTIFIER);
		pBuff = synCheckIdInRT(pCurLexListItem->pPrevLexemeInstance);
		uSemClass = SEMCLASS_VAR16;
		
		uType = pBuff->RTItemContent.pIdTableElem->pArgInstance->pArgument.uLexClass;
		if (semRetTypeSizeByType(uType) !=4 )
		{
			synAddVarXXOEGroup(uType, pBuff);
			pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_Sreg_MEM16;
			return TRUE;
		}
		else
		{
			//if not MEM16
			semError("MOV",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,
				"operand size mismatch");
		}
	}
	
	return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAsmOperandGroup_MEMXX(UINT uType, TOperationElemPtr pOpcode)
{
	UINT uType2 = 0;
	UINT uSemClass = 0;

	synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_SQAURE_BRACKET);
	synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA);
				
	// MOV memXX, immXX
	if(synIsNextNontermValue())
	{
					
	uType2 = synCheckNextNontermValue();
	synAddImmOEGroup(uType2);
	switch(semRetTypeSizeByType(uType2))
	{
	case 4:
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_MEM32_IMM32;
		break;
	case 2:
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_MEM16_IMM16;
		break;
	case 1:
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_MEM8_IMM8;
		break;
	default:
		uSemClass = SEMCLASS_UNKNOWN;	
		semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
							pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
		
		break;
	}
	return TRUE;
	}
				
	// MOV memXX, GPRegXX
	if (synIsNextNontermByteGPRegister())
	{
		uType2 = synCheckNextNontermByteGPRegister();
		synAddGPRegXXRegOpcodeOEGroup(uType2);
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_MEM8_GPReg8;
		return TRUE;	
	}
				
				
	if(synIsNextNontermWordGPRegister())
	{
		uType2 = synCheckNextNontermWordGPRegister();
		synAddGPRegXXRegOpcodeOEGroup(uType2);
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_MEM16_GPReg16;
		return TRUE;
	}
				
	if (synIsNextNontermDWordGPRegister())
	{
		uType2 = synCheckNextNontermDWordGPRegister();
		synAddGPRegXXRegOpcodeOEGroup(uType2);
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_MEM32_GPReg32;
		return TRUE;
	}

	synError();
	return FALSE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAsmOperandGroup_IMMXX(TOperationElemPtr pOpcode)
{
	UINT uType2 = 0;
	UINT uSemClass = 0;
	TRTItemPtr pBuff;

	if(synIsNextNontermValue())
	{
		uType2 = synCheckNextNontermValue();
		
		synAddImmOEGroup(uType2);
		switch(semRetTypeSizeByType(uType2))
		{
		case 4:
			pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_IMM32;
			break;
		case 2:
			pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_IMM16;
			break;
		case 1:
			pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_IMM8;
			break;
		default:
			uSemClass = SEMCLASS_UNKNOWN;	
			semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
							pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
			break;
		}
	return TRUE;
	}
	return FALSE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAsmOperandGroup_PTR16_16(TOperationElemPtr pOpcode)
{
	UINT uType1 = 0, uType2 = 0;
	UINT uSemClass = 0;
	
	
	if(synIsNextNontermValue())
	{
		uType1 = synCheckNextNontermValue();
		
		synAddImmOEGroup(uType1);
		switch(semRetTypeSizeByType(uType1))
		{
		case 4:
			semError("PTR 16:16",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"immediate value to large for this addressing mode");
			break;
		case 2:
		case 1:
			break;
		default:
			uSemClass = SEMCLASS_UNKNOWN;	
			semError("PTR 16:16",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"incorrect immediate value");
			break;
		}

		synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COLON);

		uType2 = synCheckNextNontermValue();
		
		synAddImmOEGroup(uType2);
		switch(semRetTypeSizeByType(uType2))
		{
		case 4:
			semError("PTR 16:16",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"immediate value to large for this addressing mode");
			break;
		case 2:
		case 1:
			break;
		default:
			uSemClass = SEMCLASS_UNKNOWN;	
			semError("PTR 16:16",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"incorrect immediate value");
			break;
		}
		pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_PTR16_16;
		return TRUE;
	}
	return FALSE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAsmOperandGroup_REGXX(UINT uType, TOperationElemPtr pOpcode)
{
	UINT uType2 = 0;
	UINT uSemClass = 0;
	TRTItemPtr pBuff;

	if (synIsNextNontermWordGPRegister() || synIsNextNontermByteGPRegister() || synIsNextNontermDWordGPRegister())
	{
		
		if (synIsNextNontermDWordGPRegister())
			uType2 = synCheckNextNontermDWordGPRegister();				
		
		if (synIsNextNontermWordGPRegister())
			uType2 = synCheckNextNontermWordGPRegister();				
		
		if (synIsNextNontermByteGPRegister())
			uType2 = synCheckNextNontermByteGPRegister();				
		
		if ( semRetTypeSizeByType(uType2) != semRetTypeSizeByType(uType) )
			semError ("MOV",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,
			"register size mismatch");
		else
		{
			synAddGPRegXXRegOpcodeOEGroup(uType);
			synAddGPRegXXEffAddrOEGroup(uType2);
			
			switch(semRetTypeSizeByType(uType2))
			{
			case 4:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG32_REG32;
				break;
			case 2:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG16_REG16;
				break;
			case 1:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG8_REG8;
				break;
			default:
				uSemClass = SEMCLASS_UNKNOWN;	
				semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
					pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
					pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"unknown type");
				break;
			}
		
		}
		return TRUE;
	}
	
	// MOV Rxx, immX
	if(synIsNextNontermValue())
	{
		uType2 = synCheckNextNontermValue();
		
		if ( semRetTypeSizeByType(uType2) != semRetTypeSizeByType(uType) )
			semError ("MOV",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,
						"immediate value out of range");
		else
		{
			synAddGPRegXXRWCodeOEGroup(uType);
			synAddImmOEGroup(uType2);
			switch(semRetTypeSizeByType(uType2))
			{
			case 4:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG32_IMM32;
				break;
			case 2:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG16_IMM16;
				break;
			case 1:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG8_IMM8;
				break;
			default:
				uSemClass = SEMCLASS_UNKNOWN;	
				semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
				break;
			}
			return TRUE;
		}
	return TRUE;
	}

	// MOV Rxx, VARxx
	if (synIsNextTermLexeme(LEXCLASS_RT_IDENTIFIER))
	{
		synCheckNextTermLexeme(LEXCLASS_RT_IDENTIFIER);
		pBuff = synCheckIdInRT(pCurLexListItem->pPrevLexemeInstance);
		if (pBuff)
			uType2 = pBuff->RTItemContent.pIdTableElem->pArgInstance->pArgument.uLexClass;
		else
			return TRUE;
					
		if ( semRetTypeSizeByType(uType2) != semRetTypeSizeByType(uType) )
			semError ("MOV",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
						pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,
						"bad variable type for this register");
		else
		{
			synAddGPRegXXRegOpcodeOEGroup(uType);
			synAddVarXXOEGroup(uType2, pBuff);
			switch(semRetTypeSizeByType(uType2))
			{
			case 4:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG32_MEM32;
				break;
			case 2:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG16_MEM16;
				break;
			case 1:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG8_MEM8;
				break;
			default:
				uSemClass = SEMCLASS_UNKNOWN;	
				semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 0,"unknown type");
				break;
			}
			return TRUE;
		}	
	return TRUE;
	}
	
	if (synIsNextNontermSegmentRegister())
	{
		pOpcode = pCurContrElemArg;
		synAddGPRegXXRWCodeOEGroup(uType);
		uType2 = synCheckNextNontermSegmentRegister();
		if (synIsNextTermLexeme(LEXCLASS_PUNCTUATION_COLON)) // MOV RXX,Sreg:[...
		{
			synAddPrefixSregOEGroup(uType2);
			synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COLON);
			synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET);
			uType2 = synAddMemoryLocationOEGroup();
			synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_SQAURE_BRACKET);
						
			switch(semRetTypeSizeByType(uType))
			{
			case 4:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG32_MEM32;
				break;
			case 2:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG16_MEM16;
				break;
			case 1:
				pOpcode->pArgument.uSemClass = SEMCLASS_INSTRUCTION_REG8_MEM8;
				break;
			default:
				uSemClass = SEMCLASS_UNKNOWN;	
				semError("identifier",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"unknown type");
				break;
			}
			return TRUE;
		}
		else // MOV R16, Sreg
		{
			synAddSregOEGroup(uType);
			return TRUE;
		}
	}
	return TRUE;
}

/*
*	Checks non-terminal symbol "assembler block"
*
*   @author Nail Sharipov
*/
int synCheckNontermAsmBlock()
{
	TOperationElemPtr pOpcode;
	UINT uType = 0, uType2 = 0;
	UINT uSemClass = 0;
	TOperationElemPtr pTempOE = NULL;

	synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_CURLY_BRACKET);
	while (synIsNextNontermInstruction())
	{
		
		synAddNewContrElemToCurBlock(SEMCLASS_INSTRUCTION, &cgContrElemInstruction);
		
		/*
		*	MOV instruction
		*/
		if (synIsNextTermLexeme(LEXCLASS_IA32INSTRUCTIONS_MOV))
		{
			uType = synCheckNextTermLexeme(LEXCLASS_IA32INSTRUCTIONS_MOV);
			synAddNewOperElemToContrElem(SEMCLASS_UNKNOWN, uType, NULL,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
			pOpcode = pCurContrElemArg;
			
			if (synIsNextNontermSegmentRegister())
			{
				uType = synCheckNextNontermSegmentRegister();
				if (synIsNextTermLexeme(LEXCLASS_PUNCTUATION_COLON)) // MOV Sreg:0xXXXX,0xXX
				{
					pOpcode = pCurContrElemArg;
					synAddPrefixSregOEGroup(uType);
					synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COLON);
				}
				
				else // MOV Sreg
				{
					synAsmOperandGroup_SREG(uType, pOpcode);
				}
			}
			
			// MOV memXX,
			if (synIsNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET))
			{
				synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET);
				uType = synAddMemoryLocationOEGroup();
				synAsmOperandGroup_MEMXX(uType, pOpcode);
				continue;
			}

			// MOV RXX,
			if (synIsNextNontermWordGPRegister() || synIsNextNontermByteGPRegister() || synIsNextNontermDWordGPRegister())
			{
				
				if (synIsNextNontermDWordGPRegister())
				{	
					uSemClass = SEMCLASS_REG32;
					uType = synCheckNextNontermDWordGPRegister();				
				}	
				
				if (synIsNextNontermWordGPRegister())
				{	
					uSemClass = SEMCLASS_REG16;
					uType = synCheckNextNontermWordGPRegister();				
				}	
				
				if (synIsNextNontermByteGPRegister())
				{	
					uSemClass = SEMCLASS_REG8;
					uType = synCheckNextNontermByteGPRegister();				
				}	
				
				synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_COMMA);
				synAsmOperandGroup_REGXX(uType, pOpcode);
			}
		
			continue;
		} // *** END *** MOV instruction
		
		/*
		*	INT instruction
		*/
		if (synIsNextTermLexeme(LEXCLASS_IA32INSTRUCTIONS_INT))
		{
			uType = synCheckNextTermLexeme(LEXCLASS_IA32INSTRUCTIONS_INT);
			synAddNewOperElemToContrElem(SEMCLASS_UNKNOWN, uType, NULL,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
			pOpcode = pCurContrElemArg;
			
			synAsmOperandGroup_IMMXX(pOpcode);
			
			if (pOpcode->pArgument.uSemClass != SEMCLASS_INSTRUCTION_IMM8)
				semError("INT",pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
								pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum,"constant is too large");
			continue;
		}

		/*
		*	JMP instruction
		*/
		if (synIsNextTermLexeme(LEXCLASS_IA32INSTRUCTIONS_JMP))
		{
			uType = synCheckNextTermLexeme(LEXCLASS_IA32INSTRUCTIONS_JMP);
			synAddNewOperElemToContrElem(SEMCLASS_UNKNOWN, uType, NULL,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
			pOpcode = pCurContrElemArg;
			
			synAsmOperandGroup_PTR16_16(pOpcode);
						
			continue;
		}
	}

	synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_CURLY_BRACKET);
	return FALSE;
};

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermImplementation()
{
	TContrElemPtr pCurCEContainer = NULL, pNewContrElem = NULL;
	TOperationElemPtr pCurCEArgContainer = NULL;
	UINT uType = 0;
    switch (synRetNextLCValue())
    {
    case LEXCLASS_BLOCK_LEFT_CURLY_BRACKET:
		synAddBlockContrElem();
		break;
	case LEXCLASS_PROGRAMM_ASM_ENTRY:
		synCheckNextTermLexeme(LEXCLASS_PROGRAMM_ASM_ENTRY);
		synCheckNontermAsmBlock();
		break;
    case LEXCLASS_PROGRAMM_RETURN_POINT:
		synCheckNextTermLexeme(LEXCLASS_PROGRAMM_RETURN_POINT);
////////////////////////////////////////////////////////////////////////////
		synAddNewContrElemToCurBlock(SEMCLASS_FUNCRETURN, &cgContrElemFuncRet);
		uType = pCurParseListNode->FuncNode.uType;
		synAddNewOperElemToContrElem(SEMCLASS_FUNCRETURN, uType, pCurParseListNode,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
		synAddReturnContrElem();
//////////////////////////////////////////////////////////////////////////
//		synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_SEMICOLUMN);
		break;
	case LEXCLASS_CONSTRUCTIONS_IF:
		synCheckNextTermLexeme(LEXCLASS_CONSTRUCTIONS_IF);
		synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_ROUND_BRACKET);
		synAddNewContrElemToCurBlock(SEMCLASS_CONSTRUCTION_IF, &cgContrElemConstructionIf);
		pNewContrElem = synCreateNewContrElem(&cgContrElemEquation);
		synAddNewOperElemToContrElem(SEMCLASS_EQUATION, uType, pNewContrElem,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);

		pCurCEContainer = pCurContrElem;
		pCurCEArgContainer = pCurContrElemArg;
		
		pCurContrElem = pNewContrElem;
		pCurContrElemArg = NULL;
		
		synCheckNextNontermEquation();
		
		pCurContrElem = pCurCEContainer;
		pCurContrElemArg = pCurCEArgContainer;
		
		synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET);
		
		pCurCEContainer = pCurContrElem;
		pCurCEArgContainer = pCurContrElemArg;
		
		pNewContrElem = synAddNewBlockContrElemToCurContrElem();
		
		pCurContrElem = pCurCEContainer;
		pCurContrElemArg = pCurCEArgContainer;

		synAddNewOperElemToContrElem(SEMCLASS_BLOCK, uType, pNewContrElem,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);

		if (synIsNextTermLexeme(LEXCLASS_CONSTRUCTIONS_ELSE))
		{			
			synCheckNextTermLexeme(LEXCLASS_CONSTRUCTIONS_ELSE);	
			
			pCurCEContainer = pCurContrElem;
			pCurCEArgContainer = pCurContrElemArg;
			
			pNewContrElem = synAddNewBlockContrElemToCurContrElem();
			
			pCurContrElem = pCurCEContainer;
			pCurContrElemArg = pCurCEArgContainer;
			
			synAddNewOperElemToContrElem(SEMCLASS_CONSTRUCTION_ELSE, uType, pNewContrElem,
				pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
		}
		break;
	case LEXCLASS_CONSTRUCTIONS_WHILE:
		synCheckNextTermLexeme(LEXCLASS_CONSTRUCTIONS_WHILE);
		synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_ROUND_BRACKET);
		
		synAddNewContrElemToCurBlock(SEMCLASS_CONSTRUCTION_WHILE, &cgContrElemConstructionWhile);
		
		pNewContrElem = synCreateNewContrElem(&cgContrElemEquation);
		
		synAddNewOperElemToContrElem(SEMCLASS_EQUATION, uType, pNewContrElem,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd, 
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);

		pCurCEContainer = pCurContrElem;
		pCurCEArgContainer = pCurContrElemArg;
		
		pCurContrElem = pNewContrElem;
		pCurContrElemArg = NULL;
		
		synCheckNextNontermEquation();
		
		pCurContrElem = pCurCEContainer;
		pCurContrElemArg = pCurCEArgContainer;

		synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET);

		pCurCEContainer = pCurContrElem;
		pCurCEArgContainer = pCurContrElemArg;

		pNewContrElem = synAddNewBlockContrElemToCurContrElem();

		pCurContrElem = pCurCEContainer;
		pCurContrElemArg = pCurCEArgContainer;

		synAddNewOperElemToContrElem(SEMCLASS_BLOCK, uType, pNewContrElem,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosBeg,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uPosEnd,
			pCurLexListItem->pPrevLexemeInstance->LexemeInstance.uLineNum, NULL);
		break;
	default:
		synAddNewContrElemToCurBlock(SEMCLASS_EQUATION, &cgContrElemEquation);
		synCheckNextNontermEquation();
		synCheckNextTermLexeme(LEXCLASS_PUNCTUATION_SEMICOLUMN);
        break;
    };
    return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synCheckNextNontermBlock()
{
	synFillRT();
	synAddIdentTableElem(SYNCLASS_ACCESSORIES_BEGINBLOCK, 0,0,0,NULL, pCurIdTableBegBlock, NULL, NULL, NULL);
    synCheckNextTermLexeme(LEXCLASS_BLOCK_LEFT_CURLY_BRACKET);
    
    if ( synIsNextNontermType() )
    {
        synCheckNextNontermVarDecl();
    };
    
	synFillRT();
	
 	if ( !synIsNextTermLexeme(LEXCLASS_BLOCK_RIGHT_CURLY_BRACKET) )
    {
        while (!synIsNextTermLexeme(LEXCLASS_BLOCK_RIGHT_CURLY_BRACKET) && pCurLexListItem)
			synCheckNextNontermImplementation();
	};

	synUnFillRT();
  	synAddIdentTableElem(SYNCLASS_ACCESSORIES_ENDBLOCK, 0,0,0, NULL, pCurIdTableBegBlock,NULL, NULL, NULL);
 
    synCheckNextTermLexeme(LEXCLASS_BLOCK_RIGHT_CURLY_BRACKET);
    
    return TRUE;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int synAnalysis()
{ 
	UINT uType;
    TRTItemPtr pBuff;
	UINT uLexClass;
    extern TIdTablePtr pCurIdTableBegBlock;

	pCurLexListItem = pBegLexList;
    while (pCurLexListItem)
    {
        uLexClass = synRetNextLCValue();
        switch(uLexClass)
        {
        case LEXCLASS_TYPE_VOID:
        case LEXCLASS_TYPE_SHORT:
		case LEXCLASS_TYPE_CHAR:
            synCheckNextNontermFuncDecl();
            synAddNewBlockToCurFuncNode();
            synCheckNextNontermBlock();
			
			synUnFillRT();
			synAddIdentTableElem(SYNCLASS_ACCESSORIES_ENDBLOCK, 0,0,0, NULL, pCurIdTableBegBlock,NULL, NULL, NULL);
			//lexPrintRT();
			
			break;
        
        default:
            uLexClass = synGetNextLexemeClass();
            synError();
            break;
        }
    }
    synAddIdentTableElem(SYNCLASS_ACCESSORIES_ENDBLOCK, 0,0,0, NULL, pCurIdTableBegBlock,NULL, NULL, NULL);
	return TRUE;
};

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
void synPrintIdTable()
{
    extern TIdTable firstIdTableElem;
    TIdTablePtr     pTemp, pIdTableElem = &firstIdTableElem;
    FILE * pOutInfoFile;
	
    pOutInfoFile = fopen("CompileInfo.txt","a");
    
    printf("\n");
    fprintf(pOutInfoFile, "\n");


	printf(" *** IDENTIFIER TABLE *** \n\n");
    fprintf(pOutInfoFile, " *** IDENTIFIER TABLE *** \n\n");
	while (pIdTableElem)
    {
        if (pIdTableElem->pArgInstance)
		printf("  %d  --- %s :   %d", pIdTableElem->uType, 
			
			(pIdTableElem->uType == SYNCLASS_ACCESSORIES_BEGINBLOCK)?
			"BEGIN BLOCK":((pIdTableElem->uType == SYNCLASS_ACCESSORIES_ENDBLOCK)?
			"END BLOCK":pIdTableElem->pItemContent->cpIdName),
			
			pIdTableElem->pArgInstance->pArgument.uSemClass);
		else
		printf("  %d  --- %s ", pIdTableElem->uType, 
			(pIdTableElem->uType == SYNCLASS_ACCESSORIES_BEGINBLOCK)?
			"BEGIN BLOCK":((pIdTableElem->uType == SYNCLASS_ACCESSORIES_ENDBLOCK)?
			"END BLOCK":pIdTableElem->pItemContent->cpIdName));
        fprintf(pOutInfoFile, "  %d  --- %s ", pIdTableElem->uType, 
			(pIdTableElem->uType == SYNCLASS_ACCESSORIES_BEGINBLOCK)?
			"BEGIN BLOCK":((pIdTableElem->uType == SYNCLASS_ACCESSORIES_ENDBLOCK)?
			"END BLOCK":pIdTableElem->pItemContent->cpIdName));		
		
		if ((pIdTableElem->pPrevTie) && (pIdTableElem->uType == SYNCLASS_ACCESSORIES_BEGINBLOCK))
		{
			pTemp = pIdTableElem->pPrevTie->pNextListItem;
			printf(" ( ");
			fprintf(pOutInfoFile, " ( ");
			while (pTemp->pItemContent)
			{
				printf(" %d, ", pTemp->uType);
				fprintf(pOutInfoFile, " %d, ", pTemp->uType);
				pTemp = pTemp->pNextListItem;
			}
			printf(" ) \n");
			fprintf(pOutInfoFile, " ) \n");
		}
		else
		if (pIdTableElem->uType == SYNCLASS_ACCESSORIES_ENDBLOCK)
		{
			pTemp = pIdTableElem->pPrevTie->pNextListItem;
			printf(" ( ");
			fprintf(pOutInfoFile, " ( ");
			while (pTemp->pItemContent)
			{
				printf(" %d, ", pTemp->uType);
				fprintf(pOutInfoFile, " %d, ", pTemp->uType);
				pTemp = pTemp->pNextListItem;
			}
			printf(" ) \n");
			fprintf(pOutInfoFile, " ) \n");
		}
		else
		{
			printf("\n");
			fprintf(pOutInfoFile, "\n");
		}
		pIdTableElem = pIdTableElem->pNextListItem;
    }
	fclose(pOutInfoFile);
}
