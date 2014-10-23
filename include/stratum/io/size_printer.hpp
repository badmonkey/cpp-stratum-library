#ifndef STRATUM_IO_SIZE_PRINTER
#define STRATUM_IO_SIZE_PRINTER

#include "stratum/macro/namespaces.hpp"
#include "stratum/common/constants.h"
#include <iostream>
#include <iomanip>
#include <boost/io/ios_state.hpp>


NAMESPACE_BEGIN(stratum, io)


struct size_printer
{
    double size_;
    int prec_;
    
    explicit size_printer(double size, size_t prec = 2)
    : size_(size), prec_(prec)
    { assert(size >= 0.0); }
}; // struct size_printer


inline std::ostream& operator<<(std::ostream& out, const size_printer& bsize)
{
    namespace constant = NAMESPACE(stratum, common, constant);
    
    boost::io::ios_all_saver  ias(out);

    out << std::setfill('0') << std::fixed << std::setprecision(bsize.prec_) << std::left;
    double bytes = bsize.size_;

    if ( bytes > constant::GIGABYTES )
    {
        out << ( bytes / double(constant::GIGABYTES) ) << " Gb";
    }
    else if ( bytes > constant::MEGABYTES )
    {
        out << ( bytes / double(constant::MEGABYTES) ) << " Mb";
    }
    else if ( bytes > constant::KILOBYTES )
    {
        out << ( bytes / double(constant::KILOBYTES) ) << " Kb";
    }
    else
    {
        out << bytes << " bytes";
    }

    return out;
} // operator<<()

    
NAMESPACE_END(stratum, io)

#endif // STRATUM_IO_SIZE_PRINTER
