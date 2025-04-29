#include "harness/unity.h"
#include "../src/lab.h"

// NOTE: Due to the multi-threaded nature of this project. Unit testing for this
// project is limited. I have provided you with a command line tester in
// the file app/main.cp. Be aware that the examples below do not test the
// multi-threaded nature of the queue. You will need to use the command line
// tester to test the multi-threaded nature of your queue. Passing these tests
// does not mean your queue is correct. It just means that it can add and remove
// elements from the queue below the blocking threshold.

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_create_destroy(void)
{
  queue_t q = queue_init(10);
  TEST_ASSERT_TRUE(q != NULL);
  queue_destroy(q);
}

void test_queue_dequeue(void)
{
  queue_t q = queue_init(10);
  TEST_ASSERT_TRUE(q != NULL);
  int data = 1;
  enqueue(q, &data);
  TEST_ASSERT_TRUE(dequeue(q) == &data);
  queue_destroy(q);
}

void test_queue_dequeue_multiple(void)
{
  queue_t q = queue_init(10);
  TEST_ASSERT_TRUE(q != NULL);
  int data = 1;
  int data2 = 2;
  int data3 = 3;
  enqueue(q, &data);
  enqueue(q, &data2);
  enqueue(q, &data3);
  TEST_ASSERT_TRUE(dequeue(q) == &data);
  TEST_ASSERT_TRUE(dequeue(q) == &data2);
  TEST_ASSERT_TRUE(dequeue(q) == &data3);
  queue_destroy(q);
}

void test_queue_dequeue_shutdown(void)
{
  queue_t q = queue_init(10);
  TEST_ASSERT_TRUE(q != NULL);
  int data = 1;
  int data2 = 2;
  int data3 = 3;
  enqueue(q, &data);
  enqueue(q, &data2);
  enqueue(q, &data3);
  TEST_ASSERT_TRUE(dequeue(q) == &data);
  TEST_ASSERT_TRUE(dequeue(q) == &data2);
  queue_shutdown(q);
  TEST_ASSERT_TRUE(dequeue(q) == &data3);
  TEST_ASSERT_TRUE(is_shutdown(q));
  TEST_ASSERT_TRUE(is_empty(q));
  queue_destroy(q);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void test_dequeue_empty_queue(void)
{
  queue_t q = queue_init(5);
  TEST_ASSERT_TRUE(q != NULL);
  TEST_ASSERT_NULL(dequeue(q)); // Should get NULL if empty
  queue_destroy(q);
}
void test_shutdown_empty_queue(void)
{
  queue_t q = queue_init(5);
  TEST_ASSERT_TRUE(q != NULL);
  queue_shutdown(q);
  TEST_ASSERT_TRUE(is_shutdown(q));
  TEST_ASSERT_TRUE(is_empty(q));
  TEST_ASSERT_NULL(dequeue(q)); // Should still return NULL
  queue_destroy(q);
}
void test_fill_queue_to_capacity(void)
{
  queue_t q = queue_init(3);
  TEST_ASSERT_TRUE(q != NULL);

  int a = 1, b = 2, c = 3;
  enqueue(q, &a);
  enqueue(q, &b);
  enqueue(q, &c);

  TEST_ASSERT_TRUE(dequeue(q) == &a);
  TEST_ASSERT_TRUE(dequeue(q) == &b);
  TEST_ASSERT_TRUE(dequeue(q) == &c);

  TEST_ASSERT_TRUE(is_empty(q));
  queue_destroy(q);
}
void test_enqueue_after_shutdown(void)
{
    queue_t q = queue_init(3);
    TEST_ASSERT_TRUE(q != NULL);

    int a = 1;
    queue_shutdown(q);

    // Try to enqueue after shutdown — depending on your enqueue design, it should ignore or not crash
    enqueue(q, &a);

    TEST_ASSERT_NULL(dequeue(q)); // Should still be empty
    queue_destroy(q);
}
void test_enqueue_dequeue_size_behavior(void)
{
    queue_t q = queue_init(5);
    TEST_ASSERT_TRUE(q != NULL);

    int a = 1, b = 2, c = 3;
    enqueue(q, &a);
    enqueue(q, &b);
    enqueue(q, &c);

    TEST_ASSERT_TRUE(dequeue(q) == &a);
    int *out = (int *)dequeue(q);
    TEST_ASSERT_TRUE(out == &b);
    enqueue(q, &a); // reuse slot
    TEST_ASSERT_TRUE(dequeue(q) == &c);
    TEST_ASSERT_TRUE(dequeue(q) == &a);

    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}
void test_fill_and_drain_queue(void)
{
    queue_t q = queue_init(5);
    TEST_ASSERT_TRUE(q != NULL);

    int a = 1, b = 2, c = 3, d = 4, e = 5;

    enqueue(q, &a);
    enqueue(q, &b);
    enqueue(q, &c);
    enqueue(q, &d);
    enqueue(q, &e);

    // Dequeue in the same order
    TEST_ASSERT_EQUAL_PTR(&a, dequeue(q));
    TEST_ASSERT_EQUAL_PTR(&b, dequeue(q));
    TEST_ASSERT_EQUAL_PTR(&c, dequeue(q));
    TEST_ASSERT_EQUAL_PTR(&d, dequeue(q));
    TEST_ASSERT_EQUAL_PTR(&e, dequeue(q));

    // Now the queue should be empty
    TEST_ASSERT_TRUE(is_empty(q));
    TEST_ASSERT_NULL(dequeue(q));  // nothing left to remove

    queue_destroy(q);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_create_destroy);
  RUN_TEST(test_queue_dequeue);
  RUN_TEST(test_queue_dequeue_multiple);
  RUN_TEST(test_queue_dequeue_shutdown);

  // New tests:
  RUN_TEST(test_dequeue_empty_queue);
  RUN_TEST(test_shutdown_empty_queue);
  RUN_TEST(test_fill_queue_to_capacity);
  RUN_TEST(test_enqueue_after_shutdown);
  RUN_TEST(test_enqueue_dequeue_size_behavior);
  RUN_TEST(test_fill_and_drain_queue);

  return UNITY_END();
}