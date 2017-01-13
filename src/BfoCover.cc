
/// @file BfoCover.cc
/// @brief BfoCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "BfoCover.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cube_mgr キューブマネージャ
//
// キューブを一つも持たない空のカバーとなる．
BfoCover::BfoCover(BfoCubeMgr& cube_mgr) :
  mMgr(cube_mgr)
{
}

// @brief デストラクタ
//
// ここに属しているすべてのキューブは削除される．
BfoCover::~BfoCover()
{
  clear();
}

// @brief 変数の数を返す．
ymuint
BfoCover::variable_num() const
{
  return mMgr.variable_num();
}

// @brief キューブの数を返す．
ymuint
BfoCover::cube_num() const
{
  return mCubeList.size();
}

// @brief キューブを得る．
// @param[in] pos 位置番号 ( 0 <= pos < cube_num() )
const BfoCube*
BfoCover::cube(ymuint pos) const
{
  ASSERT_COND( pos < cube_num() );
  return mCubeList[pos];
}

// @brief 内容をクリアする．
//
// ここに属しているすべてのキューブは削除される．
void
BfoCover::clear()
{
  ymuint n = cube_num();
  for (ymuint i = 0; i < n; ++ i) {
    BfoCube* cube = mCubeList[i];
    mMgr.delete_cube(cube);
  }
  mCubeList.clear();
}

// @brief 新しいキューブを追加する．
// @param[in] cube 追加するキューブ
void
BfoCover::add_cube(const BfoCube* cube)
{
  ASSERT_COND( cube->variable_num() == variable_num() );
  BfoCube* cube1 = mMgr.new_cube();
  cube1->copy(cube);
  mCubeList.push_back(cube1);
  sort(mCubeList.begin(), mCubeList.end());
}

// @brief 2つのキューブの積を計算して追加する．
// @param[in] cube1, cube2 積をとるキューブ
//
// 積が空キューブになった場合には追加しない．
void
BfoCover::add_product(const BfoCube* cube1,
		      const BfoCube* cube2)
{
  ASSERT_COND( cube1->variable_num() == variable_num() );
  ASSERT_COND( cube2->variable_num() == variable_num() );
  BfoCube* cube = mMgr.new_cube();
  cube->copy(cube1);
  cube->make_product(cube2);
  mCubeList.push_back(cube);
  sort(mCubeList.begin(), mCubeList.end());
}

END_NAMESPACE_YM_BFO
