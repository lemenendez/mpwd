#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include "core-store.hpp"
#include "core-storage.hpp"
#include "prop_t_row_t_test.cpp"
#include "store_test.cpp"

using namespace boost::unit_test;
using namespace mpwd::core::store;
using namespace mpwd::core::tools;

test_suite *init_unit_test_suite(int /*argc*/, char * /*argv*/ [])
{
  boost::shared_ptr<store_test> store_tester(new store_test);
  boost::shared_ptr<prop_t_test> prop_t_tester(new prop_t_test);

  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&store_test::test_simple_create, store_tester)));
  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&store_test::test_base_64_salsa2, store_tester)));
  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&prop_t_test::test_prop_other, prop_t_tester)));
  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&prop_t_test::test_prop, prop_t_tester)));
  framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(&prop_t_test::test_row_t, prop_t_tester)));

  return 0;
}