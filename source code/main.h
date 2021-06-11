#ifndef INCL_MAIN_H
#define INCL_MAIN_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#define TRUE    1
#define FALSE   0
#define BEG_ADDRESS 0x7C00

enum lexClassList {
    LEXCLASS_UNKNOWN = 256,
        
    //lexClassSeparators
	// Block elements
	LEXCLASS_BLOCK_LEFT_SQAURE_BRACKET,
	LEXCLASS_BLOCK_RIGHT_SQAURE_BRACKET,
	LEXCLASS_BLOCK_LEFT_CURLY_BRACKET,
	LEXCLASS_BLOCK_RIGHT_CURLY_BRACKET,
	LEXCLASS_BLOCK_LEFT_ROUND_BRACKET,
	LEXCLASS_BLOCK_RIGHT_ROUND_BRACKET,
	// Punctuation marks
	LEXCLASS_PUNCTUATION_COMMA,
	LEXCLASS_PUNCTUATION_SEMICOLUMN,
	LEXCLASS_PUNCTUATION_COLON,
	// Comparison elements
	LEXCLASS_COMPARISON_GREATER,
	LEXCLASS_COMPARISON_LESS,
	LEXCLASS_COMPARISON_EQUAL,
	// Operation symbols            
	LEXCLASS_OPERATION_MINUS,
	LEXCLASS_OPERATION_PLUS,
	LEXCLASS_OPERATION_ASTERISK,
	LEXCLASS_OPERATION_SLASH,
    
	//lexClassConstructions
	LEXCLASS_CONSTRUCTIONS_IF,
	LEXCLASS_CONSTRUCTIONS_ELSE,
	LEXCLASS_CONSTRUCTIONS_WHILE,
	//lexClassProgrammPoints
    LEXCLASS_PROGRAMM_ENTRY, //useless
    LEXCLASS_PROGRAMM_RETURN_POINT,
	LEXCLASS_PROGRAMM_ASM_ENTRY,
    //lexClassTypes
    LEXCLASS_TYPE_STRUCT,
    LEXCLASS_TYPE_VOID,		/* 0 bytes */
    LEXCLASS_TYPE_CHAR,		/* 1 byte  */
	LEXCLASS_TYPE_SHORT,	/* 2 bytes */
	LEXCLASS_TYPE_INT,		/* 4 bytes */
	//lexClassReprTableDealt
    LEXCLASS_RT_STRING_CONSTANT,
    LEXCLASS_RT_NUMERIC_CONSTANT,
	LEXCLASS_RT_IDENTIFIER,
	//lexClassIA32Registers
	LEXCLASS_IA32REGISTERS_EAX,
	LEXCLASS_IA32REGISTERS_AX,
	LEXCLASS_IA32REGISTERS_AH,
	LEXCLASS_IA32REGISTERS_AL,
	LEXCLASS_IA32REGISTERS_EBX,
	LEXCLASS_IA32REGISTERS_BX,
	LEXCLASS_IA32REGISTERS_BH,
	LEXCLASS_IA32REGISTERS_BL,
	LEXCLASS_IA32REGISTERS_ECX,
	LEXCLASS_IA32REGISTERS_CX,
	LEXCLASS_IA32REGISTERS_CH,
	LEXCLASS_IA32REGISTERS_CL,
	LEXCLASS_IA32REGISTERS_EDX,
	LEXCLASS_IA32REGISTERS_ESI,
	LEXCLASS_IA32REGISTERS_EDI,
	LEXCLASS_IA32REGISTERS_EBP,
	LEXCLASS_IA32REGISTERS_ESP,
	LEXCLASS_IA32REGISTERS_DX,
	LEXCLASS_IA32REGISTERS_DH,
	LEXCLASS_IA32REGISTERS_DL,
	
	LEXCLASS_IA32REGISTERS_CS,
	LEXCLASS_IA32REGISTERS_DS,
	LEXCLASS_IA32REGISTERS_ES,
	LEXCLASS_IA32REGISTERS_SS,
	LEXCLASS_IA32REGISTERS_FS,
	LEXCLASS_IA32REGISTERS_GS,
	
	LEXCLASS_IA32REGISTERS_SI,
	LEXCLASS_IA32REGISTERS_DI,
	LEXCLASS_IA32REGISTERS_BP,
	LEXCLASS_IA32REGISTERS_SP,
	//lexClassIA32Instructions
	LEXCLASS_IA32INSTRUCTIONS_MOV,
	LEXCLASS_IA32INSTRUCTIONS_PUSH,
	LEXCLASS_IA32INSTRUCTIONS_INT,
	LEXCLASS_IA32INSTRUCTIONS_JMP,
	
	//lexClassAccessories
	SYNCLASS_ACCESSORIES_BEGINBLOCK,
	SYNCLASS_ACCESSORIES_ENDBLOCK
};

enum semClassList{
	SEMCLASS_UNKNOWN = 1024,
	SEMCLASS_LVALUE,
	
	SEMCLASS_MEM8,
	SEMCLASS_MEM16,
	SEMCLASS_MEM32,
	SEMCLASS_IMM8,
	SEMCLASS_IMM16,
	SEMCLASS_IMM32,
	SEMCLASS_REG8,
	SEMCLASS_REG16,
	SEMCLASS_REG32,
	SEMCLASS_SYSREG,
	
	SEMCLASS_VAR8,
	SEMCLASS_VAR16,
	SEMCLASS_VAR32,
	
	// Instruction available
	SEMCLASS_INSTRUCTION_Sreg_GPReg16,
	SEMCLASS_INSTRUCTION_Sreg_MEM16,
	
	SEMCLASS_INSTRUCTION_MEM8_GPReg8,
	SEMCLASS_INSTRUCTION_MEM16_GPReg16,
	SEMCLASS_INSTRUCTION_MEM32_GPReg32,	
	SEMCLASS_INSTRUCTION_MEM8_IMM8,
	SEMCLASS_INSTRUCTION_MEM16_IMM16,
	SEMCLASS_INSTRUCTION_MEM32_IMM32,
	SEMCLASS_INSTRUCTION_REG8_REG8,
	SEMCLASS_INSTRUCTION_REG16_REG16,
	SEMCLASS_INSTRUCTION_REG32_REG32,
	SEMCLASS_INSTRUCTION_REG8_IMM8,
	SEMCLASS_INSTRUCTION_REG16_IMM16,
	SEMCLASS_INSTRUCTION_REG32_IMM32,

	SEMCLASS_INSTRUCTION_PTR16_16,

	SEMCLASS_INSTRUCTION_IMM8,
	SEMCLASS_INSTRUCTION_IMM16,
	SEMCLASS_INSTRUCTION_IMM32,

	SEMCLASS_INSTRUCTION_REG8_MEM8,
	SEMCLASS_INSTRUCTION_REG16_MEM16,
	SEMCLASS_INSTRUCTION_REG32_MEM32,
	
	SEMCLASS_PREFIX,
	SEMCLASS_INSTRUCTION,
	SEMCLASS_CONSTANT,
	SEMCLASS_FUNCTION,
	SEMCLASS_OPERATION,
	SEMCLASS_FUNCARG,
	SEMCLASS_FUNCREFTOIDTABLE,
	
	SEMCLASS_FUNCRETURN,
	SEMCLASS_EXPRESSION,
	SEMCLASS_EQUATION,
	SEMCLASS_BLOCK,
	SEMCLASS_CONSTRUCTION_ELSE,
	SEMCLASS_CONSTRUCTION_IF,
	SEMCLASS_CONSTRUCTION_WHILE
};
enum synDeclElemType{
	SYNDECLELEMTYPE_ROUND_BRACKETS = 2048,
	SYNDECLELEMTYPE_SQUARE_BRACKETS,
	SYNDECLELEMTYPE_ASTERISK
};

enum semOperationPriority{
	SEMOPERPRIORITY_EQUAL = 2148,
	SEMOPERPRIORITY_BRACKET,
	SEMOPERPRIORITY_PLUS,
	SEMOPERPRIORITY_MUL
};

enum synIdCurState{
	SYNIDCURSTATETYPE_INITIAL_VALUES = 2248,
	SYNIDCURSTATETYPE_DEREFERENCING,
	SYNIDCURSTATETYPE_INDEX_DEREFERENCING,
	SYNIDCURSTATETYPE_OPERATION
};

enum ModRMByteEffectiveAddress16{				/*	Mod	Reg/Opcode	R/M	*/
	EA16MEMLOC_BX_SI					= 0x00,	/*	00		000		000 */
	EA16MEMLOC_BX_DI					= 0x01,	/*	00		000		001 */
	EA16MEMLOC_BP_SI					= 0x02,	/*	00		000		010 */
	EA16MEMLOC_BP_DI					= 0x03,	/*	00		000		011 */
	EA16MEMLOC_SI						= 0x04,	/*	00		000		100 */
	EA16MEMLOC_DI						= 0x05,	/*	00		000		101 */
	EA16MEMLOC_DISP16					= 0x06,	/*	00		000		110 */
	EA16MEMLOC_BX						= 0x07,	/*	00		000		111 */
	EA16MEMLOC_BX_SI_DISP8				= 0x40,	/*	01		000		000 */
	EA16MEMLOC_BX_DI_DISP8				= 0x41,	/*	01		000		001 */
	EA16MEMLOC_BP_SI_DISP8				= 0x42,	/*	01		000		010 */
	EA16MEMLOC_BP_DI_DISP8				= 0x43,	/*	01		000		011 */
	EA16MEMLOC_SI_DISP8					= 0x44,	/*	01		000		100 */
	EA16MEMLOC_DI_DISP8					= 0x45,	/*	01		000		101 */
	EA16MEMLOC_BP_DISP8					= 0x46,	/*	01		000		110 */
	EA16MEMLOC_BX_DISP8					= 0x47,	/*	01		000		111 */
	EA16MEMLOC_BX_SI_DISP16				= 0x80,	/*	10		000		000 */
	EA16MEMLOC_BX_DI_DISP16				= 0x81,	/*	10		000		001 */
	EA16MEMLOC_BP_SI_DISP16				= 0x82,	/*	10		000		010 */
	EA16MEMLOC_BP_DI_DISP16				= 0x83,	/*	10		000		011 */
	EA16MEMLOC_SI_DISP16				= 0x84,	/*	10		000		100 */
	EA16MEMLOC_DI_DISP16				= 0x85,	/*	10		000		101 */
	EA16MEMLOC_BP_DISP16				= 0x86,	/*	10		000		110 */
	EA16MEMLOC_BX_DISP16				= 0x87,	/*	10		000		111 */
	EA16REG_EAX_AX_AL_MM0_XMM0			= 0xC0,	/*	11		000		000 */
	EA16REG_ECX_CX_CL_MM1_XMM1			= 0xC1,	/*	11		000		001 */
	EA16REG_EDX_DX_DL_MM2_XMM2			= 0xC2,	/*	11		000		010 */
	EA16REG_EBX_BX_BL_MM3_XMM3			= 0xC3,	/*	11		000		011 */
	EA16REG_ESP_SP_AH_MM4_XMM4			= 0xC4,	/*	11		000		100 */
	EA16REG_EBP_BP_CH_MM5_XMM5			= 0xC5,	/*	11		000		101 */
	EA16REG_ESI_SI_DH_MM6_XMM6			= 0xC6,	/*	11		000		110 */
	EA16REG_EDI_DI_BH_MM7_XMM7			= 0xC7,	/*	11		000		111 */
	//////////////////////////////////////////////////////////////////////////
	/*							For logical purposes						*/
	//////////////////////////////////////////////////////////////////////////
	EA16MEMLOCPART_BP
};

enum ModRMByteRegOpcode16{						/*	Mod	Reg/Opcode	R/M	*/		
	RO16REG_AL_AX_EAX_MM0_XMM0_0_000	= 0x00,	/*	00		000		000 */
	RO16REG_CL_CX_ECX_MM1_XMM1_1_001	= 0x08,	/*	00		001		000 */
	RO16REG_DL_DX_EDX_MM2_XMM2_2_010	= 0x10,	/*	00		010		000 */
	RO16REG_BL_BX_EBX_MM3_XMM3_3_011	= 0x18,	/*	00 		011		000 */
	RO16REG_AH_SP_ESP_MM4_XMM4_4_100	= 0x20,	/*	00 		100		000 */
	RO16REG_CH_BP_EBP_MM5_XMM5_5_101	= 0x28,	/*	00 		101		000 */
	RO16REG_DH_SI_ESI_MM6_XMM6_6_110	= 0x30,	/*	00 		110		000 */
	RO16REG_BH_DI_EDI_MM7_XMM7_7_111	= 0x38	/*	00 		111		000 */
};

enum opcodeJcc{
	JA_REL16		= 0x87,
	JB_REL16		= 0x82,
	JNZ_REL16		= 0x85,
	JZ_REL16		= 0x84
};

enum opcodeMOV{
	MOV_RM8_R8		= 0x88,
	MOV_RM16_R16	= 0x89,
	MOV_R8_RM8		= 0x8A,
	MOV_R16_RM16	= 0x8B,
	MOV_RM16_SREG	= 0x8C,
	MOV_SREG_RM16	= 0x8E,
	MOV_AL_MOFFS8	= 0xA0,
	MOV_AX_MOFFS16	= 0xA1,
	MOV_MOFFS8_AL	= 0xA2,
	MOV_MOFFS16_AX	= 0xA3,
	MOV_R8_IMM8		= 0xB0,
	MOV_R16_IMM16	= 0xB8,
	MOV_RM8_IMM8	= 0xC6,
	MOV_RM16_IMM16	= 0xC7
};

enum opcodeSUB{
	SUB_AL_IMM8		= 0x2C,
	SUB_AX_IMM16	= 0x2D,
	SUB_RM8_IMM8	= 0x80,
	SUB_RM16_IMM16	= 0x81,
	SUB_RM8_R8		= 0x28,
	SUB_RM16_R16	= 0x29,
	SUB_R8_RM8		= 0x2A,
	SUB_R16_RM16	= 0x2B
};

enum opcodePOP{
	POP_RM16		= 0x8F,
	POP_R16			= 0x58
};

enum opcodeINT{
	INT_IMM8		= 0xCD
};


enum opcodeADD{
	ADD_AL_IMM8		= 0x04,
	ADD_AX_IMM16	= 0x05,
	ADD_RM8_IMM8	= 0x80,
	ADD_RM16_IMM16	= 0x81,
	ADD_RM8_R8		= 0x00,
	ADD_RM16_R16	= 0x01,
	ADD_R8_RM8		= 0x02,
	ADD_R16_RM16	= 0x03
};

enum opcodePUSH{
	PUSH_RM16		= 0xFF,
	PUSH_R16		= 0x50,
	PUSH_IMM8		= 0x6A,
	PUSH_IMM16		= 0x68
};

enum opcodeRET{
	RET_NEAR		= 0xC3,
};

enum opcodeCALL{
	CALL_NEAR_RM16	= 0xFF,
};

enum opcodeMUL{
	MUL_RM8			= 0xF6,
	MUL_RM16		= 0xF7
}; 

enum opcodeTEST{
	TEST_RM16_R16	= 0x85
};

enum opcodeJMP{
	JMP_PTR_16_16	= 0xEA,
	JMP_REL16		= 0xE9
};

enum opcodeDIV{
	DIV_RM8			= 0xF6,
	DIV_RM16		= 0xF7
};

enum ReturnAddrSize{
	RETURNSIZE32 = 4,
	RETURNSIZE16 = 2
};

enum opcodeRegByteAddition{
	RB_AL,
	RB_CL,
	RB_DL,
	RB_BL,
	RB_AH,
	RB_CH,
	RB_DH,
	RB_BH,
};

enum opcodeRegWordAddition{
	RW_AX,
	RW_CX,
	RW_DX,
	RW_BX,
	RW_SP,
	RW_BP,
	RW_SI,
	RW_DI,
};

enum prefixSegment{
	prefCS		= 0x2E,
	prefSS		= 0x36,
	prefDS		= 0x3E,
	prefES		= 0x26,
	prefFS		= 0x64,
	prefGS		= 0x65	
};

enum opcodeSreg{				/*	Mod	Reg/Opcode	R/M	*/		
	SREG_ES_0			= 0x00,	/*	00		000		000 */
	SREG_CS_1			= 0x08,	/*	00		001		000 */
	SREG_SS_2			= 0x10,	/*	00		010		000 */
	SREG_DS_3			= 0x18,	/*	00 		011		000 */
	SREG_FS_4			= 0x20,	/*	00 		100		000 */
	SREG_GS_5			= 0x28,	/*	00 		101		000 */
};


typedef unsigned int    UINT;
typedef unsigned char   UCHAR;

typedef struct tagLexeme {
    UINT    uLexClass;		// Lexeme Class
    UINT    uPosBeg;		// Beginning position in the file
    UINT    uPosEnd;		// Ending position in the file
	UINT    uLineNum;		// Line number in the file where a lexeme appeared
    char   *cpTextFrag;		// String capture of a lexeme
} TLexeme, *TLexemePtr;

typedef struct tagLexemeListItem {
    struct tagLexemeListItem * pPrevLexemeInstance; // Previous lexeme in list
    struct tagLexemeListItem * pNextLexemeInstance; // Next lexeme in list
    TLexeme                    LexemeInstance;		// Lexeme info
} TLexemeListItem, *TLexemeListItemPtr;

// Address arithmetic 
typedef struct tagAAOperationElement{
	UINT	uDeclType;
	void	*pvValue;
} TAAOperElem, *TAAOperElemPtr;

typedef struct tagAAOperationElementList{
	TAAOperElem							AAOperElem;
	struct tagAAOperationElementList	*pPrevAAElem;
	struct tagAAOperationElementList	*pNextAAElem;
} TAAOperationElementList, *TAAOperElemListPtr;

// Identifier table structures
typedef struct tagIdTable {
	UINT	uType;
	UINT    uPosBeg;
    UINT    uPosEnd;
	UINT    uLineNum;
	UINT	uStackOffset;
    TAAOperElemListPtr		pBegIdDecl;		// Beginning of the Address Arithmetic sequence
	struct tagBlock			*pBlock;		
    struct tagIdTable       *pPrevTie;		// Previous "more global" level of identifier
    struct tagRTItemContent *pItemContent;	
	struct tagOperationElem	*pArgInstance;	// pointer to the operation element, using in function argument declaration cases;
											// pvValue of this OE will contain the stack offset of the variable
	struct tagIdTable       *pNextListItem;
	struct tagIdTable       *pPrevListItem;
} TIdTable, *TIdTablePtr;					

// Identifier current state

typedef struct tagIDCurState {
	UINT				uStateType;
	TIdTablePtr			pIdTableElem;
	TAAOperElemListPtr	pCurAAElem;
	UINT				uIndLvlCount;
	UINT				uTotalIndLevelCnt;
	void				*pvValue;
} TIDCurState, *TIDCurStatePtr;

typedef struct tagIDCurStateList {
	TIDCurStatePtr				pCurIdState;
	struct tagIDCurStateList	*pNextCurStateElem;
	struct tagIDCurStateList	*pPrevCurStateElem;
} TIDCurStateList, *TIDCurStateListPtr;


// Identifier identification error structures
typedef struct tagIdError {
    struct tagIdError * pNextError;
    char *				cpErrorMess;
	TLexeme             ErrorInstance;
} TIdError, *TIdErrorPtr;


// Representation table structures
typedef struct tagRTItemContent {
    char   *cpIdName;
    UINT	uNameLength;
	TIdTablePtr pIdTableElem;
} TRTItemContent, *TRTItemContentPtr;

typedef struct tagRTItem {
    TRTItemContent		RTItemContent;
	struct tagRTItem	*pPrevRTItem;
    struct tagRTItem	*pNextRTItem;    
} TRTItem, *TRTItemPtr;

/*
* Parse list structures
*/

typedef struct tagArgument {
    UINT				uSemClass;
	UINT				uLexClass;
	void				*pvValue;
    UINT				uBegPos;
	UINT				uEndPos;
	UINT				uStrNum;	
	TIDCurStateListPtr	pIDCurState;
} TArgument, *TArgumentPtr;

// Operation
typedef struct tagOperationElem {
    TArgument					pArgument;
	struct tagOperationElem     *pNextOperationElem;
    struct tagOperationElem		*pPrevOperationElem;
	struct tagOperationElem     *pTopOperationElem;
} TOperationElem, *TOperationElemPtr;

typedef int (* TContrElemHandlerPtr)(TOperationElemPtr);

typedef struct tagContrElem {
	UINT					uSemClass;
	TOperationElemPtr		pBegArgList;
	TContrElemHandlerPtr    pContrElemHandler;
	struct tagContrElem     *pNextContrElem;
} TContrElem, *TContrElemPtr;

typedef struct tagBlock{
	UINT				uLocVarSize;	// Size of memory which should be allocated for local variables
	UINT				uBlockOffset;	// Block offset
	UINT				uBlockSize;		// Block size
	TOperationElemPtr	pBegLocVarList;	// Local variables list
	TContrElemPtr		pBegContrList;	// 
	struct tagBlock		*pExternalBlock;// Block surrounded with
} TBlock, *TBlockPtr;

typedef struct tagFuncNode {
    UINT				uType;		// type of value to be returned
	TIdTablePtr			pFuncId;	// pointer to function identifier in ID Table
	char				*cpFuncName;// function name
	TOperationElemPtr	pBegArgList;// argument list
	TBlockPtr			pFuncBlock; // main function block
} TFuncNode, *TFuncNodePtr;

typedef struct tagParseListNode {
    TFuncNode					FuncNode;
    struct tagParseListNode     *pNextFuncNode;
    struct tagParseListNode		*pPrevFuncNode;
} TParseListNode, *TParseListNodePtr;


// Code generation structures
typedef struct tagByteList {
    unsigned char		cByte;
	struct tagByteList	*pNextByte;
	struct tagByteList	*pPrevByte;
} TByteList, *TByteListPtr;

typedef struct tagInstruction {
    UINT				uInstrType;
	UINT				uCodeSize;
	TByteListPtr		pByteList;
} TInstruction, *TInstructionPtr;

typedef struct tagInstructionList {
    TInstruction				Instruction;
	struct tagInstructionList	*pNextInstruction;
	struct tagInstructionList	*pPrevInstruction;
} TInstructionList, *TInstructionListPtr;

typedef struct tagFuncCode {
    TIdTablePtr			pFuncId;
	UINT				uType;
	UINT				uSize;
	TInstructionListPtr	pInstructionList;
} TFuncCode, *TFuncCodePtr;

typedef struct tagFuncList {
    TFuncCode			Function;
	struct tagFuncList	*pNextFuncCode;
	struct tagFuncList	*pPrevFuncCode;
} TFuncList, *TFuncListPtr;

// Code linking

typedef struct tagCallLink {
    TInstructionListPtr	pInstruction;
	TIdTablePtr			pFuncId;	
	struct tagCallLink	*pNextCallLink;
	struct tagCallLink	*pPrevCallLink;
} TCallLink, *TCallLinkPtr;

extern char bIsLValue;
extern char bIsPointer;
extern UINT uProgrammOffset;
extern UINT uCurLineNum;
extern UINT uCurType;
extern UINT uCurSemClass;
extern UINT uCurTotalIndLevelCnt;

extern TIdTable    firstIdTableElem;
extern TIdTablePtr pCurIdTableElem;
extern TIdTablePtr pCurIdTableBegBlock;
extern TIdTablePtr pCurIdTableFillingElem;

extern TLexemeListItemPtr	pCurLexListItem;
extern TLexemeListItemPtr	pBegLexList;

extern TRTItemPtr			pCurRTItem;
extern TRTItemPtr			pBegRT;
extern TRTItemPtr			pEndRT;

extern TIdErrorPtr			pIdError;
extern TIdErrorPtr			pIdErrorBegList;

extern TParseListNodePtr	pBegParseListNode; // Pointer to the first parse tree node (function tree)
extern TParseListNodePtr	pCurParseListNode; // Pointer to a current parse tree node

extern TOperationElemPtr	pCurFuncArg;
extern TBlockPtr			pCurBlock;
extern TOperationElemPtr	pCurLocVar;

extern TContrElemPtr		pCurContrElem;
extern TOperationElemPtr	pCurContrElemArg;

extern TFuncListPtr		pBegFuncList;
extern TFuncListPtr		pCurFunction;
extern TInstructionListPtr	pCurInstruction;
extern TByteListPtr		pCurByte;

extern TCallLinkPtr		pBegCallLinkList;
extern TCallLinkPtr		pCurCallLinkList;

extern TCallLink			mainCallLink;
extern TOperationElemPtr	pCurCGOperElem;

extern UINT				uTopLvlExprAsteriskCount;
extern UINT				uDeclAsteriskCount;
extern TAAOperElemListPtr	pCurIdAAE;
extern TAAOperElemListPtr	pBegAAElement;
extern TLexemeListItemPtr  pIdNameLexListItem;

extern TIDCurStateListPtr	pCurIdStateListElem;
extern TIDCurStateListPtr	pBegIdStateListElem;

#endif //INCL_MAIN_H

