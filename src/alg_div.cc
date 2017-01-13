
/// @file alg_div.cc
/// @brief alg_div の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "alg_div.h"


BEGIN_NAMESPACE_YM_BFO

void
alg_div(const BfoCover& f,
	const BfoCover& p,
	BfoCover& q,
	BfoCover& r)
{
  ymuint nf = f.cube_num();
  ymuint np = p.cube_num();
  if ( np == 0 || nf < np ) {
    // 絶対に割れない．
    q.clear();
    r = f;
    return;
  }

  vector<vector<BfoCube*> > u_array(np);
  vector<vector<BfoCube*> > v_array(np);
  for (ymuint i = 0; i < np; ++ i) {
    const BfoCube* p_cube = p.cube(i);
    for (ymuint j = 0; j < nf; ++ j) {
      const BfoCube* f_cube = f.cube(j);
      if ( f_cube->check_contain(p_cube) ) {
	BfoCube* u_cube = mgr.new_cube();
	u_cube->copy(f_cube);
	u_cube->make_diff(p_cube);
	u_array[i].push_back(u_cube);
      }
    }
    sort(u_array[i].begin(), u_array[i].end());
  }
  if ( np == 1 ) {
    // q = u_array[0];
  }
  else {
    // u_array[0] 〜 u_array[np - 1] の共通部分を計算する．
    for (ymuint i = 1; i < np; ++ i) {
      ymuint wpos = 0;
      ymuint rpos0 = 0;
      ymuint rpos1 = 0;
      ymuint n0 = u_array[0].size();
      ymuint n1 = u_array[i].size();
      while ( rpos0 < n0 && rpos1 < n1 ) {
	const BfoCube* cube0 = u_array[0][rpos0];
	const BfoCube* cube1 = u_array[i][rpos1];
	int cmp_res = compare(cube0, cube1);
	if ( cmp_res < 0 ) {
	  ++ rpos0;
	}
	else if ( cmp_res > 0 ) {
	  ++ rpos1;
	}
	else {
	  ++ rpos0;
	  ++ rpos1;
	  u_array[0][wpos] = cube0;
	}
      }
      if ( wpos < n0 ) {
	u_array[0].erase(u_array[0].begin() + wpos, u_array[0].end());
      }
    }
  }
}

END_NAMESPACE_YM_BFO
