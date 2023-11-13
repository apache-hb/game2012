#pragma once

#include <iostream>
#include <format>

template<typename... A>
void info(const char* fmt, A... args) {
    std::cout << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
}

template<typename... A>
void error(const char* fmt, A... args) {
    std::cerr << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
    std::exit(1);
}
#define SM_ASSERTF(EXPR, ...) if (!(EXPR)) { error(__VA_ARGS__); }