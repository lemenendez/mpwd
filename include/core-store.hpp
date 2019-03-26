#ifndef __MPWD_CORE_STORE_H__
#define __MPWD_CORE_STORE_H__

#include <string>
#include <map>
#include <ctime>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>

using namespace boost::archive;
using namespace std;

namespace mpwd {
namespace core {
namespace store {

enum class prop_type { username, email, url, pin, code, tags, other };

static string typeToString(prop_type);

template <class T>
class trackable
{
  unsigned _id;  
  time_t _created;
  time_t _accessed;  
  friend class boost::serialization::access;  // serialization stuff
  public:
    T _val;
    trackable(){};
    trackable(unsigned i): _id { i } { start();};
    trackable(unsigned i, T tracked): _id {i}, _val{tracked} { start();}
    template <typename Archive> void serialize(Archive &ar, const unsigned int)   { ar & _id; ar & _created; ar & _accessed;  ar & _val;  };    
    unsigned Id() { return _id;};
    time_t Created() {return _created; };
    time_t Accessed() {return _accessed; };
    void start() { _created = _accessed = time(0); };
    void seen() { _accessed = time(0); }
    string toString(time_t t) {
        char buffer[32];
        std::tm * pmt = std::localtime(&t);
        std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", pmt);
        return std::string{buffer};
    };
};

struct prop_t_h 
{  
  prop_type _type;
  string _type_name;
  string _val; 
  prop_t_h(){c_name();}
  prop_t_h(prop_type _t, string _tn, string _v):_type{_t},_type_name{_tn},_val{_v} { c_name(); }
  private:  
    friend class boost::serialization::access; 
    void serialize(text_iarchive &ar, const unsigned int) { ar & _type; ar & _val; c_name();};
    void serialize(text_oarchive &ar, const unsigned int) { ar & _type; ar & _val; };
    void c_name(){_type_name = typeToString(_type); };
};

struct string_t_h
{
  string _val;
  string_t_h(){};
  string_t_h(string _v):_val{_v}{};
  friend class boost::serialization::access; 
  void serialize(text_iarchive &ar, const unsigned int) { ar & _val;  };
  void serialize(text_oarchive &ar, const unsigned int) { ar & _val; };
  friend std::ostream& operator << (std::ostream &os, const string_t_h& v){ os<<v._val;};

};

typedef list<trackable<prop_t_h>> history;
typedef list<trackable<string_t_h>> s_history;

class prop_t
{
  private:
    history _h;  // beautiful
    friend class boost::serialization::access;  // serialization stuff
    template <typename Archive> void serialize(Archive &ar, const unsigned int) { ar & _h; };
  public:
    prop_t(){};           // needed for serialization 
    prop_t(prop_type, string);
    prop_t(string, string);
    prop_type type() const;
    string val() const;           
    string name() const; 
    void val(const string); 
    void print_changes();
    size_t changes_count() const;

    friend std::ostream& operator << (std::ostream &os, const prop_t&);

    bool operator==(const prop_t&);
    bool operator!=(const prop_t&);

    prop_t& operator=(prop_t& other) { _h = other._h; return *this; }
    
};

class row_t 
{
  private:
    friend class boost::serialization::access;      
    s_history _val;
    map<string, prop_t> _props;    
    
  public:
    row_t(){};               // needed for serialization 
    row_t(string v);
    template <typename Archive> void serialize(Archive &ar, const unsigned int version){ar & _val; ar & _props; };
    void print_changes(bool show_pwd = false);
    string val() const;
    void val(const string);

    friend std::ostream& operator << (std::ostream &os, const row_t&);    

    void add_prop(const prop_type, const string);
    void add_prop(const string, const string);

    void update_prop(const prop_type, const string);
    void update_prop(const string name, const string value);

    bool exists_prop(const prop_type);
    bool exists_prop(const string);
    map<string, prop_t> props() { return _props;};

    row_t& operator=(row_t& other) {
      _props = other._props;
      _val =other._val;
      return *this;
    }
};

class store_t
{  
  int _version_number;
  string _mpwd_version;
  string _paraphrase;
  map<string, row_t> _vals;
  friend class boost::serialization::access;

  template <typename Archive> void serialize(Archive &ar, const unsigned int version)
  {
    ar & _mpwd_version;
    ar & _version_number;
    ar & _paraphrase;
    ar & _vals;  
  };

  bool get(string);

  public:
    store_t(){};    // serialization stuff
    store_t(string);     
    void add(string, string); // simple keyval    
    void add(string, row_t);
    void add(list<row_t>);
    void update(string,row_t);
    void init(string);
    void show(string,bool history=false,bool pwd=false);
    void list();
    string paraphrase() const;
    void search(string, bool show_secure=false, bool history=false);
    size_t count() const;
    static const size_t MIN_PARAPHRASE;
    static const int VERSION_NUMBER;
    static const string VERSION_STR;
};

static string typeToString(prop_type t) 
{
  string result;
  switch(t) 
  {
    case prop_type::username: result = "username"; break;
    case prop_type::email: result = "email"; break;
    case prop_type::url: result ="url"; break;
    case prop_type::pin: result ="pin"; break;
    case prop_type::code: result ="code"; break;
    case prop_type::tags: result ="tags"; break;
    case prop_type::other: result ="other"; break;        
  }
  return result;
};
  
}
}
}

#endif