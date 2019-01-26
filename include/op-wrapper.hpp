#ifndef __MPWD_TOOLS_OP_H__
#define __MPWD_TOOLS_OP_H__

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <stdexcept>

#include <core-storage.hpp>

namespace opt = boost::program_options;

namespace mpwd {
namespace client {

class po_wrapper 
{
  public:
    static const std::string OP_HELP;
    static const std::string OP_INIT;
    static const std::string OP_SAVE;
    static const std::string OP_SEARCH;
    static const std::string OP_P;
    static const std::string OP_F;
    static const std::string OP_KEY;
    static const std::string OP_VALUE;
    static const std::string OP_USERNAME;
    static const std::string OP_TAGS;
    static const std::string OP_URL;
    static const std::string OP_KEYWORD;
    static const std::string OP_EMAIL;
    static const std::string OP_SHOW;
    static const std::string OP_HISTORY;
    static const std::string OP_LIST;
    static const std::string OP_VERSION;
    static const std::string OP_ENCRYPT;
    static const std::string OP_ENCODING;
  private:
    const std::string default_db_name =".sec-db";
    const std::string default_encrypt = "salsa2";
    const std::string default_encoding = "none";
    opt::options_description *desc;
    opt::variables_map vm;
    void handle_po(int, char *[]);    
  public:
    po_wrapper(int, char *[]);
    bool has(std::string);
    void printhelp();
    void get(std::string, std::string &);
    std::string get(std::string);
    bool check_params(const std::string);
    mpwd::core::tools::Encrypt getEncrypt();
    mpwd::core::tools::Encode getEncoding();
};

}}

#endif