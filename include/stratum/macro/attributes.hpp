#ifndef STRATUM_MACRO_ATTRIBUTES
#define STRATUM_MACRO_ATTRIBUTES


//
// void function() NORETURN { }
// NORETURN void function() {}
//
#ifdef NORETURN
// already defined
#elif defined(__GNUC__)
# define NORETURN( __attribute__((noreturn))
#elif defined(__LCLINT__)
# define NORETURN /*@unused@*/
#else
# define NORETURN
#endif


//
// void function() DEPRECATED { }
// DEPRECATED void function() { }
//
#ifdef DEPRECATED
// already defined
#elif defined(__GNUC__)
# define DEPRECATED __attribute__((warning("This function has been deprecated")))
#elif defined(__LCLINT__)
# define DEPRECATED /*@unused@*/
#else
# define DEPRECATED
#endif


// __attribute__((constructor))


// @todo PURE CONST

/*
http://www.codesynthesis.com/~boris/blog/2012/04/18/cxx11-generalized-attributes/
http://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html#Attribute-Syntax
http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
http://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html
*/

#endif //STRATUM_MACRO_ATTRIBUTES