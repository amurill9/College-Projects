
#include "hash.h"

// Ray Beaulieu, Stefan Treatman-Clark, Douglas Shors, Bryan Weeks, Jason
// Smith and Louis Wingers. "The SIMON and SPECK lightweight block ciphers," 
// In proceedings of the Design Automation Conference (DAC), 
// 2015 52nd ACM/EDAC/IEEE, pp. 1-6. IEEE, 2015.

#define LCS(X, K)                                                            \
  (X << K) | (X >> (sizeof(uint64_t) * 8 - K)) // left circular shift
#define RCS(X, K)                                                            \
  (X >> K) | (X << (sizeof(uint64_t) * 8 - K)) // right circular shift

// Core SPECK operation
#define R(x, y, k) (x = RCS(x, 8), x += y, x ^= k, y = LCS(y, 3), y ^= x)

void Speck128ExpandKeyAndEncrypt(uint64_t pt[], uint64_t ct[], uint64_t K[]) {
  uint64_t B = K[1], A = K[0];
  ct[0] = pt[0];
  ct[1] = pt[1];
  for (size_t i = 0; i < 32; i += 1) {
    R(ct[1], ct[0], A);
    R(B, A, i);
  }
  return;
}

uint64_t keyedHash(const char *s, uint32_t length, uint64_t key[]) {
  uint64_t accum = 0;

  union {
    char b[sizeof(uint64_t)]; // 16 bytes fit into the same space as
    uint64_t ll[2];           // 2 64 bit numbers.
  } in;

  uint64_t out[2]; // Speck results in 128 bits of ciphertext
  uint32_t count;

  count = 0;                      // Reset buffer counter
  in.ll[0] = 0x0; in.ll[1] = 0x0; // Reset the input buffer (zero fill)

  for (size_t i = 0; i < length; i += 1) {
    in.b[count++] = s[i]; // Load the bytes
    if (count % (2 * sizeof(uint64_t)) == 0) {
      Speck128ExpandKeyAndEncrypt(in.ll, out, key); // Encrypt 16 bytes
      accum ^= out[0] ^ out[1]; // Add (XOR) them in for a 64 bit result
      count = 0;                       // Reset buffer counter
      in.ll[0] = 0x0; in.ll[1] = 0x0;  // Reset the input buffer
    }
  }

  // There may be some bytes left over, we should use them.
  if (length % (2 * sizeof(uint64_t)) != 0) {
    Speck128ExpandKeyAndEncrypt(in.ll, out, key);
    accum ^= out[0] ^ out[1];
  }
  return accum;
}
 //
 // Constructor for a HashTable .
 //
 // length : Length of the HashTable .
 // salt : Salt for the HashTable .
 //
HashTable * ht_create ( uint32_t length ) {
  HashTable *ht = ( HashTable *) malloc ( sizeof ( HashTable ) ) ;
  if (ht) {
    ht->salt [0] = 0x85ae998311115ae3; // Il nome della rosa
    ht->salt [1] = 0xb6fac2ae33a40089;
    ht->length = length ;  
    ht->heads = ( ListNode **) calloc ( length , sizeof ( ListNode *) ) ;
    return ht;
  }

  return ( HashTable *) NIL ;
}

 // The rest of the functions you will implement yourselves .

void ht_delete (HashTable *ht){
  uint32_t i;
  for(i=0;i<ht->length;i++) ll_delete(ht->heads[i]);
  free(ht->heads);
  free(ht);
}
#include <stdio.h>
ListNode* ht_lookup ( HashTable *ht ,const char * key){
  uint32_t h = hash(ht->salt,(char*)key);
  ListNode* head = ht->heads[h%ht->length];
  if(!head) return NULL;
  return ll_lookup(&head,key);
}

void ht_insert(HashTable *ht ,GoodSpeak *gs){
  uint32_t h = hash(ht->salt,gs->old_speak);
  if(!ht->heads[h%ht->length]) ht->heads[h%ht->length] = ll_node_create(gs);
  else ll_insert(&ht->heads[h%ht->length],gs);
}
