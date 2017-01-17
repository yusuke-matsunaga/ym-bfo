
/// @file BfoMgr.cc
/// @brief BfoMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BfoMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
//
// 変数名はデフォルトのものが使用される．
BfoMgr::BfoMgr(ymuint variable_num) :
  mVarNum(variable_num),
  mVarNameList(mVarNum)
{
}

// @brief コンストラクタ
// @param[in] varname_list 変数名のリスト
//
// varname_list のサイズが変数の数になる．
BfoMgr::BfoMgr(const vector<string>& varname_list) :
  mVarNum(varname_list.size()),
  mVarNameList(varname_list)
{
}

// @brief デストラクタ
BfoMgr::~BfoMgr()
{
}

// @brief キューブ/カバー用の領域を確保する．
// @param[in] cube_num キューブ数
//
// キューブの時は cube_num = 1 とする．
ymuint64*
BfoMgr::new_body(ymuint cube_num)
{
  return new ymuint64[cube_size() * cube_num];
}

// @brief キューブ/カバー用の領域を削除する．
// @param[in] p 領域を指すポインタ
// @param[in] cube_num キューブ数
//
// キューブの時は cube_num = 1 とする．
void
BfoMgr::delete_body(ymuint64* p,
		    ymuint cube_num)
{
  delete [] p;
}

#if 0
// @brief カバーの和を計算する．
// @param[in] left, right オペランド
// @param[out] ans 結果を格納するオブジェクト
void
BfoMgr::make_sum(const BfoCover& left,
		 const BfoCover& right,
		 BfoCover& ans)
{
  ASSERT_COND( left.variable_num() == variable_num() );
  ASSERT_COND( right.variable_num() == variable_num() );
  ASSERT_COND( ans.variable_num() == variable_num() );

  ymuint n1 = left.cube_num();
  ymuint n2 = right.cube_num();
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  ans.clear();
  while ( rpos1 < n1 && rpos2 < n2 ) {
    const BfoCube* cube1 = left.cube(rpos1);
    const BfoCube* cube2 = right.cube(rpos2);
    int comp_res = compare(cube1, cube2);
    if ( comp_res == -1 ) {
      ans.add_cube(cube1);
      ++ rpos1;
    }
    else if ( comp_res == 1 ) {
      ans.add_cube(cube2);
      ++ rpos2;
    }
    else {
      ans.add_cube(cube1);
      ++ rpos1;
      ++ rpos2;
    }
  }
  for ( ; rpos1 < n1; ++ rpos1) {
    const BfoCube* cube = left.cube(rpos1);
    ans.add_cube(cube);
  }
  for ( ; rpos2 < n2; ++ rpos2) {
    const BfoCube* cube = right.cube(rpos2);
    ans.add_cube(cube);
  }
}

// @brief カバーの差を計算する．
// @param[in] left, right オペランド
// @param[out] ans 結果を格納するオブジェクト
void
BfoMgr::make_diff(const BfoCover& left,
		  const BfoCover& right,
		  BfoCover& ans)
{
  ASSERT_COND( left.variable_num() == variable_num() );
  ASSERT_COND( right.variable_num() == variable_num() );
  ASSERT_COND( ans.variable_num() == variable_num() );

  ymuint n1 = left.cube_num();
  ymuint n2 = right.cube_num();
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  ans.clear();
  while ( rpos1 < n1 && rpos2 < n2 ) {
    const BfoCube* cube1 = left.cube(rpos1);
    const BfoCube* cube2 = right.cube(rpos2);
    int comp_res = compare(cube1, cube2);
    if ( comp_res == -1 ) {
      ans.add_cube(cube1);
      ++ rpos1;
    }
    else if ( comp_res == 1 ) {
      ++ rpos2;
    }
    else {
      ++ rpos1;
      ++ rpos2;
    }
  }
  for ( ; rpos1 < n1; ++ rpos1) {
    const BfoCube* cube = left.cube(rpos1);
    ans.add_cube(cube);
  }
}

// @brief カバーの積を計算する．
// @param[in] left, right オペランド
// @param[out] ans 結果を格納するオブジェクト
void
BfoMgr::make_product(const BfoCover& left,
		     const BfoCover& right,
		     BfoCover& ans)
{
  ASSERT_COND( left.variable_num() == variable_num() );
  ASSERT_COND( right.variable_num() == variable_num() );
  ASSERT_COND( ans.variable_num() == variable_num() );

  ymuint n1 = left.cube_num();
  ymuint n2 = right.cube_num();
  ans.clear();
  for (ymuint rpos1 = 0; rpos1 < n1; ++ rpos1) {
    const BfoCube* cube1 = left.cube(rpos1);
    for (ymuint rpos2 = 0; rpos2 < n2; ++ rpos2) {
      const BfoCube* cube2 = right.cube(rpos2);
      BfoCube* new_cube = ans.add_cube(cube1);
      new_cube->make_product(cube2);
    }
  }
}

// @brief カバーの商を計算する．
// @param[in] left, right オペランド
// @param[out] ans 結果を格納するオブジェクト
void
BfoMgr::make_division(const BfoCover& left,
		      const BfoCover& right,
		      BfoCover& ans)
{
  ASSERT_COND( left.variable_num() == variable_num() );
  ASSERT_COND( right.variable_num() == variable_num() );
  ASSERT_COND( ans.variable_num() == variable_num() );

  ymuint n1 = left.cube_num();
  ymuint n2 = right.cube_num();
  ans.clear();

  // 自明なケースの除外
  if ( n2 == 0 || n1 < n2 ) {
    // 商は空となる．
    return;
  }

  vector<vector<BfoCube*> > u_array(n2);
  for (ymuint i = 0; i < n2; ++ i) {
    const BfoCube* p_cube = right.cube(i);
    for (ymuint j = 0; j < n1; ++ j) {
      const BfoCube* f_cube = left.cube(j);
      if ( f_cube->check_contain(p_cube) ) {
	BfoCube* u_cube = new_cube();
	u_cube->copy(f_cube);
	u_cube->make_diff(p_cube);
	u_array[i].push_back(u_cube);
      }
    }
    sort(u_array[i].begin(), u_array[i].end());
  }

  // u_array[0] 〜 u_array[n2 - 1] の共通部分を計算する．
  vector<BfoCube*> q_list;
  {
    ymuint n = u_array[0].size();
    q_list.reserve(n);
    for (ymuint i = 0; i < n; ++ i) {
      BfoCube* cube = u_array[0][i];
      q_list.push_back(cube);
    }
  }
  for (ymuint i = 1; i < n2; ++ i) {
    ymuint wpos = 0;
    ymuint rpos0 = 0;
    ymuint rpos1 = 0;
    ymuint n0 = q_list.size();
    ymuint n1 = u_array[i].size();
    while ( rpos0 < n0 && rpos1 < n1 ) {
      BfoCube* cube0 = q_list[rpos0];
      BfoCube* cube1 = u_array[i][rpos1];
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
	q_list[wpos] = cube0;
      }
    }
    if ( wpos < n0 ) {
      q_list.erase(q_list.begin() + wpos, q_list.end());
    }
  }

  // q_list を ans にコピーする．
  ymuint n0 = q_list.size();
  for (ymuint i = 0; i < n0; ++ i) {
    BfoCube* cube = q_list[i];
    ans.add_cube(cube);
  }

  // ここで生成したテンポラリのキューブを削除する．
  for (ymuint i = 0; i < n2; ++ i) {
    ymuint n = u_array[i].size();
    for (ymuint j = 0; j < n; ++ j) {
      BfoCube* cube = u_array[i][j];
      delete_cube(cube);
    }
  }
}
#endif

END_NAMESPACE_YM_BFO
