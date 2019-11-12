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

    tStart, // Starting type (e.g. newline, tab)

    // Function indentation
    Indent, // >> (i.e. two spaces)
    Dedent, // << (i.e. minus two spaces)

    // Numbers
    tInt, // integer

    tDoublePoint, // double in form (1.234)
    tDoubleExp, // double in form (10e2) or (12E-3)
    tDoublePointExp, // double in form (1.23e4)

    // Strings
    tString, // simple string
    tStringEscape, // String and escape letter inside (\)
    tStringEscapeNumber, // String with character in ASCII code (e.g. \xFF)


    // Identificator or Keyword
    tIdOrKeyWord,

    // Operators
	tAdd, // +
	tSub, // -
	tMul, // *
	tDiv, // /
	tMod, // %
	tLt, // <
	tGt, // >
	tLte, // <=
	tGte, // >=
	tEq, // ==
	tNe, // !=

    // Comments
    tStartComment, // Start of comment ' """ '
    tEndComment, // End of comment ' """ '

    // Special characters
    tLeftPar, // )
    tRightPar, // (
    tSemicolon, // ;
    tComma, // ,

    tEndOfLine, // EOL
    tEndOfFile, // EOF

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