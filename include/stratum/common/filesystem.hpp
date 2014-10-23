#ifndef STRATUM_COMMON_FILESYSTEM
#define STRATUM_COMMON_FILESYSTEM

#include "stratum/macro/namespaces.hpp"
#include "boost/filesystem.hpp"


NAMESPACE_BEGIN(stratum, common, fs)

namespace bfs = boost::filesystem;


template<typename FUNC>
FUNC foreach(const std::string& dir, FUNC f)
{
    bfs::recursive_directory_iterator  it(dir);
    bfs::recursive_directory_iterator  end;
    
    for( ; it != end; ++it )
    {
        f( *it );
    }
    return f;
} // foreach()


NAMESPACE_END(stratum, common, fs)

#endif // STRATUM_COMMON_FILESYSTEM
