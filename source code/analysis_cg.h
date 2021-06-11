#ifndef INCL_ANALYSIS_CG_H
#define INCL_ANALYSIS_CG_H

#include "main.h"

/** CODE GENERATION **/

// General functions
int		cgCodeCreating();
void	cgPrintFuncCode(char bIsScreenPrint);

// Code generation functions
int		cgContrElemFuncRet(TOperationElemPtr pBegArgList);
int		cgContrElemFuncCall(TOperationElemPtr pBegArgList);
int		cgContrElemExpression(TOperationElemPtr pBegArgList);
int		cgContrElemEquation(TOperationElemPtr pBegArgList);
int		cgContrElemInstruction(TOperationElemPtr pBegArgList);
int		cgContrElemConstructionIf(TOperationElemPtr pBegArgList);
int		cgContrElemConstructionWhile(TOperationElemPtr pBegArgList);
int		cgContrElemBlock(TOperationElemPtr pBegArgList);

// Code structures filling functions
int		cgAddNewFunctionToList(UINT uType, TIdTablePtr pFuncId);
int		cgAddNewInstructionToFunc(UINT uInstrType);
int		cgAddNewByteToCurInstruction(unsigned char cByte, char bIncrement);

// Size and absolute address location determinative functions
int		cgFuncSizeDetection();
int		cgFuncOffsetDetection();

// Absolute address substitution
int		cgCreateCallLink( TInstructionListPtr pInstruction, TIdTablePtr pFuncId );

// Uniting code functions
int		cgLinking();

// Output binary file creation
void	cgBinFileGeneration(char * cpInFileName);

#endif //INCL_ANALYSIS_CG_H

