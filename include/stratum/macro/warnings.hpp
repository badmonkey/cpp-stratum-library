#ifndef STRATUM_MACRO_WARNINGS
#define STRATUM_MACRO_WARNINGS


//
// Mark a function param as 'unused'
// void some_func( int a, int IGNORE(y) )
//
#ifdef IGNORE
// already defined
#elif defined(__GNUC__)
# define IGNORE(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define IGNORE(x) /*@unused@*/ x
#else
# define IGNORE(x) x
#endif


//
// 'Use' a variable to suppress unused var warnings.
//
#ifdef UNUSED
// already defined
#elif defined(__GNUC__)
# define UNUSED(x) (void)x
#elif defined(__LCLINT__)
# define UNUSED(x)
#else
# define UNUSED(x)
#endif


#endif //STRATUM_MACRO_WARNINGS
