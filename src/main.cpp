//
//  main.cpp
//  AES
//
//  Created by 张家豪 on 2018/12/11.
//  Copyright © 2018 张家豪. All rights reserved.
//

#include "AES.h"

int main(int argc, const char * argv[]) {
    state_t tmp = {0x87, 0xf2, 0x4d, 0x97,
                   0x6e, 0x4c, 0x90, 0xec,
                   0x46, 0xe7, 0x4a, 0xc3,
                   0xa6, 0x8c, 0xd8, 0x95
    };
    
//    state_t key = {0x2b,0x28,0xab,0x09,0x7e,0xae,0xf7,0xcf,0x15,0xd2,0x15,0x4f,0x16,0xa6,0x88,0x3c};
//    mes_t keys = expand_key(key);
    
    AES mes("叶广智叶广智叶广智叶广智", "叶广智");
    std::cout << mes.encrypt() << std::endl;
    AES cri("63c6da49393dd6c0fe296c2c4bd9c177660e18fb469126ab9693e7bc0f064ae1a1f337e6a0f7f70927823778e88d2f6f", "叶广智");
    std::cout << cri.decrypt() << std::endl;
    
    
//    std::string mes = "叶广智";
//    std::cout << mes << std::endl;
//    state_t mes_ = string2bytes(mes)[0];
//    std::cout << mes_ << std::endl;
//    state_t cri_ = encrypt_(mes_, keys);
//    std::cout << cri_ << std::endl;
//    std::string cri = bytes2string_c(cri_);
//    std::cout << cri << std::endl;
//    std::vector<state_t> new_cri = string2bytes_c(cri);
//    std::cout << new_cri << std::endl;
//    state_t new_mes = decrypt_(new_cri[0], keys);
//    std::cout << new_mes << std::endl;
//    std::cout << bytes2string(new_mes);
    
    
    
    
    
    
//    state_t t = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
//    std::cout << t << std::endl;
//
//    state_t c = encrypt(t,keys);
//    state_t m = decrypt(c, keys);
//    std::cout << c << std::endl;
//    std::cout << m << std::endl;

    return 0;
}
