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
int cgFuncSizeDetection()
{
	extern TFuncListPtr pBegFuncList;
	TFuncListPtr		pTempFunction;
	TInstructionListPtr pTempInstruction;
	UINT				uFuncSize;
	
	pTempFunction = pBegFuncList;
	
	while (pTempFunction)
	{
		uFuncSize = 0;
		pTempInstruction = pTempFunction->Function.pInstructionList;
		while (pTempInstruction)
		{
			uFuncSize += pTempInstruction->Instruction.uCodeSize;
			pTempInstruction = pTempInstruction->pNextInstruction;
			
		}
		pTempFunction->Function.uSize = uFuncSize;
		pTempFunction = pTempFunction->pNextFuncCode;
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
int cgFuncOffsetDetection()
{
	extern UINT uProgrammOffset;
	extern TFuncListPtr pBegFuncList;
	TFuncListPtr		pTempFunction;
	UINT				uFuncOffset;
	
	pTempFunction = pBegFuncList;
	
	uFuncOffset = uProgrammOffset; // +4 for main function call instruction
	uFuncOffset += pTempFunction->Function.uSize;
	pTempFunction = pTempFunction->pNextFuncCode;
	while (pTempFunction)
	{
		pTempFunction->Function.pFuncId->uStackOffset = uFuncOffset;
		uFuncOffset += pTempFunction->Function.uSize;
		pTempFunction = pTempFunction->pNextFuncCode;
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
int cgLinking()
{
	extern TCallLinkPtr pBegCallLinkList;
	TCallLinkPtr pTempCallLink;
	unsigned char   ucByte;
	unsigned short	usWord;
	
	pCurInstruction = mainCallLink.pInstruction;
	pCurByte = NULL;
	cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_IMM16 + RW_SI, FALSE);
	
	usWord = (unsigned short)mainCallLink.pFuncId->uStackOffset;
	ucByte = (unsigned char)usWord;
	cgAddNewByteToCurInstruction( ucByte, FALSE);
	ucByte = usWord >> 8;
	cgAddNewByteToCurInstruction( ucByte, FALSE);
	
	pTempCallLink = pBegCallLinkList;
	while (pTempCallLink)
	{
		pCurInstruction = pTempCallLink->pInstruction;
		pCurByte = NULL;
		cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_IMM16 + RW_SI, FALSE);
		
		usWord = (unsigned short)pTempCallLink->pFuncId->uStackOffset;
		ucByte = (unsigned char)usWord;
		cgAddNewByteToCurInstruction( ucByte, FALSE);
		ucByte = usWord >> 8;
		cgAddNewByteToCurInstruction( ucByte, FALSE);
		
		pTempCallLink = pTempCallLink->pNextCallLink;
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
void cgPrintFuncCode(char bIsScreenPrint)
{
	extern TFuncListPtr pBegFuncList;
	TFuncListPtr		pTempFunction;
	TInstructionListPtr pTempInstruction;
	TByteListPtr		pTempByte;
    FILE * pOutInfoFile;
	
    pOutInfoFile = fopen("CompileInfo.txt","a");
	
	pTempInstruction = pBegFuncList->Function.pInstructionList;
	
	fprintf(pOutInfoFile, "\n Function address: %X\n", uProgrammOffset);
	fprintf(pOutInfoFile, " Instruction size | Code\n");
	if (bIsScreenPrint)
	{
		printf("\n Function address: %X\n", uProgrammOffset);
		printf(" Instruction size | Code\n");
	}
	while (pTempInstruction)
	{
		if (bIsScreenPrint)
			printf(" %16X   ",pTempInstruction->Instruction.uCodeSize );
		fprintf(pOutInfoFile, " %16X   ",pTempInstruction->Instruction.uCodeSize );
		
		pTempByte = pTempInstruction->Instruction.pByteList;
		while(pTempByte)
		{
			if (bIsScreenPrint)
				printf("%X ",(unsigned char)pTempByte->cByte);
			fprintf(pOutInfoFile, "%X ",(unsigned char)pTempByte->cByte);
			pTempByte = pTempByte->pNextByte;
		}
		if (bIsScreenPrint)
			printf("\n");
		fprintf(pOutInfoFile, "\n");
		pTempInstruction = pTempInstruction->pNextInstruction;
	}
	pTempFunction = pBegFuncList->pNextFuncCode;
	while (pTempFunction)
	{
		pTempInstruction = pTempFunction->Function.pInstructionList;
		if (bIsScreenPrint)
		{
			printf("\n Function address: %X\n", pTempFunction->Function.pFuncId->uStackOffset);
			printf(" Instruction size | Code\n");
		}
		fprintf(pOutInfoFile, "\n Function address: %X\n", pTempFunction->Function.pFuncId->uStackOffset);
		fprintf(pOutInfoFile, " Instruction size | Code\n");
		while (pTempInstruction)
		{
			if (bIsScreenPrint)
				printf(" %16X   ",pTempInstruction->Instruction.uCodeSize );
			fprintf(pOutInfoFile, " %16X   ",pTempInstruction->Instruction.uCodeSize );
			
			pTempByte = pTempInstruction->Instruction.pByteList;
			while(pTempByte)
			{
				if (bIsScreenPrint)
					printf("%X ",(unsigned char)pTempByte->cByte);
				fprintf(pOutInfoFile, "%X ",(unsigned char)pTempByte->cByte);
				pTempByte = pTempByte->pNextByte;
			}
			if (bIsScreenPrint)	
				printf("\n");
			fprintf(pOutInfoFile, "\n");
			
			pTempInstruction = pTempInstruction->pNextInstruction;
		}
		
		pTempFunction = pTempFunction->pNextFuncCode;
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
void cgBinFileGeneration(char * cpInFileName)
{
	FILE * pBinOut;
	extern TFuncListPtr pBegFuncList;
	TFuncListPtr		pTempFunction;
	TInstructionListPtr pTempInstruction;
	TByteListPtr		pTempByte;
	
	pBinOut = fopen(cpInFileName, "wb");
	
	pTempInstruction = pBegFuncList->Function.pInstructionList;
	while (pTempInstruction)
	{
		pTempByte = pTempInstruction->Instruction.pByteList;
		while(pTempByte)
		{
			putc((unsigned char)pTempByte->cByte,pBinOut);
			pTempByte = pTempByte->pNextByte;
		}
		pTempInstruction = pTempInstruction->pNextInstruction;
	}
	pTempFunction = pBegFuncList->pNextFuncCode;
	while (pTempFunction)
	{
		pTempInstruction = pTempFunction->Function.pInstructionList;
		while (pTempInstruction)
		{
			pTempByte = pTempInstruction->Instruction.pByteList;
			while(pTempByte)
			{
				putc((unsigned char)pTempByte->cByte,pBinOut);
				pTempByte = pTempByte->pNextByte;
			}
			pTempInstruction = pTempInstruction->pNextInstruction;
		}
		pTempFunction = pTempFunction->pNextFuncCode;
	}
	fclose(pBinOut);
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int cgCreateCallLink( TInstructionListPtr pInstruction, TIdTablePtr pFuncId )
{
	extern TCallLinkPtr		pBegCallLinkList;
	extern TCallLinkPtr		pCurCallLinkList;
	
	TCallLinkPtr pNewCallLink;
	
	pNewCallLink = (TCallLinkPtr)malloc(sizeof(TCallLink));
	pNewCallLink->pFuncId = pFuncId;
	pNewCallLink->pInstruction = pInstruction;
	pNewCallLink->pNextCallLink = NULL;
	pNewCallLink->pPrevCallLink = NULL;
	
	if (pCurCallLinkList)
	{
		pCurCallLinkList->pNextCallLink = pNewCallLink;
		pNewCallLink->pPrevCallLink = pCurCallLinkList;
		pCurCallLinkList = pNewCallLink;
	}
	
	if (!pBegCallLinkList)
	{
		pBegCallLinkList = pNewCallLink;
		pCurCallLinkList = pNewCallLink;
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
int cgAddNewFunctionToList(UINT uType, TIdTablePtr pFuncId)
{
	extern TFuncListPtr		pBegFuncList;
	extern TFuncListPtr		pCurFunction;
	TFuncListPtr pNewFunction;
	
	pNewFunction = (TFuncListPtr)malloc(sizeof(TFuncList));
	pNewFunction->Function.pInstructionList = NULL;
	pNewFunction->Function.uSize = 0;
	pNewFunction->Function.uType = uType;
	pNewFunction->Function.pFuncId = pFuncId;
	pNewFunction->pNextFuncCode = NULL;
	pNewFunction->pPrevFuncCode = NULL;
	pCurInstruction = NULL;
	if (pCurFunction)
	{
		pCurFunction->pNextFuncCode = pNewFunction;
		pNewFunction->pPrevFuncCode = pCurFunction;
		pCurFunction = pNewFunction;
	}
	
	if (!pBegFuncList)
	{
		pBegFuncList = pNewFunction;
		pCurFunction = pNewFunction;
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
int cgAddNewInstructionToFunc(UINT uInstrType)
{
	extern TFuncListPtr			pCurFunction;
	extern TInstructionListPtr	pCurInstruction;
	
	TInstructionListPtr			pNewInstruction;
	
	pNewInstruction = (TInstructionListPtr)malloc(sizeof(TInstructionList));
	pNewInstruction->Instruction.pByteList = NULL;
	pNewInstruction->Instruction.uCodeSize = 0;
	pNewInstruction->Instruction.uInstrType = uInstrType;
	pNewInstruction->pNextInstruction = NULL;
	pNewInstruction->pPrevInstruction = NULL;
	pCurByte = NULL;
	if (pCurInstruction)
	{
		pCurInstruction->pNextInstruction = pNewInstruction;
		pNewInstruction->pPrevInstruction = pCurInstruction;
		pCurInstruction = pNewInstruction;
	}
	
	if (!pCurFunction->Function.pInstructionList)
	{
		pCurFunction->Function.pInstructionList = pNewInstruction;
		pCurInstruction = pNewInstruction;
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
int cgAddNewByteToInstruction(TInstructionListPtr pInstruction, unsigned char cByte, char bIncrement)
{
	TByteListPtr				pByte;
	TByteListPtr				pNewByte;
	
	pNewByte = (TByteListPtr)malloc(sizeof(TByteList));
	pNewByte->cByte = cByte;
	pNewByte->pNextByte = NULL;
	pNewByte->pPrevByte = NULL;
	
	pByte = pInstruction->Instruction.pByteList;
	
	if (pByte)
	{
		while (pByte->pNextByte)
		{
			pByte = pByte->pNextByte;
		}
		pByte->pNextByte = pNewByte;
		pNewByte->pPrevByte = pByte;
	}	
	else
	{
		pInstruction->Instruction.pByteList = pByte = pNewByte;
	}
	
	if (bIncrement)
		++pInstruction->Instruction.uCodeSize;
	
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
int cgAddNewByteToCurInstruction(unsigned char cByte, char bIncrement)
{
	extern TInstructionListPtr	pCurInstruction;
	extern TByteListPtr			pCurByte;
	TByteListPtr				pNewByte;
	
	pNewByte = (TByteListPtr)malloc(sizeof(TByteList));
	pNewByte->cByte = cByte;
	pNewByte->pNextByte = NULL;
	pNewByte->pPrevByte = NULL;
	if (pCurByte)
	{
		pCurByte->pNextByte = pNewByte;
		pNewByte->pPrevByte = pCurByte;
		pCurByte = pNewByte;
	}
	
	if (!pCurInstruction->Instruction.pByteList)
	{
		pCurInstruction->Instruction.pByteList = pNewByte;
		pCurByte = pNewByte;
	}
	if (bIncrement)
	{
		++pCurInstruction->Instruction.uCodeSize;
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
int cgGetCurByteOffset()
{
	UINT uOffset;
	extern TFuncListPtr	pCurFunction;
	TInstructionListPtr pTempInstr;
	pTempInstr = pCurFunction->Function.pInstructionList;
	uOffset = 0;
	while(pTempInstr)
	{
		uOffset += pTempInstr->Instruction.uCodeSize;
		pTempInstr = pTempInstr->pNextInstruction;
	}
	
	return uOffset;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int cgContrElemBlock(TOperationElemPtr pBegArgList)
{
	TOperationElemPtr	pTempOE;
	TBlockPtr			pTempBlock;
	unsigned short	usWord = 0;
	unsigned short	uLocVarTypeSum = 0;
	unsigned char	ucByte = 0;
	extern TParseListNodePtr	pBegParseListNode;
	extern TParseListNodePtr	pCurParseListNode;

	TOperationElemPtr	pLocVar = NULL;
	
	TContrElemPtr		pContrElem = NULL;
	TOperationElemPtr	pContrElemArg = NULL;
	TIdTablePtr			pTempIdTableElem = NULL;

	pTempOE = pBegArgList->pNextOperationElem;
	pTempBlock = pTempOE->pArgument.pvValue;
	pCurBlock = pTempBlock;
	
	pTempOE = pBegArgList;
	pTempBlock = pTempOE->pArgument.pvValue;
	// pTempBlock = pointer to the external block
	pCurBlock->pExternalBlock = pTempBlock;

	pLocVar = pCurBlock->pBegLocVarList;
	
	pCurBlock->uBlockOffset = cgGetCurByteOffset();
	
	// Saving previous BP
	// PUSH BP
	cgAddNewInstructionToFunc( PUSH_R16 + RW_BP );
	cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_BP, TRUE );
	if (pLocVar)
	{
		while (pLocVar->pNextOperationElem)
		{
			pLocVar = pLocVar->pNextOperationElem;
		}
		// Subtracting the size of all the local variables from SP
		// (allocating the memory)
		cgAddNewInstructionToFunc( SUB_RM16_IMM16 );
		cgAddNewByteToCurInstruction( (unsigned char)SUB_RM16_IMM16, TRUE );
		cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_CH_BP_EBP_MM5_XMM5_5_101), TRUE );
		while (pLocVar)
		{	
			pTempIdTableElem = (TIdTablePtr)pLocVar->pArgument.pvValue;
			if (pTempIdTableElem->pBegIdDecl)
				uLocVarTypeSum += 2; // for 16-bit address mode
			else
				uLocVarTypeSum += semRetTypeSizeByType(pLocVar->pArgument.uLexClass);
			
			pLocVar = pLocVar->pPrevOperationElem;
		}
		ucByte = (unsigned char)uLocVarTypeSum;
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		ucByte = uLocVarTypeSum >> 8;
		cgAddNewByteToCurInstruction( ucByte, TRUE );
	}
	// BP with SP equating
	cgAddNewInstructionToFunc( MOV_R16_RM16 );
	cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_CH_BP_EBP_MM5_XMM5_5_101), TRUE );
	
	
	pContrElem = pCurBlock->pBegContrList;
	
	while (pContrElem)
	{
		pContrElem->pContrElemHandler(pContrElem->pBegArgList);
		pContrElem = pContrElem->pNextContrElem;
	}
	
	// ADD SP,uLocVarTypeSum
	cgAddNewInstructionToFunc( ADD_RM16_IMM16 );
	cgAddNewByteToCurInstruction( (unsigned char)ADD_RM16_IMM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000, TRUE );
	
	ucByte = (unsigned char)uLocVarTypeSum;
	cgAddNewByteToCurInstruction( ucByte, TRUE );
	ucByte = uLocVarTypeSum >> 8;
	cgAddNewByteToCurInstruction( ucByte, TRUE );
	
	// Restoring previous BP value
	// POP BP
	cgAddNewInstructionToFunc( POP_R16 );
	cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_BP, TRUE );	
	
	pCurBlock->uBlockSize = cgGetCurByteOffset() - pCurBlock->uBlockOffset;
	pCurBlock = pCurBlock->pExternalBlock;
	
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
int cgContrElemFuncRet(TOperationElemPtr pBegArgList)
{
	unsigned char   ucByte = 0;
	unsigned short	usWord = 0;
	TOperationElemPtr pTempOE;
	TContrElemPtr pTempCE;
	TBlockPtr pTempBlock = NULL;
	TIdTablePtr			pTempIdTableElem = NULL;

	TParseListNodePtr pTempParseListNode = NULL;
	unsigned short uLocVarTypeSum = 0;

	pTempOE = pBegArgList;
	
	pTempParseListNode = (TParseListNodePtr)pTempOE->pArgument.pvValue;
	pTempBlock = pCurBlock;
	

	while(pTempBlock)
	{
		usWord = usWord + pTempBlock->uLocVarSize;
		pTempBlock = pTempBlock->pExternalBlock;
	}
	uLocVarTypeSum = usWord;
	
	usWord += RETURNSIZE16; 
	pTempOE = pTempParseListNode->FuncNode.pBegArgList;
	while(pTempOE)
	{
		pTempIdTableElem = (TIdTablePtr)pTempOE->pArgument.pvValue;
		
		
		if (pTempIdTableElem->pBegIdDecl)
			usWord += 2; // for 16-bit address mode
		else
			usWord += semRetTypeSizeByType(pTempOE->pArgument.uLexClass);
		
		pTempOE = pTempOE->pNextOperationElem;
	}
	usWord += semRetTypeSizeByType(LEXCLASS_IA32REGISTERS_BP);// size of BP
	pTempOE = pBegArgList->pNextOperationElem;
	pTempCE = (TContrElemPtr)pTempOE->pArgument.pvValue;
	pTempCE->pContrElemHandler(pTempCE->pBegArgList);
	// now AX   = the result of return equation
	// usWord = offset to the return value memory location
	// uLocVarTypeSum = offset to the return address

	
	// Copying AX to the return value 16-bit memory location
	// MOV [bp + usWord],AX
	cgAddNewInstructionToFunc( MOV_RM16_R16 );
	cgAddNewByteToCurInstruction( (unsigned char)MOV_RM16_R16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_BP_DISP16 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
		
	ucByte = (unsigned char)usWord;
	cgAddNewByteToCurInstruction( ucByte, TRUE);
	ucByte = usWord >> 8;
	cgAddNewByteToCurInstruction( ucByte, TRUE );

	// MOV SP,BP
	cgAddNewInstructionToFunc( MOV_R16_RM16 );
	cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EBP_BP_CH_MM5_XMM5 | RO16REG_AH_SP_ESP_MM4_XMM4_4_100), TRUE );
	
	
	// ADD SP,uLocVarTypeSum
	cgAddNewInstructionToFunc( ADD_RM16_IMM16 );
	cgAddNewByteToCurInstruction( (unsigned char)ADD_RM16_IMM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000, TRUE );
	
	ucByte = (unsigned char)uLocVarTypeSum;
	cgAddNewByteToCurInstruction( ucByte, TRUE );
	ucByte = uLocVarTypeSum >> 8;
	cgAddNewByteToCurInstruction( ucByte, TRUE );

	// RET
	cgAddNewInstructionToFunc( RET_NEAR );
	cgAddNewByteToCurInstruction( (unsigned char)RET_NEAR, TRUE );

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
int cgContrElemFuncCall(TOperationElemPtr pBegArgList)
{
	
	unsigned char   ucByte;
	unsigned short	usWord;
	TOperationElemPtr pArgDeclOE;
	TOperationElemPtr pTempOE;
	TContrElemPtr pTempCE;
	TIdTablePtr		pFuncIdTableElem = NULL, pTempIdTableElem = NULL;

	cgAddNewInstructionToFunc( SUB_RM16_IMM16 );
	cgAddNewByteToCurInstruction( (unsigned char)SUB_RM16_IMM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_CH_BP_EBP_MM5_XMM5_5_101), TRUE );
	
	usWord = 2; //(unsigned short)semRetTypeSizeByType(*((UINT*)pBegArgList->pArgument.pvValue)); !!!!!!!!!!!!! do luchshih vremen 
	//(perechital v Canade 2009-08-29, poradovalo :) )

	ucByte = (unsigned char)usWord;
	cgAddNewByteToCurInstruction( ucByte, TRUE );
	ucByte = usWord >> 8;
	cgAddNewByteToCurInstruction( ucByte, TRUE );
	
	// Saving previous function BP
	// PUSH BP
 	cgAddNewInstructionToFunc( PUSH_R16 + RW_BP );
 	cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_BP, TRUE );

	pTempOE = pBegArgList;
	
	pFuncIdTableElem = pTempOE->pArgument.pvValue;
	pArgDeclOE = pFuncIdTableElem->pArgInstance;

	pTempOE = pBegArgList->pNextOperationElem;
	
 	while (pArgDeclOE && pArgDeclOE->pNextOperationElem)
 		pArgDeclOE = pArgDeclOE->pNextOperationElem;
	
	usWord = 0;
	if (pTempOE)
	{
		while (pTempOE->pNextOperationElem)
			pTempOE = pTempOE->pNextOperationElem;
		while (pTempOE->pPrevOperationElem)
		{
			pTempCE = (TContrElemPtr)pTempOE->pArgument.pvValue;
			pTempCE->pContrElemHandler(pTempCE->pBegArgList);
			
			pTempIdTableElem = pArgDeclOE->pArgument.pvValue;

			if (pTempIdTableElem->pBegIdDecl)
			{
				usWord += 2; // for 16-bit address mode
				// PUSH AX
				cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
			}
			else
			{
				switch (semRetTypeSizeByType(pArgDeclOE->pArgument.uLexClass))
				{
				case 1:
					// SUB SP, 0x0001
					cgAddNewInstructionToFunc( SUB_RM16_IMM16 );
					cgAddNewByteToCurInstruction( (unsigned char)SUB_RM16_IMM16, TRUE );
					cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_CH_BP_EBP_MM5_XMM5_5_101), TRUE );
					cgAddNewByteToCurInstruction(0x01, TRUE);
					cgAddNewByteToCurInstruction(0x00, TRUE);

					// MOV SI,SP
					cgAddNewInstructionToFunc( MOV_R16_RM16 );
					cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
					cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_DH_SI_ESI_MM6_XMM6_6_110), TRUE );
					
					// MOV [SI],AL
					cgAddNewInstructionToFunc( MOV_RM8_R8 );
					
					cgAddNewByteToCurInstruction( (unsigned char)prefSS, TRUE ); // Prefix SS
					
					cgAddNewByteToCurInstruction( (unsigned char)MOV_RM8_R8, TRUE );
					cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_SI | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );

					break;
				case 2:
					// PUSH AX
					cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
					cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
					break;
				}
				usWord += semRetTypeSizeByType(pArgDeclOE->pArgument.uLexClass);
			
			}
			pArgDeclOE = pArgDeclOE->pPrevOperationElem;
			pTempOE = pTempOE->pPrevOperationElem;
		}
	}
	pTempOE = pBegArgList;
	cgAddNewInstructionToFunc( MOV_R16_RM16 );
	pCurInstruction->Instruction.uCodeSize = 3;
	cgCreateCallLink(pCurInstruction, pTempOE->pArgument.pvValue);
	
	cgAddNewInstructionToFunc( CALL_NEAR_RM16 );
	cgAddNewByteToCurInstruction( (unsigned char)CALL_NEAR_RM16, TRUE);
	cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESI_SI_DH_MM6_XMM6 | RO16REG_DL_DX_EDX_MM2_XMM2_2_010), TRUE);

	// Making SP to point to the memory, comprising the previous BP value
	// ADD SP, usWord
	cgAddNewInstructionToFunc( ADD_RM16_IMM16 );
	cgAddNewByteToCurInstruction( (unsigned char)ADD_RM16_IMM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000, TRUE );

	ucByte = (unsigned char)usWord;
	cgAddNewByteToCurInstruction( ucByte, TRUE );
	ucByte = usWord >> 8;
	cgAddNewByteToCurInstruction( ucByte, TRUE);

	// Restoring previous BP value
	// POP BP
 	cgAddNewInstructionToFunc( POP_R16 );
 	cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_BP, TRUE );

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
int cgContrElemExpression(TOperationElemPtr pBegArgList)
{
	unsigned char   ucByte;
	unsigned short	usWord;
	TOperationElemPtr pTempOE;
	TContrElemPtr pTempCE;
	TBlockPtr pTempBlock = NULL;
	TBlockPtr pTempCurBlock = NULL;
	TIdTablePtr pTempIdTable;
	TIDCurStateListPtr pTempCurState = NULL;
	UINT	uCounter = 0;
	UINT	uTotalIndLevelCnt = 0;	
	UINT	uCurTtlIndLevelCntContainer = 0;

	pTempOE = pBegArgList;
	while (pTempOE)
	{
		if(pTempOE->pArgument.uSemClass == SEMCLASS_IMM16)
		{
			cgAddNewInstructionToFunc( PUSH_IMM16 );
			cgAddNewByteToCurInstruction( (unsigned char)PUSH_IMM16, TRUE );
						
			usWord = (unsigned short)*((UINT *)pTempOE->pArgument.pvValue);
			
			ucByte = (unsigned char)usWord;
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			ucByte = (unsigned char)(usWord>>8);			
			cgAddNewByteToCurInstruction( ucByte, TRUE );
		}
		
		if(pTempOE->pArgument.uSemClass == SEMCLASS_IMM8)
		{
			cgAddNewInstructionToFunc( PUSH_IMM16 );
			cgAddNewByteToCurInstruction( (unsigned char)PUSH_IMM16, TRUE );
			ucByte = (unsigned char)*((UINT *)pTempOE->pArgument.pvValue);
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			cgAddNewByteToCurInstruction( 0, TRUE );
		}

		if(pTempOE->pArgument.uSemClass == SEMCLASS_OPERATION)
		{
			switch(pTempOE->pArgument.uLexClass)
			{
			case LEXCLASS_OPERATION_SLASH:
				// POP BX
				cgAddNewInstructionToFunc( POP_R16 + RW_BX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_BX, TRUE );
				// POP AX
				cgAddNewInstructionToFunc( POP_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
				
				// MOV DX,0x0000
				cgAddNewInstructionToFunc( MOV_R16_IMM16 + RW_DX );
				cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_IMM16 + RW_DX, TRUE );
				cgAddNewByteToCurInstruction( 0x00, TRUE );
				cgAddNewByteToCurInstruction( 0x00, TRUE );
				
				// DIV BX
				cgAddNewInstructionToFunc( DIV_RM16 );
				cgAddNewByteToCurInstruction( (unsigned char)DIV_RM16, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EBX_BX_BL_MM3_XMM3 | RO16REG_DH_SI_ESI_MM6_XMM6_6_110), TRUE );
				
				// PUSH AX
				cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
				break;			
			case LEXCLASS_OPERATION_ASTERISK:
				// POP BX
				cgAddNewInstructionToFunc( POP_R16 + RW_BX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_BX, TRUE );
				// POP AX
				cgAddNewInstructionToFunc( POP_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
				
				// MUL BX
				cgAddNewInstructionToFunc( MUL_RM16 );
				cgAddNewByteToCurInstruction( (unsigned char)MUL_RM16, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EBX_BX_BL_MM3_XMM3 | RO16REG_AH_SP_ESP_MM4_XMM4_4_100), TRUE );
				
				// PUSH AX
				cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
				break;
			case LEXCLASS_OPERATION_PLUS:
				// POP AX
				cgAddNewInstructionToFunc( POP_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
				// POP BX
				cgAddNewInstructionToFunc( POP_R16 + RW_BX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_BX, TRUE );
				// ADD AX,BX
				cgAddNewInstructionToFunc( ADD_R16_RM16 );
				cgAddNewByteToCurInstruction( (unsigned char)ADD_R16_RM16, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EBX_BX_BL_MM3_XMM3 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
				// PUSH AX
				cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
				break;
			case LEXCLASS_OPERATION_MINUS:
				// POP BX
				cgAddNewInstructionToFunc( POP_R16 + RW_BX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_BX, TRUE );
				// POP AX
				cgAddNewInstructionToFunc( POP_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
				// SUB AX,BX
				cgAddNewInstructionToFunc( SUB_R16_RM16 );
				cgAddNewByteToCurInstruction( (unsigned char)SUB_R16_RM16, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EBX_BX_BL_MM3_XMM3 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
				// PUSH AX
				cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
				break;
			}
		}
		///////////////////////////////////////////////////////
		if(pTempOE->pArgument.uSemClass == SEMCLASS_MEM16)
		{
			cgAddNewInstructionToFunc( PUSH_RM16 );
			cgAddNewByteToCurInstruction( (unsigned char)PUSH_RM16, TRUE );
			cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_BP_DISP16 | RO16REG_DH_SI_ESI_MM6_XMM6_6_110), TRUE );
			
			usWord = 0;
			
			pTempCurState = (TIDCurStateListPtr)pTempOE->pArgument.pvValue;
			
			uTotalIndLevelCnt = pTempCurState->pCurIdState->uTotalIndLevelCnt;
			uCurTotalIndLevelCnt = uTotalIndLevelCnt;
			
			pTempIdTable = pTempCurState->pCurIdState->pIdTableElem;
			
			pTempCurBlock = pCurBlock;
			pTempBlock = pTempIdTable->pBlock;
			
			while(pTempCurBlock != pTempBlock)
			{
				usWord = usWord + pTempCurBlock->uLocVarSize;
				pTempCurBlock = pTempCurBlock->pExternalBlock;
			}		
			
			//usWord = usWord + *((unsigned short *)pTempIdTable->pArgInstance->pArgument.pvValue); ///!!!!!!!!!! Change this
			
			usWord = usWord + pTempIdTable->uStackOffset;
			
			ucByte = (unsigned char)usWord;
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			ucByte = (unsigned char)(usWord>>8);			
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			
			if (uCurTotalIndLevelCnt == 0)
			switch(semRetTypeSizeByType(pTempOE->pArgument.uLexClass))
			{
			case 1:	
				// POP AX
				cgAddNewInstructionToFunc( POP_R16 );
				cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );		
				
				// MOV AH,0x00
				cgAddNewInstructionToFunc( MOV_R8_IMM8 + RB_AH );
				cgAddNewByteToCurInstruction( (unsigned char)MOV_R8_IMM8 + RB_AH, TRUE );
				cgAddNewByteToCurInstruction( 0x00, TRUE );
				
				// PUSH AX
				cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
				cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );

				break;
			case 2:
				break;
			}


			pTempCurState = pTempCurState->pNextCurStateElem;
			while(pTempCurState && (pTempCurState->pCurIdState->uStateType != SYNIDCURSTATETYPE_OPERATION))
			{
				switch (pTempCurState->pCurIdState->uStateType)
				{
				case SYNIDCURSTATETYPE_INDEX_DEREFERENCING:
					uCurTtlIndLevelCntContainer = uCurTotalIndLevelCnt;
					pTempCE = (TContrElemPtr)pTempCurState->pCurIdState->pvValue;
					pTempCE->pContrElemHandler(pTempCE->pBegArgList);
					uCurTotalIndLevelCnt = uCurTtlIndLevelCntContainer;
					uCurTotalIndLevelCnt --;
					
					// POP BX
					cgAddNewInstructionToFunc( POP_R16 );
					cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_BX, TRUE );

					// ADD AX,BX
					cgAddNewInstructionToFunc( ADD_R16_RM16 );
					cgAddNewByteToCurInstruction( (unsigned char)ADD_R16_RM16, TRUE );
					cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EBX_BX_BL_MM3_XMM3 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );

					if (!bIsLValue)
					{
						// MOV SI,AX
						cgAddNewInstructionToFunc( MOV_R16_RM16 );
						cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
						cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EAX_AX_AL_MM0_XMM0 | RO16REG_DH_SI_ESI_MM6_XMM6_6_110), TRUE );
						
						// MOV AX,[SI]
						cgAddNewInstructionToFunc( MOV_R16_RM16 );
						cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
						cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_SI | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );

						if (uCurTotalIndLevelCnt == 0)
						switch(semRetTypeSizeByType(pTempOE->pArgument.uLexClass))
						{
						case 1:	
							// MOV AH,0x00
							cgAddNewInstructionToFunc( MOV_R8_IMM8 + RB_AH );
							cgAddNewByteToCurInstruction( (unsigned char)MOV_R8_IMM8 + RB_AH, TRUE );
																	
							cgAddNewByteToCurInstruction( 0x00, TRUE );
							break;
						case 2:
							break;
						}
					}

					// PUSH AX
					cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
					cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
					
					break;
				case SYNIDCURSTATETYPE_DEREFERENCING:
					uCounter = pTempCurState->pCurIdState->uIndLvlCount;
					// POP AX
					cgAddNewInstructionToFunc( POP_R16 );
					cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
					
					if (bIsLValue)
						uCounter--;
					
					while(uCounter > 0)
					{
						// MOV SI,AX
						cgAddNewInstructionToFunc( MOV_R16_RM16 );
						cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
						cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EAX_AX_AL_MM0_XMM0 | RO16REG_DH_SI_ESI_MM6_XMM6_6_110), TRUE );
						
						// MOV AX,[SI]
						cgAddNewInstructionToFunc( MOV_R16_RM16 );
						cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
						cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_SI | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
					
						uCurTotalIndLevelCnt--;
						uCounter--;
					}
					uCounter = pTempCurState->pCurIdState->uIndLvlCount;
					
					if (uCurTotalIndLevelCnt == 0)
					switch(semRetTypeSizeByType(pTempOE->pArgument.uLexClass))
					{
					case 1:	
						// MOV AH,0x00
						cgAddNewInstructionToFunc( MOV_R8_IMM8 + RB_AH );
						cgAddNewByteToCurInstruction( (unsigned char)MOV_R8_IMM8 + RB_AH, TRUE );
						
						cgAddNewByteToCurInstruction( 0x00, TRUE );
						break;
					case 2:
						break;
					}

					// PUSH AX
					cgAddNewInstructionToFunc( PUSH_R16 + RW_AX );
					cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_AX, TRUE );
					
					break;
				}
				pTempCurState = pTempCurState->pNextCurStateElem;
			}

		}

		if(pTempOE->pArgument.uSemClass == SEMCLASS_FUNCTION)
		{
			pTempCE = (TContrElemPtr)pTempOE->pArgument.pvValue;
			pTempCE->pContrElemHandler(pTempCE->pBegArgList);
		}
		pTempOE = pTempOE->pNextOperationElem;
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
int cgContrElemEquation(TOperationElemPtr pBegArgList)
{
	TBlockPtr pTempBlock = NULL;
	TBlockPtr pTempCurBlock = NULL;
	TOperationElemPtr pTempOE;
	TContrElemPtr pTempCE;
	unsigned char   ucByte;
	unsigned short	usWord = 0;
	TIdTablePtr pTempIdTable;
	TIDCurStateListPtr pTempCurState = NULL;
	UINT	uCounter = 0;
	UINT	uTotalIndLevelCnt = 0;
	extern  UINT uCurTotalIndLevelCnt;

	uCurTotalIndLevelCnt = 0;

	pTempOE = pBegArgList;
	while (pTempOE->pNextOperationElem)
		pTempOE = pTempOE->pNextOperationElem;
		
	if (pTempOE)
	{
		pTempCE = (TContrElemPtr)pTempOE->pArgument.pvValue;
		pTempCE->pContrElemHandler(pTempCE->pBegArgList);
	}

	if(!pTempOE->pPrevOperationElem)
	{
		// POP AX
		cgAddNewInstructionToFunc( POP_R16 );
		cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
	
		if (uCurTotalIndLevelCnt == 0)
			switch(semRetTypeSizeByType(pTempOE->pArgument.uLexClass))
			{
			case 1:	
				// MOV AH,0x00
				cgAddNewInstructionToFunc( MOV_R8_IMM8 + RB_AH );
				cgAddNewByteToCurInstruction( (unsigned char)MOV_R8_IMM8 + RB_AH, TRUE );
				cgAddNewByteToCurInstruction( 0x00, TRUE );
				break;
			case 2:
				break;
			}
	}
	pTempOE = pTempOE->pPrevOperationElem;

	while (pTempOE)
	{
		pTempCE = (TContrElemPtr)pTempOE->pArgument.pvValue;
		
		pTempCurState = (TIDCurStateListPtr)pTempCE->pBegArgList->pArgument.pvValue;
		
		if (pTempCurState->pNextCurStateElem)
		{
			// if there is an address arithmetic
			uCurTotalIndLevelCnt = 0;
			bIsLValue = 1;
			pTempCE->pContrElemHandler(pTempCE->pBegArgList);
			bIsLValue = 0;
			// POP the 16-bit address, where the result will put
			// POP SI
			cgAddNewInstructionToFunc( POP_R16 );
			cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_SI, TRUE );

			// POP the result
			// POP AX
			cgAddNewInstructionToFunc( POP_R16 );
			cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
			
			if (uCurTotalIndLevelCnt == 0)
			switch(semRetTypeSizeByType(pTempOE->pArgument.uLexClass))
			{
			case 1:	
				// MOV [SI],AL
				cgAddNewInstructionToFunc( MOV_RM8_R8 );
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM8_R8, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_SI | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
				break;
			case 2:
				// MOV [SI],AX
				cgAddNewInstructionToFunc( MOV_RM16_R16 );
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM16_R16, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_SI | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
				break;
			}
			else
			{
				// MOV [SI],AX
				cgAddNewInstructionToFunc( MOV_RM16_R16 );
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM16_R16, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_SI | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
			}
		
		}
		else
		{
		// if there is a variable name (without address arithmetic),
		// than we just putting the AX value to the address of this variable
		
			usWord = 0;
			pTempCurBlock = pCurBlock;
			pTempIdTable = pTempCurState->pCurIdState->pIdTableElem;
			
			uTotalIndLevelCnt = pTempCurState->pCurIdState->uTotalIndLevelCnt;
			
			pTempBlock = pTempIdTable->pBlock;
			while(pTempCurBlock != pTempBlock)
			{
				usWord = usWord + pTempCurBlock->uLocVarSize;
				pTempCurBlock = pTempCurBlock->pExternalBlock;
			}		
			usWord = usWord + pTempIdTable->uStackOffset;
			
			// POP AX
			cgAddNewInstructionToFunc( POP_R16 );
			cgAddNewByteToCurInstruction( (unsigned char)POP_R16 + RW_AX, TRUE );
			
			if (uTotalIndLevelCnt > 0)
			{
				// Copying AX to the return value 16-bit memory location (pointer type)
				// MOV [bp + usWord],AX
				cgAddNewInstructionToFunc( MOV_RM16_R16 );
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM16_R16, TRUE );
			}						
			else
			{
				 	switch(semRetTypeSizeByType(pTempOE->pArgument.uLexClass))
					{
				 	case 1:	
				 		// Copying AL to the return value 8-bit memory location
				 		// MOV [bp + usWord],AL
				 		cgAddNewInstructionToFunc( MOV_RM8_R8 );
				 		cgAddNewByteToCurInstruction( (unsigned char)MOV_RM8_R8, TRUE );
						break;
				
				 	case 2:
				 		// Copying AX to the return value 16-bit memory location
				 		// MOV [bp + usWord],AX
				 		cgAddNewInstructionToFunc( MOV_RM16_R16 );
				 		cgAddNewByteToCurInstruction( (unsigned char)MOV_RM16_R16, TRUE );
				 		break;
				 	}
			}

			cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_BP_DISP16 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
			ucByte = (unsigned char)usWord;
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			ucByte = (unsigned char)(usWord>>8);			
			cgAddNewByteToCurInstruction( ucByte, TRUE );
		}
		pTempOE = pTempOE->pPrevOperationElem;
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
int cgContrElemConstructionWhile(TOperationElemPtr pBegArgList)
{
	extern TInstructionListPtr pCurInstruction;
	TInstructionListPtr pJZInstruction = NULL, pJMPInstruction = NULL;
	unsigned short	usWord = 0;
	unsigned char	ucByte = 0;
	
	TOperationElemPtr pTempOE;
	TContrElemPtr pTempCE;
	TBlockPtr pTempBlock;
	UINT uBegCondOffset = 0;
	UINT uEndCondOffset = 0;
	
	UINT uBegLoopOffset = 0;
	UINT uEndLoopOffset = 0;

	pTempOE = pBegArgList;
	
	uBegCondOffset = cgGetCurByteOffset();
	// Condition-expression implementation
	pTempCE = (TContrElemPtr)(pTempOE->pArgument.pvValue);
	pTempCE->pContrElemHandler(pTempCE->pBegArgList);
	pTempOE = pTempOE->pNextOperationElem;
	
	cgAddNewInstructionToFunc( TEST_RM16_R16 );
	cgAddNewByteToCurInstruction( (unsigned char) TEST_RM16_R16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char) (EA16REG_EAX_AX_AL_MM0_XMM0 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );
	
	cgAddNewInstructionToFunc( JZ_REL16 );
	pJZInstruction = pCurInstruction;
	cgAddNewByteToCurInstruction( (unsigned char) 0x0F, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char) JZ_REL16, TRUE );
	
	uEndCondOffset = cgGetCurByteOffset();
	// Block after if 
	switch (pTempOE->pArgument.uSemClass)
	{	
	case SEMCLASS_BLOCK:
		uBegLoopOffset = cgGetCurByteOffset();

		pTempCE = (TContrElemPtr)(pTempOE->pArgument.pvValue);
		pTempCE->pContrElemHandler(pTempCE->pBegArgList);
		pTempBlock = (TBlockPtr)(pTempCE->pBegArgList->pNextOperationElem->pArgument.pvValue);

		usWord = 0x0000 - (pTempBlock->uBlockSize + 3) - (uEndCondOffset - uBegCondOffset + 2); 
		// +2 because of the further 2 bytes adding for pJZinstr
		
		cgAddNewInstructionToFunc( JMP_REL16 );
		cgAddNewByteToCurInstruction( (unsigned char) JMP_REL16, TRUE );

		ucByte = (unsigned char)usWord;
		cgAddNewByteToCurInstruction(ucByte, TRUE );
		ucByte = (unsigned char)(usWord>>8);			
		cgAddNewByteToCurInstruction(ucByte, TRUE );
		
		uEndLoopOffset = cgGetCurByteOffset();
		
		usWord = uEndLoopOffset - uBegLoopOffset;//pTempBlock->uBlockSize + 3;
		
		ucByte = (unsigned char)usWord;
		cgAddNewByteToInstruction(pJZInstruction, ucByte, TRUE );
		ucByte = (unsigned char)(usWord>>8);			
		cgAddNewByteToInstruction(pJZInstruction, ucByte, TRUE );
		
		pTempOE = pTempOE->pNextOperationElem;
		break;
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
int cgContrElemConstructionIf(TOperationElemPtr pBegArgList)
{
	extern TInstructionListPtr pCurInstruction;
	TInstructionListPtr pJZInstruction = NULL, pJMPInstruction = NULL;
	unsigned short	usWord = 0;
	unsigned char	ucByte = 0;

	TOperationElemPtr pTempOE;
	TContrElemPtr pTempCE;
	TBlockPtr pTempBlock;
	pTempOE = pBegArgList;
	
	// Condition-expression implementation
	pTempCE = (TContrElemPtr)(pTempOE->pArgument.pvValue);
	pTempCE->pContrElemHandler(pTempCE->pBegArgList);
	pTempOE = pTempOE->pNextOperationElem;

	cgAddNewInstructionToFunc( TEST_RM16_R16 );
	cgAddNewByteToCurInstruction( (unsigned char) TEST_RM16_R16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char) (EA16REG_EAX_AX_AL_MM0_XMM0 | RO16REG_AL_AX_EAX_MM0_XMM0_0_000), TRUE );

	cgAddNewInstructionToFunc( JZ_REL16 );
	pJZInstruction = pCurInstruction;
	cgAddNewByteToCurInstruction( (unsigned char) 0x0F, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char) JZ_REL16, TRUE );
	
	// Block after if 
	switch (pTempOE->pArgument.uSemClass)
	{	
	case SEMCLASS_BLOCK:
		pTempCE = (TContrElemPtr)(pTempOE->pArgument.pvValue);
		pTempCE->pContrElemHandler(pTempCE->pBegArgList);
		
		cgAddNewInstructionToFunc( JMP_REL16 );
		pJMPInstruction = pCurInstruction;
		cgAddNewByteToCurInstruction( (unsigned char) JMP_REL16, TRUE );
		
		
		pTempBlock = (TBlockPtr)(pTempCE->pBegArgList->pNextOperationElem->pArgument.pvValue);
		usWord = pTempBlock->uBlockSize + 3;
		
		ucByte = (unsigned char)usWord;
		cgAddNewByteToInstruction(pJZInstruction, ucByte, TRUE );
		ucByte = (unsigned char)(usWord>>8);			
		cgAddNewByteToInstruction(pJZInstruction, ucByte, TRUE );
		
		pTempOE = pTempOE->pNextOperationElem;
		break;
	}
	usWord = 0;	
	if ((pTempOE) && (pTempOE->pArgument.uSemClass == SEMCLASS_CONSTRUCTION_ELSE))
	{
		pTempCE = (TContrElemPtr)(pTempOE->pArgument.pvValue);
		pTempCE->pContrElemHandler(pTempCE->pBegArgList);
		
		pTempBlock = (TBlockPtr)(pTempCE->pBegArgList->pNextOperationElem->pArgument.pvValue);
		usWord = pTempBlock->uBlockSize;
		pTempOE = pTempOE->pNextOperationElem;
	}
	ucByte = (unsigned char)usWord;
	cgAddNewByteToInstruction(pJMPInstruction, ucByte, TRUE );
	ucByte = (unsigned char)(usWord>>8);			
	cgAddNewByteToInstruction(pJMPInstruction, ucByte, TRUE );
	
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
int cgCodeCreating()
{
	unsigned short	usWord = 0;
	unsigned short	usTypeSum = 0;
	unsigned char	ucCode = 0;
	extern TParseListNodePtr	pBegParseListNode;
	extern TParseListNodePtr	pCurParseListNode;
	TOperationElemPtr	pFuncLocVar = NULL;
	
	TContrElemPtr		pContrElem = NULL;
	TOperationElemPtr	pContrElemArg = NULL;
	extern UINT			uProgrammOffset;
	TIdTablePtr	pTempIdTableElem = NULL;

	pCurParseListNode = pBegParseListNode;
	
	cgAddNewFunctionToList(LEXCLASS_TYPE_VOID, NULL);
	
	/* INITIALIZATION BLOCK */
	/* CS = 0000h
	 * SS = 0000h
	 * SP = FFFFh
	*/
	//far jump -> CS = 0000; IP = BEGADDRESS + 5 bytes (5 bytes = far jmp instruction)
	cgAddNewInstructionToFunc( JMP_PTR_16_16 );
	cgAddNewByteToCurInstruction( (unsigned char)JMP_PTR_16_16, TRUE);
	usWord = uProgrammOffset + 5;
	
	ucCode = (unsigned char)usWord;
	cgAddNewByteToCurInstruction( ucCode, TRUE );
	ucCode = usWord >> 8;
	cgAddNewByteToCurInstruction( ucCode, TRUE );
	cgAddNewByteToCurInstruction( 0, TRUE );
	cgAddNewByteToCurInstruction( 0, TRUE );

	// MOV AX,0
	cgAddNewInstructionToFunc( MOV_R16_IMM16 );
	cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_IMM16 + RW_AX, TRUE );
	cgAddNewByteToCurInstruction( 0, TRUE );
	cgAddNewByteToCurInstruction( 0, TRUE );
	
	// MOV SS,AX
	cgAddNewInstructionToFunc( MOV_SREG_RM16 );
	cgAddNewByteToCurInstruction( (unsigned char)MOV_SREG_RM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EAX_AX_AL_MM0_XMM0 | SREG_DS_3), TRUE);

	// MOV SS,AX
	cgAddNewInstructionToFunc( MOV_SREG_RM16 );
	cgAddNewByteToCurInstruction( (unsigned char)MOV_SREG_RM16, TRUE );
	cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_EAX_AX_AL_MM0_XMM0 | SREG_SS_2), TRUE);

	// MOV SP,0xFFFF
	cgAddNewInstructionToFunc( MOV_R16_IMM16 );
	cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_IMM16 + RW_SP, TRUE );
	cgAddNewByteToCurInstruction( 0xFF, TRUE );
	cgAddNewByteToCurInstruction( 0xFF, TRUE );
	
	// MOV SI, absolute main function address
	cgAddNewInstructionToFunc( MOV_R16_RM16 );
	pCurInstruction->Instruction.uCodeSize = 3;
	mainCallLink.pInstruction = pCurInstruction;
	mainCallLink.pFuncId = NULL;

	// CALL SI
	cgAddNewInstructionToFunc( CALL_NEAR_RM16 );
	cgAddNewByteToCurInstruction( (unsigned char)CALL_NEAR_RM16, TRUE);
	cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESI_SI_DH_MM6_XMM6 | RO16REG_DL_DX_EDX_MM2_XMM2_2_010), TRUE);

	/* INITIALIZATION BLOCK END */
	
	while (pCurParseListNode)
	{
		usTypeSum = 0;
	
		if (pCurParseListNode->FuncNode.uType != LEXCLASS_TYPE_STRUCT)
		{
			pCurBlock = pCurParseListNode->FuncNode.pFuncBlock;
			pFuncLocVar = pCurBlock->pBegLocVarList;
			cgAddNewFunctionToList(pCurParseListNode->FuncNode.uType, pCurParseListNode->FuncNode.pFuncId);
			if (!strcmp(pCurParseListNode->FuncNode.cpFuncName,"main"))
			{
				mainCallLink.pFuncId = pCurParseListNode->FuncNode.pFuncId;
			}
			
			// Saving previous BP
			// PUSH BP
			cgAddNewInstructionToFunc( PUSH_R16 + RW_BP );
			cgAddNewByteToCurInstruction( (unsigned char)PUSH_R16 + RW_BP, TRUE );
			
			if (pFuncLocVar)
			{
				
				while (pFuncLocVar->pNextOperationElem)
				{
					pFuncLocVar = pFuncLocVar->pNextOperationElem;
				}
				// Subtracting the size of all the local variables from SP
				// (allocating the memory)
				cgAddNewInstructionToFunc( SUB_RM16_IMM16 );
				cgAddNewByteToCurInstruction( (unsigned char)SUB_RM16_IMM16, TRUE );
				cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_CH_BP_EBP_MM5_XMM5_5_101), TRUE );
				while (pFuncLocVar)
				{	
					pTempIdTableElem = (TIdTablePtr)pFuncLocVar->pArgument.pvValue;
					
					if (pTempIdTableElem->pBegIdDecl)
						usTypeSum += 2; // for 16-bit address mode
					else
						usTypeSum += semRetTypeSizeByType(pFuncLocVar->pArgument.uLexClass);
					
					pFuncLocVar = pFuncLocVar->pPrevOperationElem;
				}
				ucCode = (unsigned char)usTypeSum;
				cgAddNewByteToCurInstruction( ucCode, TRUE );
				ucCode = usTypeSum >> 8;
				cgAddNewByteToCurInstruction( ucCode, TRUE );
			}
			// BP with SP equating
			cgAddNewInstructionToFunc( MOV_R16_RM16 );
			cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
			cgAddNewByteToCurInstruction( (unsigned char)(EA16REG_ESP_SP_AH_MM4_XMM4 | RO16REG_CH_BP_EBP_MM5_XMM5_5_101), TRUE );
			
			pContrElem = pCurParseListNode->FuncNode.pFuncBlock->pBegContrList;

			while (pContrElem)
			{
				pContrElem->pContrElemHandler(pContrElem->pBegArgList);
				pContrElem = pContrElem->pNextContrElem;
			}
		}
		pCurParseListNode = pCurParseListNode->pNextFuncNode;
	}
	if (!mainCallLink.pFuncId)
	{
		semError("linking",0,0,0,"there is no 'main' function");
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
int cgAddEABytesBySemClass(UINT uSemClass, TOperationElemPtr pTempOE, char cTempRObyte)
{
	TBlockPtr pTempBlock = NULL;
	TBlockPtr pTempCurBlock = NULL;
	unsigned char   ucByte = 0;
	unsigned short	usWord = 0;
	TIdTablePtr		pTempId;
	char			cTempEAbyte;
	TIdTablePtr		pTempIdTableElem = NULL;

	switch(uSemClass)
	{
	case SEMCLASS_VAR8:
	case SEMCLASS_VAR16:
		pTempCurBlock = pCurBlock;
		pTempBlock = ((TIdTablePtr)pTempOE->pArgument.pvValue)->pBlock;
		while(pTempCurBlock != pTempBlock)
		{
			usWord = usWord + pTempCurBlock->uLocVarSize;
			pTempCurBlock = pTempCurBlock->pExternalBlock;
		}
				
		cgAddNewByteToCurInstruction( (unsigned char)(EA16MEMLOC_BP_DISP16 | cTempRObyte), TRUE );	
		pTempId = (TIdTablePtr)pTempOE->pArgument.pvValue;
		usWord = usWord + pTempId->uStackOffset;
		ucByte = (unsigned char)usWord;
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		ucByte = (unsigned char)(usWord>>8);			
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		break;
	case SEMCLASS_REG16:
		cTempEAbyte = *((unsigned char*)pTempOE->pArgument.pvValue);
		cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
		break;
	case SEMCLASS_IMM8:
		ucByte = *((unsigned char*)pTempOE->pArgument.pvValue);
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		break;
	case SEMCLASS_IMM16:
		usWord = *((unsigned short*)pTempOE->pArgument.pvValue);
		ucByte = (unsigned char)usWord;
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		ucByte = (unsigned char)(usWord>>8);			
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		break;
	case SEMCLASS_MEM8:
		cTempEAbyte = EA16MEMLOC_DISP16;
		cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
		usWord = *((unsigned short*)pTempOE->pArgument.pvValue);
		ucByte = (unsigned char)usWord;
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		ucByte = (unsigned char)(usWord>>8);			
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		break;
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
int cgMemoryLocationIndexCase(char cTempRObyte, unsigned char	cTempEAbyte)
{
	unsigned char   ucByte;
	unsigned short	usWord;
	
	if (pCurCGOperElem)
	switch (pCurCGOperElem->pArgument.uSemClass)
	{
	case EA16MEMLOC_SI:
		pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
		if (pCurCGOperElem && pCurCGOperElem->pArgument.uSemClass == EA16MEMLOC_DISP16)
		{
			if (!cTempEAbyte)
				cTempEAbyte = (char)EA16MEMLOC_SI_DISP16;
			else
				switch (cTempEAbyte) 
				{
				case (char)EA16MEMLOC_BX:
					cTempEAbyte = (char)EA16MEMLOC_BX_SI_DISP16;
					break;
				case (char)EA16MEMLOCPART_BP:
					cTempEAbyte = (char)EA16MEMLOC_BP_SI_DISP16;
					break;
				}

			
			cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
			usWord = *((unsigned short*)pCurCGOperElem->pArgument.pvValue);
			ucByte = (unsigned char)usWord;
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			ucByte = (unsigned char)(usWord>>8);			
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
		}
		else
		{
			if (!cTempEAbyte)
				cTempEAbyte = EA16MEMLOC_SI;
			else
				switch (cTempEAbyte) 
			{
				case (char)EA16MEMLOC_BX:
					cTempEAbyte = EA16MEMLOC_BX_SI;
					break;
				case (char)EA16MEMLOCPART_BP:
					cTempEAbyte = EA16MEMLOC_BP_SI;
					break;
				}
			
			cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
		}		
		return cTempEAbyte;
		break;
	case EA16MEMLOC_DI:
		if ((pCurCGOperElem->pNextOperationElem) && (pCurCGOperElem->pNextOperationElem->pArgument.uSemClass == EA16MEMLOC_DISP16))
		{
			if (!cTempEAbyte)
				cTempEAbyte = (char)EA16MEMLOC_DI_DISP16;
			else
				switch (cTempEAbyte) 
				{
				case (char)EA16MEMLOC_BX:
					cTempEAbyte = (char)EA16MEMLOC_BX_DI_DISP16;
					break;
				case (char)EA16MEMLOCPART_BP:
					cTempEAbyte = (char)EA16MEMLOC_BP_DI_DISP16;
					break;
				}
			cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			
			usWord = *((unsigned short*)pCurCGOperElem->pArgument.pvValue);
			ucByte = (unsigned char)usWord;
			cgAddNewByteToCurInstruction( ucByte, TRUE );
			ucByte = (unsigned char)(usWord>>8);			
			cgAddNewByteToCurInstruction( ucByte, TRUE );
		}
		else
		{
			if (!cTempEAbyte)
				cTempEAbyte = (char)EA16MEMLOC_DI;
			else
				switch (cTempEAbyte) 
				{
				case (char)EA16MEMLOC_BX:
					cTempEAbyte = EA16MEMLOC_BX_DI;
					break;
				case (char)EA16MEMLOCPART_BP:
					cTempEAbyte = EA16MEMLOC_BP_DI;
					break;
				}
			cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
		}		
		return cTempEAbyte;
		break;
	case EA16MEMLOC_DISP16:
		if (!cTempEAbyte)
			cTempEAbyte = (char)EA16MEMLOC_DISP16;
		else
			switch (cTempEAbyte) 
			{
			case (char)EA16MEMLOC_BX:
				cTempEAbyte = (char)EA16MEMLOC_BX_DISP16;
				break;
			case (char)EA16MEMLOCPART_BP:
				cTempEAbyte = (char)EA16MEMLOC_BP_DISP16;
				break;
			}
		cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
		
		usWord = *((unsigned short*)pCurCGOperElem->pArgument.pvValue);
		ucByte = (unsigned char)usWord;
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		ucByte = (unsigned char)(usWord>>8);			
		cgAddNewByteToCurInstruction( ucByte, TRUE );
		pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
		return cTempEAbyte;
		break;
	}
	
	cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
	return cTempEAbyte;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int cgMemoryLocationCase(char cTempRObyte)
{
	char	cTempEAbyte;
	switch (pCurCGOperElem->pArgument.uSemClass)
	{
	case EA16MEMLOC_BX:
		pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
		cTempEAbyte = cgMemoryLocationIndexCase(cTempRObyte, EA16MEMLOC_BX);
		break;
	case EA16MEMLOCPART_BP:
		pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
		cTempEAbyte = cgMemoryLocationIndexCase(cTempRObyte, EA16MEMLOCPART_BP);
		break;
	case EA16MEMLOC_SI:
	case EA16MEMLOC_DI:
	case EA16MEMLOC_DISP16:
		cTempEAbyte = cgMemoryLocationIndexCase(cTempRObyte, 0);
		break;
	}
	
	return cTempEAbyte;
}

/*
*	
*
*	@param 
*	@param 
*	@param 
*   @author Nail Sharipov
*/
int cgContrElemInstruction(TOperationElemPtr pBegArgList)
{

	TByteListPtr	pTempByte;
	char	cTempRObyte;
	char	cTempEAbyte;
	
	cgAddNewInstructionToFunc( MOV_RM16_R16 );
	pCurCGOperElem = pBegArgList;
	while (pCurCGOperElem)
	{
		
		switch(pCurCGOperElem->pArgument.uSemClass)
		{	
		case SEMCLASS_PREFIX:
			
			cgAddNewByteToCurInstruction( *((unsigned char*)pCurCGOperElem->pArgument.pvValue), TRUE );
			break;
		
		case SEMCLASS_INSTRUCTION_PTR16_16:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_JMP:
				cgAddNewByteToCurInstruction( (unsigned char)JMP_PTR_16_16, TRUE );
				break;
			}	
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem->pNextOperationElem;
			
			switch(pCurCGOperElem->pArgument.uSemClass)
			{
			case SEMCLASS_IMM8:
				cgAddEABytesBySemClass(SEMCLASS_IMM8, pCurCGOperElem, 0);
				cgAddNewByteToCurInstruction( 0, TRUE );	
				break;
			case SEMCLASS_IMM16:
				cgAddEABytesBySemClass(SEMCLASS_IMM16, pCurCGOperElem, 0);
				break;
			}
			
			pCurCGOperElem = pCurCGOperElem->pPrevOperationElem;

			switch(pCurCGOperElem->pArgument.uSemClass)
			{
			case SEMCLASS_IMM8:
				cgAddEABytesBySemClass(SEMCLASS_IMM8, pCurCGOperElem, 0);
				cgAddNewByteToCurInstruction( 0, TRUE );	
				break;
			case SEMCLASS_IMM16:
				cgAddEABytesBySemClass(SEMCLASS_IMM16, pCurCGOperElem, 0);
				break;
			}

			break;


		case SEMCLASS_INSTRUCTION_IMM8:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_INT:
				cgAddNewByteToCurInstruction( (unsigned char)INT_IMM8, TRUE );
				break;
			}	
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;

			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_IMM8)
				cgAddEABytesBySemClass(SEMCLASS_IMM8, pCurCGOperElem, 0);
			
			break;
				
	
		/* 
		 *	MOV Sreg, r/m16
		 */
		case SEMCLASS_INSTRUCTION_Sreg_MEM16:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_SREG_RM16, TRUE );
				break;
			}			
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
		
			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_REG16)
				cgAddEABytesBySemClass(SEMCLASS_REG16, pCurCGOperElem, cTempRObyte);
			
			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_VAR16)
				cgAddEABytesBySemClass(SEMCLASS_VAR16, pCurCGOperElem, cTempRObyte);

			break;
		case SEMCLASS_INSTRUCTION_Sreg_GPReg16:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_SREG_RM16, TRUE );
				break;
			}
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_REG16)
				cgAddEABytesBySemClass(SEMCLASS_REG16, pCurCGOperElem, cTempRObyte);
			break;
	
		/* 
		 *	MOV r/m8, r8
		 */
		case SEMCLASS_INSTRUCTION_MEM8_GPReg8:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM8_R8, TRUE );
				break;
			}
			
			
			
			pTempByte = pCurByte;
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
						
			cTempEAbyte = cgMemoryLocationCase(0);
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pTempByte->pNextByte->cByte = (unsigned char)(cTempEAbyte | cTempRObyte);
			break;
		case SEMCLASS_INSTRUCTION_MEM16_GPReg16:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM16_R16, TRUE );
				break;
			}
			
			pTempByte = pCurByte;
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempEAbyte = cgMemoryLocationCase(0);
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pTempByte->pNextByte->cByte = (unsigned char)(cTempEAbyte | cTempRObyte);
			break;

		case SEMCLASS_INSTRUCTION_MEM32_GPReg32:
			break;
				
		case SEMCLASS_INSTRUCTION_REG8_IMM8:
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;			
			cgAddNewByteToCurInstruction( (unsigned char)(MOV_R8_IMM8 + cTempRObyte), TRUE );
			
			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_IMM8)
				cgAddEABytesBySemClass(SEMCLASS_IMM8, pCurCGOperElem, cTempRObyte);
			
			break;
		
		case SEMCLASS_INSTRUCTION_REG16_IMM16:
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;			
			cgAddNewByteToCurInstruction( (unsigned char)(MOV_R16_IMM16 + cTempRObyte), TRUE );
			
			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_IMM16)
				cgAddEABytesBySemClass(SEMCLASS_IMM16, pCurCGOperElem, cTempRObyte);
			break;
		case SEMCLASS_INSTRUCTION_REG16_MEM16:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
				break;
			}
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;			
			cTempEAbyte = cgMemoryLocationCase(cTempRObyte);
			
			if (pCurCGOperElem)
			switch (pCurCGOperElem->pArgument.uSemClass)
			{
			case SEMCLASS_VAR16:
				cgAddEABytesBySemClass(SEMCLASS_VAR16, pCurCGOperElem, cTempRObyte);
				break;
			}
			break;
		case SEMCLASS_INSTRUCTION_REG8_MEM8:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_R8_RM8, TRUE );
				break;
			}
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;			
			cTempEAbyte = cgMemoryLocationCase(cTempRObyte);

			if (pCurCGOperElem)
			switch (pCurCGOperElem->pArgument.uSemClass)
			{
			case SEMCLASS_VAR8:
				cgAddEABytesBySemClass(SEMCLASS_VAR8, pCurCGOperElem, cTempRObyte);
				break;
			}
			break;
		
		case SEMCLASS_INSTRUCTION_MEM8_IMM8:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM8_IMM8, TRUE );
				break;
			}
			
			cTempRObyte = RO16REG_AL_AX_EAX_MM0_XMM0_0_000;
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;			
			cTempEAbyte = cgMemoryLocationCase(cTempRObyte);
			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_IMM8)
				cgAddEABytesBySemClass(SEMCLASS_IMM8, pCurCGOperElem, cTempRObyte);
			break;

		case SEMCLASS_INSTRUCTION_MEM16_IMM16:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_RM16_IMM16, TRUE );
				break;
			}
			
			cTempRObyte = RO16REG_AL_AX_EAX_MM0_XMM0_0_000;
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;			
			
			cTempEAbyte = cgMemoryLocationCase(0);

			if (pCurCGOperElem->pArgument.uSemClass == SEMCLASS_IMM16)
				cgAddEABytesBySemClass(SEMCLASS_IMM16, pCurCGOperElem, cTempRObyte);
			break;
		
		case SEMCLASS_INSTRUCTION_REG8_REG8:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_R8_RM8, TRUE );
				break;
			}
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempEAbyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
			break;
		case SEMCLASS_INSTRUCTION_REG16_REG16:
			switch(pCurCGOperElem->pArgument.uLexClass)
			{
			case LEXCLASS_IA32INSTRUCTIONS_MOV:
				cgAddNewByteToCurInstruction( (unsigned char)MOV_R16_RM16, TRUE );
				break;
			}
			
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempRObyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
			cTempEAbyte = *((unsigned char*)pCurCGOperElem->pArgument.pvValue);
			cgAddNewByteToCurInstruction( (unsigned char)(cTempEAbyte | cTempRObyte), TRUE );
			break;
		case SEMCLASS_INSTRUCTION_REG32_REG32:
			break;
		}
		if (pCurCGOperElem)
		pCurCGOperElem = pCurCGOperElem->pNextOperationElem;
	}
	return TRUE;
}
