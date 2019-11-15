#include <gtest/gtest.h>
#include <string>

extern "C" {
#include <scaner.h>
}

/**
 * TESTS FOR LEXER
 * RUN IT WITH stdin input which is written below
 * ############INPUT##############
 * ./(testname) <5+9 5.93 + 2129 * ((0.59 - 0.50) - 0.09) / 7 5     -                   7.059e3                     5         99.5634                448646             8.06    , ;
 *
 */

/**
 * Help function, which expect token.type as INT
 * and expect token.value == expected param
 * @param expected -> expected value from function getToken()
 */
void EXPECT_INT(int expected){
    auto token = getToken();
    EXPECT_EQ(token.type, TINT);
    EXPECT_EQ(*((int*)token.value), expected);
}

/**
 * Help function, which expect token.type as FLOAT
 * and expect token.value == expected param
 * @param expected -> expected value from function getToken()
 */
void EXPECT_FLOAT(float expected){
    auto token = getToken();
    EXPECT_EQ(token.type, TFLOAT);
    EXPECT_EQ(*((float*)token.value), expected);
}

std::string load_input() {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
    }

    return line;
}

namespace {

    auto input = load_input();

    class LexerMath : public testing::Test {
    protected:
        tToken token;
    };

    class LexerSpecialChars : public testing::Test {
    protected:
        tToken token;
    };

    TEST_F(LexerMath, SimpleAdd){
        //INPUT: <(5+9)
        EXPECT_INT(5);

        token = getToken();
        EXPECT_EQ(token.type, TADD);

        EXPECT_INT(9);
    }

    TEST_F(LexerMath, Expr){
        //INPUT: <(5.93 + 2129 * ((0.59 - 0.50) - 0.09) / 7)
        EXPECT_FLOAT(5.93);

        token = getToken();
        EXPECT_EQ(token.type, TADD);

        EXPECT_INT(2129);

        token = getToken();
        EXPECT_EQ(token.type, TMUL);

        token = getToken();
        EXPECT_EQ(token.type, TLEFTPAR);

        token = getToken();
        EXPECT_EQ(token.type, TLEFTPAR);

        EXPECT_FLOAT(0.59);

        token = getToken();
        EXPECT_EQ(token.type, TSUB);

        EXPECT_FLOAT(0.50);

        token = getToken();
        EXPECT_EQ(token.type, TRIGHTPAR);

        token = getToken();
        EXPECT_EQ(token.type, TSUB);

        EXPECT_FLOAT(0.09);

        token = getToken();
        EXPECT_EQ(token.type, TRIGHTPAR);

        EXPECT_INT(7);
    }

    TEST_F(LexerMath, Spaces){
        //INPUT: <(5     -                   7.059e3)
        EXPECT_INT(5);

        token = getToken();
        EXPECT_EQ(token.type, TADD);

        EXPECT_FLOAT(7.059e3);
    }

    TEST_F(LexerMath, SpacesBefore){
        EXPECT_INT(                    5);
        EXPECT_FLOAT(        99.5634    );
        EXPECT_INT(               448646);
        EXPECT_FLOAT(            8.06   );
    }

    TEST_F(LexerSpecialChars, ComAndSemiCol){
        //INPUT: ,
        token = getToken();
        EXPECT_EQ(token.type, TCOMMA);

        //INPUT: ;
        token = getToken();
        EXPECT_EQ(token.type, TSEMICOLON);
    }

    TEST_F(LexerSpecialChars, TryToBreak){
        //End of LINE + FILE Test
        token = getToken();
        EXPECT_EQ(token.type, TENDOFLINE);

        token = getToken();
        EXPECT_EQ(token.type, TENDOFFILE);
    }

    //end of tests
}
