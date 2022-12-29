# Cpp JSON

## Introduction

This library converts between JSON and an object representation. The aim of this project is to imitate a JSON/js object type environment for quickly building memory objects, and a JSON interface for parsing and stringifying. The ultimate aim is to provide a library for microcontrollers with small amounts of memory which gives the ability to manipulate JavaScript-like objects to be recieved from or for the transmission to external sources while protecting the allocation table to ensure minimal memory fragmentation.

## Examples
``` c++
js_object & a = * new js_object;  // undefined js_object
a = true;                         // true
a = -1.23;                        // -1.23
a = "hello"                       // "hello"
a[2] = "test";                    // [,,"test"]
a[1] = 123;                       // [,123,"test"]
a["a"] = 123;                     // {a:123}
a["b"] = null;                    // {a:123,b:null}
```
## Future
``` c++
char * JSON_string[JSON.length(a)+1];
sprintf(JSON_string, JSON.stringify(a));    // JSON_string = "{\"a\":123,\"b\":null}"
js_object & z = JSON.parse(JSON_string);    // z = {a:123,b:null}
z["c"] = z["a"] + " is a string";           // z = {a:123,b:null,c:"123 is a string"}
```

## Files
Noteworthy files:
- element.([h](element.h)): Implementation - note: has not been abstracted for external linkage
