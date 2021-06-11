#ifndef INCL_ANALYSIS_SEM_H
#define INCL_ANALYSIS_SEM_H

#include "main.h"

/** SEMANTIC ANALYSIS **/
// General functions 
void	semAnalisys();

// General functions 
int		semRetValueType(UINT uValueLength);
int		semRetTypeSizeByType(UINT uType);
void	semError(char * cpTextFrag, UINT uPosBeg,UINT uPosEnd, UINT uLineNum, char * cpErrMess);

int		semCheckContrElemFuncRet(TOperationElemPtr pBegArgList);
int		semCheckContrElemFuncCall(TOperationElemPtr pBegArgList);
int		semCheckContrElemExpression(TContrElemPtr pContrElem);
int		semCheckContrElemEquation(TOperationElemPtr pBegArgList);
int		semCheckContrElemBlock(TOperationElemPtr pBegArgList);
int		semCheckBlock(TBlockPtr pBlock);

int		semRetSregOpcodeByType(UINT uType);
int		semRetSregPrefixByType(UINT uType);
int		semRetRegRegisterOpcodeByType(UINT uType);
int		semRetRegEffectivAddrByType(UINT uType);

#endif //INCL_ANALYSIS_SEM_H

