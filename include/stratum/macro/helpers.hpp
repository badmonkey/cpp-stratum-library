#ifndef STRATUM_MACRO_HELPERS
#define STRATUM_MACRO_HELPERS


// copied from https://github.com/toeb/cppnamespace/blob/master/includes/cppnamespace/cppnamespace.h


// concatenates to elements
#define MACRO_CAT_IMPL(x, ...)  x ## __VA_ARGS__
#define MACRO_CAT(x, ...)  MACRO_CAT_IMPL(x, __VA_ARGS__)

#define MACRO_DEC(x)  MACRO_CAT(MACRO_DEC_, x)
#define MACRO_DEC_0 0
#define MACRO_DEC_1 0
#define MACRO_DEC_2 1
#define MACRO_DEC_3 2
#define MACRO_DEC_4 3
#define MACRO_DEC_5 4
#define MACRO_DEC_6 5
#define MACRO_DEC_7 6
#define MACRO_DEC_8 7
#define MACRO_DEC_9 8
#define MACRO_DEC_10 9


#define MACRO_EXPAND(...)  __VA_ARGS__
#define MACRO_EAT(...)
#define MACRO_EMPTY()
#define MACRO_DEFER(id) id MACRO_EMPTY()
#define MACRO_OBSTRUCT(...) __VA_ARGS__ MACRO_DEFER(MACRO_EMPTY)()

#define MACRO_COMPL(b) MACRO_CAT(MACRO_COMPL_, b)
#define MACRO_COMPL_0 1
#define MACRO_COMPL_1 0

#define MACRO_CHECK_N(x, n, ...)  n
#define MACRO_CHECK(...)  MACRO_CHECK_N(__VA_ARGS__, 0,)
#define MACRO_PROBE(x) x, 1,

#define MACRO_NOT(x)  MACRO_CHECK(MACRO_CAT(MACRO_NOT_, x))
#define MACRO_NOT_0  MACRO_PROBE(~)

#define MACRO_SELECT(c) MACRO_CAT(MACRO_SELECT_, c)
#define MACRO_SELECT_0(t, ...)  __VA_ARGS__
#define MACRO_SELECT_1(t, ...)  t

#define MACRO_BOOL(x)  MACRO_COMPL(MACRO_NOT(x))
#define MACRO_IF(c)  MACRO_SELECT(MACRO_BOOL(c))

#define MACRO_WHEN(c)  MACRO_IF(c)(MACRO_EXPAND, MACRO_EAT)


#define MACRO_EVAL(...)  MACRO_EVAL1(MACRO_EVAL1(MACRO_EVAL1(__VA_ARGS__)))
#define MACRO_EVAL1(...) MACRO_EVAL2(MACRO_EVAL2(MACRO_EVAL2(__VA_ARGS__)))
#define MACRO_EVAL2(...) MACRO_EVAL3(MACRO_EVAL3(MACRO_EVAL3(__VA_ARGS__)))
#define MACRO_EVAL3(...) MACRO_EVAL4(MACRO_EVAL4(MACRO_EVAL4(__VA_ARGS__)))
#define MACRO_EVAL4(...) MACRO_EVAL5(MACRO_EVAL5(MACRO_EVAL5(__VA_ARGS__)))
#define MACRO_EVAL5(...) __VA_ARGS__


#define MACRO_REPEAT(count, macro, ...) \
    MACRO_WHEN(count) \
    ( \
        MACRO_OBSTRUCT(MACRO_REPEAT_INDIRECT) () \
        ( \
            MACRO_DEC(count), macro, __VA_ARGS__ \
        ) \
        MACRO_OBSTRUCT(macro) \
        ( \
            MACRO_DEC(count), __VA_ARGS__ \
        ) \
    )
#define MACRO_REPEAT_INDIRECT() MACRO_REPEAT


#define MACRO_WHILE(pred, op, ...) \
    MACRO_IF(pred(__VA_ARGS__)) \
    ( \
        MACRO_DEFER(MACRO_WHILE_INDIRECT) () \
        ( \
            pred, op, op(__VA_ARGS__) \
        ), \
        __VA_ARGS__ \
    )
#define MACRO_WHILE_INDIRECT() MACRO_WHILE




#define MACRO_NUM_ARGS_(X100, X99, X98, X97, X96, X95, X94, X93, X92, X91, X90, X89, X88, X87, X86, X85, X84, X83, X82, X81, X80, X79, X78, X77, X76, X75, X74, X73, X72, X71, X70, X69, X68, X67, X66, X65, X64, X63, X62, X61, X60, X59, X58, X57, X56, X55, X54, X53, X52, X51, X50, X49, X48, X47, X46, X45, X44, X43, X42, X41, X40, X39, X38, X37, X36, X35, X34, X33, X32, X31, X30, X29, X28, X27, X26, X25, X24, X23, X22, X21, X20, X19, X18, X17, X16, X15, X14, X13, X12, X11, X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...)   N
// expands to the number of arguments passed to the macro or rather the number of commas + 1
#define MACRO_NUM_ARGS(...)  MACRO_EXPAND(MACRO_NUM_ARGS_(__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))



// returns the first element of a preprocessor argument list
#define MACRO_HEAD(x, ...)  x

// returns all elements except the first of a preprocessor argument list
#define MACRO_TAIL(x, ...)  __VA_ARGS__



#define MACRO_ISNIL(x)  MACRO_CHECK(MACRO_CAT(MACRO_ISNIL_, x))
#define MACRO_ISNIL_NIL  MACRO_PROBE(~)

/*
// Still not working
#define MACRO_LAST_STAGE(v, x, ...)  \
    MACRO_IF(MACRO_ISNIL(x)) \
    (  \
        v,  \
        MACRO_OBSTRUCT(MACRO_LAST_STAGE_INDIRECT) (x, __VA_ARGS__)  \
    )
#define MACRO_LAST_STAGE_INDIRECT  MACRO_LAST_STAGE
#define MACRO_LAST(...)  MACRO_EVAL(MACRO_LAST_STAGE(__VA_ARGS__, NIL, NIL))
*/
#define MACRO_LAST(...)  MACRO_CAT(MACRO_LAST_, MACRO_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define MACRO_LAST_1(x1) x1
#define MACRO_LAST_2(x1, x2) x2
#define MACRO_LAST_3(x1, x2, x3) x3
#define MACRO_LAST_4(x1, x2, x3, x4) x4
#define MACRO_LAST_5(x1, x2, x3, x4, x5) x5
#define MACRO_LAST_6(x1, x2, x3, x4, x5, x6) x6
#define MACRO_LAST_7(x1, x2, x3, x4, x5, x6, x7) x7
#define MACRO_LAST_8(x1, x2, x3, x4, x5, x6, x7, x8) x8
#define MACRO_LAST_9(x1, x2, x3, x4, x5, x6, x7, x8 x9) x9
#define MACRO_LAST_10(x1, x2, x3, x4, x5, x6, x7, x8 x9 x10) x10


#define MACRO_REDUCE_1(M, SEPARATOR,X)  M(X)
#define MACRO_REDUCE_2(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_1(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_3(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_2(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_4(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_3(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_5(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_4(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_6(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_5(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_7(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_6(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_8(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_7(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_9(M, SEPARATOR,...)      MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_8(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_10(M, SEPARATOR,...)     MACRO_EXPAND(M(MACRO_HEAD(__VA_ARGS__))) SEPARATOR MACRO_REDUCE_9(M,SEPARATOR,MACRO_EXPAND(MACRO_TAIL(__VA_ARGS__)))
#define MACRO_REDUCE_N(N, M, SEPARATOR, ...)  MACRO_CAT(MACRO_REDUCE_,N) (M,SEPARATOR,__VA_ARGS__)

// reduce macro expands M for every element of the VAR_ARGS and separates them by the separator given
// e.g.  #define MYOP(X) X##X 
//       MACRO_REDUCE(MYOP,_,a,b,c) will expand to aa_bb_cc
#define MACRO_REDUCE(M,SEPARATOR,...)  MACRO_REDUCE_N(MACRO_NUM_ARGS(__VA_ARGS__),M,SEPARATOR,__VA_ARGS__)


#endif //STRATUM_MACRO_HELPERS
