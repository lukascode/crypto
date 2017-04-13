
#include "utils.h"
#include <string.h>

/* Ciphertext Autokey */
class CA {
public:
    CA(std::vector<int> seed, std::vector<int> poly) : rg(seed, poly) {}

    void encrypt(uint8_t* data, size_t bytes) {
        for(int i=0; i<bytes; ++i) {
            Byte byte(data[i]);
            for(int j=7; j>=0; --j) {
                int bit = rg.next(byte.getbit(j));
                byte.setbit(j, bit);
            } data[i] = byte.get();
        }
    }

    void decrypt(uint8_t* data, size_t bytes) {
        for(int i=0; i<bytes; ++i) {
            Byte byte(data[i]);
            for(int j=7; j>=0; --j) {
                int bit = rg.next(byte.getbit(j), true);
                byte.setbit(j, bit);
            } data[i] = byte.get();
        }
    }

private:
    RandomGenerator rg;
};

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Icorrect number of parameters\n");
        help();
        exit(1);
    }
    std::vector<int> seed, poly;
    FILE* in = NULL;
    FILE* out = NULL;
    if(strcmp(argv[1], "--help") == 0) {
        help();
        exit(1);
    }
    else if(strcmp(argv[1], "--encrypt") == 0) { //ENCRYPT
        getCheckAndFill(seed, poly, &in, &out);
        CA ca(seed, poly);
        uint8_t data[BSIZE];
        while(true) {
            size_t s = fread(data, 1, BSIZE, in);
            if(s < BSIZE) {
                ca.encrypt(data, s);
                fwrite(data, 1, s, out);
                break;
            }
            ca.encrypt(data, BSIZE);
            fwrite(data, 1, BSIZE, out);
        }
        fclose(in);
        fclose(out);
    } else if(strcmp(argv[1], "--decrypt") == 0) { //DECRYPT
        getCheckAndFill(seed, poly, &in, &out);
        CA ca(seed, poly);
        uint8_t data[BSIZE];
        while(true) {
            size_t s = fread(data, 1, BSIZE, in);
            if(s < BSIZE) {
                ca.decrypt(data, s);
                fwrite(data, 1, s, out);
                break;
            }
            ca.decrypt(data, BSIZE);
            fwrite(data, 1, BSIZE, out);
        }
        fclose(in);
        fclose(out);
    } else {
        printf("Undefined operation (encrypt|decrypt available)\n");
        exit(2);
    }

    return 0;
}