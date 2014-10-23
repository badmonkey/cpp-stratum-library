#ifndef STRATUM_COMMON_MEMORYX
#define STRATUM_COMMON_MEMORYX

#include "stratum/macro/namespaces.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <memory>


NAMESPACE_BEGIN(stratum, common, memoryx)


template< int N >
void clear_char_array( char (&buffer)[N] )
{
    memset( buffer, 0, N );
} // clear_char_array()


template< typename T >
void clear( T* t )
{
    *t = T();
} // clear


template< typename T >
void clear( T& t )
{
    t = T();
} // clear()


template< int N >
void for_each( char (&buffer)[N], std::function< void (char&) > func )
{
    for ( int i = 0; i < buffer; i++ )
    {
        func( buffer[i] );
    }
} // for_each()


template< typename T >
void copy( const T& src, T& dest )
{
    dest = src;
} // copy()


template< typename T >
bool is_all_zeros( const T& t )
{
    T tmp = T();
    return memcmp( &t, &tmp, sizeof( T ) ) == 0;
} // is_all_zeros()


NAMESPACE_END(stratum, common, memoryx)

#endif // STRATUM_COMMON_MEMORYX
