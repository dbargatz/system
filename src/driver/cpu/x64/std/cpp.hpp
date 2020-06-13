/**
 * @file
 * @brief Implementations of functions normally provided by the C++ runtime and
 * required by the compiler.
 */
#ifndef _STD_CPP_HPP
#define _STD_CPP_HPP

/**
 * @brief Panics if a pure virtual function is invoked (one without an
 * implementation).
 * 
 * @note Calls emitted by compiler for pure virtual functions; should not need
 * to be invoked directly.
 * @note Should be included via compiler command-line; does not need to be
 * #included in source.
 * @see https://stackoverflow.com/questions/920500
 */
extern "C" void __cxa_pure_virtual();

#endif // _STD_CPP_HPP
