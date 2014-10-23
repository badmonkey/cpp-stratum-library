#ifndef STRATUM_MACRO_STATIC_INITIALIZER
#define STRATUM_MACRO_STATIC_INITIALIZER

//
// STATIC_INITIALIZER(function_name)
// {
//   // some code
// }
//
#if defined(_MSC_VER)

#pragma section(".CRT$XCU",read)
#define STATIC_INITIALIZER(FUNC)                                          \
  static void __cdecl FUNC(void);                                         \
  __declspec(allocate(".CRT$XCU")) void (__cdecl*FUNC##_)(void) = FUNC;   \
  static void __cdecl FUNC(void)

#else

#define STATIC_INITIALIZER(FUNC)                                          \
  static void FUNC(void) __attribute__((constructor));                    \
  static void FUNC(void)

#endif


#endif // STRATUM_MACRO_STATIC_INITIALIZER