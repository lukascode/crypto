#include "utils.h"
#include <string.h>
#include <iostream>

int main(int argc, char** argv) {

    std::string seed, poly; int len;
    std::cout<<"Seed: "; std::cin>>seed;
    std::cout<<"Poly: "; std::cin>>poly;

    std::cout<<"Length: "; std::cin>>len;
    if(!std::cin) {
        std::cout<<"length should be an integer\n";
        exit(1);
    }

    std::vector<int> vseed, vpoly; int e;
    vseed = getvectorbin(seed, &e);
    if(e != 0) {
        std::cout<<"error. seed should contains only 0 or 1\n";
        exit(2);
    }
    vpoly = getvectorbin(poly, &e);
    if(e != 0) {
        std::cout<<"error. poly should contains only 0 or 1\n";
        exit(3);
    }

    RandomGenerator rg(vseed, vpoly);
    for(int i=0; i<len; ++i) {
        std::cout<<rg.next();
    } std::cout<<std::endl;

    return 0;
}