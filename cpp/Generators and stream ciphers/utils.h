#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <deque>
#include <cstdint>
#include <string>
#include <iostream>

#define BSIZE 1024

/* LFSR generator */
class RandomGenerator {
public:
    RandomGenerator(std::vector<int> seed, std::vector<int> poly) {
        if(seed.size() != poly.size()) 
            throw "seed length and poly length should be equal";
        this->seed = seed;
        this->poly = poly;
        this->lfsr = NULL;
        reset();
    }

    virtual int next(int bitin=0, bool bit_in_to_front=false) {
        int sum = getSumPoly();
        int _xor = (sum+bitin) % 2;
        lfsr->pop_back(); 
        // lfsr->push_front(_xor);
        lfsr->push_front( (bit_in_to_front==false)?_xor:bitin );
        return _xor;
    }

    void reset() {
        if(lfsr != NULL)
            delete lfsr;
        lfsr = new std::deque<int>();
        for(int i=0; i<seed.size(); ++i) {
            lfsr->push_back(seed[i]);
        }
    }

    ~RandomGenerator() {
        delete lfsr;
    }
    
protected:
    std::vector<int> seed;
    std::vector<int> poly;
    std::deque<int>* lfsr;

    int getSumPoly() {
        int sum = 0;
        for(int i=0; i<poly.size(); ++i) {
            if(poly[i]) {
                if( (*lfsr)[i] ) 
                    sum += 1;
            }
        } return sum;
    }

};

class Byte {
public:
    Byte(uint8_t byte) {
        this->byte = byte;
    }

    void xorbit(size_t which, int value) {
        if(value != 1) return;
        which %= 8;
        uint8_t arg = (0x01 << which);
        byte ^= arg;
    }

    int getbit(size_t which) {
        which %= 8;
        uint8_t arg = (0x01 << which);
        if(byte & arg)
            return 1;
        return 0;
    }

    int setbit(size_t which, int value) {
        which %= 8; value %= 2;
        if(value == 0) { //set to 0
            uint8_t arg = 0xFF, arg2 = (0x01 << which);
            arg ^= arg2;
            byte &= arg;
        } else { //set to 1
            uint8_t arg = (0x01 << which);
            byte |= arg;
        }
    }

    uint8_t get() {
        return byte;
    }
    uint8_t set(uint8_t byte) {
        this->byte = byte;
    }
private:
    uint8_t byte;
};

std::vector<int> getvectorbin(std::string binary, int* e) {
    *e = 0; std::vector<int> result;
    for(size_t i=0; i<binary.length(); ++i) {
        if(binary[i] == '0') result.push_back(0);
        else if(binary[i] == '1') result.push_back(1);
        else { *e = -1; return std::vector<int>(); }
    } return result;
}

struct Cinfo {
    std::string seed;
    std::string poly; 
    std::string in, out;
};

void getparams(Cinfo* cinfo) {
    if(cinfo == NULL) return;
    std::cout << "Seed: "; std::cin>>cinfo->seed;
    std::cout << "Poly: "; std::cin>>cinfo->poly;
    std::cout << "Input: "; std::cin>>cinfo->in;
    std::cout << "Output: "; std::cin>>cinfo->out;
}

void getCheckAndFill(std::vector<int>& seed, std::vector<int>& poly, FILE** in, FILE** out) {
     Cinfo cinfo;
     getparams(&cinfo);
    if(cinfo.seed.length() != cinfo.poly.length())
        { printf("error. seed.len and poly.len should be equal\n");   exit(3); }
    int e; seed = getvectorbin(cinfo.seed, &e);
    if(e != 0) { printf("error. seed should contains only 0 or 1\n"); exit(4); }
    poly = getvectorbin(cinfo.poly, &e);
    if(e != 0) { printf("error. poly should contains only 0 or 1\n"); exit(5); }
    *in = fopen(cinfo.in.c_str(), "rb");
    if(*in == NULL) { perror(cinfo.in.c_str()); exit(6); }
    *out = fopen(cinfo.out.c_str(), "wb");
    if(*out == NULL) { perror(cinfo.out.c_str()); exit(7); }
}

void help() {
    printf("encryption: cipher1 --encrypt\n");
    printf("dencryption: cipher1 --decrypt\n");
}

#endif