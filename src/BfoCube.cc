
/// @file BfoCube.cc
/// @brief BfoCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BfoCube.h"
#include "ym/BfoLiteral.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoCube
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ネージャ
// @param[in] lit_list キューブを表すリテラルのリスト
BfoCube::BfoCube(BfoMgr& mgr,
		 const vector<BfoLiteral>& lit_list) :
  mMgr(mgr)
{
  mBody = mgr.new_body();

  // まず空に初期化する．
  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] = 0ULL;
  }

  // lit_list に従って内容を設定する．
  for (ymuint i = 0; i < lit_list.size(); ++ i) {
    BfoLiteral lit = lit_list[i];
    ymuint var = lit.varid();
    ymuint blk = BfoMgr::block_pos(var);
    ymuint sft = BfoMgr::shift_num(var);
    ymuint64 pat = 0ULL;
    if ( lit.is_positive() ) {
      pat = kBfoPolP;
    }
    else {
      pat = kBfoPolN;
    }
    mBody[blk] |= (pat << sft);
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
BfoCube::BfoCube(const BfoCube& src) :
  mMgr(src.mMgr)
{
  mBody = mMgr.new_body();

  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] = src.mBody[i];
  }
}

// @brief 内容を指定するコンストラクタ
// @param[in] mgr マネージャ
// @param[in] body キューブのパタンを表す本体
//
// 危険なので普通は使わないように
BfoCube::BfoCube(BfoMgr& mgr,
		 ymuint64* body) :
  mMgr(mgr),
  mBody(body)
{
}

// @brief デストラクタ
BfoCube::~BfoCube()
{
  mMgr.delete_body(mBody);
}

/// @brief リテラル数を返す．
ymuint
BfoCube::literal_num() const
{
#warning "TODO: 効率のよい実装に変える．"
  ymuint ans = 0;
  ymuint n = variable_num();
  for (ymuint i = 0; i < n; ++ i) {
    if ( literal(i) != kBfoPolX ) {
      ++ ans;
    }
  }
  return ans;
}

// @brief オペランドのキューブに含まれていたら true を返す．
// @param[in] src オペランドのキューブ
//
// ここではキューブの表す論理関数の含意を考える<br>
// だからリテラル集合としては真逆になる．
bool
BfoCube::check_containment(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    if ( (~mBody[i] & right.mBody[i]) != 0ULL ) {
      return false;
    }
  }
  return true;
}

// @brief 2つのキューブに共通なリテラルがあれば true を返す．
// @param[in] right オペランドのキューブ
bool
BfoCube::operator&&(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    if ( mBody[i] & right.mBody[i] ) {
      return true;
    }
  }
  return false;
}

// @brief 論理積を計算し自身に代入する．
// @param[in] right オペランドのキューブ
// @return 演算後の自身の参照を返す．
//
// 相反するリテラルとの積があったら答は空のキューブとなる．
const BfoCube&
BfoCube::operator&=(const BfoCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] |= right.mBody[i];
    // x・x' のチェックを行う．
    ymuint64 tmp = mBody[i];
    ymuint64 mask1 = 0x5555555555555555ULL;
    ymuint64 mask2 = 0xAAAAAAAAAAAAAAAAULL;
    ymuint64 tmp1 = tmp & mask1;
    ymuint64 tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      // 空のキューブにする．
      for (ymuint j = 0; j < n; ++ j) {
	mBody[j] = 0ULL;
      }
      break;
    }
  }

  return *this;
}

#if 0
// @brief 共通部分を計算し自身に代入する．
// @param[in] src オペランドのキューブ
// @return 結果が NULL Cube になったら false を返す．
bool
BfoCube::make_intersect(const BfoCube* src)
{
  ASSERT_COND( variable_num() == src->variable_num() );

  ymuint64 tmp = 0ULL;
  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] &= src->mBody[i];
    tmp |= mBody[i];
  }

  return tmp != 0ULL;
}

// @brief オペランドに含まれているリテラルを取り除く
// @param[in] src オペランドのキューブ
void
BfoCube::make_diff(const BfoCube* src)
{
  ASSERT_COND( variable_num() == src->variable_num() );

  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] &= ~src->mBody[i];
  }
}
#endif

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
void
BfoCube::print(ostream& s) const
{
  mgr().print(s, 1, mBody);
}

// @relates BfoCube
// @brief BfoCubeの比較演算子
// @param[in] left, right オペランド
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
int
compare(const BfoCube& left,
	const BfoCube& right)
{
  ASSERT_COND( left.variable_num() == right.variable_num() );

  ymuint nb = left.mMgr.cube_size();
  for (ymuint i = 0; i < nb; ++ i) {
    ymuint64 p1 = left.mBody[i];
    ymuint64 p2 = right.mBody[i];
    if ( p1 < p2 ) {
      return -1;
    }
    if ( p1 > p2 ) {
      return 1;
    }
  }
  // ここまで来たら等しい
  return 0;
}

END_NAMESPACE_YM_BFO
