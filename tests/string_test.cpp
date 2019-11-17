#include <gtest/gtest.h>
#include <string>

extern "C"
{
#include <ptr_string.h>
}

namespace
{
class BlankString : public testing::Test
{
protected:
    ptr_string_t blank_string;

    void SetUp() override
    {
        blank_string = ptr_string_new_with_length(8);
    }
    void TearDown() override
    {
        ptr_string_delete(blank_string);
    }
};

class EmptyString : public testing::Test
{
protected:
    ptr_string_t empty_string;

    void SetUp() override
    {
        empty_string = ptr_string_new();
    }
    void TearDown() override
    {
        ptr_string_delete(empty_string);
    }
};

class NonEmptyString : public testing::Test
{
protected:
    ptr_string_t non_empty_string;
    const char * const str = "This Is Test"; 
    const size_t size = 12; 

    void SetUp() override
    {
        non_empty_string = ptr_string(NonEmptyString::str);
    }
    void TearDown() override
    {
        ptr_string_delete(non_empty_string);
    }
};

TEST_F(BlankString, DefaultValues) {
    ASSERT_EQ(ptr_string_length(blank_string), 8);
}

TEST_F(BlankString, InitWithLength)
{
    ptr_string_set_char(blank_string, 3, 'a');
    EXPECT_EQ(ptr_string_get_char(blank_string, 3), 'a');
}

TEST_F(BlankString, Clone)
{
    ptr_string_set_char(blank_string, 6, 'c');
    ptr_string_set_char(blank_string, 2, 'f');

    auto string_to_clone = ptr_string_clone(blank_string);

    EXPECT_EQ(ptr_string_length(string_to_clone), 8);
    EXPECT_EQ(ptr_string_get_char(string_to_clone, 6), 'c');
    EXPECT_EQ(ptr_string_get_char(string_to_clone, 2), 'f');
    ptr_string_delete(string_to_clone);
}

TEST_F(EmptyString, DefaultValues)
{
    ASSERT_EQ(ptr_string_length(empty_string), 0);
    auto c_str = ptr_string_c_string(empty_string);
    EXPECT_STREQ(c_str, "");
    free(c_str);
}

TEST_F(EmptyString, CConstructor)
{
    auto str = ptr_string("");
    ASSERT_EQ(ptr_string_length(str), 0);
    auto c_str = ptr_string_c_string(empty_string);
    EXPECT_STREQ(c_str, "");
    free(c_str);
    ptr_string_delete(str);
}

TEST_F(EmptyString, Clone)
{
    auto string_to_clone = ptr_string_clone(empty_string);
    EXPECT_EQ(ptr_string_length(string_to_clone), 0);
    ptr_string_delete(string_to_clone);
}

TEST_F(EmptyString, Length)
{
    EXPECT_EQ(ptr_string_length(empty_string), 0);
}

TEST_F(EmptyString, Capacity)
{
    EXPECT_EQ(ptr_string_capacity(empty_string), 15);
}

TEST_F(EmptyString, CString)
{
    auto c_str = ptr_string_c_string(empty_string);
    EXPECT_STREQ(c_str, "");
    free(c_str);
}

TEST_F(EmptyString, Concat)
{
    auto helper = ptr_string("hello world");
    ptr_string_concat(empty_string, helper);
    auto c_str = ptr_string_c_string(empty_string);
    EXPECT_STREQ(c_str, "hello world");
    free(c_str);
    ptr_string_delete(helper);
}

TEST_F(EmptyString, Insert)
{
    auto helper = ptr_string("hello world");
    ASSERT_NE(ptr_string_insert(empty_string, helper), nullptr);
    auto c_str = ptr_string_c_string(empty_string);
    EXPECT_STREQ(c_str, "hello world");
    free(c_str);
    ptr_string_delete(helper);
}

TEST_F(EmptyString, Append)
{
    ptr_string_append(empty_string, 'h');
    auto c_str = ptr_string_c_string(empty_string);
    EXPECT_STREQ(c_str, "h");
    free(c_str);
}

TEST_F(EmptyString, Substring)
{
    // Dont know how to test asserts
    //
    // auto sub = ptr_string_substring(empty_string, 0, 0);
    // auto c_str = ptr_string_c_string(sub);
    // EXPECT_STREQ(c_str, "");
    // EXPECT_DEATH({ptr_string_substring(empty_string, 0, 1);}, "Error on line .* of ptr_string_substring()");
    // EXPECT_DEATH({ptr_string_substring(empty_string, 1, 0);}, "Error on line .* of ptr_string_substring()");
    // EXPECT_DEATH({ptr_string_substring(empty_string, 1, 1);}, "Error on line .* of ptr_string_substring()");
    // ptr_string_delete(sub);
    // free(c_str);
}

TEST_F(EmptyString, FindChar)
{
    EXPECT_EQ(ptr_string_find_char(empty_string, 'c'), -1);
    EXPECT_EQ(ptr_string_find_char(empty_string, '\0'), -1);
}

TEST_F(EmptyString, FindCharFromIndex)
{
    EXPECT_EQ(ptr_string_find_char_from_index(empty_string, 'c', 2), -1);
    EXPECT_EQ(ptr_string_find_char_from_index(empty_string, '\0', 2), -1);
}

// Non Empty tests

TEST_F(NonEmptyString, DefaultValues)
{
    ASSERT_EQ(ptr_string_length(non_empty_string), NonEmptyString::size);
    auto c_str = ptr_string_c_string(non_empty_string);
    EXPECT_STREQ(c_str, NonEmptyString::str);
    free(c_str);
}

TEST_F(NonEmptyString, CConstructor)
{
    auto str = ptr_string(NonEmptyString::str);
    ASSERT_EQ(ptr_string_length(str), NonEmptyString::size);
    auto c_str = ptr_string_c_string(non_empty_string);
    EXPECT_STREQ(c_str, NonEmptyString::str);
    free(c_str);
    ptr_string_delete(str);
}

TEST_F(NonEmptyString, Clone)
{
    auto string_to_clone = ptr_string_clone(non_empty_string);
    EXPECT_EQ(ptr_string_length(non_empty_string), NonEmptyString::size);
    ptr_string_delete(string_to_clone);
}

TEST_F(NonEmptyString, Length)
{
    EXPECT_EQ(ptr_string_length(non_empty_string), NonEmptyString::size);
}

TEST_F(NonEmptyString, Capacity)
{
    EXPECT_EQ(ptr_string_capacity(non_empty_string), NonEmptyString::size);
}

TEST_F(NonEmptyString, CString)
{
    auto c_str = ptr_string_c_string(non_empty_string);
    EXPECT_STREQ(c_str, NonEmptyString::str);
    free(c_str);
}

TEST_F(NonEmptyString, Concat)
{
    auto helper = ptr_string("hello world");
    ptr_string_concat(non_empty_string, helper);
    auto c_str = ptr_string_c_string(non_empty_string);
    EXPECT_STREQ(c_str, (std::string(NonEmptyString::str) + std::string("hello world")).c_str());
    free(c_str);
    ptr_string_delete(helper);
}

TEST_F(NonEmptyString, Insert)
{
    auto helper = ptr_string("hello world");
    ASSERT_NE(ptr_string_insert(non_empty_string, helper), nullptr);
    auto c_str = ptr_string_c_string(non_empty_string);
    EXPECT_STREQ(c_str, (std::string("hello world") + std::string(NonEmptyString::str)).c_str());
    free(c_str);
    ptr_string_delete(helper);
}

TEST_F(NonEmptyString, Append)
{
    ptr_string_append(non_empty_string, 'h');
    auto c_str = ptr_string_c_string(non_empty_string);
    EXPECT_STREQ(c_str, (std::string("hello world") + 'h').c_str());
    free(c_str);
}

TEST_F(NonEmptyString, Substring)
{
    auto sub = ptr_string_substring(non_empty_string, 3, 7);
    auto c_str = ptr_string_c_string(sub);
    EXPECT_STREQ(c_str, std::string(NonEmptyString::str).substr(3, 4).c_str());
    ptr_string_delete(sub);
    free(c_str);

    sub = ptr_string_substring(non_empty_string, 2, 9);
    c_str = ptr_string_c_string(sub);
    EXPECT_STREQ(c_str, std::string(NonEmptyString::str).substr(2, 7).c_str());
    ptr_string_delete(sub);
    free(c_str);

    sub = ptr_string_substring(non_empty_string, 0, NonEmptyString::size);
    c_str = ptr_string_c_string(sub);
    EXPECT_STREQ(c_str, std::string(NonEmptyString::str).substr(0, NonEmptyString::size).c_str());
    ptr_string_delete(sub);
    free(c_str);
}

TEST_F(NonEmptyString, FindChar)
{
    EXPECT_EQ(ptr_string_find_char(non_empty_string, 'h'), 0);
    EXPECT_EQ(ptr_string_find_char(non_empty_string, '\0'), -1);
    EXPECT_EQ(ptr_string_find_char(non_empty_string, 'l'), 2);
}

TEST_F(NonEmptyString, FindCharFromIndex) {
    EXPECT_EQ(ptr_string_find_char_from_index(non_empty_string, 'h', 0), 0);
    EXPECT_EQ(ptr_string_find_char_from_index(non_empty_string, '\0', 0), -1);
    EXPECT_EQ(ptr_string_find_char_from_index(non_empty_string, 'l', 3), 3);
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
} // namespace
