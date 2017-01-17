
/// @file factor.cc
/// @brief factor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "factor.h"

Expr
gen_factor(const BfoCover& f)
{
  BfoCover p = find_divisor(f);

  BfoCover q = f / p;
  BfoCover r = f - (p * q);

  Expr expr_p = gen_factor(p);
  Expr expr_q = gen_factor(q);
  Expr expr_r = gen_factor(r);

  return expr_p * expr_q + expr_r;
}

kernel(const BfoCover& f)
{
  // f 中に現れるリテラルの出現回数を調べて
  // 昇順に並べる．
  vector<BfoLiteral> lit_list;

  BfoCube ccube0(mgr);
  kern_sub(f, lit_list, 0, ccube0);
}

void
kern_sub(const BfoCocver& f,
	 const vector<BfoLiteral>& lit_list,
	 ymuint next_pos,
	 const BfuCube& ccube)
{
  for (ymuint i = 0; i < lit_list.size(); ++ i) {
    BfoLiteral lit = lit_list[i];
    if ( f.literal_num(lit) <= 1 ) {
      continue;
    }

    BfoCover f1 = f / lit;
    BfoCube ccube1 = f1.common_cube();
    if ( ccube && ccube1 ) {
      // これは以前に処理している．
      continue;
    }

    f1 /= ccube1;
    ccube1 &= ccube;

    // f1, ccube1 を kernel/co-kernel として記録

    kern_sub(f1, lit_list, i + 1, ccube1);
  }
}
