#include "ruby.h"
#include <stdlib.h>

// Defining a space for information and references about the module to be
// stored internally
VALUE MyTest = Qnil;

// Prototype for the initialization method - Ruby calls this, not you
void Init_mytest();

// methods are prefixed by 'method_' here
VALUE TestInit(VALUE, VALUE, VALUE, VALUE, VALUE);
VALUE method_radd(VALUE, VALUE);
VALUE method_get(VALUE, VALUE);
VALUE method_hashing(VALUE, VALUE, VALUE);
VALUE method_rmedian(VALUE self, VALUE read);
VALUE method_kmer_size(VALUE self);
VALUE method_run(VALUE self, VALUE, VALUE);

long hash(char *, int, int, int);
uint8_t median(char *);
void add(char *);

long set_len;
int set_count;
uint8_t *set;
int kmer_size;
int *offsets;
int *counts;
int cutoff;

// The initialization method for this module
void Init_mytest() {
    MyTest = rb_define_class("MyTest", rb_cObject);
    rb_define_method(MyTest, "initialize", TestInit, 4);
    rb_define_method(MyTest, "radd", method_radd, 1);
    rb_define_method(MyTest, "get", method_get, 1);
    rb_define_method(MyTest, "hashing", method_hashing, 2);
    rb_define_method(MyTest, "rmedian", method_rmedian, 1);
    rb_define_method(MyTest, "kmer_size", method_kmer_size, 0);
    rb_define_method(MyTest, "run", method_run, 2);
}

// called when creating a new class
// ks = size of kmer to use in bases
// size = length of array
// count = number of buckets
VALUE TestInit(VALUE self, VALUE ks, VALUE size, VALUE count, VALUE cut)
{
    int i;
    set_len = NUM2INT(size);
    set_count = NUM2INT(count);
    cutoff = NUM2INT(cut);
    set = malloc(set_len * set_count * sizeof(uint8_t));
    offsets = malloc(set_count * sizeof(int));
    counts = malloc(100 * sizeof(int));
    for (i = 0; i < 100; i++) {
        counts[i] = 0;
    }
    for (i = 0; i < set_len * set_count; i++) {
        set[i] = 0;
    }
    for (i = 0; i < set_count; i++) {
        srand(i+1);
        offsets[i] = rand() % set_len;
        // printf("setting up offsets = %i \n", offsets[i]);
    }
    kmer_size = NUM2INT(ks);
    return Qnil;
}

// take in two input files and export two output files
VALUE method_run(VALUE self, VALUE left, VALUE right) {
    int med1, med2;
    long keeper, counter;
    FILE *lfp;
    FILE *rfp;
    FILE *lout;
    FILE *rout;
    char * line_name1 = NULL;
    char * line_seq1 = NULL;
    char * line_plus1 = NULL;
    char * line_qual1 = NULL;
    char * line_name2 = NULL;
    char * line_seq2 = NULL;
    char * line_plus2 = NULL;
    char * line_qual2 = NULL;
    char * filename_left;
    char * filename_right;
    char * outname_left;
    char * outname_right;
    size_t len;
    filename_left = StringValueCStr(left);
    filename_right = StringValueCStr(right);

    outname_left = strdup(filename_left);
    outname_right = strdup(filename_right);

    strcat(outname_left, ".out");
    strcat(outname_right, ".out");

    // printf("in  left  = %s \n", filename_left);
    // printf("in  right = %s \n", filename_right);
    // printf("out  left  = %s \n", outname_left);
    // printf("out  right = %s \n", outname_right);

    lfp = fopen(filename_left, "r");
    rfp = fopen(filename_right, "r");

    if (lfp == NULL) {
      fprintf(stderr, "Can't open left read file\n");
      exit(1);
    }
    if (rfp == NULL) {
      fprintf(stderr, "Can't open right read file\n");
      exit(1);
    }

    lout = fopen(outname_left, "w");
    rout = fopen(outname_right, "w");
    if (lout == NULL) {
        printf("Error opening left output file for writing\n");
        exit(1);
    }
    if (rout == NULL) {
        printf("Error opening right output file for writing\n");
        exit(1);
    }
    counter = 0;
    keeper = 0;
    while ( getline(&line_name1, &len, lfp) != -1 ) {
        getline(&line_seq1, &len, lfp);
        getline(&line_plus1, &len, lfp);
        getline(&line_qual1, &len, lfp);
        getline(&line_name2, &len, rfp);
        getline(&line_seq2, &len, rfp);
        getline(&line_plus2, &len, rfp);
        getline(&line_qual2, &len, rfp);

        med1 = median(line_seq1);
        med2 = median(line_seq2);

        if (med1 < cutoff || med2 < cutoff) {
            add(line_seq1);
            add(line_seq2);
            fprintf(lout,"%s",line_name1);
            fprintf(lout,"%s",line_seq1);
            fprintf(lout,"%s",line_plus1);
            fprintf(lout,"%s",line_qual1);

            fprintf(rout,"%s",line_name2);
            fprintf(rout,"%s",line_seq2);
            fprintf(rout,"%s",line_plus2);
            fprintf(rout,"%s",line_qual2);
            keeper++;
        }
        counter++;
        if (counter % 500000 == 0) {
            printf("reads: %lu \t kept: %lu \n", counter, keeper);
        }
    }

    fclose(lfp);
    fclose(rfp);

    fclose(lout);
    fclose(rout);

    return INT2NUM(0);
}

void add(char * read) {
    int b, start, len;
    long h;
    uint8_t count;
    len = strlen(read) - kmer_size;
    for (start = 0; start < len; start++) {
        for (b = 0; b < set_count; b++) {
            h = hash(read, start, kmer_size, b);
            count = set[b*set_len+h];
            if (count < 254) {
                set[b*set_len+h]++;
            }
        }
    }
}

// takes in a fastq read sequence eq 100 bases
// kmerise the string, hash each kmer and add to set
VALUE method_radd(VALUE self, VALUE read) {
    char * str;
    int b,i,len;
    long h;
    h = 0;
    len = RSTRING_LEN(read);
    str = StringValueCStr(read);
    for (i = 0; i < len-kmer_size+1; i++) {
        for(b = 0; b < set_count; b++) {
            h = hash(str, i, kmer_size, b);
            set[b*set_len+h] += 1; // TODO check for overflows
        }
    }
    return INT2NUM(1); // return 1 to indicate true /shrug
}

// returns the count for a kmer
VALUE method_get(VALUE self, VALUE kmer) {
    char * str;
    int b,v=-1,p,len;
    long h;
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

uint8_t median(char * read) {
    int b,start,len,pos;
    uint8_t v = 255,p;
    long h;
    len = strlen(read) - kmer_size;
    for (start = 0; start < len; start++) {
        for (b = 0; b < set_count; b++) {
            h = hash(read, start, kmer_size, b);
            p = set[b*set_len+h];
            if (p < v) { // this is the min part of count-min-sketch
                v = p;
            }
        }
        counts[start] = v;
    }
    // insertion sort
    uint8_t tmp; // temporary storage while swapping
    uint8_t smallest; // smallest value from counts found
    //pos; // position of smallest value found
    for (start = 0; start < len; start++) {
        smallest = 255;
        pos=0;
        for (b = start; b < len; b++) {
            if (counts[b] < smallest) {
                smallest = counts[b];
                pos = b;
            }
        }
        // swap element in position start and position pos
        // printf("swapping %i and %i \n", start, pos);
        tmp = counts[start];
        counts[start] = counts[pos];
        counts[pos] = tmp;
    }
    b = (int)len/2;
    return counts[b];
}

// returns a ruby array of counts for each kmer in a read
VALUE method_rmedian(VALUE self, VALUE read) {
    char * str;
    int b, start,len,v=-1,p;
    long h;
    VALUE array = rb_ary_new();
    len = RSTRING_LEN(read);
    str = StringValueCStr(read);
    len = len - kmer_size + 1;
    for (start = 0; start < len; start++) {
        for (b = 0; b < set_count; b++) {
            h = hash(str, start, kmer_size, b);
            if (v < 0) {
                v = set[b*set_len+h];
            } else {
                p = set[b*set_len+h];
                if (p < v) {
                    v = p;
                }
            }
        }
        rb_ary_push(array, INT2NUM(v));
    }
    return array;
}

//calculates the hashing function on a kmer and the
// reverse complement of that kmer and takes the larger
// of the two and return it.
long hash(char * str, int start, int len, int n) {
    int i;
    long hash=0, hash2=0;
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
        // hash = hash % set_len;
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
        // hash2 = hash2 % set_len;
    }
    hash += offsets[n];
    hash2 += offsets[n];
    if (hash2 > hash) {
        hash2 = hash2 % set_len;
        return hash2;
    } else {
        hash = hash % set_len;
        return hash;
    }
}

// ruby method. returns hash value of a kmer
VALUE method_hashing(VALUE self, VALUE kmer, VALUE n) {
    char * str;
    int len;
    long output;
    int b;
    len = RSTRING_LEN(kmer);
    str = StringValueCStr(kmer);
    b = NUM2INT(n);
    output = hash(str, 0, len, b);
    return INT2NUM(output);
}

VALUE method_kmer_size(VALUE self) {
    return INT2NUM(kmer_size);
}