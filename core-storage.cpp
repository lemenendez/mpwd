#include "core-store.hpp"
#include "include/core-storage.hpp"

#include <fstream>    // ifstream, ofstream
#include <sstream>      // ostringstream
#include "cryptlib.h"
#include "secblock.h"
#include "salsa.h"
#include "files.h"
#include "osrng.h"    // SHA256
// #include "files.h"
// #include "hex.h"
// #include "base64.h"

using namespace mpwd::core::store;
using namespace mpwd::core::tools;
using namespace CryptoPP;

void local_storage::read(store_t& s, const string paraphrase, const string filename, Encode encode, Encrypt encrypt)
{  
  if(encode==Encode::None && encrypt==Encrypt::None) 
  {
    ifstream ifs(filename);
    if(ifs.is_open()) 
    {
      text_iarchive ia(ifs);
      ia >> s;
    }
    else
        throw "Error opening file";
  }
  if(encode==Encode::None && encrypt==Encrypt::Salsa2) 
  {
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    Salsa20::Decryption dec;
    SecByteBlock iv(8);
    memset(iv,0x10101110,iv.size());
    CryptoPP::SHA256().CalculateDigest(digest, (const CryptoPP::byte *) paraphrase.c_str(), paraphrase.length());
    dec.SetKeyWithIV(digest, CryptoPP::SHA256::DIGESTSIZE, iv, iv.size());
    string decrypted;
    FileSource s2(filename.c_str(), true, new StreamTransformationFilter(dec, new StringSink(decrypted)));
    istringstream k(decrypted);
    text_iarchive ia(k);
    ia >> s;
  }  
}

void local_storage::save(const store_t& s, const string filename, Encode encode, Encrypt encrypt)
{  
  if(encode==Encode::None && encrypt==Encrypt::None) 
  {
      ofstream out;
      out.open(filename.c_str(), std::ios::out); // opening for write and text mode
      if(out.is_open()) {
        text_oarchive oa{out};
        oa << s;
        out.close();
      }     
      else
        throw "Error opening file";
  }
  if(encode==Encode::None && encrypt==Encrypt::Salsa2) 
  {
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    SecByteBlock iv(8);
    Salsa20::Encryption enc;

    memset(iv,0x10101110,iv.size());
    CryptoPP::SHA256().CalculateDigest(digest, (const CryptoPP::byte *) s.paraphrase().c_str(), s.paraphrase().length());
    enc.SetKeyWithIV(digest, CryptoPP::SHA256::DIGESTSIZE, iv, iv.size());

    ostringstream m_stream;
    text_oarchive oa{m_stream};
    oa << s;
    StringSource s (m_stream.str(),true, new StreamTransformationFilter(enc, new FileSink(filename.c_str())));     
  }
}

bool local_storage::file_exists(const string filename) 
{   
  ifstream file(filename.c_str());
  return file.good();
}