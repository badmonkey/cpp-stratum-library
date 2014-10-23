#ifndef STRATUM_IO_LONG_DURATION_PRINTER
#define STRATUM_IO_LONG_DURATION_PRINTER

#include "stratum/macro/namespaces.hpp"
#include "stratum/io/units_printer.hpp"
#include "stratum/io/small_duration_printer.hpp"


NAMESPACE_BEGIN(stratum, io)


struct long_duration_printer
{
    double secs_;
    
    explicit long_duration_printer(double secs)
    : secs_(secs)
    { assert(secs >= 0.0); }
}; // struct long_duration_printer


inline std::ostream& operator<<(std::ostream& out, const long_duration_printer& val)
{
    namespace constant = NAMESPACE(stratum, common, constant);
    
    double time = val.secs_;


    if ( time >= double(constant::SECS_PER_WEEK) )
    {
        double wks = ulong( time / double(constant::SECS_PER_WEEK) );
        out << units_printer(wks, " week ", " weeks ", 0);
        time -= wks * double(constant::SECS_PER_WEEK);
    }

    if ( time >= double(constant::SECS_PER_DAY) )
    {
        double days = ulong( time / double(constant::SECS_PER_DAY) );
        out << units_printer(days, " day ", " days ", 0);
        time -= days * double(constant::SECS_PER_DAY);
    }

    if ( time >= double(constant::SECS_PER_HOUR) )
    {
        double hours = ulong( time / double(constant::SECS_PER_HOUR) );
        out << units_printer(hours, " hour ", " hours ", 0);
        time -= hours * double(constant::SECS_PER_HOUR);
    }

    if ( time >= double(constant::SECS_PER_MIN) )
    {
        double minutes = ulong( time / double(constant::SECS_PER_MIN) );
        out << units_printer(minutes, " min ", " mins ", 0);
        time -= minutes * double(constant::SECS_PER_MIN);
    }

    // print remainder as a single value/unit
    out << small_duration_printer(time);

    return out;
} // operator<<()

    
NAMESPACE_END(stratum, io)

#endif // STRATUM_IO_LONG_DURATION_PRINTER
