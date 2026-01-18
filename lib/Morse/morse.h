#ifndef MORSE_H
#define MORSE_H

#include <cstdint>

namespace MorseTiming {
    constexpr int8_t DOT          =  1;
    constexpr int8_t DASH         =  3;
    constexpr int8_t INTER_SIGNAL = -1;
    constexpr int8_t INTER_LETTER = -3;
    constexpr int8_t INTER_WORD   = -7;
}
uint16_t toMorse(char *input, int8_t *&resultHead);

#endif