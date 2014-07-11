#include "ruby.h"

// Defining a space for information and references about the module to be
// stored internally
VALUE MyTest = Qnil;

// Prototype for the initialization method - Ruby calls this, not you
void Init_mytest();

// methods are prefixed by 'method_' here
VALUE TestInit(VALUE, VALUE, VALUE, VALUE);
VALUE method_add(VALUE, VALUE);
VALUE method_get(VALUE, VALUE);
VALUE method_hashing(VALUE, VALUE, VALUE);

int hash(char *, int, int, int);

long set_len;
int set_count;
int *set;
int kmer_size;

// The initialization method for this module
void Init_mytest() {
    MyTest = rb_define_class("MyTest", rb_cObject);
    rb_define_method(MyTest, "initialize", TestInit, 3);
    rb_define_method(MyTest, "add", method_add, 1);
    rb_define_method(MyTest, "get", method_get, 1);
    rb_define_method(MyTest, "hashing", method_hashing, 2);
}

// called when creating a new class
// ks = size of kmer to use in bases
// size = length of array
// buckets = number of buckets
VALUE TestInit(VALUE self, VALUE ks, VALUE size, VALUE count)
{
    int i;
    set_len = NUM2INT(size);
    set_count = NUM2INT(count);
    set = malloc(set_len * set_count * sizeof(int));
    for (i = 0; i < set_len * set_count; i++) {
        set[i] = 0;
    }
    kmer_size = NUM2INT(ks);
    return Qnil;
}

// takes in a fastq read sequence eq 100 bases
// kmerise the string, hash each kmer and add to set
VALUE method_add(VALUE self, VALUE read) {
    char * str;
    int b,i,h,len;
    h = 0;
    len = RSTRING_LEN(read);
    str = StringValueCStr(read);
    for (i = 0; i < len-kmer_size+1; i++) {
        for(b = 0; b < set_count; b++) {
            h = hash(str, i, kmer_size, b);
            set[b*set_len+h] += 1;
        }
    }
    return INT2NUM(1); // return 1 to indicate true /shrug
}

// This is the method we added to test out passing parameters
VALUE method_get(VALUE self, VALUE kmer) {
    char * str;
    int b,v=-1,p,len,h;
    len = RSTRING_LEN(kmer);
    str = StringValueCStr(kmer);
    // inPlaceReverse(str, kmer_size);
    // printf("kmer = %s\n",str);
    if (len <= kmer_size) {
        for (b = 0; b < set_count; b++) {
            h = hash(str, 0, kmer_size, b);
            if (v < 0) {
                v = set[b*set_len+h];
            } else {
                p = set[b*set_len+h];
                if (p < v) {
                    v = p;
                }
            }
            // printf("hash = %i \n", h);
        }
        return INT2NUM(v);
    } else {
        return 0;
    }
}

int hash(char * str, int start, int len, int n) {
    int r,i,hash=0,hash2=0;
    srand(n+1);
    for(i = start; i < start+len; i++) {
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
    // calculate reverse complement hashing function
    for(i = start+len-1;i>=start; i--) {
        hash2 = hash2 << 2;
        if (str[i]=='A') {
            hash2 += 3;
        } else if (str[i]=='C') {
            hash2 += 2;
        } else if (str[i]=='G') {
            hash2 += 1;
        } else if (str[i]=='T') {
            hash2 += 0;
        }
    }
    r = rand();
    hash += r;
    hash2 += r;
    if (hash2 > hash) {
        hash2 = hash2 % set_len;
        return hash2;
    } else {
        hash = hash % set_len;
        return hash;
    }
}

VALUE method_hashing(VALUE self, VALUE kmer, VALUE n) {
    char * str;
    int len;
    int output;
    int b;
    len = RSTRING_LEN(kmer);
    str = StringValueCStr(kmer);
    b = NUM2INT(n);
    output = hash(str, 0, len, b);
    return INT2NUM(output);
}