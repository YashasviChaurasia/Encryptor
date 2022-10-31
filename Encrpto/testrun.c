#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

void handleErrors(void);
int gcm_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *aad, int aad_len,
                unsigned char *key,
                unsigned char *iv, int iv_len,
                unsigned char *ciphertext,
                unsigned char *tag);
int gcm_decrypt(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *aad, int aad_len,
                unsigned char *tag,
                unsigned char *key,
                unsigned char *iv, int iv_len,
                unsigned char *plaintext);

int main(void)
{

    char *filename = "million.txt";
    FILE *fp = fopen(filename, "r");

    fseek(fp, 0, SEEK_END); // seek to end of file
    int size = ftell(fp);       // get current file pointer
    fseek(fp, 0, SEEK_SET);
    // printf("%d file size",size);

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }
    const unsigned MAX_LENGTH = 64;
    char buffer[MAX_LENGTH];

    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    unsigned char *iv = (unsigned char *)"0123456789012345";
    size_t iv_len = 16;

    unsigned char *additional =
        (unsigned char *)"The five boxing wizards jump quickly.";

    unsigned char ciphertext[128];

    unsigned char decryptedtext[128];

    unsigned char tag[16];

    int ciphertext_len;
    clock_t begin = clock();
    while (fgets(buffer, MAX_LENGTH, fp))
    {
        unsigned char *plaintext =
            (unsigned char *)buffer;

        ciphertext_len = gcm_encrypt(plaintext, strlen((char *)plaintext),
                                     additional, strlen((char *)additional),
                                     key,
                                     iv, iv_len,
                                     ciphertext, tag);
        printf("Ciphertext is:\n");
        BIO_dump_fp(stdout, (const char *)ciphertext, ciphertext_len);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n ______BenchMarks______\n\nTime Taken: %f\nData Encrypted: %d Bytes\nBytes per second: %f \nPackets per second: %f\n",time_spent,size, size/time_spent,(size/MAX_LENGTH)/time_spent);
    return 0;
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int gcm_encrypt(unsigned char *plaintext, int plaintext_len,
                unsigned char *aad, int aad_len,
                unsigned char *key,
                unsigned char *iv, int iv_len,
                unsigned char *ciphertext,
                unsigned char *tag)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        handleErrors();

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
        handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv))
        handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
        handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
        handleErrors();

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}
