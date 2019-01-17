#include "op-wrapper.hpp"
#include "core-store.hpp"
#include "core-storage.hpp"

using namespace mpwd::core::store;
using namespace mpwd::core::tools;
using namespace mpwd::client;

int main(int argc, char *argv[]) 
{ 
  po_wrapper argm(argc, argv);  // first thing first

  // help message
  if(argm.has(po_wrapper::OP_HELP)) 
  {
    argm.printhelp();
    return 1;
  }
  string username, tags, url, val, email;
  // init
  if (argm.check_params(po_wrapper::OP_INIT)) 
  {   
    store_t store(argm.get(po_wrapper::OP_P));
    local_storage::save(store,argm.get(po_wrapper::OP_F));
    return 0;
  }
  // save
  if (argm.check_params(po_wrapper::OP_SAVE))    
  {
    row_t r;

    if(argm.has(po_wrapper::OP_VALUE)) {
      argm.get(po_wrapper::OP_VALUE, val);
      r.val(val);
    }
    argm.get(po_wrapper::OP_USERNAME, username);
    argm.get(po_wrapper::OP_TAGS, tags);
    argm.get(po_wrapper::OP_URL, url);
    argm.get(po_wrapper::OP_EMAIL, email);

    if(!username.empty())      
      r.add_prop(prop_type::username, username);
    if(!tags.empty()) 
      r.add_prop(prop_type::tags, tags);
    if(!url.empty())
      r.add_prop(prop_type::url, url);
    if(!email.empty())
      r.add_prop(prop_type::email, email);
      
    store_t store;    
    local_storage::read(store, argm.get(po_wrapper::OP_P), argm.get(po_wrapper::OP_F)); // it won't decrypt if key is different
    store.update(argm.get(po_wrapper::OP_KEY), r);
    local_storage::save(store, argm.get(po_wrapper::OP_F));  
    return 0;
  }

  //search
  if (argm.check_params(po_wrapper::OP_SEARCH))
  {    
    store_t store;
    local_storage::read(store, argm.get(po_wrapper::OP_P), argm.get(po_wrapper::OP_F));
    store.search(argm.get(po_wrapper::OP_KEYWORD), argm.has(po_wrapper::OP_SHOW), argm.has(po_wrapper::OP_HISTORY));
    return 0;
  }
  
  argm.printhelp();
  return 1;  
}