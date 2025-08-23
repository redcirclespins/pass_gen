#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFLEN 32
#define CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:,.<>?/\\\"'`"
#define CHARSETLEN strlen(CHARSET)

static void genPass(unsigned char* pass,const unsigned len){
    unsigned char temp;
    register int i=0;
    register int j=0;

    //seed the random
    srand(time(NULL));

    //rand array
    for(i=0;i<len;i++)
        pass[i]=CHARSET[rand()%CHARSETLEN];

    //shuffle
    for(i=len-1;i>0;i--){
        j=rand()%(i+1);
        temp=pass[i];
        pass[i]=pass[j];
        pass[j]=temp;
    }

    //shuffle more
    //(more spread between identical symbols)
    for(i=0;i<len;i++){
        for(j=i+1;j<len;j++){
            if(pass[i]==pass[j])
                pass[j]=CHARSET[rand()%CHARSETLEN];
        }
    }
    pass[len]=0;
}

static unsigned checkKeyLen(const char* len_char){
    if(len_char==NULL)
        return DEFLEN;
    for(int i=0;len_char[i];i++){
        if(isdigit((unsigned char)len_char[i])==0){
            puts("key-len must be uint -> fallback to 32 char");
            return DEFLEN;
        }
    }
    return atoi(len_char);
}

int main(int argc,char** argv){
    if(argc!=1&&argc!=2){
        printf("usage: %s <key-len> (default: 32)\n",argv[0]);
        return 1;
    }

    const unsigned len=checkKeyLen(argc==2?argv[1]:NULL);
    unsigned char* pass=malloc(sizeof(unsigned char)*(len+1)); //+\0
    if(pass==NULL){
        perror("malloc failed");
        return 1;
    }
    genPass(pass,len);
    printf("%s\n",pass);
    free(pass);

    return 0;
}
