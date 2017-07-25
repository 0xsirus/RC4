/*
  Cyrus.Sh
  ~cyn
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char                               s[256];
unsigned char                               t[256];
unsigned char                               key[256];

void swap(unsigned char *p1, unsigned char *p2){
    unsigned char t = *p1;
    *p1 = *p2;
    *p2 = t;
}

void rc4_init(unsigned char *key,int key_len){
    int                                     i,j=0;
    char                                    *k=key;
    //Initial values of both vectors
    for (i=0;i<256;i++){
        s[i] = i;
        t[i] = k[i%key_len];
    }
    //Initial permutation 
    for (i=0;i<256;i++){
        j = (j + s[i] + t[i])%256;
        swap(&s[i],&s[j]);
    }
    
}

void rc4(unsigned char  *key,int key_len,char *buff,int len){   
    rc4_init(key,key_len);
    int i=0;
    //process one byte at a time
    unsigned long t1,t2;
    unsigned char val;
    unsigned char out; 
    t1=0;t2=0;
    for (i=0;i<len;i++){     
        t1 = (t1 + 1)%256;
        t2 = (t2 + s[t1])%256;
        swap(&s[t1],&s[t2]);
        val = (s[t1] + s[t2])%256;
        out = *buff ^ val;
        *buff=out;        
        buff++;        
   }     
}

int main(int argc,char ** argv){  
    unsigned char message[1000];
    strcpy(message,"Find the treasure 3 meters away from that tall tree where the kid passes by!");
    int message_len = strlen(message);
    unsigned char result[1000];
    FILE *f = fopen("/dev/urandom","r");
    unsigned char key[10];
    fread(key,10,1,f);
    int i;
    for (i=0;i<10;i++){
        key[i] = (key[i]/256.0*(rand()%26))+65;
    }  
    printf("> Generated sample key: \033[96m%s\033[0m\n",key);
    printf("> Secret text: \033[96m%s\033[0m\n",message);
    printf("> Encryted message: \033[96m");
    rc4(key,10,message,message_len);
    //print HEX encoding of the encrypted message 
    for (i=0;i<message_len;i++){
        printf("%02X",message[i]);
    }
    printf("\n\033[0m> Decryted RC4 data with the same key: ");
    rc4(key,10,message,message_len);
    printf("\033[96m%s\033[0m\n",message);
}
