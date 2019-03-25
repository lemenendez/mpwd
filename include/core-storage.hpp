#ifndef __MPWD_TOOLS_PERSIST_H__
#define __MPWD_TOOLS_PERSIST_H__

#include "core-store.hpp"
#include <string>

using namespace mpwd::core::store;

namespace mpwd {
namespace core {
namespace tools {
  
  enum class Encode { None, Url, Base64 };
  enum class Encrypt { None, Salsa2 };

  class local_storage
  {
    public:
      static void read(store_t& s, const string paraphrase, const string filename, Encode encode=Encode::None, Encrypt encrypt=Encrypt::Salsa2);
      static void save(const store_t& s, const string filename, Encode encode=Encode::None, Encrypt encrypt=Encrypt::Salsa2);
      static bool file_exists(const string);
      static string decode64(const string &);
      static string encode64(const string &);
  }; 

}}}


#endif