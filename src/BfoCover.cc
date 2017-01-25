
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
  ymuint nb = mMgr.cube_size();
  ymuint offset = 0;
  for (ymuint i = 0; i < mCubeNum; ++ i) {
    for (ymuint j = 0; j < cube_list[i].size(); ++ j) {
      BfoLiteral lit = cube_list[i][j];
      ymuint var_id = lit.varid();
      BfoPol pat = lit.is_positive() ? kBfoPolP : kBfoPolN;
      ymuint blk = BfoMgr::block_pos(var_id) + offset;
      ymuint sft = BfoMgr::shift_num(var_id);
      mBody[blk] |= (pat << sft);
    }
    offset += nb;
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
BfoCover::BfoCover(const BfoCover& src) :
  mMgr(src.mMgr),
  mCubeNum(src.mCubeNum)
{
  mBody = mMgr.new_body(mCubeNum);
  ymuint n = mMgr.cube_size() * mCubeNum;
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] = src.mBody[i];
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
    ymuint n = mMgr.cube_size() * mCubeNum;
    for (ymuint i = 0; i < n; ++ i) {
      mBody[i] = src.mBody[i];
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
  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] = cube.mBody[i];
  }
}

// @brief デストラクタ
//
// ここに属しているすべてのキューブは削除される．
BfoCover::~BfoCover()
{
  mMgr.delete_body(mBody, mCubeNum);
}

// @brief リテラル数を返す．
ymuint
BfoCover::literal_num() const
{
#warning "TODO: もっとスマートなやり方にする．"
  ymuint nc = cube_num();
  ymuint nv = variable_num();
  ymuint ans = 0;
  for (ymuint i = 0; i < nc; ++ i) {
    for (ymuint j = 0; j < nv; ++ j) {
      if ( literal(i, j) != kBfoPolX ) {
	++ ans;
      }
    }
  }

  return ans;
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

#if 0
// @brief 論理和を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
const BfoCover&
BfoCover::operator+=(const BfoCover& right)
{
}

// @brief 差分を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
const BfoCover&
BfoCover::operator-=(const BfoCover& right)
{
}

// @brief 論理積を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
const BfoCover&
BfoCover::operator*=(const BfoCover& right)
{
}

// @brief algebraic division を行って代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
const BfoCover&
BfoCover::operator/=(const BfoCover& right)
{
}

// @brief キューブのコファクターを計算して代入する．
// @param[in] cube 対象のキューブ
// @return 演算後の自身への参照を返す．
BfoCover
BfoCover::operator/=(const BfoCube& cube)
{
}

// @brief リテラルのコファクターを計算して代入する．
// @param[in] lit 対象のリテラル
// @return 演算後の自身への参照を返す．
const BfoCover&
BfoCover::operator/=(BfoLiteral lit)
{
}
#endif

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
void
BfoCover::print(ostream& s) const
{
  mgr().print(s, cube_num(), mBody);
}

END_NAMESPACE_YM_BFO
