#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <stdlib.h>

int aes(char *plainfile, char *cipherfile, char *keyword)
{   
    struct stat st;
    FILE* file=fopen(plainfile,"r");
    if(file == NULL)
    {
        printf("open plain file failed");
        return -1;
    }

    stat(plainfile, &st);
    char filesize[16] = {0};
    sprintf(filesize, "%u", st.st_size);

    unsigned char plain[17]={0};
    unsigned char *userKey=(unsigned char *)keyword;
    AES_KEY aes_key;
    int ret=AES_set_encrypt_key(userKey,128,&aes_key);
    if(ret<0){
        printf("设置密钥失败!!\n");
        return -2;          
    }

    FILE* cfile=fopen(cipherfile,"w+");
    if(cfile==NULL)
    {
        printf("open cipher file failed!!\n");
        return -1;
    }
    AES_ecb_encrypt(filesize,filesize,&aes_key,AES_ENCRYPT);
    fwrite(filesize,1,16,cfile);
    unsigned char cipher[17]={0};
    int num=fread(plain,1,16,file);
    while(num!=0){
        AES_ecb_encrypt(plain,cipher,&aes_key,AES_ENCRYPT);
        memset(plain,0,17);
        num=fread(plain,1,16,file);
        fwrite(cipher,1,16,cfile);
    }
    return 0;
}

int main(int argc,char* argv[])
{
    if(argc<4)
    {
        printf("Usage:./enc <plainfile> <cipherfile> <keyword>\n");
        return 0;
    }
    
    aes(*(argv+1),*(argv+2),*(argv+3));

    return 0;
}
