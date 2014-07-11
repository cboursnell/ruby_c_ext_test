#include "ruby.h"

// Defining a space for information and references about the module to be stored internally
VALUE MyTest = Qnil;

// Prototype for the initialization method - Ruby calls this, not you
void Init_mytest();

// methods are prefixed by 'method_' here
VALUE method_test1(VALUE self);
VALUE method_add(VALUE, VALUE, VALUE);
VALUE method_subtract(VALUE, VALUE, VALUE);


// The initialization method for this module
void Init_mytest() {
    int set[10];
    MyTest = rb_define_module("MyTest");
    rb_define_method(MyTest, "test1", method_test1, 0);
    rb_define_method(MyTest, "add", method_add, 2);
    rb_define_method(MyTest, "subtract", method_subtract, 2);

}

// Our 'test1' method.. it simply returns a value of '10' for now.
VALUE method_test1(VALUE self) {
    int x = 10;
    return INT2NUM(x);
}

// This is the method we added to test out passing parameters
VALUE method_add(VALUE self, VALUE first, VALUE second) {
    int a = NUM2INT(first);
    int b = NUM2INT(second);
    return INT2NUM(a + b);
}

// This is the method we added to test out passing parameters
VALUE method_subtract(VALUE self, VALUE first, VALUE second) {
    int a = NUM2INT(first);
    int b = NUM2INT(second);
    return INT2NUM(a - b);
}