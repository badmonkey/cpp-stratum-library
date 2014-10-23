#ifndef STRATUM_COMMON_STRING_PARSER
#define STRATUM_COMMON_STRING_PARSER

#include "stratum/macro/namespaces.hpp"
#include <string>
#include <algorithm>
#include <map>
#include "stringx.hpp"
#include "debug.hpp"
#include "macro/warnings.hpp"


NAMESPACE_BEGIN(stratum, common, parse)


typedef std::map< std::string, std::string > dictionary;
typedef const std::function< bool ( char ) > predicate;


struct is_any_of_impl
{
    std::string delims_;
    is_any_of_impl(const std::string& delims) : delims_(delims) {}
    bool operator() ( char x )
    {
        return delims_.find( x ) != std::string::npos;
    }
};

inline is_any_of_impl is_any_of( const std::string& delims )
{
    return is_any_of_impl(delims);
} // is_any_of()


struct is_equal_impl
{
    char ch_;
    is_equal_impl( char ch ) : ch_(ch) {}
    bool operator() ( char x )
    {
        return ( x == ch_ );
    }
};

inline is_equal_impl is_equal( char ch )
{
    return is_equal_impl(ch);
} // is_equal()


inline predicate constant( bool b )
{
    return [b]( char IGNORE( x ) )
           {
               return b;
           };
} // constant()


enum flags
{
      none     = 0
    , dontskip = 0
    , skip     = 1  // the parser skips the delimiter
    , capture  = 2  // the parser captures the delimiter
};



class state
{
public:
    typedef std::string::const_iterator const_iterator;


    state( const std::string& input )
        : text_( input )
        , current_( text_.begin() )
        , end_( text_.end() )
    {
    } // state()


    state& operator =( const state& rhs )
    {
        if ( &rhs != this )
        {
            current_ = rhs.current_;
            end_     = rhs.end_;
        }
        return *this;
    } // operator=()


    void reset( const std::string& rhs )
    {
        current_ = rhs.begin();
        end_     = rhs.end();
    } // reset()


    bool empty() const
    {
        return ( current_ == end_ );
    } // empty()


    const_iterator::difference_type length() const
    {
        return std::distance( current_, end_ );
    } // length()


    char front() const
    {
        return *current_;
    } // front()


    bool pop_front()
    {
        if ( empty() )
            return false;

        ++current_;

        return !empty();
    } // pop_front()


    std::string as_string() const
    {
        return std::string( current_, end_ );
    } // as_string()


    bool contains( const std::string& needle ) const
    {
        return std::search( current_, end_
                          , needle.begin(), needle.end()
                            ) != end_;
    } // contains()


    // This doesn't eat start text
    // eg  "aaaaa starting bbbbb" search for "start" => "starting bbbbb"
    state search_for_text( const std::string& start ) const
    {
        return state( std::search( current_, end_
                                 , start.begin(), start.end() )
                    , end_ );

    } // search_for_text()


    // returns parser state for text up to endtag (doesn't include endtag)
    // eg  "aaaaa starting bbbbb" search for "start" => "aaaaa " (note space)
    state select_until_text( const std::string& endtag ) const
    {
        return state( current_
                    , std::search( current_, end_
                                 , endtag.begin(), endtag.end() ) );

    } // select_until_text()


    template< typename PRED >
    void trim( PRED pred )
    {
        while ( !empty() )
        {
            if ( !pred( *current_ ) )
                return;
            ++current_;
        }
    } // trim()


    template< typename PRED >
    void rtrim( PRED pred )
    {
        while ( !empty() )
        {
            --end_;
            if ( !pred( *end_ ) )
            {
                ++end_;
                return;
            }
        }
    } // rtrim()


    template< typename PRED >
    bool skip_until( PRED pred, flags f = flags::skip )
    {
        if ( empty() )
            return false;

        std::string::const_iterator  matching;

        matching = std::find_if( current_
                               , end_
                               , pred );

        if ( matching == end_ )
            return false;


        if ( f & flags::skip )
        {
            ++matching;
        }

        current_ = matching;

        return true;
    } // skip_until()


    // capture chars upto some delimiter (delimiter is required)
    // example delimter is "X"
    // input:       return:     capture:            output:
    // ===================================================
    //	""          false       unchanged           ""
    //	"abcX.."    true        "abc" or "abcX"     ".." or "X.."
    //	"abc"       false       unchanged           "abc"
    //	"X.."       true        "" or "X"           ".." or "X.."
    template< typename PRED >
    bool capture_until( PRED pred, std::string& capture, flags f = flags::skip )
    {
        if ( empty() )
            return false;

        std::string::const_iterator  matching = std::find_if( current_, end_, pred );

        if ( matching == end_ )
            return false;


        // process the delimiter character
        // + the parser can skip the delimiter in output state
        // + the parser can capture the delimiter into 'capture'
        switch ( f & ( flags::skip|flags::capture ) )
        {
            case flags::none:	// don't skip or capture
                capture  = std::string( current_, matching );
                current_ = matching;
                break;

            default:
            case flags::skip:	// don't capture
                capture = std::string( current_, matching );
                ++matching;
                current_ = matching;
                break;

            case flags::capture:// don't skip
                current_ = matching;
                ++matching;
                capture = std::string( current_, matching );
                break;

            case flags::skip|flags::capture:
                ++matching;
                capture  = std::string( current_, matching );
                current_ = matching;
                break;
        }	// switch


        return true;
    } // capture_until()


    // capture chars that meet criteria (requires 1 valid char)
    // example predicate is {char is digit}
    // input:       return:     capture:            output:
    // ====================================================
    //	""          false       unchanged           ""
    //	"123x.."    true        "123" or "123x"     ".." or "x.."
    //	"123"       true        "123"               ""
    //	"x.."       false       unchanged           "x.."
    template< typename PRED >
    bool capture_while( PRED pred, std::string& capture, flags f = flags::skip )
    {
        if ( empty() )
            return false;

        std::string::const_iterator  matching = std::find_if( current_, end_, pred );

        if ( matching == current_ )
            return false;


        if ( matching == end_ )
        {
            capture  = std::string( current_, end_ );
            current_ = end_;

            return true;
        }


        // process the delimiter character
        // + the parser can skip the delimiter in output
        // + the parser can capture the delimiter into 'capture'
        switch ( f & ( flags::skip|flags::capture ) )
        {
            case flags::none:	// don't skip or capture
                capture  = std::string( current_, matching );
                current_ = matching;
                break;

            default:
            case flags::skip:	// don't capture
                capture = std::string( current_, matching );
                ++matching;
                current_ = matching;
                break;

            case flags::capture:// don't skip
                current_ = matching;
                ++matching;
                capture = std::string( current_, matching );
                break;

            case flags::skip|flags::capture:
                ++matching;
                capture  = std::string( current_, matching );
                current_ = matching;
                break;
        }	// switch


        return true;
    } // capture_while()


    // remove text after the first char that matches predicate (requires 1 invalid char)
    // example predicate is {char is digit}
    // input:       return:     new state:
    // ===================================
    //	""          false       ""
    //	"123x.."    true        "123" or "123x"
    //	"123"       false       "123"
    //	"x.."       true        "" or "x"
    template< typename PRED >
    bool trim_after( PRED pred, flags f = flags::skip )
    {
        if ( empty() )
            return false;

        std::string::const_iterator  matching;

        matching = std::find_if( current_
                               , end_
                               , pred );

        if ( matching == end_ )
            return false;


        if ( !( f & flags::skip ) )
        {
            ++matching;
        }

        end_ = matching;

        return true;
    } // trim_after()


    bool match( const std::string& txt )
    {
        if ( empty() )
            return false;

        std::string::const_iterator  matching = current_;
        std::string::const_iterator  txt_itr  = txt.begin();
        std::string::const_iterator  txt_end  = txt.end();


        while (  ( matching != end_ )
              && ( txt_itr != txt_end )
              && ( *matching == *txt_itr ) )
        {
            ++matching;
            ++txt_itr;
        }

        if ( txt_itr == txt_end )
        {
            current_ = matching;
            return true;
        }
        else
        {
            return false;
        }
    } // match()


    void skip( const std::string& txt )
    {
        match( txt );
    } // skip()


    bool skip_until_text( const std::string& txt, flags f = flags::skip )
    {
        if ( empty() )
            return false;

        std::string::const_iterator  matching;

        matching = std::search( current_
                              , end_
                              , txt.begin()
                              , txt.end() );

        if ( matching == end_ )
            return false;


        if ( f & flags::skip )
        {
            matching += txt.length();
        }

        current_ = matching;

        return true;
    } // skip_until_text()


    void capture_remaining( std::string& capture )
    {
        if ( !empty() )
        {
            capture  = std::string( current_, end_ );
            current_ = end_;
        }
    } // capture_remaining()


    template< typename SEPFUNC, typename NVFUNC >
    dictionary split_into_dictionary( SEPFUNC sepf
                                    , NVFUNC nvf
                                    , stringx::split_processor_type  processor = &stringx::split_identity )
    {
        dictionary  dict;

        while ( !empty() )
        {
            std::string  tmp;

            if ( !capture_until( sepf, tmp ) )
            {
                capture_remaining( tmp );
            }

            dict.insert( stringx::split( tmp, nvf, processor ) );
        }

        return dict;
    } // split_into_dictionary()


    // helper, mostly this is what we'd want to use
    dictionary split_into_dictionary()
    {
        return split_into_dictionary( is_equal( ';' )
                                    , is_equal( '=' ) );

    } // split_into_dictionary()


protected:
    state( const_iterator txt_begin, const_iterator txt_end )
        : current_( txt_begin ), end_( txt_end )
    {
    } // state()


private:
    std::string     text_;
    const_iterator  current_;
    const_iterator  end_;

}; // class state



inline int hex_to_int(char ch)
{
    if ( ch < '0' )
        return -1;

    if ( ch <= '9' )
        return ch - '0';

    if ( ch < 'A' )
        return -1;

    if ( ch <= 'F' )
        return ch - 'A' + 10;

    if ( ch < 'a' )
        return -1;

    if ( ch <= 'f' )
        return ch - 'a' + 10;

    return -1;
} // hex_to_int()


inline bool convert_hex_to_char(char ch1, char ch2, char& value)
{
    int v1 = hex_to_int(ch1);
    if ( v1 >= 0 )
    {
        int v2 = hex_to_int(ch2);
        if ( v2 >= 0 )
        {
            value = ( v1 * 16 ) + v2;
            return true;
        }
    }

    return false;
} // convert_hex_to_char()


inline std::string url_decode( const std::string& url )
{
    std::string  result;
    std::size_t  size = url.size();

    result.reserve( size );

    TRACE_STREAM( "url = " << url );


    for ( std::size_t i = 0; i < size; ++i )
    {
        if ( url[i] == '+' )
        {
            result.append( 1, ' ' );
        }
        else if ( url[i] == '%' )
        {
            if ( ( i + 3 ) > size )
                return std::string();

            char value = 0;
            
            if ( !convert_hex_to_char( url[i + 1], url[i + 2], value ) )
                return std::string();
            
            result.append( 1, value );
            i += 2;

        }
        else
        {
            result.append( 1, url[i] );
        }	// if(url[i])

    }	// for()

    TRACE_STREAM( "result = " << result );

    return result;
} // url_decode()


inline std::string substring( const std::string& text, const std::string& start )
{
    std::string::size_type  pos = text.find( start );

    if ( pos != std::string::npos )
        return text.substr( pos );

    return std::string();
} // substring()


inline bool contains( const dictionary& dict
                    , const std::string& item
                    , std::string& value )
{
    dictionary::const_iterator  itr = dict.find( item );
    if ( itr == dict.end() )
        return false;

    value = itr->second;

    return true;
} // contains()


inline void dump_dictionary(const dictionary& dict)
{
    for( auto& nv : dict )
    {
        TRACE_STREAM("key [" << nv.first << "] => [" << nv.second << "]");
    }
}// dump_map();


NAMESPACE_BEGIN(stratum, common, parse)


#endif // STRATUM_COMMON_STRING_PARSER