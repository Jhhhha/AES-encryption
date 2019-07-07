//
//  AES.h
//  AES
//
//  Created by 张家豪 on 2018/12/11.
//  Copyright © 2018 张家豪. All rights reserved.
//

#ifndef AES_h
#define AES_h
#include <iostream>
#include <vector>
#include <string>
#include "table.h"


typedef std::vector<unsigned> state_t;
typedef std::vector<state_t>  mes_t;

state_t string2state(std::string, int type);
state_t substitude(state_t);
state_t inv_substitude(state_t);
state_t row_shift(state_t);
state_t column_confuse(state_t);
state_t inv_shift(state_t);
state_t inv_confuse(state_t);
state_t operator ^(state_t, state_t);
std::string state2string(state_t, int type);
state_t encrypt(state_t, std::vector<state_t>);
state_t decrypt(state_t, std::vector<state_t>);


std::ostream& operator <<(std::ostream & out, state_t input){
    for(int i = 0, size = input.size(); i < size; i++){
        out << std::hex << input[i] << ' ';
        if((i+1) % 4 == 0)
            out << std::endl;
    }
    return out;
}

std::ostream& operator <<(std::ostream & out, mes_t input){
    for(int i = 0, size = input.size(); i < size; i++){
        out << input[i] << std::endl;
    }
    return out;
}

class AES{
private:
    std::string message;
    state_t key;
    
public:
    AES(std::string m, std::string k);
    std::string encrypt();
    std::string decrypt();
};

state_t gen_key(state_t key, int round){
    state_t res(16,0);
    for(int i = 0; i < 4; i++){
        res[i*4] = s_box[key[(i * 4+ 7)%16]] ^ key[i*4]; //第一列
        if(i == 0)
            res[0] = res[0] ^ RC[round];
    }
    for(int i = 1; i < 4; i++){
        for(int j = 0; j < 4; j++){//j是行数
            res[j*4 + i] = res[j*4 + i-1] ^ key[j*4 + i];
        }
    }
    return res;
}

std::vector<state_t> expand_key(state_t key){
    //    std::cout << key;
    std::vector<state_t> res;
    res.push_back(key);
    for(int i = 0; i < 10; i++){
        key = gen_key(key, i);
        //                std::cout << i+1 << "次：" << std::endl;
        //                std::cout << key << std::endl;
        res.push_back(key);
    }
    return res;
}

state_t substitude(state_t input){
    state_t res;
    for(int i = 0,size = input.size(); i < size; i++)
        res.push_back(s_box[input[i]]);
    return res;
}

state_t inv_substitude(state_t input){
    state_t res;
    for(int i = 0,size = input.size(); i < size; i++)
        res.push_back(inv_s_box[input[i]]);
    return res;
}

state_t row_shift(state_t input){
    state_t res;
    for(int i = 0,size = input.size(); i < size; i++){
        int row = i/4;
        res.push_back(input[row*4 + (i+row)%4]);
    }
    return res;
}

state_t inv_shift(state_t input){
    state_t res;
    for(int i = 0,size = input.size(); i < size; i++){
        int row = i/4;
        //        std::cout << i << " " << row << " " << row*4 + (i+row)%4 << " " << input[row*i + (i+row)%4] << std::endl;
        //
        res.push_back(input[row*4 + (i-row)%4]);
    }
    return res;
}

state_t column_confuse(state_t input){
    state_t res;
    for(int i = 0,size = 16; i < size; i++){
        unsigned sum = 0;
        int row = i / 4;
        int column = i % 4;
        for(int j = 0; j < 4; j++){
            if(mix_box[row * 4 + j] == 2 && input[4*j + column] >= 128)
                sum ^= (2*input[4*j + column]) ^ 0x1b;
            else if(mix_box[row * 4 + j] == 3){
                unsigned tmp = (2 *input[4*j + column]) % 256;
                if(input[4*j + column] >= 128)
                    tmp ^= 0x1b;
                sum ^= tmp ^ input[4*j + column];
            }
            else
                sum ^= (mix_box[row * 4 + j] * input[4*j + column]) % 256;
            //            sum ^= (mix_box[row * 4 + j] * input[4*j + column]) % 0x11b;
        }
        res.push_back(sum % 256);
    }
    return res;
}

unsigned mut(unsigned a, unsigned b){
    unsigned sum = 0;
    std::vector<unsigned> parts;
    parts.push_back(b);
    int t = 1;
    int i = 0;
    while(t*2 < a){
        unsigned tmp = parts[i++];
        if(tmp >= 128)
            parts.push_back( ((tmp<<1) ^ 0x1b) % 256);
        else
            parts.push_back((tmp << 1) % 256);
        t = t*2;
    }
    i = 0;
    while(a > 0){
        if(a&1){
            sum ^= parts[i];
        }
        a >>= 1;
        i++;
    }
    return sum;
}

state_t inv_confuse(state_t input){
    state_t res;
    for(int i = 0,size = 16; i < size; i++){
        unsigned sum = 0;
        int row = i / 4;
        int column = i % 4;
        for(int j = 0; j < 4; j++){
            sum ^= mut(inv_mix_box[row * 4 + j],input[4*j + column]);
        }
        res.push_back(sum % 256);
    }
    return res;
}

state_t operator ^(state_t a, state_t b){
    state_t res;
    for(int i = 0,size = a.size(); i < size; i++)
        res.push_back((a[i] ^ b[i]) % 256);
    return res;
}



state_t encrypt_(state_t mes, std::vector<state_t> keys){
    mes = mes ^ keys[0];
    for(int i = 1; i <= 9; i++){
        mes = substitude(mes);
        mes = row_shift(mes);
        mes = column_confuse(mes);
        mes = mes ^ keys[i];
    }
    mes = substitude(mes);
    mes = row_shift(mes);
    mes = mes ^ keys[10];
    return mes;
}

state_t decrypt_(state_t mes, std::vector<state_t> keys){
    mes = mes ^ keys[10];
    for(int i = 9; i > 0; i--){
        mes = inv_shift(mes);
        mes = inv_substitude(mes);
        mes = mes ^ keys[i];
        mes = inv_confuse(mes);
    }
    mes = inv_shift(mes);
    mes = inv_substitude(mes);
    mes = mes ^ keys[0];
    return mes;
}



char int2char(int input){
    if(input < 10)
        return input + '0';
    else
        return input-10+'a';
}

std::string bytes2string_c(state_t input){ //用于密文转换
    std::string res;
    for(int i = 0, size = input.size(); i < size; i++){
        //        std::cout << input[i] << " ";
        res += int2char((input[i] >> 4) % 16);
        res += int2char(input[i] % 16);
        //        std::cout << res << std::endl;
    }
    return res;
}

unsigned char2int(char input){
    unsigned res = 0;
    unsigned tmp = 1;
    for(int i = 0;i < 8; i++){
        if(input & 1 == 1)
            res += tmp;
        tmp <<= 1;
        input >>= 1;
    }
    return res;
}

std::vector<state_t> string2bytes(std::string mes){ //用于明文
    std::vector<state_t> res;
    for(int i = 0; i < 15; i++)
        mes += char(0);
    //    mes += "               ";//补长
    for(int i = 0, size = mes.size(); i < size - 16; i = i + 16){
        state_t tmp(16,0);
        for(int j = 0; j < 16; j++){
            tmp[j] = char2int(mes[i+j]);
        }
        res.push_back(tmp);
    }
    return res;
}

unsigned hex2bytes(char top, char low){
    int a = (top <= '9' && top >= '0')? top - '0' : top - 'a' + 10;
    int b = (low <= '9' && low >= '0')? low - '0' : low - 'a' + 10;
    return (a << 4) + b;
}

std::vector<state_t> string2bytes_c(std::string mes){ //密文转换
    std::vector<state_t> res;
    for(int i = 0, size = mes.size(); i < size; i = i + 32){
        state_t tmp(16,0);
        for(int j = 0; j < 32; j = j + 2){
            tmp[j/2] = hex2bytes(mes[i+j], mes[i+j+1]);
        }
        res.push_back(tmp);
    }
    return res;
}

std::string bytes2string(state_t input){
    std::string res;
    for(int i = 0, size = input.size(); i < size; i++)
        res += input[i];
    return res;
}

AES::AES(std::string mes, std::string k){//key 128bits
    key = string2bytes(k)[0];
    message = mes;
}

std::string AES::encrypt(){
    std::vector<state_t> mes = string2bytes(message);
    std::string cripher;
    mes_t keys = expand_key(key);
    for(int i = 0, size = mes.size(); i < size; i++){
        cripher += bytes2string_c(encrypt_(mes[i], keys));
        //        std::cout << cripher << std::endl;
    }
    return cripher;
}



std::string AES::decrypt(){
    std::vector<state_t> mes = string2bytes_c(message);
    std::string res;
    mes_t keys = expand_key(key);
    for(int i = 0, size = mes.size(); i < size; i++){
        res += bytes2string(decrypt_(mes[i], keys));
    }
    return res;
}
#endif /* AES_h */
