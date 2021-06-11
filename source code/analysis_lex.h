#ifndef INCL_ANALYSIS_LEX_H
#define INCL_ANALYSIS_LEX_H

#include "main.h"
/** LEXICAL ANALISYS **/
// General functions 
int		lexAnalysis(FILE * pInFile);
int		lexAddLexemeListItem ( char * cOccurrence, UINT uPosBeg, UINT uPosEnd);
int		lexRetLexClassByOccurence(char * cOccurrence);
int		lexPrintLexemeList ( TLexemeListItemPtr pLexeme, char bIsScreenPrint );

// Functions for work with representation table 
int		lexIsItemInRT(char *cpItem, UINT uPosBeg, UINT uPosEnd);
int		lexAddRTItem (char *cpItem, UINT uPosBeg, UINT uPosEnd);
void	lexPrintRT();

// Initial text analysis
int		lexIsHex(char cSymbol);
int		lexIsOccurrenceHex(char * cOccurrence);
int		lexIsNotSepSymb(char cSymbol);

#endif //INCL_ANALYSIS_LEX_H

