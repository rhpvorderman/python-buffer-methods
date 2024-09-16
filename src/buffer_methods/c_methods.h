#include <stddef.h>
#include <stdint.h>

#define SUM_FUNC(input_type, accumulator_type)                                    \
    static accumulator_type _##input_type##_sum(input_type *numbers, size_t size) \
    {                                                                             \
        accumulator_type sum = 0;                                                 \
        for (size_t i = 0; i < size; i++) {                                       \
            sum += (accumulator_type)numbers[i];                                  \
        }                                                                         \
        return sum;                                                               \
    }

SUM_FUNC(uint8_t, uint64_t)
SUM_FUNC(uint16_t, uint64_t)
SUM_FUNC(uint32_t, uint64_t)
SUM_FUNC(uint64_t, uint64_t)

SUM_FUNC(int8_t, int64_t)
SUM_FUNC(int16_t, int64_t)
SUM_FUNC(int32_t, int64_t)
SUM_FUNC(int64_t, int64_t)

SUM_FUNC(float, double)
SUM_FUNC(double, double)
