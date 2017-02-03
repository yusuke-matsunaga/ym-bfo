#! /usr/bin/env python3
#
# @file gen_bitcount_tbl.py
# @brief ビットベクタからリテラル数を計算する表を作るスクリプト
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

from gen_16x16tbl import gen_16x16tbl

# @brief 引数をビットベクタと見なして1のビット数を数える．
# @param[in] pat ビットベクタ
#
# pat は8ビットと仮定する．
def count(pat) :
    num = 0
    for i in range(4) :
        pat1 = (pat >> (i * 2)) & 3
        if pat1 == 1 or pat1 == 2 :
            num += 1
        elif pat1 == 3 :
            num = 0
            break
    return '{:2}'.format(num)

# メインルーティン
if __name__ == '__main__' :
    gen_16x16tbl(count)
