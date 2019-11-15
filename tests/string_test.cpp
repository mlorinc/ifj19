#include <gtest/gtest.h>

extern "C" {
#include <ptr_string.h>
}

namespace{
    class EmptyString{
        ptr_string_t empty_string;
    };

    class NonEmptyString{
        ptr_string_t non_empty_string;
        ptr_string_t non_empty_string = malloc(sizeof(ptr_string_t));
        ptr_string_t non_empty_string = malloc(13);
        ptr_string_t non_empty_string->buffer = "This Is Test";
        ptr_string_t non_empty_string->length = 13;
    };

    TEST_F(EmptyString, StringInitWithLength){
        empty_string = ptr_string_new_with_length(8);
        empty_string->buffer[3] = 'a';

        EXPECT_EQ(empty_string->length, 8);
        EXPECT_EQ(empty_string->buffer[3], 'a');
    }

    TEST_F(EmptyString, CloneEmptyString){
        empty_string = ptr_string_new_with_length(8);
        empty_string->buffer[6] = 'c';
        empty_string->buffer[2] = 'f';
        ptr_string_t string_to_clone = ptr_string_clone(empty_string);

        EXPECT_EQ(string_to_clone->length, 8);
        EXPECT_EQ(string_to_clone->buffer[6], 'c');
        EXPECT_EQ(string_to_clone->buffer[2], 'f');
    }

    TEST_F(EmptyString, CloneEmptyString){
        empty_string = ptr_string_new_with_length(8);
    }

    /**
     * M
     * O
     * R
     * E
     *
     * T
     * E
     * S
     * T
     * S
     *
     * S
     * O
     * O
     * N
     */

    //end of ptr_string tests
}