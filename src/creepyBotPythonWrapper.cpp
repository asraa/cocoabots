#include <boost/python.hpp>
#include "creepybot.h"
BOOST_PYTHON_MODULE(creepybot)
{
   using namespace boost::python;
   class_<creepybot>("creepybot")
        .def("sendInstructions", &creepybot::sendInstructions)
        .def("start", &creepybot::start)
    ;
}

