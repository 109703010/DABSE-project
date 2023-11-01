#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

/*#define DEBUG = 1*/

unsigned char iv[EVP_MAX_IV_LENGTH] = "0123456789012345"; // 16字节IV

// 加密函数
int SEnc(const unsigned char *plaintext, int plaintext_len,
	mpz_t k, unsigned char *ciphertext);

// 解密函数
int SDec(const unsigned char *ciphertext, int ciphertext_len,
	mpz_t k, unsigned char *plaintext);

#if defined(DEBUG)
int main() {
  // 明文
  unsigned char plaintext[] = "Hello, world!";
  int plaintext_len = strlen((char *)plaintext);
  mpz_t k;
  mpz_init_set_ui(k, 9861321);

  // 分配足够的内存来存储密文
  unsigned char ciphertext[plaintext_len + EVP_MAX_BLOCK_LENGTH];
  memset(ciphertext, 0, sizeof(ciphertext));

  // 加密
  int ciphertext_len = SEnc(plaintext, plaintext_len, k, ciphertext);
  if (ciphertext_len == -1) {
	printf("加密失败\n");
	return -1;
  }

  // 输出密文
  printf("密文：");
  for (int i = 0; i < ciphertext_len; i++) {
	printf("%02x", ciphertext[i]);
  }
  printf("\n");

  // 分配足够的内存来存储解密后的明文
  unsigned char decryptedtext[plaintext_len];
  memset(decryptedtext, 0, sizeof(decryptedtext));

  // 解密
  int decryptedtext_len = SDec(ciphertext, ciphertext_len, k, decryptedtext);
  if (decryptedtext_len == -1) {
	printf("解密失败\n");
	return -1;
  }
  decryptedtext[decryptedtext_len] = '\0';

  // 输出解密后的明文
  printf("解密后的明文：%s\n", decryptedtext);

  return 0;
}
#endif

int SEnc(const unsigned char *plaintext, int plaintext_len,
	mpz_t k, unsigned char *ciphertext) {
  EVP_CIPHER_CTX *ctx;
  // 密钥和初始化向量（IV）
  unsigned char key[EVP_MAX_KEY_LENGTH] = "01234567890123456789012345678901"; // 32字节密钥
  mpz_get_str(key, 10, k);

  int len;
  int ciphertext_len;

  // 创建并初始化加密上下文
  if (!(ctx = EVP_CIPHER_CTX_new())) {
	return -1;
  }

  // 初始化加密操作，使用AES-256-CBC算法
  if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
	EVP_CIPHER_CTX_free(ctx);
	return -1;
  }

  // 执行加密操作
  if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1) {
	EVP_CIPHER_CTX_free(ctx);
	return -1;
  }
  ciphertext_len = len;

  // 完成加密操作
  if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
	EVP_CIPHER_CTX_free(ctx);
	return -1;
  }
  ciphertext_len += len;

  // 释放加密上下文
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

// 解密函数
int SDec(const unsigned char *ciphertext, int ciphertext_len,
   	mpz_t k, unsigned char *plaintext) {
  EVP_CIPHER_CTX *ctx;
  unsigned char key[EVP_MAX_KEY_LENGTH] = "01234567890123456789012345678901"; // 32字节密钥
  mpz_get_str(key, 10, k);

  int len;
  int plaintext_len;

  // 创建并初始化解密上下文
  if (!(ctx = EVP_CIPHER_CTX_new())) {
	return -1;
  }

  // 初始化解密操作，使用AES-256-CBC算法
  if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
	EVP_CIPHER_CTX_free(ctx);
	return -1;
  }

  // 执行解密操作
  if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1) {
	EVP_CIPHER_CTX_free(ctx);
	return -1;
  }
  plaintext_len = len;

  // 完成解密操作
  if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
	EVP_CIPHER_CTX_free(ctx);
	return -1;
  }
  plaintext_len += len;

  // 释放解密上下文
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}
