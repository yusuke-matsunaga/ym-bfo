
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

BEGIN_NONAMESPACE

// @brief キューブの比較を行う．
// @param[in] pat1, pat2 比較対象のキューブのパタン
// @param[in] nb パタンのワード長
int
compare(const ymuint64* pat1,
	const ymuint64* pat2,
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
     const ymuint64* src,
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
check_product(const ymuint64* pat1,
	      const ymuint64* pat2,
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
_make_product(ymuint64* dst,
	      const ymuint64* pat1,
	      const ymuint64* pat2,
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
check_containment(const ymuint64* pat1,
		  const ymuint64* pat2,
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
	      const ymuint64* pat1,
	      const ymuint64* pat2,
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
    int res = compare(bv1 + rpos1 * nb, bv2 + rpos2 * nb, nb);
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
    int res = compare(bv1 + rpos1 * nb, bv2 + rpos2 * nb, nb);
    if ( res < 0 ) {
      copy(cbody + wpos * nb, bv1 + rpos1 * nb, nb);
      ++ rpos1;
    }
    else if ( res > 0 ) {
      copy(cbody + wpos * nb, bv2 + rpos2 * nb, nb);
      ++ rpos2;
    }
    else {
      copy(cbody + wpos * nb, bv1 + rpos1 * nb, nb);
      ++ rpos1;
      ++ rpos2;
    }
    ++ wpos;
  }
  while ( rpos1 < nc1 ) {
    copy(cbody + wpos * nb, bv1 + rpos1 * nb, nb);
    ++ rpos1;
    ++ wpos;
  }
  while ( rpos2 < nc2 ) {
    copy(cbody + wpos * nb, bv2 + rpos2 * nb, nb);
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
    int res = compare(bv1 + rpos1 * nb, bv2 + rpos2 * nb, nb);
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
    int res = compare(bv1 + rpos1 * nb, bv2 + rpos2 * nb, nb);
    if ( res < 0 ) {
      copy(cbody + wpos * nb, bv1 + rpos1 * nb, nb);
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
    copy(cbody + wpos * nb, bv1 + rpos1 * nb, nb);
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
      if ( check_product(bv1 + rpos1 * nb, bv2 + rpos2 * nb, nb) ) {
	++ nans;
      }
    }
  }

  ymuint64* cbody = new_body(nans);
  ymuint wpos = 0;
  for (ymuint rpos1 = 0; rpos1 < nc1; ++ rpos1) {
    for (ymuint rpos2 = 0; rpos2 < nc2; ++ rpos2) {
      if ( _make_product(cbody + wpos * nb, bv1 + rpos1 * nb, bv2 + rpos2 * nb, nb) ) {
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
      if ( make_cofactor(tmp + i * nb, bv1 + i * nb, bv2 + j * nb, nb) ) {
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
      if ( compare(tmp + i * nb, tmp + i2 * nb, nb) == 0 ) {
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
      copy(cbody + i * nb, tmp + pos * nb, nb);
    }
  }

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
    if ( check_containment(bv1 + i * nb, bv2, nb) ) {
      ++ nans;
    }
  }

  ymuint64* cbody = new_body(nans);
  ymuint wpos = 0;
  for (ymuint i = 0; i < nc1; ++ i) {
    if ( make_cofactor(cbody + wpos * nb, bv1 + i * nb, bv2, nb) ) {
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
      copy(cbody + wpos * nb, bv1 + i * nb, nb);
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

#if 0

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
