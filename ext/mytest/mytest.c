#include "ruby.h"

// Defining a space for information and references about the module to be stored internally
VALUE MyTest = Qnil;

// Prototype for the initialization method - Ruby calls this, not you
void Init_mytest();

// methods are prefixed by 'method_' here
VALUE method_test1(VALUE self);
VALUE method_add(VALUE, VALUE, VALUE);
VALUE method_get(VALUE, VALUE);
VALUE method_hashing(VALUE, VALUE, VALUE);
int hash(char *, int);
int set[10];

// The initialization method for this module
void Init_mytest() {
    MyTest = rb_define_module("MyTest");
    rb_define_method(MyTest, "add", method_add, 2);
    rb_define_method(MyTest, "get", method_get, 1);
    rb_define_method(MyTest, "hashing", method_hashing, 2);
}

// This is the method we added to test out passing parameters
VALUE method_add(VALUE self, VALUE first, VALUE second) {
    int a = NUM2INT(first);
    int b = NUM2INT(second);
    set[0] = a+b;
    return INT2NUM(set[0]);
}

// This is the method we added to test out passing parameters
VALUE method_get(VALUE self, VALUE index) {
    int i = NUM2INT(index);
    return INT2NUM(set[i]);
}

int hash(char * str, int len) {
    int i,hash=0;
    for(i = 0; i < len; i++) {
        hash = hash << 2;
        if (str[i]=='A') {
            hash += 0;
        } else if (str[i]=='C') {
            hash += 1;
        } else if (str[i]=='G') {
            hash += 2;
        } else if (str[i]=='T') {
            hash += 3;
        }
    }
    return hash;
}

VALUE method_hashing(VALUE self, VALUE kmer, VALUE n) {
    char * str;
    int len;
    int output;
    len = RSTRING_LEN(kmer);
    str = StringValueCStr(kmer);
    output = hash(str, len);
    return INT2NUM(output);
}