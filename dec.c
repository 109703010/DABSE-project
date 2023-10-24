#include <stdio.h>

void dec(Pk* pk, Sk* sk, Ct ctpl, char* message){
    message = "abc";
}

int main(){
    Pk pk;
    Sk sk;
    Ct ctpl;
    char* message;
    dec(pk, sk, ctpl, message);
}