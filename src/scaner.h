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


typedef enum {

    // Automata states
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
    
    // Identificator
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

typedef struct {

    void * value;   // Content of the token
    tToken_type type;

} tToken;

/**
 * Gets token from input
 */
tToken getToken();

#endif //IFJ19_SCANER_H