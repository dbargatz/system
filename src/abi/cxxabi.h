#ifndef __ABI_CXXABI_H
#define __ABI_CXXABI_H

/**
 * @brief Contains all C++ ABI functions.
 * @see Itanium C++ ABI, Section 1.3
 */
namespace __cxxabiv1 {

/**
 * @brief Panics if a pure virtual function is invoked (one without an
 * implementation).
 * 
 * @note Satisfies Itanium C++ ABI, Section 3.2.6.
 * @note Calls emitted by compiler for pure virtual functions; should not need
 * to be invoked directly.
 * @see https://stackoverflow.com/questions/920500
 */
extern "C" [[noreturn]] void __cxa_pure_virtual();

}; // namespace __cxxabiv1

#endif // __ABI_CXXABI_H
