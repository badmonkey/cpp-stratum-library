#ifndef STRATUM_COMMON_STRINGX
#define STRATUM_COMMON_STRINGX

#include "stratum/macro/namespaces.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <arpa/inet.h>


NAMESPACE_BEGIN(stratum, common, stringx)


// Modified implementation of strncpy.
// returns the new end-of-string.
// doesn't fill remainder of array with \0 (but does append _one_ if there is room)
inline char* copy_string( char* dest, const char* src, std::size_t n )
{
    size_t i;

    for ( i = 0; i < n && src[i] != '\0'; i++ )
        dest[i] = src[i];
    if ( i < n )
        dest[i] = '\0';

    return dest + i;
} // copy_string()


class fillarray
{

protected:
    fillarray( char* p, std::size_t s )
        : size_( s ), ptr_( p )
    {
    } // fillarray()


public:
    template< int N >
    fillarray( char (&instr)[N] )
        : size_( N ), ptr_( instr ), truncated_( false )
    {
        static_assert( N > 0, "Array can't be zero length" );

        ptr_[0] = '\0';
    } // fillarray()


    fillarray( fillarray&& other )
        : size_( other.size_ ), ptr_( other.ptr_ ), truncated_( other.truncated_ )
    {
        other.size_      = 0;
        other.ptr_       = NULL;
        other.truncated_ = true;
    } // fillarray()


    // Don't allow this because it is better accomplished via assignment
    //fillarray(const std::string&);


    // copy and swap idiom  handles both copy and moves
    fillarray& operator =( fillarray that )
    {
        std::swap( size_, that.size_ );
        std::swap( ptr_, that.ptr_ );
        std::swap( truncated_, that.truncated_ );

        return *this;
    } // operator =()


    // we don't know how long str so use copy_string
    fillarray& append( const char* str )
    {
        // no more room in the buffer, ignore the append
        if ( size_ == 0 )
        {
            truncated_      = true;
            return *this;
        }

        char*       newptr = copy_string( ptr_, str, size_ );
        std::size_t amt    = std::size_t( newptr - ptr_ );

        assert( amt <= size_ );

        if ( amt == size_ )
        {
            ptr_[size_ - 1] = '\0';
            truncated_      = true;
        }

        ptr_  = newptr;
        size_ = size_ - amt;

        return *this;
    } // append()


    // we know how long str is (sz shouldn't include the \0)
    fillarray& append( const char* str, std::size_t sz )
    {
        // no more room in the buffer, ignore the append
        if ( size_ == 0 )
        {
            truncated_      = true;
            return *this;
        }
        
        std::size_t amt2copy = std::min(sz, size_);
        
        std::memcpy( ptr_, str, amt2copy );
        
        ptr_ += amt2copy;
        size_ -= amt2copy;
        
        if ( size_ == 0 )
        {
            ptr_[-1]        = '\0';
            truncated_      = true;
        }
        else
        {
            *ptr_           = '\0';
        }
        
        return *this;
    } // append()


    fillarray& append( int val )
    {
        std::stringstream ss;
        ss << val;

        return append( ss.str() );
    } // append()


    fillarray& append( const std::ostringstream& stream )
    {
        return append( stream.str() );
    } // append()


    fillarray& append( const std::string& str )
    {
        return append( str.c_str(), str.length() );
    } // append()


    void set( const char* str )
    {
        append( str );
    }	// set()


    void set( const std::string& str )
    {
        append( str.c_str(), str.length() );
    } // set()


    void istruncated( bool& trunc )
    {
        trunc = truncated_;
    } // istruncated()


private:
    std::size_t size_;
    char*       ptr_;
    bool        truncated_;

}; // class fillarray



inline const std::string& get_with_default( const std::string& str, const std::string& ifempty )
{
    if ( !str.empty() )
        return str;
    else
        return ifempty;
} // get_with_default()


template< typename T >
T get_number( const std::string& input )
{
    T data;

    std::istringstream( input ) >> data;

    return data;
} // get_number()


template< typename T >
T get_number( const char* input )
{
    T data;

    std::istringstream( input ) >> data;

    return data;
} // get_number()


inline bool equals( const std::string& x, const std::string& y )
{
    return ( x == y );
} // equals()


template< int N1, int N2 >
bool equals( char (&left)[N1], char (&right)[N2] )
{
    constexpr int N = ( N1 <= N2 ? N1 : N2 );

    for ( int i = 0; i < N; i++ )
    {
        if ( left[i] != right[i] )
            return false;

        if ( left[i] == 0 && right[i] == 0 )
            return true;
    }

    return false;
} // equals()


inline bool iequals( const std::string& x, const std::string& y )
{
    std::string::const_iterator x_itr = x.begin();
    std::string::const_iterator y_itr = y.begin();

    std::string::const_iterator x_end = x.end();
    std::string::const_iterator y_end = y.end();


    while (  ( x_itr != x_end )
          && ( y_itr != y_end ) )
    {
        if ( tolower( *x_itr ) != tolower( *y_itr ) )
            return false;

        ++x_itr;
        ++y_itr;
    }

    return (  x_itr == x_end
           && y_itr == y_end
           && !x.empty() );
} // iequals()


template< int N >
bool empty( char (&txt)[N] )
{
    assert( N > 0 );

    return txt[0] == '\0';
} // empty()


inline bool empty( const std::string& x )
{
    return x.empty();
} // empty()


inline void to_lower( std::string& x )
{
    std::transform( x.begin(), x.end()
                  , x.begin()
                  , ::tolower );
} // tolower()


inline bool prefix( const char *x, const char *pre )
{
    while (  *pre
          && *x
          && ( *pre == *x ) )
    {
        x++;
        pre++;
    }
    return !*pre;
} // prefix()


inline bool prefix( const std::string& x, const std::string& pre )
{
    std::string::const_iterator x_itr   = x.begin();
    std::string::const_iterator pre_itr = pre.begin();

    std::string::const_iterator x_end   = x.end();
    std::string::const_iterator pre_end = pre.end();


    while (  ( x_itr != x_end )
          && ( pre_itr != pre_end )
          && ( *x_itr == *pre_itr ) )
    {
        ++x_itr;
        ++pre_itr;
    }

    return ( pre_itr == pre_end );
} // prefix()


inline std::string hexify( unsigned char* p, size_t len )
{
    std::ostringstream tmp;

    for ( int i = 0; i < len; ++i )
    {
        tmp << " " << std::hex << p[i];
    }

    return tmp.str();
} // hexify()


inline bool contains( const std::string& str, const std::string& needle )
{
    return std::search( str.begin(), str.end()
                      , needle.begin(), needle.end()
                        ) != str.end();
} // contains()


inline void replace_first_with( std::string& input, char needle, char replace )
{
    std::string::size_type pos = input.find( needle );
    if ( pos != std::string::npos )
        input.replace( pos, 1, 1, replace );
} // replace_first()


inline void replace_first_with( std::string& input
                              , const std::string& needle
                              , const std::string& replace )
{
    std::string::iterator itr = std::search( input.begin(), input.end()
                                           , needle.begin(), needle.end() );

    if ( itr != input.end() )
    {
        input.replace( itr, itr + needle.length(), replace );
    }
} // replace_first()


inline void remove_all( std::string& input, char x )
{
    input.erase( std::remove( input.begin()
                            , input.end()
                            , x )
               , input.end() );
} // remove_all()


typedef  std::pair< std::string, std::string >     name_value_type;
typedef
    std::function< name_value_type ( std::string, std::string ) >
                    split_processor_type;


inline name_value_type  split_identity( std::string name, std::string value )
{
    return std::make_pair( name, value );
} // split_identity()


template< typename PRED >
name_value_type split( const std::string& str
                     , PRED delim
                     , split_processor_type processor = &split_identity )
{
    std::string::const_iterator itr = std::find_if( str.begin()
                                                  , str.end()
                                                  , delim );

    if ( itr == str.end() )
    {
        return processor( std::string( str ), std::string() );
    }

    return processor( std::string( str.begin(), itr )
                    , std::string( itr + 1, str.end() ) );

} // split()


inline std::string make_string( const struct in_addr& addr )
{
    return std::string( inet_ntoa( addr ) );
} // make_string()


template< int N >
bool is_one_of( std::string tst, const char* ( &data )[N] )
{
    to_lower( tst );

    for ( int i = 0; i < N; ++i )
    {
        if ( tst == data[i] )
            return true;
    }
    return false;
} // is_one_of()


template< int N >
int index_of( std::string tst, const char* ( &data )[N] )
{
    to_lower( tst );

    for ( int i = 0; i < N; ++i )
    {
        if ( tst == data[i] )
            return i;
    }
    return -1;
} // index_of()


inline std::string preview( const std::string& txt, size_t numchars )
{
    assert( numchars > 4 );

    if ( txt.length() <= numchars )
        return txt;

    return txt.substr( 0, numchars - 3 ) + std::string( "..." );

} // preview()


NAMESPACE_END(stratum, common, stringx)

#endif // STRATUM_COMMON_STRINGX
