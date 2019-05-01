#include <string>
#include <iostream>
#include <core-store.hpp>

using namespace std;


namespace mpwd {
namespace core {
namespace store {

ostream& operator << (ostream &os, const prop_t& p) 
{
  if(p._h.empty()) throw "no property set";  
  os << p._h.back()._val._type_name <<":" << p._h.back()._val._val;
} 

std::ostream& operator << (std::ostream &os, const row_t& p) 
{  
  for(auto v:p._props)
    os << "\t"  << v.second << endl;
  os << endl;
} 

}}}


using namespace mpwd::core::store;

const int store_t::VERSION_NUMBER = 2;
const string store_t::VERSION_STR = "0.0.2";
const size_t store_t::MIN_PARAPHRASE = sizeof(char) * 30;

prop_t::prop_t(prop_type t, string val)
{
  if(t==prop_type::other)
    throw invalid_argument("property type cannot be \"other\" without a name, use another constructor instead");
  _h.push_back({0, {t, "", val}});  
}

prop_t::prop_t(string name, string val)
{  
  _h.push_back({0, {prop_type::other, name, val}});  
};

bool operator==(const prop_t&a, const prop_t&b)
{
  return (a.type()==b.type() && a.val() == b.val() && b.val()==b.name());
}

bool operator!=(const prop_t&a, const prop_t&b){ return !(a==b);}

prop_type prop_t::type() const  {
  if(_h.empty()) throw "no property set";
  const_cast<prop_t*>(this)->_h.back().seen(); // ugly  
  return _h.back()._val._type;
}

string prop_t::val() const  {
  if(_h.empty()) throw "no property set";
  const_cast<prop_t*>(this)->_h.back().seen(); // ugly
  return _h.back()._val._val;
}

string prop_t::name() const  {
  if(_h.empty()) throw "no property set";
  const_cast<prop_t*>(this)->_h.back().seen(); // ugly
  return _h.back()._val._type_name;
}

void prop_t::val(const string newval) 
{
  if(_h.empty()) throw "no property set";    
  if(_h.size()>UINT_MAX)
    throw "max limit reached";
  unsigned id = static_cast<unsigned>(_h.size());
  trackable<prop_t_h> new_r { id };  
  new_r._val._type = _h.back()._val._type;
  new_r._val._type_name.assign(_h.back()._val._type_name);
  new_r._val._val.assign(newval);
  
  _h.push_back(new_r);
}

void prop_t::print_changes() 
{
  for(auto v:_h) 
  {
    cout << "\t\tId:" << v.Id() << " Accessed:" << v.toString(v.Accessed()) << " Created:" << v.toString(v.Created()) <<" Name:" <<  v._val._type_name << " Value:"<< v._val._val << endl;
  }
}

void prop_t::print_back() 
{
  if(_h.empty()) throw "no property set"; 
  cout << " Name:" << _h.back()._val._val;
  cout << " Type:" << _h.back()._val._type_name << "\n";  
}

size_t prop_t::changes_count() const
{
  return _h.size();
}

//// start row_t methods
row_t::row_t(string v) {   
  _val.push_back({0,{v}});
};

string row_t::val() const  {     
  if(!_val.empty())
    return string{_val.back()._val._val};   // get the back element
  return string{""};
};

void row_t::val(const string newval) 
{
  unsigned id = (unsigned)_val.size();   // ugly
  _val.push_back({id, std::string{newval}});  // push to back
};

void row_t::print_changes(bool show_pwd) 
{
  cout << "\t" << "Properties:" << endl;; 
  for(auto v:_props)
  {
    cout << "\t\t"; 
    v.second.print_changes();     
  }
  if(show_pwd) 
  {
    cout << "\t" << "Pwd changes:" << endl;
    for( auto v:_val)
      cout << "\t\tId:" << v.Id() << " Accessed:" << v.toString(v.Accessed()) << " Created:" << v.toString(v.Created()) << " Value:"<< v._val._val << endl;
  }
}

bool row_t::exists_prop(const prop_type type) 
{  
  return (_props.find(typeToString(type))!=_props.end()); 
}

bool row_t::exists_prop(const string name) 
{  
  return (_props.find(name)!=_props.end()); 
}

void row_t::add_prop(const prop_type type, const string value) 
{
  if(value.empty())
    throw invalid_argument("value must not be empty");
  prop_t new_prop {type, value};
  if(!exists_prop(type))
    _props.emplace(new_prop.name(), new_prop);
  else
    throw invalid_argument("property already exists");
}

void row_t::add_prop(const string name, const string value) 
{
  if(value.empty())
    throw invalid_argument("value must not be empty");
  prop_t new_prop {name, value};  
  if(!exists_prop(name))
    _props.emplace(new_prop.name(),new_prop);
  else
    throw invalid_argument("property already exists");
}

void row_t::update_prop(const prop_type type, const string value) 
{
  prop_t prop {type, value};  
  if(exists_prop(type))
    _props[prop.name()].val(prop.val());
  else
    throw invalid_argument("property doesn't exist");
}

void row_t::update_prop(const string name, const string value) 
{
  prop_t prop {name, value};
  if(exists_prop(name))
    _props[prop.name()] = prop;
  else
    throw invalid_argument("property doesn't exist");
}

store_t::store_t(string p) 
{ 
  // cout << "calling constructor"<< endl;
  if(p.size()< MIN_PARAPHRASE) 
     throw "invalid paraphase size"; 
  _paraphrase =  p;
  // cout << "exit constructor" << endl;
}

bool store_t::get(string key) 
{  
  for(auto v:_vals)
  {     
    if(v.first==key)
    {     
      return true;
    }
  }
  return false;
}

void store_t::add(string key, row_t r) 
{  
  if(!get(key)) 
  {
    _vals[key] = r;
  } 
}

string store_t::paraphrase() const
{
  return _paraphrase;
}

void store_t::show(string key, bool pwd, bool history) 
{
  for(auto v:_vals) {
    if(v.first==key) 
    {
      if(pwd)
        cout << key << ":[" << v.second.val() << "]" <<endl;
      else
        cout << key << ":" << endl;

      if(history)
        v.second.print_changes(pwd);
      else 
        cout << v.second;
    }
  }
}

void store_t::add(string key, string value)
{
  if(!get(key)) 
  {
    row_t new_v{value};    
    _vals[key] = new_v;  // just like PHP
  }  
};

void store_t::update(string k, row_t u_r) 
{ 
  if(get(k)) 
  {    
    for(auto p:u_r.props())
    {
      if(_vals[k].exists_prop(p.first))
        _vals[k].update_prop(p.second.type(), p.second.val());
      else 
        _vals[k].add_prop(p.second.type(), p.second.val());
    }
    if(_vals[k].val()!=u_r.val() && !u_r.val().empty()) 
      _vals[k].val(u_r.val());
  }
  else
    _vals[k] = u_r;
}

void store_t::search(string keyword, bool show_secure, bool history) 
{  
  cout << "search results for:" <<keyword << endl;
  unsigned i=0;
  for(auto r:_vals) 
  { 
    std::size_t found = r.first.find(keyword);    
    if(found!=std::string::npos) {
      show(r.first, show_secure, history);
      i++;
    }
  }
  cout << "total:" << i << endl;
}

void store_t::list() {
  for(auto r:_vals) {
    show(r.first, false, false);
  }
}

size_t store_t::count() const {
  return _vals.size();
}

