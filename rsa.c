#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "rsa.h"

int rsa1024(uint64_t res[], uint64_t data[], uint64_t expo[],uint64_t key[])
{
    int32_t i,j,expo_len;
    uint64_t mod_data[18]={0},result[18]={0};
    uint64_t temp_expo=0;
    
    modbignum(mod_data,data,key,16);
    result[0] = 1;
    expo_len = bit_length(expo,16) /64;
    for(i=0;i<expo_len+1;i++)
    {
        temp_expo = expo[i];
        for(j=0;j<64;j++)
        {
            if(temp_expo & 0x1UL)
                modmult1024(result,result,mod_data,key);
            
            modmult1024(mod_data,mod_data,mod_data,key);
            temp_expo = temp_expo >> 1;
        }
    }
    for(i=0;i<16;i++)
        res[i]=result[i];
    
    
    return 1;
}

int addbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n)
{
	uint32_t i;
	uint64_t j,k,carry=0;
	for(i = 0; i<n; i++)
	{
		j = (op1[i] & 0xffffffff) + (op2[i] & 0xffffffff) + carry;
		
		k = ((op1[i]>>32) & 0xffffffff) + ((op2[i]>>32) & 0xffffffff) + ((j>>32) & 0xffffffff);
        
		carry = ((k>>32) & 0xffffffff);
		
		res[i] = ((k & 0xffffffff)<<32)  | (j & 0xffffffff);
	}
	res[i] = carry;
	return 0;
} 

int multbignum(uint64_t res[], uint64_t op1[], uint32_t op2 ,uint32_t n)
{
	uint32_t i;
	uint64_t j,k,carry1=0,carry2=0;
	for(i = 0; i<n; i++)
	{
		j = (op1[i] & 0xffffffff) * (op2 & 0xffffffff);
		
		k = ((op1[i]>>32) & 0xffffffff) * (op2 & 0xffffffff);
		carry1 = ((k>>32) & 0xffffffff);
		k = (k & 0xffffffff) + ((j>>32) & 0xffffffff);
		j = (j & 0xffffffff) + carry2;
		k = k + ((j>>32) & 0xffffffff);
		carry2 = carry1 + ((k>>32) & 0xffffffff);
		
		res[i] = ((k & 0xffffffff)<<32)  | (j & 0xffffffff);
	}
	res[i] = carry2;
	return 0;
}

int modmult1024(uint64_t res[], uint64_t op1[], uint64_t op2[],uint64_t mod[]) //optimized
{
    int32_t i,j;
    uint64_t mult1[33]={0},mult2[33]={0},
    result[33]={0},xmod[33]={0};
    
    for(i=0;i<16;i++)
        xmod[i]=mod[i];
    
    for(i=0;i<16;i++)
    {
        for(j=0;j<33;j++)
        {
            mult1[j]=0;
            mult2[j]=0;
        }
        multbignum(mult1,op1,(op2[i]&0xffffffff),16);
        multbignum(mult2,op1,((op2[i]>>32)&0xffffffff),16);
        slnbignum(mult2,mult2,33,32);
        addbignum(mult2,mult2,mult1,32);
        
        slnbignum(mult2,mult2,33,64*i);
        
        addbignum(result,result,mult2,32);
        
    }
    modbignum(result,result,xmod,33);
    for(i=0;i<16;i++)
        res[i]=result[i];
    
    return 0;
}

int modbignum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n)//optimized
{
    uint32_t i;
    int32_t len_op1,len_op2,len_dif;
    
    len_op1 = bit_length(op1,n);
    len_op2 = bit_length(op2,n);
    len_dif = len_op1 - len_op2;
    
    
    
    for(i=0;i<n;i++)
        res[i]=op1[i];
    
    if(len_dif < 0)
    {
        return 1;
    }
    
    if(len_dif == 0)
    {
        while(compare(res,op2,n)>=0)
        {
            subbignum(res,res,op2,n);
        }
        return 1;
    }
    
    slnbignum(op2,op2,n,len_dif);
    for(i=0;i<len_dif;i++)
    {
        srnbignum(op2,op2,n,1);
        while(compare(res,op2,n)>=0)
        {
            subbignum(res,res,op2,n);
        }
    }
    
    return 1;
}

/****************************************************************
 * bool modnum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n)
 * res = op1 % op2
 * n is bit length/64
 * res must have extra 64 bits to avoid errors 
 ****************************************************************/
int modnum(uint64_t res[],uint64_t op1[], uint64_t op2[],uint32_t n)
{
	uint32_t i;
    int result = 0;
	for(i=0;i<n;i++)
		res[i]=op1[i];
    
	while(!result)
	{
		result = subbignum(res,res,op2,n);
	}
	
	addbignum(res,res,op2,n);
	res[n]=0;
    
	return 0;
}

/****************************************************************
* int32_t compare(uint64_t op1[], uint64_t op2[],uint32_t n)
* returns 1 if op1>op2
* 		 -1 if op1<op2
* 		  0 if op1=op2
*****************************************************************/
int32_t compare(uint64_t op1[], uint64_t op2[],uint32_t n)
{
	for( ; n>0; n--)
	{
		if(op1[n-1]>op2[n-1])
		{
			return 1;
		}
		else if(op1[n-1]<op2[n-1])
		{
			return -1;
		}
	}
    
	return 0;
}

/****************************************************************
 * bool subbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n)
 * subtracts op2 from op1
 * returns 0 if op1>=op2
 * 		   1 if op1<op2
 * result is not valid if return value is 1 (or is in 2's compliment :P)
 * **************************************************************/
int subbignum(uint64_t res[], uint64_t op1[], uint64_t op2[],uint32_t n)
{
    int carry=0;
	uint32_t i;
	for(i=0;i<n;i++)
	{
		if(carry)
		{
			if(op1[i]!=0)
				carry=0;
			op1[i]--;		
		}
		if(op1[i]<op2[i])
			carry = 1;
        
		res[i]= op1[i] - op2[i];
	}	
	return carry;
}

int slnbignum(uint64_t res[], uint64_t op[],uint32_t len, uint32_t n)//shift left by n
{
    uint32_t i,x,y;
    uint64_t j,k,carry = 0;
    x = n / 64;
    y = n % 64;
    
    for(i=len; i - x >0; i--)
    {
        res[i-1] = op[i - 1 - x];
    }
    for(;i>0;i--)
    {
        res[i-1] = 0;
    }
    for(i=0;i<len;i++)
    {
        j = res[i];
        k=0;
        for(x=0;x<y;x++)
        {
            if(j & 0x8000000000000000)
            {
                k = (k<<1) | 1;
            }
            else
            {
                k = (k<<1);
            }
            j = j <<1;
        }
        res[i] = j | carry;
        carry = k;
    }
    return 1;
}

int srnbignum(uint64_t res[], uint64_t op[],uint32_t len, uint32_t n)//shift right by n
{
    uint32_t i,x,y;
    uint64_t j,k,carry = 0;
    x = n / 64;
    y = n % 64;
    
    for(i=0; i + x < len; i++)
    {
        res[i] = op[i + x];
    }
    for(;i<len;i++)
    {
        res[i] = 0;
    }
    for(i=len;i>0;i--)
    {
        j = res[i-1];
        k=0;
        for(x=0;x<y;x++)
        {
            if(j & 0x0000000000000001)
            {
                k = (k>>1) | 0x8000000000000000;
            }
            else
            {
                k = (k>>1);
            }
            j = j >>1;
        }
        res[i-1] = j | carry;
        carry = k;
    }
    return 1;
    
}

/****************************************************************
 * uint32_t bit_length(uint64_t op[],uint32_t n)
 * returns position of MSB present
 *
 *
 ****************************************************************/
uint32_t bit_length(uint64_t op[],uint32_t n)
{
    uint32_t len=0;
    uint32_t i;
    uint64_t unit = 1;
    for( ;n>0;n--)
    {
        if(op[n-1]==0)
            continue;
        for(i=64;i>0;i--)
        {
            if(op[n-1] & (unit<<(i-1)))
            {
                len = (64*(n-1)) + i;
                break;
            }
            
        }
        if(len)
            break;
    }
    return len;
}

#define MAX_ITERATIONS 50  // Adjust the number of Miller-Rabin iterations based on your requirements

// Function to perform modular exponentiation: (base^exp) % mod
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Miller-Rabin primality test
int is_probable_prime(uint64_t n, int k) {
    if (n <= 1 || n == 4) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    
    // Find d such that n-1 = 2^r * d
    uint64_t d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }
    
    // Perform Miller-Rabin test k times
    for (int i = 0; i < k; i++) {
        uint64_t a = 2 + rand() % (n - 3);
        uint64_t x = mod_exp(a, d, n);
        
        if (x == 1 || x == n - 1) {
            continue;
        }
        
        int prime_candidate = 0;
        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;
            
            if (x == 1) {
                return 0; // Not prime
            }
            if (x == n - 1) {
                prime_candidate = 1;
                break;
            }
        }
        
        if (prime_candidate == 0) {
            return 0; // Not prime
        }
    }
    
    return 1; // Probably prime
}

void set_dummy_prime(uint64_t prime[], uint32_t size, uint64_t value) {
    for (uint32_t i = 0; i < size; ++i) {
        prime[i] = value;
    }
}

// Function to generate a probable prime number of a given bit length
void generate_prime(uint64_t prime[], uint32_t bit_length) {
    srand(time(NULL));
    
    // Ensure the most significant bit is set (to achieve the desired bit length)
    prime[0] = 1ULL << (bit_length - 1);
    
    // Set random bits for the rest of the number
    for (int i = 1; i < (bit_length / 64) + 1; i++) {
        prime[i] = rand();
    }
    
    // Set the last bit to make it odd
    prime[(bit_length / 64)] |= 1;
    
    // Make sure the number is odd
    if (prime[(bit_length / 64)] % 2 == 0) {
        prime[(bit_length / 64)] += 1;
    }
    
    // Adjust other bits as necessary to achieve the desired bit length
    
    // Perform Miller-Rabin test to check primality
    while (!is_probable_prime(prime[0], MAX_ITERATIONS)) {
        // Increment by 2 to keep it odd
        prime[0] += 2;
    }
}

#if 0
void rsa_gen_key(uint64_t publicKey[], uint64_t privateKey[], uint64_t modulus[])
{
    uint64_t p[16] = {0};
    uint64_t q[16] = {0};
    // generate_prime(p, 512);
    // generate_prime(q, 512);
    
    set_dummy_prime(p, 16, 0x1234567890ABCDEFULL);
    set_dummy_prime(q, 16, 0xFEDCBA0987654321ULL);
    
    // Step 1: Choose two distinct prime numbers, p and q
    // (You need to implement a function to generate prime numbers)
    
    // Step 2: Calculate the modulus N
    modmult1024(modulus, p, q, NULL);
    
    // Step 3: Calculate the totient φ(N)
    uint64_t phiN[16]; // Adjust the size based on your implementation
    subbignum(phiN, modbignum(phiN, p, 1, NULL), modbignum(phiN, q, 1, NULL), NULL);
    
    // Step 4: Choose a public exponent (e)
    uint64_t e[16]; // Adjust the size based on your implementation
    e[0] = 65537;
    
    // Step 5: Calculate the private exponent (d)
    uint64_t d[16]; // Adjust the size based on your implementation
    rsa1024(d, e, phiN, NULL);
    
    // Set public key
    publicKey[0] = modulus[0];
    publicKey[1] = e[0];
    
    // Set private key
    privateKey[0] = modulus[0];
    privateKey[1] = d[0];
}
#endif

void rsa_gen_key(uint64_t publicKey[], uint64_t privateKey[], uint64_t modulus[])
{
    uint64_t p[16] = {0};
    uint64_t q[16] = {0};
    generate_prime(p, 512);
    generate_prime(q, 512);
    
    // Step 1: Choose two distinct prime numbers, p and q
    
    // Step 2: Calculate the modulus N
    modmult1024(modulus, p, q, NULL);
    
    // Step 3: Calculate the totient Ï†(N)
    // uint64_t phiN[16]; // Adjust the size based on your implementation
    // subbignum(phiN, modbignum(phiN, p, 1, NULL), modbignum(phiN, q, 1, NULL), NULL);
    uint64_t phiN[16]; // Adjust the size based on your implementation
    uint64_t one[] = {1};
    modbignum(phiN, p, one, 16);
    multbignum(phiN, q, 1, 16);
    subbignum(phiN, phiN, modulus, 16);
    
    // Step 4: Choose a public exponent (e)
    uint64_t e[16]; // Adjust the size based on your implementation
    e[0] = 65537;
    
    // Step 5: Calculate the private exponent (d)
    uint64_t d[16]; // Adjust the size based on your implementation
    rsa1024(d, e, phiN, NULL);
    
    // Set public key
    for (int i = 0; i < 16; i++) {
        publicKey[i] = modulus[i];
    }
    publicKey[16] = e[0];
    
    // Set private key
    for (int i = 0; i < 16; i++) {
        privateKey[i] = modulus[i];
    }
    privateKey[16] = d[0];
}


void rsa_decrypt(uint64_t decryptedMessage[], uint64_t encryptedMessage[], uint64_t privateKey[]) {
    // Use rsa1024 function with the private key
    rsa1024(decryptedMessage, encryptedMessage, privateKey, NULL);
}
