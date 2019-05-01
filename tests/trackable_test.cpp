#include <string>
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include "core-store.hpp"

using namespace boost::unit_test;
using namespace mpwd::core::store;

class trackable_test
{
  public:

    void list_of_trackable_prop_t() 
    {
        list<trackable<prop_t_h>> lst;
        BOOST_TEST((lst.size()==0)) ;

        trackable<prop_t_h> new_v {0,{prop_type::username, "jdoe"}};
        lst.push_back(new_v);

        trackable<prop_t_h> new_v_01 {1,{prop_type::username, "jlopez"}};
        lst.push_back(new_v_01);

        BOOST_TEST((lst.back()._val._val=="jlopez"));

        prop_t prop {prop_type::username, "john@doe.com"};
        BOOST_TEST((prop.changes_count()==1));        
        BOOST_TEST( prop.val()=="john@doe.com");
        
        prop.val("john@doe.org");        
        prop.val("john@doe.co");
        prop.val("john@doe.gt");
        prop.val("john@doe.ch");
        BOOST_TEST( prop.val()=="john@doe.ch");
    }
    void trackable_prop_t_basic()
    {
        
        trackable<prop_t_h> p1;

        BOOST_TEST((p1.Created()!=0));
        BOOST_TEST((p1.Accessed()!=0));

        trackable<prop_t_h> p2 = {0, { prop_type::username, "jdoe" }};

        BOOST_TEST((p2._val._type_name == "username"));
        BOOST_TEST((p2.Created()!=0));
        BOOST_TEST((p2.Accessed()!=0));

    }
    
};
