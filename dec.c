#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <openssl/aes.h>

int aes(char *plainfile, char *cipherfile, char *keyword)
{
    FILE* file=fopen(cipherfile,"r");
    if(file == NULL)
    {
        printf("open cipher file failed");
        return -1;
    }

    unsigned char cipher[17]={0};
    unsigned char *userKey=(unsigned char *)keyword;
    AES_KEY aes_key;
    int ret=AES_set_decrypt_key(userKey,128,&aes_key);
    if(ret<0){
        printf("设置密钥失败!!\n");
        return -2;          
    }

    char filesize[16] = {0};
    fread(filesize,1,16,file);
    AES_ecb_encrypt(filesize,filesize,&aes_key,AES_DECRYPT);
    long long st_size = atoll(filesize);
    long long remain = st_size;
    int outsize = 16;

    FILE* pfile=fopen(plainfile,"w+");
    if(pfile==NULL)
    {
        printf("open plain file failed!!\n");
        return -1;
    }

    unsigned char plain[17]={0};
    int num=fread(cipher,1,16,file);
    while(num!=0){
        if(remain < 16){
            outsize = remain;
        } else {
            outsize = 16;
        }
        AES_ecb_encrypt(cipher,plain,&aes_key,AES_DECRYPT);
        memset(cipher,0,17);
        num=fread(cipher,1,16,file);
        fwrite(plain,1,outsize,pfile);
        memset(plain,0,17);
        remain = remain - outsize;
    }

    fclose(file);
    fclose(pfile);
    return 0;
}

int main(int argc,char* argv[])
{
    if(argc<4)
    {
        printf("Usage:./dec <cipherfile> <plainfile> <keyword>\n");
        return 0;
    }
    
    aes(*(argv+2),*(argv+1),*(argv+3));

    return 0;
}
