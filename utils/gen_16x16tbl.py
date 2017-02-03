#! /usr/bin/env python3
#
# @file gen_16x16tbl.py
# @brief 16x16 サイズのテーブルを生成するスクリプト
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

# @brief テーブルを生成する．
# @param[in] elem_func 個々の要素に対する値を出力する関数
def gen_16x16tbl(elem_func) :
    for y in range(0, 16) :
        line = "  "
        comma = ""
        for x in range(0, 16) :
            c = y * 16 + x
            val_str = elem_func(c)
            line += '{} {}'.format(comma, val_str)
            comma = ','
        line += ','
        print(line)

# テスト用の関数
# 7 のモジュロを返す．
def test_func(c) :
    return '{:3}'.format(c % 7)

# メインルーティン
# かんたんなテストを行う．
if __name__ == '__main__' :
    gen_16x16tbl(test_func)
