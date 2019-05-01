#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include "core-store.hpp"
#include "core-storage.hpp"

using namespace boost::unit_test;
using namespace mpwd::core::store;
using namespace mpwd::core::tools;

class store_test
{
public:
  void test_simple_create()
  {
    store_t t("myuglyandinseureparaphrase_myuglyandinseureparaphrase");
    local_storage::save(t, "test_file");
    BOOST_TEST(local_storage::file_exists("test_file"));
  }
  void test_base_64_salsa2()
  {
    store_t t("myuglyandinseureparaphrase_myuglyandinseureparaphrase");
    local_storage::save(t, "test_file", Encode::Base64, Encrypt::Salsa2);
    BOOST_TEST(local_storage::file_exists("test_file"));
  }

  void test_store_keys()
  {
    store_t t("myuglyandinseureparaphrase_myuglyandinseureparaphrase");
    local_storage::save(t, "test_file", Encode::Base64, Encrypt::Salsa2);
    BOOST_TEST(local_storage::file_exists("test_file"));
  }
  
};
