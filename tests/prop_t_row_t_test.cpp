#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include "core-store.hpp"
using namespace boost::unit_test;
using namespace mpwd::core::store;

class prop_t_test
{
public:
  void test_prop_other()
  {   
    prop_t prop_other{"PIN", "4488"};
    BOOST_TEST((prop_other.type()==prop_type::other));

    prop_other.val("4489");
    BOOST_TEST((prop_other.changes_count()==2));
    // printf("New Value:%s\n",prop_other.val().c_str());
    //BOOST_TEST((prop_other.val()=="4489"));
    //prop_other.print_changes();
  }
  void test_prop()
  {
    prop_t prop {prop_type::username, "john@doe.com"};

    BOOST_TEST((prop.changes_count()==1));
    BOOST_TEST( prop.val()=="john@doe.com");

    prop.val("john@doe.org");
    BOOST_TEST( prop.val()=="john@doe.org");

  }

  void test_row_t() 
  {
    row_t row {"myinsecurepwd"};  
    row.add_prop(prop_type::username,"john.doe");
    row.add_prop(prop_type::email,"john.doe@gmail.com");
    row.add_prop(prop_type::url,"https://gmail.com");

    BOOST_TEST(row.exists_prop(prop_type::username));
    BOOST_TEST(row.exists_prop(prop_type::email));
    BOOST_TEST(row.exists_prop(prop_type::url));

    BOOST_TEST(row.val()=="myinsecurepwd");

  }
};

/*
{
  
  boost::shared_ptr<prop_t_test> tester(new prop_t_test);

  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&prop_t_test::test_prop_other, tester)));
  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&prop_t_test::test_prop, tester)));
  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&prop_t_test::test_row_t, tester)));
  return 0;
}
*/