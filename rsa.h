/*
 * rsa.h
 *
 *  Created on: 09-May-2017
 *      Author: navin
 */

#include <stdint.h>

int addbignum(uint64_t *res, uint64_t *op1, uint64_t *op2,uint32_t n);
int subbignum(uint64_t *res, uint64_t *op1, uint64_t *op2,uint32_t n);
int modbignum(uint64_t *res,uint64_t *op1, uint64_t *op2,uint32_t n);
int modnum(uint64_t *res,uint64_t *op1, uint64_t *op2,uint32_t n);
int modmult1024(uint64_t *res, uint64_t *op1, uint64_t *op2,uint64_t *mod);
int rsa1024(uint64_t *res, uint64_t *data, uint64_t *expo,uint64_t *key);
int multbignum(uint64_t *res, uint64_t *op1, uint32_t op2 ,uint32_t n);
unsigned int bit_length(uint64_t *op,uint32_t n);
int compare(uint64_t *op1, uint64_t *op2,uint32_t n);
int slnbignum(uint64_t *res, uint64_t *op,uint32_t len, uint32_t n);//shift left by n
int srnbignum(uint64_t *res, uint64_t *op,uint32_t len, uint32_t n);

void rsa_gen_key(uint64_t *publicKey, uint64_t *privateKey, uint64_t *modulus);
void rsa_decrypt(uint64_t *decryptedMessage, uint64_t *encryptedMessage, uint64_t *privateKey);
