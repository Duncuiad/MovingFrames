#pragma once

#include <cassert>
#include <format>
#include <iostream>

#define _ASSERT_INTERNAL(CONDITION, MESSAGE)                                                                           \
    if (!(CONDITION))                                                                                                  \
    {                                                                                                                  \
        std::cerr << "ASSERT FAILED: " << MESSAGE << std::endl;                                                        \
        assert(CONDITION);                                                                                             \
    }

#ifdef NDEBUG
#define ASSERT(CONDITION, MESSAGE_FMT, ...) 0
#else
#define ASSERT(CONDITION, MESSAGE_FMT, ...) _ASSERT_INTERNAL(CONDITION, std::format(MESSAGE_FMT, __VA_ARGS__).c_str())
#endif
