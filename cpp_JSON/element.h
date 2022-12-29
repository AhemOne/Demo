#ifndef ELEMENT_H
#define ELEMENT_H

#ifndef JS_RECURSE
#define JS_RECURSE
#endif

#include <cstddef>
#include <cstdio>
#include <cstring>

enum js_type : size_t {
  Undefined = 0,
  Null,
  False,
  True,
  Number,
  String,
  Array,
  Object
};

const char * js_type_str(js_type type) {
  const char * str[] = {
    "undefined",
    "null",
    "false",
    "true",
    "number",
    "string",
    "array",
    "object"
  };
  return str[type];
}

class js_element;

class js_null { };

class js_number {
  friend js_element;
  
  double value;
  
  protected:
  size_t snprint(char * str, size_t n) {
    return snprintf(str, n, "%g", value);
  };
  
  public:
  js_number & operator = (double d) {
    value = d;
    return *this;
  }
  operator double () {
    return value;
  }
};

class js_string {
  friend js_element;
  
  char * data;
  
  protected:
  size_t snprint(char * str, size_t n) {
    return snprintf(str, n, "\"%s\"", data);
  };
  
  public:
  js_string() : data(nullptr) {
    //printf("js_string() => %p\n", this);
  }
  ~js_string() {
    //printf("~js_string() <= %p\n", this);
    if ( data ) delete data;
  }
  js_string & operator = (const char * str) {
    if ( data ) delete data;
    size_t length = 0;
    while ( str[length++] );
    data = new char[length];
    snprintf(data, length, str);
    //printf("js_string.data = %s => %p\n", str, data);
    return * this;
  }
  operator const char * () {
    return data;
  }
  bool operator != (const char * str) {
    return strcmp(data, str);
  }
};

class js_array {
  friend js_element;
  
  js_element * data;
  js_element * next;
  
  protected:
  size_t snprint(char *, size_t, bool = false);
  
  public:
  js_array();
  ~js_array();
  js_element & operator [] (size_t);
};

class js_object {
  friend js_element;
  
  js_element * key;
  js_element * data;
  js_element * next;
  
  protected:
  size_t snprint(char *, size_t, bool = false);
  
  public:
  js_object();
  ~js_object();
  js_element & operator [] (const char *);
};

class js_element {

# ifdef JS_NO_OFFSET
  js_type type;
  union {
    void * _pointer;
    js_number * _number;
    js_string * _string;
    js_array * _array;
    js_object * _object;
  };
  
  void set(void * p) {
    _pointer = p;
  }
  
  void * get() {
    return _pointer;
  }
  
  js_number * number() {
    return _number;
  }
  
  js_string * string() {
    return _string;
  }
  
  js_array * array() { 
    return _array;
  }
  
  js_object * object() {
    return _object;
  }
  
  void * pointer() {
    return _pointer;
  }
# else
  js_type type : 3;
  
  union data_u {
    void * pointer;
    js_number * number;
    js_string * string;
    js_array * array;
    js_object * object;
  };
  
  size_t offset : (sizeof (size_t) * 8) - 3;
  
  // p - t = o
  // p = t + o
  
  void set(void * p) {
    offset = (size_t) p - (size_t) this;
    //printf("set: %u - %u = %u\n", p, this, offset);
  }
  
  data_u get() {
    data_u d;
    d.pointer = (void *) ((size_t) this + (size_t) offset);
    //printf("get: %u + %u = %u\n", this, offset, d.pointer);
    return d;
  }
  
  js_number * number() {
    return get().number;
  }
  
  js_string * string() {
    return get().string;
  }
  
  js_array * array() { 
    return get().array;
  }
  
  js_object * object() {
    return get().object;
  }
  
  void * pointer() {
    return get().pointer;
  }
# endif
  
  void free() {
    //printf("freeing %s <= %p\n", js_type_str(type), pointer());
    if ( pointer() ) switch (type) {
      case Number:  delete number();  break;
      case String:  delete string();  break;
      case Array:   delete array();   break;
      case Object:  delete object();  break;
    };
    type = Undefined;
    set(this);
  }
  
  protected:
  
  public:
  js_element() {
    //printf("js_element() => %p\n", this);
    type = Undefined;
    set(this);
  }
  
  ~js_element() {
    //printf("~js_element() <= %p\n", this);
    free();
  }
  
  js_element & operator = (const js_null n) {
    free();
    type = Null;
    return *this;
  }
  
  js_element & operator = (const bool b) {
    free();
    type = b ? True : False;
    return *this;
  }
  
  js_element & operator = (const double v) {
    if ( type != Number ) *this = new js_number;
    *number() = v;
    //printf("js_element = (%g) => %p\n", v, number());
    return *this;
  }
  
  js_element & operator = (js_number * number_p) {
    free();
    type = Number;
    set(number_p);
    return *this;
  }
  
  js_element & operator = (const char * str) {
    if ( type != String ) *this = new js_string;
    *string() = str;
    //printf("js_element = (%s) => %p\n", str, string());
    return *this;
  }
  
  js_element & operator = (js_string * string_p) {
    free();
    type = String;
    set(string_p);
    return *this;
  }
  
  js_element & operator [] (const int i) {
    if ( i >= 0 ) return (*this)[(size_t) i];
    else throw "negative indicies to arrays not allowed";
  }
  
  js_element & operator [] (size_t i) {
    if ( type != Array ) *this = new js_array;
    return (*array())[i];
  }
  
  js_element & operator = (js_array * array_p) {
    free();
    type = Array;
    set(array_p);
    return *this;
  }
  
  js_element & operator [] (const char * key) {
    if ( type != Object ) *this = new js_object;
    return (*object())[key];
  }
  
  js_element & operator = (js_object * object_p) {
    free();
    type = Object;
    set(object_p);
    return *this;
  }
  
  size_t snprint(char * str, size_t n, bool first = true) {
    switch (type) {
      case Number:
        return number()->snprint(str, n);
      case String:
        return string()->snprint(str, n);
      case Array:
        return array()->snprint(str, n, first);
      case Object:
        return object()->snprint(str, n, first);
    };
    
    return snprintf(str, n, js_type_str(type));
  }
  
  friend js_object;
};

#define snprint_str   str ? &str[len] : nullptr
#define snprint_n     str ? n - len + 1 : 0

js_array::js_array() : data(nullptr), next(nullptr) {
  //printf("js_array() => %p\n", this);
}

js_array::~js_array() {
  //printf("~js_array() <= %p\n", this);
  if ( data ) delete data;
  if ( next ) delete next;
}

js_element & js_array::operator [] (const size_t i) {
# ifdef JS_RECURSE
    if ( i ) {
      if ( !next ) next = new js_element;
      return (*next)[i - 1];
    }
    if ( !data ) data = new js_element;
    return *data;
# else
  js_element * element_p = this;
  while ( i-- ) {
    if ( !element_p->next ) element_p->next = new js_element;
    element_p = next;
  }
  if ( !element_p->data ) element_p->data = new js_element;
  return *(element_p->data);
# endif
}

size_t js_array::snprint(char * str, size_t n, bool first) {
  size_t len = 0;
  if ( first ) len += snprintf(snprint_str, snprint_n, "[");
  
  if ( data ) len += data->snprint(snprint_str, snprint_n);
  else len += snprintf(snprint_str, snprint_n, "undefined");
  
  if ( next ) {
    len += snprintf(snprint_str, snprint_n, ",");
    len += next->snprint(snprint_str, snprint_n, false);
  } else {
    len += snprintf(snprint_str, snprint_n, "]");
  }
  return len;
};

js_object::js_object() : key(nullptr), data(nullptr), next(nullptr) {
  //printf("js_object() => %p\n", this);
}

js_object::~js_object() {
  //printf("~js_object() <= %p\n", this);
  if ( key  ) delete key;
  if ( data ) delete data;
  if ( next ) delete next;
}

js_element & js_object::operator [] (const char * search_key) {
  if ( key ) {
    if ( *(key->string()) != search_key ) {
      if ( !next ) next = new js_element;
      return (*next)[search_key];
    }
  } else {
    key = new js_element;
    *key = search_key;
  }
  if ( !data ) data = new js_element;
  return *data;
}

size_t js_object::snprint(char * str, size_t n, bool first) {
  size_t len = 0;
  if ( first ) len += snprintf(snprint_str, snprint_n, "{");
  
  if ( key ) {
    len += key->snprint(snprint_str, snprint_n);
    len += snprintf(snprint_str, snprint_n, ":");
  }
  
  if ( data ) len += data->snprint(snprint_str, snprint_n);
  else len += snprintf(snprint_str, snprint_n, "undefined");
  
  if ( next ) {
    len += snprintf(snprint_str, snprint_n, ",");
    len += next->snprint(snprint_str, snprint_n, false);
  } else {
    len += snprintf(snprint_str, snprint_n, "}");
  }
  return len;
};


#endif