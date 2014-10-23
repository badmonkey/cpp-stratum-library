#ifndef STRATUM_COMMON_STREAM_THEN
#define STRATUM_COMMON_STREAM_THEN

#include "stratum/macro/namespaces.hpp"
#include <sstream>
#include <iostream>
#include <functional>

NAMESPACE_BEGIN(stratum, common)


namespace // anon
{


std::function< void (const std::string&) >
func_factory( std::string& target, bool do_append )
{
    if ( do_append )
        return [&target]( const std::string& s ) { target.append( s ); };
    else
        return [&target]( const std::string& s ) { target = s; };
} // func_factory()


} // namespace <anon>


class stream_then
{
public:
    typedef std::function< void ( const std::string& s ) > then_function_type;
    typedef std::basic_ostream< char >                     stream_type;

   
    stream_then( then_function_type f )
        : then_( f )
    {
    } // stream_then()


    explicit stream_then( std::string& target, bool appendp = false )
        : then_( func_factory( target, appendp ) )
    {
    } // stream_then()


    stream_then( stream_then&& other )
        : then_( std::move( other.then_ ) )
    {
        stream_ << other.stream_.str();
    } // stream_then()


    ~stream_then()
    {
        then_( stream_.str() );
    } // ~stream_then()


    template< class T >
    stream_type& operator <<( T t )
    {
        return stream_ << t;
    } // operator<<()
    
    
    void process()
    {
        then_( stream_.str() );
        stream_.str("");
    } // process()


private:
    std::ostringstream stream_;
    then_function_type then_;

}; // class stream_then


//#if !defined( TRACE_STREAM )
//#  define TRACE_STREAM( STREAM )    stream_then(                               \
//        std::bind( [] ( const std::string& s, const char* func )               \
//                   { erl_log( "debug"                                          \
//                            , __FILE__, func, __LINE__                         \
//                            , "SIPIntf", "", "", "message", s.c_str(), NULL ); \
//                                   }                                                           \
//                 , std::placeholders::_1, __FUNCTION__ ) ) << "TRACE: " << STREAM
//
//#endif // if !defined( TRACE_STREAM )


NAMESPACE_END(stratum, common)

#endif // STRATUM_COMMON_STREAM_THEN