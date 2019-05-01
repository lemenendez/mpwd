#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include "core-store.hpp"

using namespace boost::unit_test;
using namespace mpwd::core::store;

class prop_t_h_test
{
    public:
    void prop_t_h_basic()
    {
        
        prop_t_h p1;       

        BOOST_TEST((p1._type_name == ""));

        prop_t_h p2 { prop_type::username, "username", "jdoe" };
        BOOST_TEST((p2._type_name == "username"));
        BOOST_TEST((p2._val == "jdoe"));

        // shouldn't be the case , backward compatibility
        prop_t_h p3 { prop_type::username, "ignorethis", "jdoe" };
        BOOST_TEST((p3._type ==  prop_type::username));
        BOOST_TEST((p3._val == "jdoe"));

        prop_t_h p4 { prop_type::username, "jdoe"};
        BOOST_TEST((p4._type ==  prop_type::username));
        BOOST_TEST((p4._val == "jdoe"));
        p4._val = "jmartin";
        BOOST_TEST((p4._val == "jmartin"));

    }   
    
};
