#! /usr/bin/env python3
#
# @file gen_validchar_table.py
# @brief 変数に使える文字種を判定するテーブルを作るスクリプト
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

from gen_16x16tbl import gen_16x16tbl

# @brief 判別関数
# @param[in] c 入力する文字
# @return c が該当する文字の場合 True を返す．
def validchar(c) :
    true_str  = 'true '
    false_str = 'false'
    if c >= ord('a') and c <= ord('z') :
        return true_str
    if c >= ord('A') and c <= ord('Z') :
        return true_str
    if c >= ord('0') and c <= ord('9') :
        return true_str
    if c == ord('_') :
        return true_str
    return false_str

# メインルーティン
if __name__ == '__main__' :
    gen_16x16tbl(validchar)
