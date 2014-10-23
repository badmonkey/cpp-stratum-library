#ifndef STRATUM_IO_UNITS_PRINTER
#define STRATUM_IO_UNITS_PRINTER

#include "stratum/macro/namespaces.hpp"
#include <iostream>
#include <iomanip>
#include <boost/io/ios_state.hpp>
#include "stratum/math/functions.hpp"


NAMESPACE_BEGIN(stratum, io)


struct units_printer
{
    double value_;
    const char* unit_;
    const char* units_;
    uint prec_;
    
    units_printer(double value, const char* unit, const char* units = NULL, uint precision = 2)
    : value_(value), unit_(unit), units_(units), prec_(precision)
    { assert(value >= 0.0); }
}; // struct units_printer


inline std::ostream& operator<<(std::ostream& out, const units_printer& value)
{
    boost::io::ios_all_saver  ias(out);
    out << std::setfill('0') << std::fixed << std::setprecision(value.prec_) << std::left;

    double rnd_value = stratum::math::round_digits(value.value_, value.prec_);

    out << value.value_;
    if ( rnd_value == 1.0f )
    {
        out << value.unit_;
    }
    else
    {
        if ( value.units_ )
            out << value.units_;
        else
            out << value.unit_ << "s";

    }
    return out;
} // operator<<()


NAMESPACE_END(stratum, io)

#endif // STRATUM_IO_UNITS_PRINTER