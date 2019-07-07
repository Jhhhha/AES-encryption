//
//  AES.cpp
//  AES
//
//  Created by 张家豪 on 2018/12/11.
//  Copyright © 2018 张家豪. All rights reserved.
//

//#include "AES.h"
//state_t substitude(state_t input){
//    state_t res;
//    for(int i = 0,size = input.size(); i < size; i++)
//        res.push_back(s_box[input[i]]);
//    return res;
//}
//
//state_t inv_substitude(state_t input){
//    state_t res;
//    for(int i = 0,size = input.size(); i < size; i++)
//        res.push_back(inv_s_box[input[i]]);
//    return res;
//}
//
//state_t row_shift(state_t input){
//    state_t res;
//    for(int i = 0,size = input.size(); i < size; i++){
//        int row = i/4;
//        res.push_back(input[row*4 + (i+row)%4]);
//    }
//    return res;
//}
//
//state_t inv_shift(state_t input){
//    state_t res;
//    for(int i = 0,size = input.size(); i < size; i++){
//        int row = i/4;
//        //        std::cout << i << " " << row << " " << row*4 + (i+row)%4 << " " << input[row*i + (i+row)%4] << std::endl;
//        //
//        res.push_back(input[row*4 + (i-row)%4]);
//    }
//    return res;
//}
//
////static unsigned mix_box[16] = {0x02, 0x03, 0x01, 0x01,
////                               0x01, 0x02, 0x03, 0x01,
////                               0x01, 0x01, 0x02, 0x03,
////                               0x03, 0x01, 0x01, 0x02
////};
//
////0 1 2 3 - 0 4 8 12
////0 1 2 3 - 1 5 9 13
////0 1 2 3
////0 1 2 3
////4 5 6 7
//state_t column_confuse(state_t input){
//    state_t res;
//    for(int i = 0,size = 16; i < size; i++){
//        unsigned sum = 0;
//        int row = i / 4;
//        int column = i % 4;
//        for(int j = 0; j < 4; j++){
//            //            std::cout << row * 4 + j << " " << 4*j+column << std::endl;
//            sum += (mix_box[row * 4 + j] * input[4*j + column]) % 256;
//        }
//        res.push_back(sum % 256);
//    }
//    return res;
//}
//
//state_t inv_confuse(state_t input){
//    state_t res;
//    for(int i = 0,size = 16; i < size; i++){
//        unsigned sum = 0;
//        int row = i / 4;
//        int column = i % 4;
//        for(int j = 0; j < 4; j++){
//            //            std::cout << row * 4 + j << " " << 4*j+column << std::endl;
//            sum += (inv_mix_box[row * 4 + j] * input[4*j + column]);
//        }
//        res.push_back(sum % 256);
//    }
//    return res;
//}
//
//state_t operator ^(state_t a, state_t b){
//    state_t res;
//    for(int i = 0,size = a.size(); i < size; i++)
//        res.push_back((a[i] ^ b[i]) % 256);
//    return res;
//}
//
//state_t gen_key(state_t key, int round){
//    state_t res(16,0);
//    for(int i = 0; i < 4; i++){
//        res[i*4] = s_box[key[(i * 4+ 7)%16]] ^ key[i*4]; //第一列
//        if(i == 0)
//            res[0] = res[0] ^ RC[round];
//    }
//    for(int i = 1; i < 4; i++){
//        for(int j = 0; j < 4; j++){//j是行数
//            res[j*4 + i] = res[j*4 + i-1] ^ key[j*4 + i];
//        }
//    }
//    return res;
//}
//
//std::vector<state_t> expand_key(state_t key){
//    std::vector<state_t> res;
//    res.push_back(key);
//    for(int i = 0; i < 10; i++){
//        key = gen_key(key, i);
//        std::cout << i+1 << "次：" << std::endl;
//        std::cout << key << std::endl;
//        res.push_back(key);
//    }
//    return res;
//}
