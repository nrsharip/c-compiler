#include "analysis_lex.h"
#include "analysis_syn.h"
#include "analysis_sem.h"
#include "analysis_cg.h"

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int semRetTypeSizeByType(UINT uType)
{
	switch	(uType)
	{
	case LEXCLASS_TYPE_INT:
	case LEXCLASS_IA32REGISTERS_EAX:	
	case LEXCLASS_IA32REGISTERS_EBX:
	case LEXCLASS_IA32REGISTERS_ECX:
	case LEXCLASS_IA32REGISTERS_EDX:
	case LEXCLASS_IA32REGISTERS_ESI:
	case LEXCLASS_IA32REGISTERS_EDI:
	case LEXCLASS_IA32REGISTERS_ESP:
	case LEXCLASS_IA32REGISTERS_EBP:
		return 4;
		break;
	case LEXCLASS_TYPE_SHORT:
	case LEXCLASS_IA32REGISTERS_AX:
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_CX:
	case LEXCLASS_IA32REGISTERS_DX:
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DI:
	case LEXCLASS_IA32REGISTERS_BP:
	case LEXCLASS_IA32REGISTERS_SP:
		return 2;
		break;
	case LEXCLASS_TYPE_CHAR:	// there are some problems with char because of 
		// impossibility 8-bit value from mem16 to push into stack 
	case LEXCLASS_IA32REGISTERS_AH:
	case LEXCLASS_IA32REGISTERS_AL:
	case LEXCLASS_IA32REGISTERS_BH:
	case LEXCLASS_IA32REGISTERS_BL:
	case LEXCLASS_IA32REGISTERS_CH:
	case LEXCLASS_IA32REGISTERS_CL:
	case LEXCLASS_IA32REGISTERS_DH:
	case LEXCLASS_IA32REGISTERS_DL:		
		return 1; 
		break;
	case LEXCLASS_TYPE_VOID:
		return 0;
		break;
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
void semError(char * cpTextFrag, UINT uPosBeg,UINT uPosEnd, UINT uLineNum, char * cpErrMess)
{
	extern TIdErrorPtr pIdErrorBegList;
	extern TIdErrorPtr pIdError;
	TIdErrorPtr pNewIdError;
	
	pNewIdError = (TIdErrorPtr)malloc(sizeof(TIdError));
	pNewIdError->cpErrorMess = (char *)malloc(strlen(cpErrMess)+1);
	strcpy(pNewIdError->cpErrorMess, cpErrMess);
	pNewIdError->ErrorInstance.cpTextFrag = (char *)malloc(strlen(cpTextFrag)+1);
	strcpy(pNewIdError->ErrorInstance.cpTextFrag, cpTextFrag);
	pNewIdError->ErrorInstance.uPosBeg = uPosBeg;
	pNewIdError->ErrorInstance.uPosEnd = uPosEnd;
	pNewIdError->ErrorInstance.uLineNum = uLineNum;
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
int semCheckContrElemFuncRet(TOperationElemPtr pBegArgList)
{
	TContrElemPtr	  pTempCE;
	/*	TODO: uncomment this errors checking - VERY IMPORTANT WARNINGS!!! */
// 	if (pBegArgList->pArgument.uLexClass != pBegArgList->pNextOperationElem->pArgument.uLexClass)
// 	{
// 		semError ("return", pBegArgList->pArgument.uBegPos, pBegArgList->pArgument.uEndPos, 
// 			pBegArgList->pArgument.uStrNum,"function and returning value type mismatch");
// 	}
	pTempCE = (TContrElemPtr)pBegArgList->pNextOperationElem->pArgument.pvValue;
	semCheckContrElemEquation(pTempCE->pBegArgList);
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
int semCheckContrElemFuncCall(TOperationElemPtr pBegArgList)
{
	TOperationElemPtr pOEInDecl,pOEInCurFunc;
	TOperationElemPtr pBegCurFuncDeclArgList, pBegCurFuncArgList;
	TContrElemPtr	  pTempCE;	
	TIdTablePtr		  pTempIdTableElem;
	
	pTempIdTableElem = (TIdTablePtr)pBegArgList->pArgument.pvValue;
	pBegCurFuncDeclArgList = pTempIdTableElem->pArgInstance;
	pBegCurFuncArgList = pBegArgList->pNextOperationElem;
	pOEInCurFunc = pBegCurFuncArgList;
	pOEInDecl = pBegCurFuncDeclArgList;
	// pBegCurFuncDeclArgList - pointer to the beginning of the argument list of current function declaration
	// pBegCurFuncArgList - pointer to the beginning of the argument list of current function
				
	
	while(pOEInCurFunc)
	{
		pTempCE = (TContrElemPtr)(pOEInCurFunc->pArgument.pvValue);
		
		semCheckContrElemEquation(pTempCE->pBegArgList);
		
		/*	TODO: uncomment this errors checking - VERY IMPORTANT WARNINGS!!! */
// 		if (pOEInDecl && pOEInDecl->pArgument.uLexClass != pOEInCurFunc->pArgument.uLexClass)
// 		{
// 			semError(pTempIdTableElem->pItemContent->cpIdName, pOEInCurFunc->pArgument.uBegPos,pOEInCurFunc->pArgument.uEndPos, 
// 				pOEInCurFunc->pArgument.uStrNum, "formal and actual argument type mismatch");
// 		}
		if ( pOEInDecl && !pOEInCurFunc->pNextOperationElem && pOEInDecl->pNextOperationElem )
		{
			semError(pTempIdTableElem->pItemContent->cpIdName, pOEInCurFunc->pArgument.uBegPos,pOEInCurFunc->pArgument.uEndPos, 
				pOEInCurFunc->pArgument.uStrNum, "too few actual parameters");
			break;
		}
		if ( !pOEInDecl || pOEInCurFunc->pNextOperationElem && !pOEInDecl->pNextOperationElem )
		{
			semError(pTempIdTableElem->pItemContent->cpIdName, pOEInCurFunc->pArgument.uBegPos,pOEInCurFunc->pArgument.uEndPos, 
				pOEInCurFunc->pArgument.uStrNum, "too many actual parameters");
			break;
		}
		pOEInCurFunc = pOEInCurFunc->pNextOperationElem;
		pOEInDecl = pOEInDecl->pNextOperationElem;
		
	}
	if ( !pOEInCurFunc && pOEInDecl)
	{
		semError(pTempIdTableElem->pItemContent->cpIdName, pTempIdTableElem->uPosBeg,
			pTempIdTableElem->uPosEnd, 
			pTempIdTableElem->uLineNum, "too few actual parameters");
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
int semRetOperPriorityByLexClass(int uClass)
{
	switch(uClass)
	{
	case LEXCLASS_OPERATION_ASTERISK:
	case LEXCLASS_OPERATION_SLASH:
		return SEMOPERPRIORITY_MUL;
		break;
	case LEXCLASS_OPERATION_PLUS:
	case LEXCLASS_OPERATION_MINUS:
		return SEMOPERPRIORITY_PLUS;
		break;
	case LEXCLASS_COMPARISON_EQUAL:
		return SEMOPERPRIORITY_EQUAL;
		break;
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
int semCheckContrElemExpression(TContrElemPtr pContrElem)
{
	TIDCurStatePtr		pTempCS;
	TOperationElemPtr	pContrElemArg;
	TContrElemPtr		pTempCE;
	TOperationElemPtr	pTempStack = NULL;
	TOperationElemPtr	pNewArg = NULL;
	TOperationElemPtr	pCurArg = NULL;
	TOperationElemPtr	pBegNewList = NULL;
	TIdTablePtr			pTempIdElem;
	TIDCurStatePtr		pTempIdCurState;
	TAAOperElemListPtr		pTempAAE;

	//reverse Polish notation
	pContrElemArg = pContrElem->pBegArgList;

	while (pContrElemArg)
	{
		if (pContrElemArg->pArgument.uSemClass != SEMCLASS_OPERATION)
		{
			if (pContrElemArg->pArgument.uSemClass == SEMCLASS_FUNCTION)
			{
				pTempCE = (TContrElemPtr)pContrElemArg->pArgument.pvValue;
				semCheckContrElemFuncCall(pTempCE->pBegArgList);
			}
			
			pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
			*pNewArg = *pContrElemArg;
			if (pCurArg)
			{
				pCurArg->pNextOperationElem = pNewArg;
				pNewArg->pPrevOperationElem = pCurArg;
				pCurArg = pNewArg;
			}

			if (!pBegNewList)
			{
				pBegNewList = pNewArg;
				pCurArg = pNewArg;
			}
		}
		else
		{
			if (!pTempStack)
			{
				pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
				*pNewArg = *pContrElemArg;
				pNewArg->pPrevOperationElem = NULL;
				pNewArg->pNextOperationElem = NULL;
				pTempStack = pNewArg;
			}
			else
			{
				if (pContrElemArg->pArgument.uLexClass == LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET)
				{
					while ( pTempStack->pArgument.uLexClass != LEXCLASS_BLOCK_LEFT_ROUND_BRACKET)
					{
						pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
						*pNewArg = *pTempStack;
						pCurArg->pNextOperationElem = pNewArg;
						pNewArg->pPrevOperationElem = pCurArg;
						pNewArg->pNextOperationElem = NULL;
						pCurArg = pNewArg;
						pTempStack = pTempStack->pPrevOperationElem;
						free(pTempStack->pNextOperationElem);
					}
					if(pTempStack->pPrevOperationElem)
					{
						pTempStack = pTempStack->pPrevOperationElem;
						free(pTempStack->pNextOperationElem);
						pTempStack->pNextOperationElem = NULL;
					}
					else
					{
						free(pTempStack);
						pTempStack = NULL;
					}
				}
				else if (pContrElemArg->pArgument.uLexClass == LEXCLASS_BLOCK_LEFT_ROUND_BRACKET)
				{
					if (pTempStack)
					{
						pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
						*pNewArg = *pContrElemArg;
						pTempStack->pNextOperationElem = pNewArg;
						pNewArg->pPrevOperationElem = pTempStack;
						pNewArg->pNextOperationElem = NULL;
						pTempStack = pNewArg;
					}
					else
					{
						pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
						*pNewArg = *pContrElemArg;
						pNewArg->pPrevOperationElem = NULL;
						pNewArg->pNextOperationElem = NULL;
						pTempStack = pNewArg;
					}
				}
				else
				{
					while ( pTempStack && (semRetOperPriorityByLexClass(pContrElemArg->pArgument.uLexClass) < semRetOperPriorityByLexClass(pTempStack->pArgument.uLexClass)))
					{
						pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
						*pNewArg = *pTempStack;
						pCurArg->pNextOperationElem = pNewArg;
						pNewArg->pPrevOperationElem = pCurArg;
						pNewArg->pNextOperationElem = NULL;
						pCurArg = pNewArg;
						pTempStack = pTempStack->pPrevOperationElem;
					}
					
					if (pTempStack)
					{
						pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
						*pNewArg = *pContrElemArg;
						pTempStack->pNextOperationElem = pNewArg;
						pNewArg->pPrevOperationElem = pTempStack;
						pNewArg->pNextOperationElem = NULL;
						pTempStack = pNewArg;
					}
					else
					{
						pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
						*pNewArg = *pContrElemArg;
						pNewArg->pPrevOperationElem = NULL;
						pNewArg->pNextOperationElem = NULL;
						pTempStack = pNewArg;
					}
				}			
			}		
		}
		pContrElemArg = pContrElemArg->pNextOperationElem;
	}
	
	while (pTempStack)
	{
		pNewArg = (TOperationElemPtr)malloc(sizeof(TOperationElem));
		*pNewArg = *pTempStack;
					
		pCurArg->pNextOperationElem = pNewArg;
		pNewArg->pPrevOperationElem = pCurArg;
		pNewArg->pNextOperationElem = NULL;
		pCurArg = pNewArg;
		pTempStack = pTempStack->pPrevOperationElem;
	}
	
	pContrElem->pBegArgList = pBegNewList;
	pContrElemArg = pContrElem->pBegArgList;

	while (pContrElemArg)
	{
		if ( 
			 (pContrElemArg->pArgument.uSemClass != SEMCLASS_OPERATION) && 
			 (pContrElemArg->pArgument.uSemClass != SEMCLASS_FUNCTION)  &&
			 (pContrElemArg->pArgument.uSemClass != SEMCLASS_IMM8) &&
			 (pContrElemArg->pArgument.uSemClass != SEMCLASS_IMM16) &&
			 (pContrElemArg->pArgument.uSemClass != SEMCLASS_IMM32)	 
			 )
		{
			pTempIdCurState = (TIDCurStatePtr)pContrElemArg->pArgument.pvValue;
			pTempIdElem = pTempIdCurState->pIdTableElem;
		}
		pContrElemArg = pContrElemArg->pNextOperationElem;
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
int semCheckContrElemEquation(TOperationElemPtr pBegArgList)
{
	TIDCurStateListPtr	pTempCurStateListElem = NULL, pTempCurStateBegList = NULL, pEIContainer = NULL;
	TIdTablePtr			pTempIdTableElem = NULL, pExprIdTableElem = NULL;
	TAAOperElemListPtr		pDeclTempAAE = NULL, pTempAAE = NULL, pExprAAE = NULL;
	TOperationElemPtr	pContrElemArg;
	TContrElemPtr		pTempCE;
	UINT				uBaseSubLvl = 0, uTempSubLvl = 0, uExprIndLevel = 0, uTempIndLevel = 0;

	pContrElemArg = pBegArgList;
	while (pContrElemArg->pNextOperationElem)
		pContrElemArg = pContrElemArg->pNextOperationElem;
	
	if (pContrElemArg)
	{
		pTempCE = (TContrElemPtr)pContrElemArg->pArgument.pvValue;
		semCheckContrElemExpression(pTempCE);
		pTempCurStateBegList = pContrElemArg->pArgument.pIDCurState;
	}
	pTempCurStateListElem = pTempCurStateBegList;
	if (pTempCurStateListElem)
	{
		pTempAAE = pTempCurStateListElem->pCurIdState->pIdTableElem->pBegIdDecl;
		while(pTempAAE)
		{
			switch(pTempAAE->AAOperElem.uDeclType)
			{
			case SYNDECLELEMTYPE_SQUARE_BRACKETS:
				uExprIndLevel++;
				break;
			case SYNDECLELEMTYPE_ASTERISK:
				uExprIndLevel = uExprIndLevel + *((UINT *)pTempAAE->AAOperElem.pvValue);
			}
			pTempAAE = pTempAAE->pNextAAElem;
		}
	}
	if (pTempCurStateBegList)
		pTempCurStateBegList->pCurIdState->uTotalIndLevelCnt = uExprIndLevel;

	pTempCurStateListElem = pTempCurStateBegList;
	if (pTempCurStateListElem)
	{
		pTempAAE = pTempCurStateListElem->pCurIdState->pIdTableElem->pBegIdDecl;
		pTempCurStateListElem = pTempCurStateListElem->pNextCurStateElem;
		while (pTempCurStateListElem)
		{
			if (pTempCurStateListElem->pCurIdState->uStateType == SYNIDCURSTATETYPE_INDEX_DEREFERENCING)
			{
				pTempCE = pTempCurStateListElem->pCurIdState->pvValue;
				semCheckContrElemEquation(pTempCE->pBegArgList);
			}
			pTempCurStateListElem = pTempCurStateListElem->pNextCurStateElem;
		}
		uTempIndLevel = 0;
		if(pTempCurStateListElem)
		while (pTempAAE && (pTempAAE != pTempCurStateListElem->pCurIdState->pCurAAElem))
		{
			switch(pTempAAE->AAOperElem.uDeclType)
			{
			case SYNDECLELEMTYPE_SQUARE_BRACKETS:
				uTempIndLevel++;
				break;
			case SYNDECLELEMTYPE_ASTERISK:
				uTempIndLevel = uTempIndLevel + *((UINT *)pTempAAE->AAOperElem.pvValue);
				break;
			}
					
			pTempAAE = pTempAAE->pNextAAElem;
		}
		if(pTempCurStateListElem)
		switch(pTempCurStateListElem->pCurIdState->uStateType)
		{
		case SYNIDCURSTATETYPE_INDEX_DEREFERENCING:
			uTempIndLevel++;
			break;
		case SYNIDCURSTATETYPE_DEREFERENCING:
			uTempIndLevel += *((UINT *)pTempAAE->AAOperElem.pvValue) - *((UINT *)pTempCurStateListElem->pCurIdState->pvValue);
			break;
		}
	}
	uBaseSubLvl = uExprIndLevel - uTempIndLevel;
	pContrElemArg = pContrElemArg->pPrevOperationElem;

	while (pContrElemArg)
	{
		pTempCE = (TContrElemPtr)pContrElemArg->pArgument.pvValue;
		semCheckContrElemExpression(pTempCE);
		
		uExprIndLevel = 0;
		uTempIndLevel = 0;
		pTempCurStateBegList = pContrElemArg->pArgument.pIDCurState;
		
		pTempCurStateListElem = pTempCurStateBegList;
		
		if (pTempCurStateListElem)
		{
			pTempAAE = pTempCurStateListElem->pCurIdState->pIdTableElem->pBegIdDecl;
			while(pTempAAE)
			{
				switch(pTempAAE->AAOperElem.uDeclType)
				{
				case SYNDECLELEMTYPE_SQUARE_BRACKETS:
					uExprIndLevel++;
					break;
				case SYNDECLELEMTYPE_ASTERISK:
					uExprIndLevel = uExprIndLevel + *((UINT *)pTempAAE->AAOperElem.pvValue);
				}
			pTempAAE = pTempAAE->pNextAAElem;
			}
		}
		
		if (pTempCurStateBegList)
			pTempCurStateBegList->pCurIdState->uTotalIndLevelCnt = uExprIndLevel;

		pTempCurStateListElem = pTempCurStateBegList;
		

		
		if (pTempCurStateListElem)
		{
			pTempAAE = pTempCurStateListElem->pCurIdState->pIdTableElem->pBegIdDecl;
			
			while (pTempCurStateListElem->pNextCurStateElem)
			{
				pTempCurStateListElem = pTempCurStateListElem->pNextCurStateElem;
				if (pTempCurStateListElem->pCurIdState->uStateType == SYNIDCURSTATETYPE_INDEX_DEREFERENCING)
				{
					pTempCE = pTempCurStateListElem->pCurIdState->pvValue;
					semCheckContrElemEquation(pTempCE->pBegArgList);
				}
						
			}
			
			uTempIndLevel = 0;
			
			
			if(pTempCurStateListElem)
			while (pTempAAE && (pTempAAE != pTempCurStateListElem->pCurIdState->pCurAAElem))
			{
				switch(pTempAAE->AAOperElem.uDeclType)
				{
				case SYNDECLELEMTYPE_SQUARE_BRACKETS:
					uTempIndLevel++;
					break;
				case SYNDECLELEMTYPE_ASTERISK:
					uTempIndLevel = uTempIndLevel + *((UINT *)pTempAAE->AAOperElem.pvValue);
					break;
				}
				pTempAAE = pTempAAE->pNextAAElem;
			}

			if(pTempCurStateListElem)	
			switch(pTempCurStateListElem->pCurIdState->uStateType)
			{
			case SYNIDCURSTATETYPE_INDEX_DEREFERENCING:
				uTempIndLevel++;
				break;
			case SYNIDCURSTATETYPE_DEREFERENCING:
				uTempIndLevel += *((UINT *)pTempAAE->AAOperElem.pvValue) - *((UINT *)pTempCurStateListElem->pCurIdState->pvValue);
				break;
			}
			
			while(pTempAAE && pTempAAE->pNextAAElem)
				pTempAAE = pTempAAE->pNextAAElem;
		}
		uTempSubLvl = uExprIndLevel - uTempIndLevel;
			

	/*	TODO: uncomment this errors checking - VERY IMPORTANT WARNINGS!!! */

	// 	if ((uTempSubLvl != uBaseSubLvl))
	// 		semError("equation", pContrElemArg->pArgument.uBegPos, pContrElemArg->pArgument.uEndPos, pContrElemArg->pArgument.uStrNum, "difference in levels of indirection");
	// 	if (!(uTempSubLvl>0 && pContrElemArg->pNextOperationElem->pArgument.uSemClass == SEMCLASS_IMM16) )
	// 	if (!(uTempSubLvl>0 && pContrElemArg->pNextOperationElem->pArgument.uSemClass == SEMCLASS_MEM16) )
	// 	if (pContrElemArg->pArgument.uLexClass != pContrElemArg->pNextOperationElem->pArgument.uLexClass)
	// 		semError("equation", pContrElemArg->pArgument.uBegPos, pContrElemArg->pArgument.uEndPos, pContrElemArg->pArgument.uStrNum, "type mismatch");
		
		
		if ((pContrElemArg->pArgument.uSemClass != SEMCLASS_MEM16) || (pTempCurStateListElem && pTempCurStateListElem->pCurIdState->uStateType == SYNIDCURSTATETYPE_OPERATION))
			semError("=", pContrElemArg->pArgument.uBegPos, pContrElemArg->pArgument.uEndPos, pContrElemArg->pArgument.uStrNum, "left operand must be l-value");	
			
		pContrElemArg = pContrElemArg->pPrevOperationElem;
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
int semRetValueType(UINT uValueLength)
{
	return (uValueLength > 8)?LEXCLASS_UNKNOWN:((
		uValueLength > 4)?LEXCLASS_TYPE_INT:((
		uValueLength > 2)?LEXCLASS_TYPE_SHORT:((
		uValueLength > 0)?LEXCLASS_TYPE_CHAR:LEXCLASS_UNKNOWN)));
};

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int semRetSregOpcodeByType(UINT uType)
{
	switch (uType)
	{
	case LEXCLASS_IA32REGISTERS_CS:
		return SREG_CS_1;
		break;
	case LEXCLASS_IA32REGISTERS_ES:
		return SREG_ES_0;
		break;
	case LEXCLASS_IA32REGISTERS_FS:
		return SREG_FS_4;
		break;
	case LEXCLASS_IA32REGISTERS_GS:
		return SREG_GS_5;
		break;
	case LEXCLASS_IA32REGISTERS_SS:
		return SREG_SS_2;
		break;
	case LEXCLASS_IA32REGISTERS_DS:
		return SREG_DS_3;
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
int semRetSregPrefixByType(UINT uType)
{
	switch (uType)
	{
	case LEXCLASS_IA32REGISTERS_CS:
		return prefCS;
		break;
	case LEXCLASS_IA32REGISTERS_ES:
		return prefES;
		break;
	case LEXCLASS_IA32REGISTERS_FS:
		return prefFS;
		break;
	case LEXCLASS_IA32REGISTERS_GS:
		return prefGS;
		break;
	case LEXCLASS_IA32REGISTERS_SS:
		return prefSS;
		break;
	case LEXCLASS_IA32REGISTERS_DS:
		return prefDS;
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
int semRetRegRegisterOpcodeByType(UINT uType)
{
	switch (uType)
	{
	case LEXCLASS_IA32REGISTERS_EAX:
	case LEXCLASS_IA32REGISTERS_AX:
	case LEXCLASS_IA32REGISTERS_AL:
		return RO16REG_AL_AX_EAX_MM0_XMM0_0_000;
		break;
	case LEXCLASS_IA32REGISTERS_ECX:
	case LEXCLASS_IA32REGISTERS_CX:
	case LEXCLASS_IA32REGISTERS_CL:
		return RO16REG_CL_CX_ECX_MM1_XMM1_1_001;
		break;
	case LEXCLASS_IA32REGISTERS_EDX:
	case LEXCLASS_IA32REGISTERS_DX:
	case LEXCLASS_IA32REGISTERS_DL:
		return RO16REG_DL_DX_EDX_MM2_XMM2_2_010;
		break;
	case LEXCLASS_IA32REGISTERS_EBX:
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_BL:
		return RO16REG_BL_BX_EBX_MM3_XMM3_3_011;
		break;
	case LEXCLASS_IA32REGISTERS_ESP:
	case LEXCLASS_IA32REGISTERS_SP:
	case LEXCLASS_IA32REGISTERS_AH:
		return RO16REG_AH_SP_ESP_MM4_XMM4_4_100;
		break;
	case LEXCLASS_IA32REGISTERS_EBP:
	case LEXCLASS_IA32REGISTERS_BP:
	case LEXCLASS_IA32REGISTERS_CH:
		return RO16REG_CH_BP_EBP_MM5_XMM5_5_101;
		break;
	case LEXCLASS_IA32REGISTERS_ESI:
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DH:
		return RO16REG_DH_SI_ESI_MM6_XMM6_6_110;
		break;	
	case LEXCLASS_IA32REGISTERS_EDI:
	case LEXCLASS_IA32REGISTERS_DI:
	case LEXCLASS_IA32REGISTERS_BH:
		return RO16REG_BH_DI_EDI_MM7_XMM7_7_111;
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
int semRetRegEffectivAddrByType(UINT uType)
{
	switch (uType)
	{
	case LEXCLASS_IA32REGISTERS_EAX:
	case LEXCLASS_IA32REGISTERS_AX:
	case LEXCLASS_IA32REGISTERS_AL:
		return EA16REG_EAX_AX_AL_MM0_XMM0;
		break;
	case LEXCLASS_IA32REGISTERS_ECX:
	case LEXCLASS_IA32REGISTERS_CX:
	case LEXCLASS_IA32REGISTERS_CL:
		return EA16REG_ECX_CX_CL_MM1_XMM1;
		break;
	case LEXCLASS_IA32REGISTERS_EDX:
	case LEXCLASS_IA32REGISTERS_DX:
	case LEXCLASS_IA32REGISTERS_DL:
		return EA16REG_EDX_DX_DL_MM2_XMM2;
		break;
	case LEXCLASS_IA32REGISTERS_EBX:
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_BL:
		return EA16REG_EBX_BX_BL_MM3_XMM3;
		break;
	case LEXCLASS_IA32REGISTERS_ESP:
	case LEXCLASS_IA32REGISTERS_SP:
	case LEXCLASS_IA32REGISTERS_AH:
		return EA16REG_ESP_SP_AH_MM4_XMM4;
		break;
	case LEXCLASS_IA32REGISTERS_EBP:
	case LEXCLASS_IA32REGISTERS_BP:
	case LEXCLASS_IA32REGISTERS_CH:
		return EA16REG_EBP_BP_CH_MM5_XMM5;
		break;
	case LEXCLASS_IA32REGISTERS_ESI:
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DH:
		return EA16REG_ESI_SI_DH_MM6_XMM6;
		break;	
	case LEXCLASS_IA32REGISTERS_EDI:
	case LEXCLASS_IA32REGISTERS_DI:
	case LEXCLASS_IA32REGISTERS_BH:
		return EA16REG_EDI_DI_BH_MM7_XMM7;
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
int semRetRegRWByType(UINT uType)
{
	switch (uType)
	{
	case LEXCLASS_IA32REGISTERS_EAX:
	case LEXCLASS_IA32REGISTERS_AX:
	case LEXCLASS_IA32REGISTERS_AL:
		return RW_AX;
		break;
	case LEXCLASS_IA32REGISTERS_ECX:
	case LEXCLASS_IA32REGISTERS_CX:
	case LEXCLASS_IA32REGISTERS_CL:
		return RW_CX;
		break;
	case LEXCLASS_IA32REGISTERS_EDX:
	case LEXCLASS_IA32REGISTERS_DX:
	case LEXCLASS_IA32REGISTERS_DL:
		return RW_DX;
		break;
	case LEXCLASS_IA32REGISTERS_EBX:
	case LEXCLASS_IA32REGISTERS_BX:
	case LEXCLASS_IA32REGISTERS_BL:
		return RW_BX;
		break;
	case LEXCLASS_IA32REGISTERS_ESP:
	case LEXCLASS_IA32REGISTERS_SP:
	case LEXCLASS_IA32REGISTERS_AH:
		return RW_SP;
		break;
	case LEXCLASS_IA32REGISTERS_EBP:
	case LEXCLASS_IA32REGISTERS_BP:
	case LEXCLASS_IA32REGISTERS_CH:
		return RW_BP;
		break;
	case LEXCLASS_IA32REGISTERS_ESI:
	case LEXCLASS_IA32REGISTERS_SI:
	case LEXCLASS_IA32REGISTERS_DH:
		return RW_SI;
		break;	
	case LEXCLASS_IA32REGISTERS_EDI:
	case LEXCLASS_IA32REGISTERS_DI:
	case LEXCLASS_IA32REGISTERS_BH:
		return RW_DI;
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
int semCheckContrElemBlock(TOperationElemPtr pBegArgList)
{
	TOperationElemPtr	pTempOE;
	TBlockPtr			pTempBlock;
	pTempOE = pBegArgList->pNextOperationElem;
	pTempBlock = pTempOE->pArgument.pvValue;
	
	pTempOE = pBegArgList;
	pTempBlock->pExternalBlock = pTempOE->pArgument.pvValue;
	semCheckBlock(pTempBlock);
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
int semCheckBlock(TBlockPtr pBlock)
{
	unsigned int uStackOffset = 0;
	TOperationElemPtr	pBlockLocVar = NULL, pTempOE = NULL;
	TContrElemPtr		pContrElem = NULL, pTempCE = NULL;
	TIdTablePtr			pTempIdTableElem = NULL;
	
	pBlockLocVar = pBlock->pBegLocVarList;
	while (pBlockLocVar)
	{
		pTempIdTableElem = (TIdTablePtr)pBlockLocVar->pArgument.pvValue;
		pTempIdTableElem->uStackOffset = uStackOffset;
		
		if (pTempIdTableElem->pBegIdDecl)
		{	
			uStackOffset += 2; // for 16-bit address mode
		}
		else
		{
			uStackOffset += semRetTypeSizeByType(pBlockLocVar->pArgument.uLexClass);
		}
		pBlockLocVar = pBlockLocVar->pNextOperationElem;
	}
	uStackOffset = uStackOffset + semRetTypeSizeByType(LEXCLASS_IA32REGISTERS_BP);
	pBlock->uLocVarSize = uStackOffset;
	pContrElem = pBlock->pBegContrList;
	while (pContrElem)
	{
		switch (pContrElem->uSemClass)
		{
		case SEMCLASS_BLOCK:
			semCheckContrElemBlock(pContrElem->pBegArgList);
			break;
		case SEMCLASS_EQUATION:
			semCheckContrElemEquation(pContrElem->pBegArgList);
			break;
		case SEMCLASS_FUNCRETURN:
			semCheckContrElemFuncRet(pContrElem->pBegArgList);
			break;
		case SEMCLASS_CONSTRUCTION_IF:
			// IF Condition checking
			pTempOE = pContrElem->pBegArgList;
			pTempCE = pTempOE->pArgument.pvValue;
			semCheckContrElemEquation(pTempCE->pBegArgList);
			
			// IF Block checking
			pTempOE = pTempOE->pNextOperationElem;
			pTempCE = pTempOE->pArgument.pvValue;
			semCheckContrElemBlock(pTempCE->pBegArgList);
						
			// ELSE Block checking
			pTempOE = pTempOE->pNextOperationElem;
			if (pTempOE && (pTempOE->pArgument.uSemClass == SEMCLASS_CONSTRUCTION_ELSE))
			{
				pTempCE = pTempOE->pArgument.pvValue;
				semCheckContrElemBlock(pTempCE->pBegArgList);
			}
			break;
		case SEMCLASS_CONSTRUCTION_WHILE:
			//printf("$$$$$\n");
			// WHILE Condition checking
			pTempOE = pContrElem->pBegArgList;
			pTempCE = pTempOE->pArgument.pvValue;
			semCheckContrElemEquation(pTempCE->pBegArgList);
			
			// WHILE Block checking
			pTempOE = pTempOE->pNextOperationElem;
			pTempCE = pTempOE->pArgument.pvValue;
			semCheckContrElemBlock(pTempCE->pBegArgList);
			
			break;
		case SEMCLASS_INSTRUCTION:
			break;
		}
		pContrElem = pContrElem->pNextContrElem;
	
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
void semAnalisys()
{
	unsigned int uStackOffset = 0;
	extern TParseListNodePtr	pBegParseListNode;
	
	TParseListNodePtr	pParseListNode = NULL;
	TOperationElemPtr	pFuncArg = NULL;

	TOperationElemPtr	pContrElemArg = NULL;
	TBlockPtr			pBlock;
	TIdTablePtr			pTempIdTableElem = NULL;

	pParseListNode = pBegParseListNode;
	while (pParseListNode)
	{
		uStackOffset = 0;
		pFuncArg = pParseListNode->FuncNode.pBegArgList;
		if (pParseListNode->FuncNode.uType != LEXCLASS_TYPE_STRUCT)
		{
			uStackOffset += RETURNSIZE16;
			while (pFuncArg)
			{
				pTempIdTableElem = pFuncArg->pArgument.pvValue;
				pTempIdTableElem->uStackOffset = uStackOffset;
			
				if (pTempIdTableElem->pBegIdDecl)
					uStackOffset += 2; // for 16-bit address mode
				else
					uStackOffset += semRetTypeSizeByType(pFuncArg->pArgument.uLexClass);				
								
				pFuncArg = pFuncArg->pNextOperationElem;
			}

			pParseListNode->FuncNode.pFuncBlock->uLocVarSize = uStackOffset;
			pBlock = pParseListNode->FuncNode.pFuncBlock;
			if (pBlock)
			{
				semCheckBlock(pBlock);
			}
		}
		pParseListNode = pParseListNode->pNextFuncNode;
	}
}