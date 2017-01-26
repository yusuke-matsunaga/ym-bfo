
/// @file BfoCover.cc
/// @brief BfoCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BfoCover.h"
#include "ym/BfoCube.h"
#include "ym/BfoLiteral.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr キューブマネージャ
// @param[in] cube_list キューブを表すリテラルのリストのリスト
BfoCover::BfoCover(BfoMgr& mgr,
		   const vector<vector<BfoLiteral> >& cube_list) :
  mMgr(mgr),
  mCubeNum(cube_list.size())
{
  mBody = mMgr.new_body(mCubeNum);
  for (ymuint i = 0; i < mCubeNum; ++ i) {
    mMgr.set_cube(mBody, i, cube_list[i]);
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
BfoCover::BfoCover(const BfoCover& src) :
  mMgr(src.mMgr),
  mCubeNum(src.mCubeNum)
{
  mBody = mMgr.new_body(mCubeNum);
  for (ymuint i = 0; i < mCubeNum; ++ i) {
    mMgr.copy(mBody, i, src.mBody, i);
  }
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身の参照を返す．
const BfoCover&
BfoCover::operator=(const BfoCover& src)
{
  if ( &src != this ) {
    ASSERT_COND( &mMgr == &src.mMgr );
    mMgr.delete_body(mBody, mCubeNum);
    mCubeNum = src.mCubeNum;
    mBody = mMgr.new_body(mCubeNum);
    for (ymuint i = 0; i < mCubeNum; ++ i) {
      mMgr.copy(mBody, i, src.mBody, i);
    }
  }

  return *this;
}

// @brief キューブからの変換コンストラクタ
// @param[in] cube 対象のキューブ
//
// 指定されたキューブのみのカバーとなる．
BfoCover::BfoCover(const BfoCube& cube) :
  mMgr(cube.mgr()),
  mCubeNum(1)
{
  mBody = mMgr.new_body(1);
  mMgr.copy(mBody, 0, cube.mBody, 0);
}

// @brief デストラクタ
//
// ここに属しているすべてのキューブは削除される．
BfoCover::~BfoCover()
{
  mMgr.delete_body(mBody, mCubeNum);
}

// @brief 指定されたリテラルの出現回数を返す．
// @param[in] lit 対象のリテラル
ymuint
BfoCover::literal_num(BfoLiteral lit) const
{
  ymuint var_id = lit.varid();
  ASSERT_COND( var_id < variable_num() );

  BfoPol pat = lit.is_positive() ? kBfoPolP : kBfoPolN;
  ymuint ans = 0;
  ymuint n = cube_num();
  for (ymuint cube_pos = 0; cube_pos < n; ++ cube_pos) {
    if ( literal(cube_pos, var_id) == pat ) {
      ++ ans;
    }
  }

  return ans;
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
void
BfoCover::print(ostream& s) const
{
  mgr().print(s, cube_num(), mBody);
}

END_NAMESPACE_YM_BFO
