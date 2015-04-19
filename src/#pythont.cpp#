char const* greet()
{
    return "hello, world";
}

#include<boost/python.hpp>
BOOST_PYTHON_MODULE(sharedLib)
{
   using namespace boost::python;
   def("greet",greet);
} 
