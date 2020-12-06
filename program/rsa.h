#ifndef _RSA_100_H
#define _RSA_100_H


//implementation of rsa-100 (330 bits)
typedef unsigned long long long long long int uint512_t;
typedef struct {
  uint512_t prime_1;
  uint512_t prime_2;
  uint512_t d_num;
}rsa_private_key_t;

typedef struct {
  uint512_t prod_n;
  uint512_t e_num;
}rsa_public_key_t;

typedef struct {
  rsa_private_key_t private;
  rsa_public_key_t public;
}key_pair_t;



void save_keys(key_pair_t keys, const char *path);
//Will be saved in two files path_public & path_private
key_pair_t load_keys(const char *path);

key_pair_t gen_keys(void);
void rsa_encrypt(rsa_public_key_t key, uint512_t *message);
void rsa_decrypt(rsa_private_key_t key, uint512_t *secret);



#endif
