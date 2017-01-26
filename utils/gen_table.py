#! /usr/bin/env python3
#
# @file gen_table.py
# @brief ビットベクタからリテラル数を計算する表を作るスクリプト
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

# @brief 引数をビットベクタと見なして1のビット数を数える．
# @param[in] pat ビットベクタ
#
# pat は8ビットと仮定する．
def count(pat) :
    ans = 0
    for i in range(4) :
        pat1 = (pat >> (i * 2)) & 3
        if pat1 == 1 or pat1 == 2 :
            ans += 1
        elif pat1 == 3 :
            return 0
    return ans

# @brief ビットベクタ中の1のビット数を返す表を作る．
def gen_table() :
    for y in range(16) :
        line = "  "
        comma = ""
        for x in range(16) :
            pat = y * 16 + x
            num = count(pat)
            line += '{} {:2}'.format(comma, num)
            comma = ','
        line += ','
        print(line)

# メインルーティン
if __name__ == '__main__' :
    gen_table()
