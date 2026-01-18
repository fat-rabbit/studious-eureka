#include "morse.h"
#include <iostream>
#include <cstring>
#include "alphabet.h"

uint16_t toMorse(char *input, int8_t *&resultHead)
{
    uint16_t index = 0;
    uint16_t inputLen = strlen(input);

    // Each symbol contains up to 4 signals, they are spaced by short signals so logic here is 4 * 2;
    uint16_t size = inputLen * 8;

    int8_t *result = new int8_t[size];
    resultHead = result;

    while (*input != '\0')
    {
        char letter = *input;
        input++;

        if (letter == ' ')
        {
            if (index > 0)
            {
                --result;
                --index;
            }

            *result = MorseTiming::INTER_WORD;

            ++result;
            ++index;

            continue;
        }

        // Handle uppercase
        if (letter >= 'A' && letter <= 'Z')
            letter |= 0x20;

        // Skip special symbols
        if (letter < 'a' || letter > 'z')
            continue;

        uint8_t letterIdx = letter - 'a';
        uint8_t pattern = DICTIONARY[letterIdx];
        uint8_t signalsCount = LEN[letterIdx];

        while (signalsCount > 0)
        {
            bool isDot = (pattern >> (signalsCount - 1)) & 1;

            *result = isDot ? MorseTiming::DOT : MorseTiming::DASH;
            result++;
            index++;

            *result = MorseTiming::INTER_SIGNAL;
            result++;
            index++;

            --signalsCount;
        }
        --result;
        --index;

        *result = MorseTiming::INTER_LETTER;
        ++index;
        ++result;
    }

    return index;
}