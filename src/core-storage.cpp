#include "core-store.hpp"
#include "core-storage.hpp"

#include <fstream> // ifstream, ofstream
#include <sstream> // ostringstream
#include "cryptlib.h"
#include "secblock.h"
#include "salsa.h"
#include "files.h"
#include "osrng.h" // SHA256

// start base 64 headers
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>
// end base 64 headers

// #include "files.h"
// #include "hex.h"
// #include "base64.h"
using namespace CryptoPP;
using namespace boost::archive::iterators;

using namespace mpwd::core::store;
using namespace mpwd::core::tools;

string local_storage::decode64(const std::string &val)
{
  using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
  return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
    return c == '\0';
  });
}

string local_storage::encode64(const std::string &val)
{
  using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
  auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
  return tmp.append((3 - val.size() % 3) % 3, '=');
}

void local_storage::read(store_t &s, const string paraphrase, const string filename, Encode encode, Encrypt encrypt)
{
  if (encode == Encode::Base64 && encrypt == Encrypt::None)
  {
    ifstream ifs(filename);
    if (ifs.is_open())
    {
      stringstream buffer;
      buffer << ifs.rdbuf();
      buffer.str(decode64(buffer.str()));
      text_iarchive ia(buffer);
      ia >> s;
    }
  }
  if (encode == Encode::Base64 && encrypt == Encrypt::Salsa2)
  {
    // encription setup
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    Salsa20::Decryption dec;
    SecByteBlock iv(8);
    memset(iv, 0x10101110, iv.size());
    CryptoPP::SHA256().CalculateDigest(digest, (const CryptoPP::byte *)paraphrase.c_str(), paraphrase.length());
    dec.SetKeyWithIV(digest, CryptoPP::SHA256::DIGESTSIZE, iv, iv.size());
    // end encription setup
    ifstream ifs(filename);
    if (ifs.is_open())
    {
      stringstream buffer;
      buffer << ifs.rdbuf();
      buffer.str(decode64(buffer.str()));
      string decrypted;
      StringSource stringSource(buffer.str(), true, new StreamTransformationFilter(dec, new StringSink(decrypted)));
      istringstream k(decrypted);
      text_iarchive ia(k);
      ia >> s;
    }
  }
  if (encode == Encode::None && encrypt == Encrypt::None)
  {
    ifstream ifs(filename);
    if (ifs.is_open())
    {
      text_iarchive ia(ifs);
      ia >> s;
    }
    else
      throw "Error opening file";
  }
  if (encode == Encode::None && encrypt == Encrypt::Salsa2)
  {
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    Salsa20::Decryption dec;
    SecByteBlock iv(8);
    memset(iv, 0x10101110, iv.size());
    CryptoPP::SHA256().CalculateDigest(digest, (const CryptoPP::byte *)paraphrase.c_str(), paraphrase.length());
    dec.SetKeyWithIV(digest, CryptoPP::SHA256::DIGESTSIZE, iv, iv.size());
    string decrypted;
    FileSource s2(filename.c_str(), true, new StreamTransformationFilter(dec, new StringSink(decrypted)));
    istringstream k(decrypted);
    text_iarchive ia(k);
    ia >> s;
  }
}

void local_storage::save(const store_t &s, const string filename, Encode encode, Encrypt encrypt)
{
  if (encode == Encode::Base64 && encrypt == Encrypt::None)
  {
    stringstream buffer;
    ofstream out;
    out.open(filename.c_str(), std::ios::out); // opening for write and text mode
    if (out.is_open())
    {
      text_oarchive oa{buffer};
      oa << s;
      out << encode64(buffer.str());
      out.close();
    }
    else
      throw "Error opening file";
  }
  if (encode == Encode::Base64 && encrypt == Encrypt::Salsa2)
  {
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    SecByteBlock iv(8);
    Salsa20::Encryption enc;

    memset(iv, 0x10101110, iv.size());
    CryptoPP::SHA256().CalculateDigest(digest, (const CryptoPP::byte *)s.paraphrase().c_str(), s.paraphrase().length());
    enc.SetKeyWithIV(digest, CryptoPP::SHA256::DIGESTSIZE, iv, iv.size());

    stringstream buffer;
    text_oarchive oa{buffer};
    oa << s;

    string encrypted;
    StringSource s(buffer.str(), true, new StreamTransformationFilter(enc, new StringSink(encrypted)));

    ofstream out;
    out.open(filename.c_str(), std::ios::out); // opening for write and text mode
    if (out.is_open())
    {
      out << encode64(encrypted);
      out.close();
    }
  }
  if (encode == Encode::None && encrypt == Encrypt::Salsa2)
  {
    CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
    SecByteBlock iv(8);
    Salsa20::Encryption enc;

    memset(iv, 0x10101110, iv.size());
    CryptoPP::SHA256().CalculateDigest(digest, (const CryptoPP::byte *)s.paraphrase().c_str(), s.paraphrase().length());
    enc.SetKeyWithIV(digest, CryptoPP::SHA256::DIGESTSIZE, iv, iv.size());

    ostringstream m_stream;
    text_oarchive oa{m_stream};
    oa << s;
    StringSource s(m_stream.str(), true, new StreamTransformationFilter(enc, new FileSink(filename.c_str())));
  }
  if (encode == Encode::None && encrypt == Encrypt::None)
  {
    ofstream ofs(filename);
    text_oarchive oa{ofs};
    oa << s;
  }
}

bool local_storage::file_exists(const string filename)
{
  ifstream file(filename.c_str());
  return file.good();
}