#include <op-wrapper.hpp>

using namespace mpwd::client;
using namespace std;

const string po_wrapper::OP_HELP = "help";
const string po_wrapper::OP_INIT = "init";
const string po_wrapper::OP_SAVE = "save";
const string po_wrapper::OP_SEARCH = "search";
const string po_wrapper::OP_P = "p";
const string po_wrapper::OP_F = "f";
const string po_wrapper::OP_KEY = "key";
const string po_wrapper::OP_VALUE = "value";
const string po_wrapper::OP_TAGS = "tags";
const string po_wrapper::OP_URL = "url";
const string po_wrapper::OP_USERNAME = "username";
const string po_wrapper::OP_KEYWORD = "keyword";
const string po_wrapper::OP_SHOW = "show";
const string po_wrapper::OP_HISTORY = "history";

string po_wrapper::get(string name) 
{  
  if(has(name)) 
    return vm[name].as<string>();
  else
     std::invalid_argument("param doesn't exist");
}

void po_wrapper::get(string name, string &value) 
{  
  if(has(name)) 
    value = vm[name].as<string>();   
  else
    value = "";
}

bool po_wrapper::has(string optionname) { return vm.count(optionname)?true:false; }

void po_wrapper::printhelp() {  desc->print(std::cout); }

po_wrapper::po_wrapper(int argc, char *argv[]) {  handle_po(argc, argv); };

void po_wrapper::handle_po(int argc, char *argv[]) 
{
  desc = new opt::options_description("all");  
  desc->add_options()
    (OP_HELP.c_str(), "Print this message ")
    (OP_INIT.c_str(), "Initialize a secure storage (database)")
    (OP_SEARCH.c_str(), "search for entries")
    (OP_SAVE.c_str(), "Add or update an entry in the secure storage (database)")
    (OP_P.c_str(), opt::value<string>(), "A very secure paragraph without spaces or tabs")
    (OP_F.c_str(), opt::value<string>()->default_value(default_db_name), "File name and path for the secure storage(database)")
    (OP_KEY.c_str(), opt::value<string>(), "A unique name for an entry in the database")
    (OP_VALUE.c_str(), opt::value<string>(), "A strong password")
    (OP_USERNAME.c_str(), opt::value<string>(), "The user name associated with the password (value param), example: jonh.doe@gmail.com")
    (OP_TAGS.c_str(), opt::value<string>(), "coma separated tags, example: personal, email")
    (OP_URL.c_str(), opt::value<string>(), "an Url, example: https://gmail.com")
    (OP_KEYWORD.c_str(), opt::value<string>(), "a text to find inside our secure storage")
    (OP_SHOW.c_str(), " show the actual password")
    (OP_HISTORY.c_str(), " shows history")
    ;
  opt::store(opt::parse_command_line(argc, argv, *desc), vm);
  opt::notify(vm);
}

bool po_wrapper::check_params(const std::string param)
{
  if(!(has(OP_P) && has(OP_F))) return false;
  if(param==OP_INIT && has(OP_INIT) && has(OP_P) && has(OP_F) ) return true;   
  if(param==OP_SAVE && has(OP_SAVE) && has(OP_P) && has(OP_F) && has(OP_KEY) && (has(OP_USERNAME) || has(OP_VALUE) || has(OP_TAGS)|| has(OP_URL))) return true;  
  if(param==OP_SEARCH && has(OP_SEARCH) && has(OP_P) && has(OP_F) && has(OP_KEYWORD)) return true;
  std::invalid_argument("invalid param combination see help");
}