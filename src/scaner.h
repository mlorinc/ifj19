/**
 * IFJ2019 @ VUT FIT Brno
 * 09.11.2019
 *
 * Variant 2
 * Team: 054
 *
 * Authors:
 * Marian Lorinc (xlorin01)
 * Lukas Javorsky (xjavor20)
 * Patrik Ondriga (xondri08)
 * Peter Vinarcik (xvinar00)
 *
 * File: scaner.h
 *
 */

#ifndef IFJ19_SCANER_H
#define IFJ19_SCANER_H


#define CHUNK 15    // Chunk for string that will be set as token's content

typedef enum {

    THEADER, // Header (.ifj19)

    TNEWLINE, // Newline

    // Function indentation
    TINDENT, // >> (i.e. two spaces)
    TDEDENT, // << (i.e. minus two spaces)

    // Numbers
    TINT, // integer

    TFLOAT, //float

    // Strings
    TSTRING, // simple string
    
    // Keyword (def,else,if,None,pass,return,while)
    TKEYWORD,

    // Identificator
    TIDENTIFICATOR,

    // Operators
	TADD, // +
	TSUB, // -
	TMUL, // *
	TDIV, // /
	TMOD, // %
	TLT, // <
	TGT, // >
	TLTE, // <=
	TGTE, // >=
	TEQ, // ==
	TNE, // !=

    // Special characters
    TLEFTPAR, // )
    TRIGHTPAR, // (
    TSEMICOLON, // ;
    TCOMMA, // ,

    TENDOFLINE, // EOL
    TENDOFFILE, // EOF

} tToken_type;

// Automata states
typedef enum {

    sStart, // Not ending (Can be tab, newline)
    
    // Identificator and Keyword
    sIdentificator, // Ending (Can start with "_")
    sIdentificatorOrKeyWord,    // Not ending
    sKeyWord,   // Ending

    // Data types
    sInteger0,   // Ending (includes only 0)
    sInteger,   // Ending (e.g. 5906)

    sFloat,    // Ending (e.g. 1.332)
    sFloatDot,  // Not ending (.)
    sFloatExponent,    // Not ending
    sFloatExponentOperator,    // Not ending (e.g. 1.2e-)
    sFloatExpondent,    // Ending

    // String
    sStringStart,   // Not ending (')
    sStringEscape,  // Not ending (\)
    sStringEscapeNumber,    // Not ending ([\x27] it is ['])
    sString,    // Ending

    // Operators
    sAssign, // =
	sAdd, // +
	sSub, // -
	sMul, // *
	sDiv, // /
	sMod, // %
	sLt, // <
	sGt, // >
	sLte, // <=
	sGte, // >=
	sEq, // ==
    sExclMark, // Not ending (!)
	sNe, // !=

    // Special characters
    sLeftPar, // )
    sRightPar, // (
    sSemicolon, // ;
    sComma, // ,

    sEOL,
    sEOF,

    sLineComment,  // Not ending (#)
    sBlockCommentStart1,  // Not ending (")
    sBlockCommentStart2,  // Not ending ("")
    sBlockCommentStart3,  // Not ending (""")

    sBlockComment,  // Not ending (everything inside comment)

    sBlockCommentEnd1,  // Not ending (")
    sBlockCommentEnd2,  // Not ending ("")
    sBlockCommentEnd3,  // Ending     (""")

    sLexErr = -1,

} tState;

typedef struct {

    void * value;   // Content of the token
    tToken_type type;

} tToken;

/**
 * Gets token from input
 */
tToken get_token();

#endif //IFJ19_SCANER_H