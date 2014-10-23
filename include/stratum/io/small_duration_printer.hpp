#define STRATUM_IO_SMALL_DURATION_PRINTER
#define STRATUM_IO_SMALL_DURATION_PRINTER

#include "stratum/macro/namespaces.hpp"
#include "stratum/common/constants.h"
#include "stratum/io/units_printer.hpp"


NAMESPACE_BEGIN(stratum, io)


struct small_duration_printer
{
    double secs_;

    explicit small_duration_printer(double secs)
    : secs_(secs)
    { assert(secs >= 0.0); }
}; // struct small_duration_printer


inline std::ostream& operator<<(std::ostream& out, const small_duration_printer& val)
{
    namespace constant = NAMESPACE(stratum, common, constant);
    

    double time = val.secs_;

    if ( time >= constant::SECS_PER_HOUR )
    {
        out << units_printer(time / constant::SECS_PER_HOUR, " hr");
    }
    else if ( time >= constant::SECS_PER_MIN )
    {
        out << units_printer(time / constant::SECS_PER_MIN, " min");
    }
    else if ( time >= 1 || time == 0 )
    {
        out << units_printer(time, " sec");
    }
    else if ( time >= constant::MILLISEC_PER_SEC )
    {
        out << units_printer(time / constant::MILLISEC_PER_SEC, " mSec");
    }
    else if ( time >= constant::MICROSEC_PER_SEC )
    {
        out << units_printer(time / constant::MICROSEC_PER_SEC, " uSec");
    }
    else
    {
        out << units_printer(time / constant::NANOSEC_PER_SEC, " nSec");
    }

    return out;
} // operator<<()


NAMESPACE_END(stratum, io)

#endif // STRATUM_IO_SMALL_DURATION_PRINTER
