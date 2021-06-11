#ifndef INCL_ANALYSIS_SYN_H
#define INCL_ANALYSIS_SYN_H

#include "main.h"
/** SYNTACTICAL ANALYSIS **/
// General functions 
int		synAnalysis();
int		synRetNextLCValue();
int		synGetNextLexemeClass();
void	synPrintIdErrors();
void	synError();
void	synIdError(TLexemeListItemPtr pErrorLexListItem, char * cpErrMess);
void	synPrintIdTable();

// Next lex step viewing functions
int		synIsNextTermLexeme (UINT uVerifiableLexClass);
int		synIsNextNontermType();
int		synIsNextNontermFuncImpl();
int		synIsNextNontermInstruction();
int		synIsNextNontermSegmentRegister();
int		synIsNextNontermByteGPRegister();
int		synIsNextNontermWordGPRegister();
int		synIsNextNontermDWordGPRegister();
int		synIsNextNontermValue();
int		synIsNextNontermArgEnum();

// Next lex step checking functions
int		synCheckNextTermLexeme (UINT uVerifiableLexClass);
int		synCheckNextNontermType();
int		synCheckNontermAsmBlock();
int		synCheckNextNontermImplementation();
int		synCheckNextNontermBlock();
int		synCheckNextNontermSegmentRegister();
int		synCheckNextNontermByteGPRegister();
int		synCheckNextNontermWordGPRegister();
int		synCheckNextNontermDWordGPRegister();
int		synCheckNextNontermInstruction();
int		synCheckNextNontermValue();
int		synCheckNextNontermArgEnum();
int		synCheckNextNontermTransVarBrac();
int		synCheckNextNontermTransVarMul(UINT uType);
int		synCheckNextNontermTransVarPlus(UINT uType);
int		synCheckNextNontermVarEqual();
int		synCheckNextNontermFuncDecl();
int		synCheckNextNontermVarDecl();
int		synCheckNextNontermEquation();

int		synCheckVarDeclaration();
int		synIdDeclaration();
int		synIdProperDeclaration();

//	Parse tree creating functions
int		synAddNewFuncNode (char * cpFuncName, UINT uType, TIdTablePtr pFuncId);
int		synAddNewStructNode (char * cpStructName, UINT uType);
int		synAddNewContrElemToCurBlock(UINT uSemClass, TContrElemHandlerPtr pHandler);
int		synAddNewOperElemToContrElem(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum, TIDCurStateListPtr pIDCurState);
int		synAddNewFuncArg(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum, TIDCurStateListPtr pIDCurState);
int		synAddNewFuncLocVar(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum);
int		synAddReturnContrElem();
int		synAddBlockContrElem();
TContrElemPtr		synCreateNewContrElem(TContrElemHandlerPtr pHandler);
TOperationElemPtr synCreateNewOperElem(UINT uSemClass, UINT uType, void * pvValue, UINT uBegPos, UINT uEndPos, UINT uStrNum, TIDCurStateListPtr pIDCurState);

// Identifier identification functions (these part of compilation was merged with syntactical analysis)
TRTItemPtr	synGetRTElemAddrByName( char * cpName );
TIdTablePtr synAddIdentTableElem(UINT uType, UINT uPosBeg, UINT uPosEnd, UINT uLineNum, TRTItemContentPtr pItemContent, TIdTablePtr pPrevTie, TOperationElemPtr pArgInstance, TBlockPtr pBlock, TAAOperElemListPtr pBegIdDecl);
TRTItemPtr	synCheckIdInRT(TLexemeListItemPtr pLexIdentifier);
int			synFillRT();
int			synUnFillRT();

#endif //INCL_ANALYSIS_SYN_H

