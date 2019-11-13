#include <gtest/gtest.h>
#include <deque>

extern "C" {
#include <deque.h>
}

namespace {
class EmptyDeque : public testing::Test {
 protected:
  deque_t deque;
  int testElement = 5;
  std::deque<int> cppDeque;

  void SetUp() override {
    deque = deque_init();
    testElement = 5;
  }
};

class NonEmptyDeque : public testing::Test {
 protected:
  deque_t deque = nullptr;
  std::deque<int> cppDeque;

  void SetUp() override {
    if (deque != nullptr) {
      deque_destroy(deque);
    }

    deque = deque_init();
    cppDeque.clear();
  }
};

TEST_F(EmptyDeque, PopBack) {
  EXPECT_EQ(deque_pop_back(deque), ERROR_INTERNAL);
}

TEST_F(EmptyDeque, PopFront) {
  EXPECT_EQ(deque_pop_front(deque), ERROR_INTERNAL);
}

TEST_F(EmptyDeque, PushBack) {
  EXPECT_EQ(deque_back(deque), nullptr);
  EXPECT_EQ(deque_push_back(deque, &testElement), 0);
  EXPECT_EQ(*((int *)deque_back(deque)), testElement);
}

TEST_F(EmptyDeque, PushFront) {
  EXPECT_EQ(deque_front(deque), nullptr);
  EXPECT_EQ(deque_push_front(deque, &testElement), 0);
  EXPECT_EQ(*((int *)deque_front(deque)), testElement);
}

TEST_F(EmptyDeque, Iterator) {
  EXPECT_TRUE(iterator_equal(deque_begin(deque), deque_end(deque)));
}

TEST_F(EmptyDeque, ReversedIterator) {
  EXPECT_TRUE(iterator_equal(deque_rbegin(deque), deque_rend()));
}

TEST_F(NonEmptyDeque, PushBack) {
  int values[5];

  for (size_t i = 0; i < 5; i++) {
    values[i] = i;
    deque_push_back(deque, &values[i]);
    cppDeque.push_back(i);
  }

  for (size_t i = 0; i < 5; i++) {
    int *data = (int *)deque_back(deque);
    EXPECT_EQ(*data, cppDeque.back());
    deque_pop_back(deque);
    cppDeque.pop_back();
  }
}

TEST_F(NonEmptyDeque, PushFront) {
  int values[5];

  for (size_t i = 0; i < 5; i++) {
    values[i] = i;
    deque_push_front(deque, &values[i]);
    cppDeque.push_front(i);
  }

  for (size_t i = 0; i < 5; i++) {
    int *data = (int *)deque_front(deque);
    EXPECT_EQ(*data, cppDeque.front());
    deque_pop_front(deque);
    cppDeque.pop_front();
  }
}

TEST_F(NonEmptyDeque, Iterator) {
  int values[5];

  for (size_t i = 0; i < 5; i++) {
    values[i] = i;
    deque_push_front(deque, &values[i]);
    cppDeque.push_front(i);
  }

  iterator_t ite = deque_begin(deque);
  auto ite1 = cppDeque.begin();

  for (; !iterator_equal(ite, deque_end(deque)) || ite1 != cppDeque.end();
       ite1++, ite = iterator_next(ite)) {
    int *data = (int *)deque_iterator_value(ite);
    EXPECT_EQ(*data, *ite1);
  }
}

TEST_F(NonEmptyDeque, ReversedIterator) {
  int values[5];

  for (size_t i = 0; i < 5; i++) {
    values[i] = i;
    deque_push_front(deque, &values[i]);
    cppDeque.push_front(i);
  }

  iterator_t ite = deque_rbegin(deque);
  auto ite1 = cppDeque.rbegin();

  for (; !iterator_equal(ite, deque_rend()) && ite1 != cppDeque.rend();
       ite1++, ite = iterator_prev(ite)) {
    int *data = (int *)deque_iterator_value(ite);
    EXPECT_EQ(*data, *ite1);
  }
}

}  // namespace
