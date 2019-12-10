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

#include "ptr_string.h"

#define DEREFENCE_AS(value, type) (*((type*)value))
#define CHUNK 15    // Chunk for string that will be set as token's content

typedef enum {

    TNEWLINE, // Newline

    // Function indentation
    TINDENT, // >> (i.e. two spaces)
    TDEDENT, // << (i.e. minus two spaces)
    TFIRSTINDENT, // for declarate indent on begin

    // Numbers
    TINT, // integer

    TFLOAT, //float

    // Strings
    TSTRING, // simple string
    
    // Keyword (def,else,if,None,pass,return,while)
    TKEYWORD,

    // Identificator
    TIDENTIFICATOR,

    // Block comment or literal
    TBLOCKCOMMENTORLITERAL,

    // Operators
	TADD, // +
	TSUB, // -
	TMUL, // *
	TDIV, // /
	TLT, // <
	TGT, // >
    TFLOORDIV,  // //
	TLTE, // <=
	TGTE, // >=
	TEQ, // ==
	TNE, // !=

    // Special characters
    TLEFTPAR, // (
    TRIGHTPAR, // )
    TSEMICOLON, // ;
    TCOLON, // :
    TCOMMA, // ,
    TASSIGN,    // =

    TENDOFLINE, // EOL
    TENDOFFILE, // EOF

    TERR, // Systerm error (e.g. malloc)
    TLEXERR,    // Lexical error
    TNOTHING // do nothing


} tToken_type;

// Automata states
typedef enum {

    sStart, // Not ending
    sNewLine,   // '\n'

    // Identificator and Keyword
    sIdentificatorOrKeyWord,    // Ending

    // Data types
    sInteger0,   // Ending (includes only 0)
    sInteger,   // Ending (e.g. 5906)
    sIntWithDot,    // Not ending (e.g. 10.)

    sFloat,    // Ending (e.g. 1.332 or 0.233 or 1.2e-22)
    sExponent,    // Not ending
    sExponentNumber,    // Ending (e.g. 12e32)
    sExponentOperator,    // Not ending (e.g. 1.2e-)

    // String
    sString,   // Not ending (') and getting all the characters
    sStringEscape,  // Not ending (\)
    sStringEscapeNumber1,    // Not ending first digit of hexa number
    sStringEscapeNumber2,    // Not ending second digit of hexa number
    // sString after getting (') returns token

    // Operators
    sAssignOrEqual, // Ending '=', but can be '=='
    sDivOrFloorDiv, // Ending '/', but can be '//'
    sLtOrLte, // Ending '<', but can be '<='
    sGtOrGte, // Ending '>', but can be '>='

    // Special characters
    sExclMark, // Not ending '!'
//    sEOL,
//    sEOF,

    sLineComment,  // Not ending (#)
    sBlockCommentStart1,  // Not ending (")
    sBlockCommentStart2,  // Not ending ("")

    sBlockComment,  // Not ending (everything inside comment)

    sBlockCommentEnd1,  // Not ending (")
    sBlockCommentEnd2,  // Not ending ("")

//    sLexErr = -1

} tState;

typedef struct {

    ptr_string_t value;   // Content of the token
    tToken_type type;

} tToken;

/**
 * Gets token from input
 */
tToken get_token();

#endif //IFJ19_SCANER_H
