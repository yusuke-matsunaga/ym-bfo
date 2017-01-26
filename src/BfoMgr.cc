
/// @file BfoMgr.cc
/// @brief BfoMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BfoMgr.h"
#include "ym/BfoCover.h"
#include "ym/BfoCube.h"


BEGIN_NAMESPACE_YM_BFO

BEGIN_NONAMESPACE

// 名前を作る．
string
_varname(ymuint id)
{
  string ans;
  if ( id >= 26 ) {
    ans = _varname(id / 26);
  }
  char c = static_cast<char>('a' + (id % 26));
  ans += c;
  return ans;
}

END_NONAMESPACE

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
  // 変数名を作る．
  // 変数番号を26進数で表して文字列にする．
  for (ymuint i = 0; i < mVarNum; ++ i) {
    mVarNameList[i] = _varname(i);
  }
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

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
ymuint
_count(ymuint64 pat)
{
  ymuint table[] = {
    // utils/gen_table.py で生成
    0,  1,  1,  0,  1,  2,  2,  0,  1,  2,  2,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  };

  return table[pat];
}

END_NONAMESPACE

// @brief ビットベクタ上のリテラル数を数える．
// @param[in] nc キューブ数
// @param[in] bv カバーを表すビットベクタ
ymuint
BfoMgr::literal_num(ymuint nc,
		    const ymuint64* bv)
{
  ymuint n = nc * cube_size();
  // 8 ビットごとに区切って表引きで計算する．
  ymuint ans = 0;
  for (ymuint i = 0; i < n; ++ i, ++ bv) {
    ymuint64 pat = *bv;
    ymuint64 p1 = pat & 0xFFUL;
    ans += _count(p1);
    ymuint64 p2 = (pat >> 8) & 0xFFUL;
    ans += _count(p2);
    ymuint64 p3 = (pat >> 16) & 0xFFUL;
    ans += _count(p3);
    ymuint64 p4 = (pat >> 24) & 0xFFUL;
    ans += _count(p4);
    ymuint64 p5 = (pat >> 32) & 0xFFUL;
    ans += _count(p5);
    ymuint64 p6 = (pat >> 40) & 0xFFUL;
    ans += _count(p6);
    ymuint64 p7 = (pat >> 48) & 0xFFUL;
    ans += _count(p7);
    ymuint64 p8 = (pat >> 56);
    ans += _count(p8);
  }
  return ans;
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

// @brief 2つのカバーの論理和を計算する．
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバーのキューブ数
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @return 結果のカバーを表すビットベクタを返す．
BfoCover
BfoMgr::make_sum(ymuint nc1,
		 const ymuint64* bv1,
		 ymuint nc2,
		 const ymuint64* bv2)
{
  // 最初は結果のキューブ数を数えるだけ．
  ymuint nans = 0;
  ymuint nb = cube_size();
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(bv1, rpos1, bv2, rpos2);
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
  ymuint64* cbody = new_body(nans);
  rpos1 = 0;
  rpos2 = 0;
  ymuint wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(bv1, rpos1, bv2, rpos2);
    if ( res < 0 ) {
      copy(cbody, wpos, bv1, rpos1);
      ++ rpos1;
    }
    else if ( res > 0 ) {
      copy(cbody, wpos, bv2, rpos2);
      ++ rpos2;
    }
    else {
      copy(cbody, wpos, bv1, rpos1);
      ++ rpos1;
      ++ rpos2;
    }
    ++ wpos;
  }
  while ( rpos1 < nc1 ) {
    copy(cbody, wpos, bv1, rpos1);
    ++ rpos1;
    ++ wpos;
  }
  while ( rpos2 < nc2 ) {
    copy(cbody, wpos, bv2, rpos2);
    ++ rpos2;
    ++ wpos;
  }

  return BfoCover(*this, nans, cbody);
}

// @brief 2つのカバーの差分を計算する．
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバーのキューブ数
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @return 結果のカバーを返す．
BfoCover
BfoMgr::make_diff(ymuint nc1,
		  const ymuint64* bv1,
		  ymuint nc2,
		  const ymuint64* bv2)
{
  // 最初は結果のキューブ数を数えるだけ．
  ymuint nans = 0;
  ymuint nb = cube_size();
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(bv1, rpos1, bv2, rpos2);
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
  ymuint64* cbody = new_body(nans);
  rpos1 = 0;
  rpos2 = 0;
  ymuint wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = compare(bv1, rpos1, bv2, rpos2);
    if ( res < 0 ) {
      copy(cbody, wpos, bv1, rpos1);
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
    copy(cbody, wpos, bv1, rpos1);
    ++ rpos1;
    ++ wpos;
  }
  ASSERT_COND( wpos == nans );

  return BfoCover(*this, nans, cbody);
}

// @brief 2つのカバーの論理積を計算する．
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバーのキューブ数
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @return 結果のカバーを返す．
BfoCover
BfoMgr::make_product(ymuint nc1,
		     const ymuint64* bv1,
		     ymuint nc2,
		     const ymuint64* bv2)
{
  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  ymuint nans = 0;
  ymuint nb = cube_size();
  for (ymuint rpos1 = 0; rpos1 < nc1; ++ rpos1) {
    for (ymuint rpos2 = 0; rpos2 < nc2; ++ rpos2) {
      if ( check_product(bv1, rpos1, bv2, rpos2) ) {
	++ nans;
      }
    }
  }

  ymuint64* cbody = new_body(nans);
  ymuint wpos = 0;
  for (ymuint rpos1 = 0; rpos1 < nc1; ++ rpos1) {
    for (ymuint rpos2 = 0; rpos2 < nc2; ++ rpos2) {
      if ( make_product(cbody, wpos, bv1, rpos1, bv2, rpos2) ) {
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

  return BfoCover(*this, nans, cbody);
}

// @brief カバーの代数的除算を行う．
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバー(除数)のキューブ数
// @param[in] bv2 2つめのカバー(除数)を表すビットベクタ
// @return 結果のカバーを返す．
BfoCover
BfoMgr::make_division(ymuint nc1,
		      const ymuint64* bv1,
		      ymuint nc2,
		      const ymuint64* bv2)
{
  // 作業領域のビットベクタを確保する．
  // 大きさは nc1
  ymuint64* tmp = new_body(nc1);
  ymuint nb = cube_size();
  // bv1 の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  for (ymuint i = 0; i < nc1; ++ i) {
    bool found = false;
    for (ymuint j = 0; j < nc2; ++ j) {
      if ( make_cofactor(tmp, i, bv1, i, bv2, j) ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      // このキューブは割れない．
      for (ymuint j = 0; j < nb; ++ j) {
	tmp[i * nb + j] = 0ULL;
      }
    }
  }

  // 商のキューブは tmp 中に nc2 回現れているはず．
  vector<ymuint> pos_list;
  pos_list.reserve(nc1);
  for (ymuint i = 0; i < nc1; ++ i) {
    bool valid = false;
    for (ymuint j = 0; j < nb; ++ j) {
      if ( tmp[i * nb + j] != 0ULL ) {
	valid = true;
	break;
      }
    }
    if ( !valid ) {
      continue;
    }
    ymuint c = 1;
    for (ymuint i2 = i + 1; i2 < nc1; ++ i2) {
      if ( compare(tmp, i, tmp, i2) == 0 ) {
	++ c;
      }
    }
    if ( c == nc2 ) {
      // 見つけた
      pos_list.push_back(i);
    }
  }

  ymuint nans = pos_list.size();
  ymuint64* cbody = nullptr;
  if ( nans > 0 ) {
    cbody = new_body(nans);
    for (ymuint i = 0; i < nans; ++ i) {
      ymuint pos = pos_list[i];
      copy(cbody, i, tmp, pos);
    }
  }
  // 作業領域のビットベクタを解放する．
  delete_body(tmp, nc1);

  return BfoCover(*this, nans, cbody);
}

// @brief カバーをキューブで割る．
// @param[in] nc1 カバーのキューブ数
// @param[in] bv1 カバーを表すビットベクタ
// @param[in] bv2 キューブを表すビットベクタ
// @return 結果のカバーを返す．
BfoCover
BfoMgr::make_division(ymuint nc1,
		      const ymuint64* bv1,
		      const ymuint64* bv2)
{
  // 結果のキューブ数を数える．
  ymuint nans = 0;
  ymuint nb = cube_size();
  for (ymuint i = 0; i < nc1; ++ i) {
    if ( check_containment(bv1, i, bv2, 0) ) {
      ++ nans;
    }
  }

  ymuint64* cbody = new_body(nans);
  ymuint wpos = 0;
  for (ymuint i = 0; i < nc1; ++ i) {
    if ( make_cofactor(cbody, wpos, bv1, i, bv2, 0) ) {
      ++ wpos;
    }
    if ( wpos == nans ) {
      break;
    }
  }
  ASSERT_COND( wpos == nans );

  return BfoCover(*this, nans, cbody);
}

// @brief カバーをリテラルで割る(コファクター演算)．
// @param[in] nc1 カバーのキューブ数
// @param[in] bv1 カバーを表すビットベクタ
// @param[in] lit 対象のリテラル
BfoCover
BfoMgr::make_division(ymuint nc1,
		      const ymuint64* bv1,
		      BfoLiteral lit)
{
  // 結果のキューブ数を数える．
  ymuint nans = 0;
  ymuint var_id = lit.varid();
  ymuint blk = block_pos(var_id);
  ymuint sft = shift_num(var_id);
  BfoPol pat = lit.is_positive() ? kBfoPolP : kBfoPolN;
  ymuint64 mask = 3UL << sft;
  ymuint64 pat1 = pat << sft;
  ymuint nb = cube_size();
  for (ymuint i = 0; i < nc1; ++ i) {
    if ( (bv1[i * nb + blk] & mask) == pat1 ) {
      ++ nans;
    }
  }

  ymuint64* cbody = new_body(nans);
  ymuint64 nmask = ~mask;
  ymuint wpos = 0;
  for (ymuint i = 0; i < nc1; ++ i) {
    if ( (bv1[i * nb + blk] & mask) == pat1 ) {
      copy(cbody, wpos, bv1, i);
      *(cbody + wpos * nb + blk) &= nmask;
      ++ wpos;
      if ( wpos == nans ) {
	break;
      }
    }
  }
  ASSERT_COND( wpos == nans );

  return BfoCover(*this, nans, cbody);
}

// @brief カバー中のすべてのキューブの共通部分を求める．
// @param[in] nc1 カバーのキューブ数
// @param[in] bv1 カバーを表すビットベクタ
// @return 結果のキューブを返す．
//
// 共通部分がないときは空のキューブを返す．
BfoCube
BfoMgr::common_cube(ymuint nc1,
		    const ymuint64* bv1)
{
  ymuint nb = cube_size();

  // 結果を格納する領域
  ymuint64* cbody = new_body();

  // 最初のキューブをコピーする．
  for (ymuint i = 0; i < nb; ++ i) {
    cbody[i] = bv1[i];
  }

  // 2番目以降のキューブとの共通部分を求める．
  ymuint offset = nb;
  for (ymuint cube_pos = 1; cube_pos < nc1; ++ cube_pos) {
    ymuint64 tmp = 0ULL;
    for (ymuint i = 0; i < nb; ++ i) {
      cbody[i] &= bv1[offset + i];
      tmp |= cbody[i];
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
    offset += nb;
  }

  return BfoCube(*this, cbody);
}

// @brief キューブ(を表すビットベクタ)の比較を行う．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
// @retval -1 bv1 <  bv2
// @retval  0 bv1 == bv2
// @retval  1 bv1 >  bv2
int
BfoMgr::compare(const ymuint64* bv1,
		ymuint pos1,
		const ymuint64* bv2,
		ymuint pos2)
{
  ymuint nb = cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ bv1, ++ _bv2) {
    int res = *_bv1 - *_bv2;
    if ( res < 0 ) {
      return -1;
    }
    else if ( res > 0 ) {
      return 1;
    }
  }
  return 0;
}

// @brief 2つのキューブの積が空でない時 true を返す．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
bool
BfoMgr::check_product(const ymuint64* bv1,
		      ymuint pos1,
		      const ymuint64* bv2,
		      ymuint pos2)
{
  ymuint nb = cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ _bv1, ++ _bv2) {
    ymuint64 tmp = *_bv1 | *_bv2;
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

// @brief 一方のキューブが他方のキューブに含まれているか調べる．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
bool
BfoMgr::check_containment(const ymuint64* bv1,
			  ymuint pos1,
			  const ymuint64* bv2,
			  ymuint pos2)
{
  ymuint nb = cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ _bv1, ++ _bv2) {
    if ( (~(*_bv1) & *_bv2) != 0ULL ) {
      return false;
    }
  }
  return true;
}

// @brief リテラルの集合からキューブを表すビットベクタにセットする．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ番号
// @param[in] lit_list リテラルのリスト
void
BfoMgr::set_cube(ymuint64* dst_bv,
		 ymuint dst_pos,
		 const vector<BfoLiteral>& lit_list)
{
  // まず空に初期化する．
  ymuint nb = cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  for (ymuint i = 0; i < nb; ++ i) {
    dst[i] = 0ULL;
  }

  // lit_list に従って内容を設定する．
  for (ymuint i = 0; i < lit_list.size(); ++ i) {
    BfoLiteral lit = lit_list[i];
    ymuint var = lit.varid();
    ymuint blk = block_pos(var);
    ymuint sft = shift_num(var);
    ymuint64 pat = lit.is_positive() ? kBfoPolP : kBfoPolN;
    dst[blk] |= (pat << sft);
  }
}

// @brief キューブ(を表すビットベクタ)のコピーを行う．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ番号
// @param[in] src_bv ソースのビットベクタ
// @param[in] src_pos ソースのキューブ番号
void
BfoMgr::copy(ymuint64* dst_bv,
	     ymuint dst_pos,
	     const ymuint64* src_bv,
	     ymuint src_pos)
{
  ymuint nb = cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  const ymuint64* src = src_bv + src_pos * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ dst, ++ src) {
    *dst = *src;
  }
}

// @brief 2つのキューブの積を計算する．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ番号
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
// @retval true 積が空でなかった．
// @retval false 積が空だった．
bool
BfoMgr::make_product(ymuint64* dst_bv,
		     ymuint dst_pos,
		     const ymuint64* bv1,
		     ymuint pos1,
		     const ymuint64* bv2,
		     ymuint pos2)
{
  ymuint nb = cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ dst, ++ _bv1, ++ _bv2) {
    ymuint64 tmp = *_bv1 | *_bv2;
    ymuint64 mask1 = 0x5555555555555555ULL;
    ymuint64 mask2 = 0xAAAAAAAAAAAAAAAAULL;
    ymuint64 tmp1 = tmp & mask1;
    ymuint64 tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return false;
    }
    *dst = tmp;
  }
  return true;
}

// @brief キューブによるコファクターを求める．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ番号
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
// @return 正しく割ることができたら true を返す．
bool
BfoMgr::make_cofactor(ymuint64* dst_bv,
		      ymuint dst_pos,
		      const ymuint64* bv1,
		      ymuint pos1,
		      const ymuint64* bv2,
		      ymuint pos2)
{
  ymuint nb = cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ dst, ++ _bv1, ++ _bv2) {
    if ( (~(*_bv1) & *_bv2) != 0ULL ) {
      // この場合の dst の値は不定
      return false;
    }
    *dst = *_bv1 & ~(*_bv2);
  }
  return true;
}

// @brief カバー/キューブの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] nc キューブ数
// @param[in] bv カバー/キューブを表すビットベクタ
//
// キューブの場合は nc を 1 とする．
void
BfoMgr::print(ostream& s,
	      ymuint nc,
	      const ymuint64* bv)
{
  const char* plus = "";
  for (ymuint i = 0; i < nc; ++ i) {
    s << plus;
    plus = " + ";
    const char* spc = "";
    for (ymuint j = 0; j < variable_num(); ++ j) {
      BfoPol pol = literal(bv, i, j);
      if ( pol == kBfoPolP ) {
	s << spc << varname(j);
	spc = " ";
      }
      else if ( pol == kBfoPolN ) {
	s << spc << varname(j) << "'";
	spc = " ";
      }
    }
  }
}

END_NAMESPACE_YM_BFO
