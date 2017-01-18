
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

BEGIN_NONAMESPACE

// @brief キューブの比較を行う．
// @param[in] pat1, pat2 比較対象のキューブのパタン
// @param[in] nb パタンのワード長
int
compare(ymuint64* pat1,
	ymuint64* pat2,
	ymuint nb)
{
  for (ymuint i = 0; i < nb; ++ i) {
    int res = pat1[i] - pat2[i];
    if ( res < 0 ) {
      return -1;
    }
    else if ( res > 0 ) {
      return 1;
    }
  }
  return 0;
}

// @brief キューブのパタンをコピーする．
// @param[out] dst 結果を格納するパタン
// @param[in] src コピー元のパタン
// @param[in] nb パタンのワード朝
void
copy(ymuint64* dst,
     ymuint64* src,
     ymuint nb)
{
  for (ymuint i = 0; i < nb; ++ i) {
    dst[i] = src[i];
  }
}

// @brief 2つのキューブの積が空でない時 true を返す．
// @param[in] pat1, pat2 比較対象のキューブのパタン
// @param[in] nb パタンのワード長
bool
check_product(ymuint64* pat1,
	      ymuint64* pat2,
	      ymuint nb)
{
  for (ymuint i = 0; i < nb; ++ i) {
    ymuint64 tmp = pat1[i] | pat2[i];
    ymuint64 mask1 = 0x5555555555555555ULL;
    ymuint64 mask2 = 0xAAAAAAAAAAAAAAAAULL;
    ymuint64 tmp1 = tmp & mask1;
    ymuint64 tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return false;
    }
  }
  return true;
}

// @brief 2つのキューブの積を計算する．
// @param[out] dst 結果を格納するパタン
// @param[in] pat1, pat2 比較対象のキューブのパタン
// @param[in] nb パタンのワード長
// @return 結果が空でなければ true を返す．
bool
make_product(ymuint64* dst,
	     ymuint64* pat1,
	     ymuint64* pat2,
	     ymuint nb)
{
  for (ymuint i = 0; i < nb; ++ i) {
    ymuint64 tmp = pat1[i] | pat2[i];
    ymuint64 mask1 = 0x5555555555555555ULL;
    ymuint64 mask2 = 0xAAAAAAAAAAAAAAAAULL;
    ymuint64 tmp1 = tmp & mask1;
    ymuint64 tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return false;
    }
    dst[i] = tmp;
  }
  return true;
}

// @brief 一方のキューブが他方のキューブに含まれているか調べる．
// @param[in] pat1, pat2 オペランドのキューブを表すパタンベクタ
// @param[in] nb パタンのワード長
// @return pat1 が pat2 に含まれていたら true を返す．
bool
check_containment(ymuint64* pat1,
		  ymuint64* pat2,
		  ymuint nb)
{
  for (ymuint i = 0; i < nb; ++ i) {
    if ( (~pat1[i] & pat2[i]) != 0ULL ) {
      return false;
    }
  }
  return true;
}

// @brief キューブによるコファクターを求める．
// @param[out] dst 結果を格納するパタン
// @param[in] pat1, pat2 オペランドのキューブを表すパタンベクタ
// @param[in] nb パタンのワード長
// @return 正しく割ることができたら true を返す．
bool
make_cofactor(ymuint64* dst,
	      ymuint64* pat1,
	      ymuint64* pat2,
	      ymuint nb)
{
  for (ymuint i = 0; i < nb; ++ i) {
    if ( (~pat1[i] & pat2[i]) != 0ULL ) {
      // この場合の dst の値は不定
      return false;
    }
    dst[i] = pat1[i] & ~pat2[i];
  }
  return true;
}

END_NONAMESPACE

// @brief 論理和を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
BfoCover
BfoCover::operator+(const BfoCover& right) const
{
  // 最初は結果のキューブ数を数えるだけ．
  ymuint nans = 0;
  ymuint nb = mgr().cube_size();
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(mBody + rpos1 * nb, right.mBody + rpos2 * nb, nb);
    if ( res < 0 ) {
      ++ rpos1;
    }
    else if ( res > 0 ) {
      ++ rpos2;
    }
    else {
      ++ rpos1;
      ++ rpos2;
    }
    ++ nans;
  }
  nans += (nc1 - rpos1);
  nans += (nc2 - rpos2);

  // 2回めは内容を設定してゆく．
  ymuint64* cbody = mgr().new_body(nans);
  rpos1 = 0;
  rpos2 = 0;
  ymuint wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(mBody + rpos1 * nb, right.mBody + rpos2 * nb, nb);
    if ( res < 0 ) {
      copy(cbody + wpos * nb, mBody + rpos1 * nb, nb);
      ++ rpos1;
    }
    else if ( res > 0 ) {
      copy(cbody + wpos * nb, right.mBody + rpos2 * nb, nb);
      ++ rpos2;
    }
    else {
      copy(cbody + wpos * nb, mBody + rpos1 * nb, nb);
      ++ rpos1;
      ++ rpos2;
    }
    ++ wpos;
  }
  while ( rpos1 < nc1 ) {
    copy(cbody + wpos * nb, mBody + rpos1 * nb, nb);
    ++ rpos1;
    ++ wpos;
  }
  while ( rpos2 < nc2 ) {
    copy(cbody + wpos * nb, right.mBody + rpos2 * nb, nb);
    ++ rpos2;
    ++ wpos;
  }

  return BfoCover(mgr(), nans, cbody);
}

// @brief 差分を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
BfoCover
BfoCover::operator-(const BfoCover& right) const
{
  // 最初は結果のキューブ数を数えるだけ．
  ymuint nans = 0;
  ymuint nb = mgr().cube_size();
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(mBody + rpos1 * nb, right.mBody + rpos2 * nb, nb);
    if ( res < 0 ) {
      ++ rpos1;
      ++ nans;
    }
    else if ( res > 0 ) {
      ++ rpos2;
    }
    else {
      ++ rpos1;
      ++ rpos2;
    }
  }
  nans += (nc1 - rpos1);

  // 2回めは内容を設定してゆく．
  ymuint64* cbody = mgr().new_body(nans);
  rpos1 = 0;
  rpos2 = 0;
  ymuint wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(mBody + rpos1 * nb, right.mBody + rpos2 * nb, nb);
    if ( res < 0 ) {
      copy(cbody + wpos * nb, mBody + rpos1 * nb, nb);
      ++ rpos1;
    }
    else if ( res > 0 ) {
      ++ rpos2;
    }
    else {
      ++ rpos1;
      ++ rpos2;
    }
    ++ wpos;
  }
  while ( rpos1 < nc1 ) {
    copy(cbody + wpos * nb, mBody + rpos1 * nb, nb);
    ++ rpos1;
    ++ wpos;
  }

  return BfoCover(mgr(), nans, cbody);
}

// @brief 論理積を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
BfoCover
BfoCover::operator*(const BfoCover& right) const
{
  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  ymuint nans = 0;
  ymuint nb = mgr().cube_size();
  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  for (ymuint rpos1 = 0; rpos1 < nc1; ++ rpos1) {
    for (ymuint rpos2 = 0; rpos2 < nc2; ++ rpos2) {
      if ( check_product(mBody + rpos1 * nb, right.mBody + rpos2 * nb, nb) ) {
	++ nans;
      }
    }
  }

  ymuint64* cbody = mMgr.new_body(nans);
  ymuint wpos = 0;
  for (ymuint rpos1 = 0; rpos1 < nc1; ++ rpos1) {
    for (ymuint rpos2 = 0; rpos2 < nc2; ++ rpos2) {
      if ( make_product(cbody + wpos * nb, mBody + rpos1 * nb, right.mBody + rpos2 * nb, nb) ) {
	++ wpos;
      }
      if ( wpos == nans ) {
	break;
      }
    }
    if ( wpos == nans ) {
      break;
    }
  }
  ASSERT_COND( wpos == nans );

  return BfoCover(mgr(), nans, cbody);
}

// @brief algebraic division を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
BfoCover
BfoCover::operator/(const BfoCover& right) const
{
}

// @brief キューブのコファクターを計算する．
// @param[in] cube 対象のキューブ
// @return 計算結果を返す．
BfoCover
BfoCover::operator/(const BfoCube& cube) const
{
  // 結果のキューブ数を数える．
  ymuint nans = 0;
  ymuint nb = mgr().cube_size();
  ymuint nc = cube_num();
  for (ymuint i = 0; i < nc; ++ i) {
    if ( check_containment(mBody + i * nb, cube.mBody, nb) ) {
      ++ nans;
    }
  }

  ymuint64* cbody = mgr().new_body(nans);
  ymuint wpos = 0;
  for (ymuint i = 0; i < nc; ++ i) {
    if ( make_cofactor(cbody + wpos * nb, mBody + i * nb, cube.mBody, nb) ) {
      ++ wpos;
    }
    if ( wpos == nans ) {
      break;
    }
  }
  ASSERT_COND( wpos == nans );

  return BfoCover(mgr(), nans, cbody);
}

// @brief リテラルのコファクターを計算する．
// @param[in] lit 対象のリテラル
// @return 計算結果を返す．
BfoCover
BfoCover::operator/(BfoLiteral lit) const
{
  // 結果のキューブ数を数える．
  ymuint nans = 0;
  ymuint var_id = lit.varid();
  BfoPol pat = lit.is_positive() ? kBfoPolP : kBfoPolN;
  ymuint nc = cube_num();
  for (ymuint i = 0; i < nc; ++ i) {
    if ( literal(i, var_id) == pat ) {
      ++ nans;
    }
  }

  ymuint64* cbody = mgr().new_body(nans);
  ymuint nb = mgr().cube_size();
  ymuint blk = BfoMgr::block_pos(var_id);
  ymuint sft = BfoMgr::shift_num(var_id);
  ymuint64 mask = ~(3UL << sft);
  ymuint wpos = 0;
  for (ymuint i = 0; i < nc; ++ i) {
    if ( literal(i, var_id) == pat ) {
      copy(cbody + wpos * nb, mBody + i * nb, nb);
      cbody[blk] &= mask;
      ++ wpos;
      if ( wpos == nans ) {
	break;
      }
    }
  }
  ASSERT_COND( wpos == nans );

  return BfoCover(mgr(), nans, cbody);
}

// @brief 共通なキューブを返す．
//
// 共通なキューブがない場合には空のキューブを返す．
BfoCube
BfoCover::common_cube() const
{
  ymuint nb = mMgr.cube_size();

  // 結果を格納する領域
  ymuint64* cbody = mMgr.new_body();

  // 最初のキューブをコピー
  for (ymuint i = 0; i < nb; ++ i) {
    cbody[i] = mBody[i];
  }

  // 2番目以降のキューブとの共通部分を求める．
  ymuint offset = nb;
  for (ymuint cube_pos = 1; cube_pos < mCubeNum; ++ cube_pos) {
    ymuint64 tmp = 0ULL;
    for (ymuint i = 0; i < nb; ++ i) {
      cbody[i] &= mBody[offset + i];
      tmp |= cbody[i];
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
    offset += nb;
  }

  return BfoCube(mMgr, cbody);
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
void
BfoCover::print(ostream& s) const
{
  ymuint nc = cube_num();
  ymuint nv = variable_num();
  const char* plus = "";
  for (ymuint i = 0; i < nc; ++ i) {
    s << plus;
    plus = " + ";
    const char* spc = "";
    for (ymuint j = 0; j < nv; ++ j) {
      BfoPol pol = literal(i, j);
      if ( pol == kBfoPolP ) {
	s << spc << mgr().varname(j);
	spc = " ";
      }
      else if ( pol == kBfoPolN ) {
	s << spc << mgr().varname(j) << "'";
	spc = " ";
      }
    }
  }
}

END_NAMESPACE_YM_BFO
