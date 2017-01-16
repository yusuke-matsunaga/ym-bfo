
/// @file BfoCover.cc
/// @brief BfoCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BfoCover.h"
#include "ym/BfoCube.h"
#include "ym/BfoMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cube_mgr キューブマネージャ
//
// キューブを一つも持たない空のカバーとなる．
BfoCover::BfoCover(BfoMgr& cube_mgr) :
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

// @brief リテラル数を返す．
ymuint
BfoCover::literal_num() const
{
  ymuint ans = 0;
  ymuint n = cube_num();
  for (ymuint i = 0; i < n; ++ i) {
    ans += cube(i)->literal_num();
  }
  return ans;
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

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
// @param[in] varname_list 変数名のリスト
//
// varname_list が省略された時には適当な名前を作る．<br>
// varname_list のサイズは variable_num() 以上でなければならない．
void
BfoCover::print(ostream& s,
		const vector<string>& varname_list) const
{
  ymuint n = cube_num();
  const char* plus = "";
  for (ymuint i = 0; i < n; ++ i) {
    s << plus;
    plus = " + ";
    cube(i)->print(s, varname_list);
  }
}

END_NAMESPACE_YM_BFO
