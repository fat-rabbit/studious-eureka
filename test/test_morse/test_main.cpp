#include <unity.h>
#include <morse.h>
#include <cstdint>

void setUp(void) {
}

void tearDown(void) {
}

void test_toMorse_handles_simple_letter(void) {
    // Arrange
    char input[] = "a";
    int8_t *result = nullptr;

    // Act
    uint16_t length = toMorse(input, result);

    // Assert
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_UINT16(4, length); // 'a' is Dot (1), Pause (-1), Dash (3), LetterPause (-3)
    TEST_ASSERT_EQUAL_INT8(MorseTiming::DOT, result[0]);
    TEST_ASSERT_EQUAL_INT8(MorseTiming::INTER_SIGNAL, result[1]);
    TEST_ASSERT_EQUAL_INT8(MorseTiming::DASH, result[2]);
    TEST_ASSERT_EQUAL_INT8(MorseTiming::INTER_LETTER, result[3]);

    delete[] result; // Important: Clean up the heap
}

void test_toMorse_handles_spaces_correctly(void) {
    // Arrange
    char input[] = " ";
    int8_t *result = nullptr;

    // Act
    uint16_t length = toMorse(input, result);

    // Assert
    TEST_ASSERT_EQUAL_UINT16(1, length);
    TEST_ASSERT_EQUAL_INT8(MorseTiming::INTER_WORD, result[0]);

    delete[] result;
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_toMorse_handles_simple_letter);
    RUN_TEST(test_toMorse_handles_spaces_correctly);
    return UNITY_END();
}