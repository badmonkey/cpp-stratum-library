#ifndef GROUNDWORK_UTILS_FILESYSTEM_HPP
#define GROUNDWORK_UTILS_FILESYSTEM_HPP

#include "boost/filesystem.hpp"
#include <iostream>


namespace groundwork
{
namespace utils
{
namespace filesystem
{
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


} // namespace filesystem
} // namespace utils
} // namespace groundwork

#endif //GROUNDWORK_UTILS_FILESYSTEM_HPP
