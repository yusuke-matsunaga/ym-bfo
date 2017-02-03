
/// @file AlgMgr.cc
/// @brief AlgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/AlgMgr.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


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

const
AlgLiteral kAlgLiteralUndef;

//////////////////////////////////////////////////////////////////////
// クラス AlgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
//
// 変数名はデフォルトのものが使用される．
AlgMgr::AlgMgr(ymuint variable_num) :
  mVarNum(variable_num),
  mVarNameList(mVarNum)
{
  // 変数名を作る．
  // 変数番号を26進数で表して文字列にする．
  for (ymuint i = 0; i < mVarNum; ++ i) {
    string name = _varname(i);
    mVarNameList[i] = name;
    mVarNameMap.add(name, i);
  }

  _init_buff();
}

// @brief コンストラクタ
// @param[in] varname_list 変数名のリスト
//
// varname_list のサイズが変数の数になる．
AlgMgr::AlgMgr(const vector<string>& varname_list) :
  mVarNum(varname_list.size()),
  mVarNameList(varname_list)
{
  // 変数名を登録する．
  for (ymuint i = 0; i < mVarNum; ++ i) {
    mVarNameMap.add(mVarNameList[i], i);
  }

  _init_buff();
}

// @brief デストラクタ
AlgMgr::~AlgMgr()
{
  delete_body(mTmpBuff, mTmpBuffSize);
}

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
ymuint
_count(ymuint64 pat)
{
  ymuint table[] = {
    // utils/gen_bitcount_tbl.py で生成
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
AlgMgr::literal_num(ymuint nc,
		    const ymuint64* bv)
{
  ymuint n = nc * _cube_size();
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
AlgMgr::new_body(ymuint cube_num)
{
  ymuint size = _cube_size() * cube_num;
  ymuint64* body = new ymuint64[size];
  for (ymuint i = 0; i < size; ++ i) {
    body[i] = 0ULL;
  }
  return body;
}

// @brief キューブ/カバー用の領域を削除する．
// @param[in] p 領域を指すポインタ
// @param[in] cube_num キューブ数
//
// キューブの時は cube_num = 1 とする．
void
AlgMgr::delete_body(ymuint64* p,
		    ymuint cube_num)
{
  delete [] p;
}


BEGIN_NONAMESPACE

// 変数名として適切な文字なら true を返す．
inline
bool
is_validchar(char c)
{
  // utils/gen_validchar_tbl.py で生成
  bool table[] = {
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false,
   false, true , true , true , true , true , true , true , true , true , true , true , true , true , true , true ,
   true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, true ,
   false, true , true , true , true , true , true , true , true , true , true , true , true , true , true , true ,
   true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
   false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
  };
  return table[c];
}

END_NONAMESPACE

// @brief カバー/キューブを表す文字列をパーズする．
// @param[in] str 対象の文字列
// @param[out] lit_list パーズ結果のリテラルのリスト
// @return キューブ数を返す．
//
// lit_list 中の kAlgLiteralUndef はキューブの区切りとみなす．
ymuint
AlgMgr::parse(const char* str,
	      vector<AlgLiteral>& lit_list)
{
  lit_list.clear();

  char c;
  string name;
  ymuint var;

  // 有限状態機械で実装する．
  // なんか書いててスッキリしないコード
 state0:
  c = *str;
  // EOF なら終わる．
  if ( c == '\0' ) {
    goto state_end;
  }
  // 空白文字ならこの状態にとどまる．
  if ( isspace(c) ) {
    ++ str;
    goto state0;
  }
  // [a-zA-Z0-9_] なら state1 に遷移する．
  if ( is_validchar(c) ) {
    name = string();
    goto state1;
  }
  // '+' ならキューブの区切りマークを追加する．
  if ( c == '+' ) {
    ++ str;
    lit_list.push_back(kAlgLiteralUndef);
    goto state0;
  }
  // それ以外はエラー
  goto state_error;

 state1:
  c = *str;
  // [a-zA-Z0-9_]* ならこの状態にとどまる．
  if ( is_validchar(c) ) {
    name += c;
    ++ str;
    goto state1;
  }
  // ' なら inv をセットしたあとで name を処理する．
  if ( c == '\'' ) {
    ++ str;
    if ( !mVarNameMap.find(name, var) ) {
      goto state_error;
    }
    lit_list.push_back(AlgLiteral(var, true));
    goto state0;
  }
  // それ以外なら一旦現在の name を登録する．
  if ( !mVarNameMap.find(name, var) ) {
    goto state_error;
  }
  lit_list.push_back(AlgLiteral(var, false));
  goto state0;

 state_error:
  lit_list.clear();
  return 0;

 state_end:
  ymuint cube_num = 0;
  bool first = true;
  for (ymuint i = 0; i < lit_list.size(); ++ i) {
    if ( lit_list[i] == kAlgLiteralUndef ) {
      first = true;
    }
    else if ( first ) {
      first = false;
      ++ cube_num;
    }
  }
  return cube_num;
}

// @brief リテラルをセットする．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] dst_pos 対象のキューブ位置
// @param[in] lit_list リテラルのリスト
//
// lit_list 中の kAlgLiteralUndef はキューブの区切りとみなす．
void
AlgMgr::set_literal(ymuint64* dst_bv,
		    ymuint dst_pos,
		    const vector<AlgLiteral>& lit_list)
{
  ymuint nb = _cube_size();
  ymuint64* _dst = dst_bv + dst_pos * nb;
  ymuint n = lit_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    AlgLiteral lit = lit_list[i];
    if ( lit == kAlgLiteralUndef ) {
      _dst += nb;
    }
    else {
      ymuint var = lit.varid();
      ymuint blk = _block_pos(var);
      ymuint sft = _shift_num(var);
      ymuint64 pat = lit.is_negative() ? kAlgPolN : kAlgPolP;
      _dst[blk] |= (pat << sft);
    }
  }
}

// @brief 2つのカバーの論理和を計算する．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバーのキューブ数
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @return 結果のキューブ数を返す．
//
// dst_bv には十分な容量があると仮定する．<br>
// dst_bv == bv1 の場合もあり得る．<br>
ymuint
AlgMgr::sum(ymuint64* dst_bv,
	    ymuint nc1,
	    const ymuint64* bv1,
	    ymuint nc2,
	    const ymuint64* bv2)
{
  // dst_bv == bv1 の時は bv1 のコピーを作る．
  _resize_buff(nc1);
  copy(nc1, mTmpBuff, 0, bv1, 0);
  bv1 = mTmpBuff;

  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  ymuint wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = cube_compare(bv1, rpos1, bv2, rpos2);
    if ( res < 0 ) {
      cube_copy(dst_bv, wpos, bv1, rpos1);
      ++ rpos1;
    }
    else if ( res > 0 ) {
      cube_copy(dst_bv, wpos, bv2, rpos2);
      ++ rpos2;
    }
    else {
      cube_copy(dst_bv, wpos, bv1, rpos1);
      ++ rpos1;
      ++ rpos2;
    }
    ++ wpos;
  }
  while ( rpos1 < nc1 ) {
    cube_copy(dst_bv, wpos, bv1, rpos1);
    ++ rpos1;
    ++ wpos;
  }
  while ( rpos2 < nc2 ) {
    cube_copy(dst_bv, wpos, bv2, rpos2);
    ++ rpos2;
    ++ wpos;
  }

  return wpos;
}

// @brief 2つのカバーの差分を計算する．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバーのキューブ数
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @return 結果のキューブ数を返す．
ymuint
AlgMgr::diff(ymuint64* dst_bv,
	     ymuint nc1,
	     const ymuint64* bv1,
	     ymuint nc2,
	     const ymuint64* bv2)
{
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  ymuint wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = cube_compare(bv1, rpos1, bv2, rpos2);
    if ( res < 0 ) {
      cube_copy(dst_bv, wpos, bv1, rpos1);
      ++ rpos1;
      ++ wpos;
    }
    else if ( res > 0 ) {
      ++ rpos2;
    }
    else {
      ++ rpos1;
      ++ rpos2;
    }
  }
  while ( rpos1 < nc1 ) {
    cube_copy(dst_bv, wpos, bv1, rpos1);
    ++ rpos1;
    ++ wpos;
  }

  return wpos;
}

// @brief 2つのカバーの論理積を計算する．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバーのキューブ数
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @return 結果のキューブ数を返す．
ymuint
AlgMgr::product(ymuint64* dst_bv,
		ymuint nc1,
		const ymuint64* bv1,
		ymuint nc2,
		const ymuint64* bv2)
{
  // dst_bv == bv1 の時は bv1 のコピーを作る．
  _resize_buff(nc1);
  copy(nc1, mTmpBuff, 0, bv1, 0);
  bv1 = mTmpBuff;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  ymuint wpos = 0;
  for (ymuint rpos1 = 0; rpos1 < nc1; ++ rpos1) {
    for (ymuint rpos2 = 0; rpos2 < nc2; ++ rpos2) {
      if ( cube_product(dst_bv, wpos, bv1, rpos1, bv2, rpos2) ) {
	++ wpos;
      }
    }
  }

  return wpos;
}

// @brief カバーの代数的除算を行う．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめのカバー(除数)のキューブ数
// @param[in] bv2 2つめのカバー(除数)を表すビットベクタ
// @return 結果のキューブ数を返す．
ymuint
AlgMgr::division(ymuint64* dst_bv,
		 ymuint nc1,
		 const ymuint64* bv1,
		 ymuint nc2,
		 const ymuint64* bv2)
{
  // 作業領域のビットベクタを確保する．
  // 大きさは nc1
  _resize_buff(nc1);

  // bv1 の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  for (ymuint i = 0; i < nc1; ++ i) {
    bool found = false;
    for (ymuint j = 0; j < nc2; ++ j) {
      if ( cube_cofactor(mTmpBuff, i, bv1, i, bv2, j) ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      // このキューブは割れない．
      cube_clear(mTmpBuff, i);
    }
  }

  // 商のキューブは tmp 中に nc2 回現れているはず．
  vector<ymuint> pos_list;
  pos_list.reserve(nc1);
  vector<bool> mark(nc1, false);
  for (ymuint i = 0; i < nc1; ++ i) {
    if ( mark[i] ) {
      // すでに調べている．
      continue;
    }
    ymuint c = 1;
    vector<ymuint> tmp_list;
    for (ymuint i2 = i + 1; i2 < nc1; ++ i2) {
      if ( cube_compare(mTmpBuff, i, mTmpBuff, i2) == 0 ) {
	++ c;
	// i 番目のキューブと等しかったキューブ位置を記録する．
	tmp_list.push_back(i2);
      }
    }
    if ( c == nc2 ) {
      // 見つけた
      pos_list.push_back(i);
      // tmp_list に含まれるキューブはもう調べなくて良い．
      for (ymuint j = 0; j < tmp_list.size(); ++ j) {
	ymuint pos = tmp_list[j];
	mark[pos] = true;
      }
    }
  }

  ymuint nc = pos_list.size();
  for (ymuint i = 0; i < nc; ++ i) {
    ymuint pos = pos_list[i];
    cube_copy(dst_bv, i, mTmpBuff, pos);
  }

  return nc;
}

// @brief カバーをリテラルで割る(コファクター演算)．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] nc1 カバーのキューブ数
// @param[in] bv1 カバーを表すビットベクタ
// @param[in] lit 対象のリテラル
// @return 結果のキューブ数を返す．
ymuint
AlgMgr::division(ymuint64* dst_bv,
		 ymuint nc1,
		 const ymuint64* bv1,
		 AlgLiteral lit)
{
  ymuint var_id = lit.varid();
  ymuint blk = _block_pos(var_id);
  ymuint sft = _shift_num(var_id);
  AlgPol pat = lit.is_positive() ? kAlgPolP : kAlgPolN;
  ymuint64 pat1 = pat << sft;
  ymuint64 mask = 3UL << sft;
  ymuint64 nmask = ~mask;
  ymuint nb = _cube_size();
  ymuint wpos = 0;
  for (ymuint i = 0; i < nc1; ++ i) {
    if ( (bv1[i * nb + blk] & mask) == pat1 ) {
      cube_copy(dst_bv, wpos, bv1, i);
      *(dst_bv + wpos * nb + blk) &= nmask;
      ++ wpos;
    }
  }

  return wpos;
}

// @brief カバー中のすべてのキューブの共通部分を求める．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] nc1 カバーのキューブ数
// @param[in] bv1 カバーを表すビットベクタ
//
// 共通部分がないときは空のキューブとなる．
void
AlgMgr::common_cube(ymuint64* dst_bv,
		    ymuint nc1,
		    const ymuint64* bv1)
{
  ymuint nb = _cube_size();

  // 最初のキューブをコピーする．
  cube_copy(dst_bv, 0, bv1, 0);

  // 2番目以降のキューブとの共通部分を求める．
  ymuint offset = nb;
  for (ymuint cube_pos = 1; cube_pos < nc1; ++ cube_pos) {
    ymuint64 tmp = 0ULL;
    for (ymuint i = 0; i < nb; ++ i) {
      dst_bv[i] &= bv1[offset + i];
      tmp |= dst_bv[i];
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
    offset += nb;
  }
}

// @brief カバー(を表すビットベクタ)のコピーを行う．
// @param[in] cube_num キューブ数
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ位置
// @param[in] src_bv ソースのビットベクタ
// @param[in] src_pos ソースのキューブ位置
void
AlgMgr::copy(ymuint cube_num,
	     ymuint64* dst_bv,
	     ymuint dst_pos,
	     const ymuint64* src_bv,
	     ymuint src_pos)
{
  ymuint nb = _cube_size();
  ymuint n = nb * cube_num;
  ymuint64* dst = dst_bv + dst_pos * nb;
  const ymuint64* src = src_bv + src_pos * nb;
  for (ymuint i = 0; i < n; ++ i, ++ dst, ++ src) {
    *dst = *src;
  }
}

// @brief マージソートを行う下請け関数
// @param[in] bv 対象のビットベクタ
// @param[in] start 開始位置
// @param[in] end 終了位置
//
// bv[start] - bv[end - 1] の領域をソートする．
void
AlgMgr::_sort(ymuint64* bv,
	      ymuint start,
	      ymuint end)
{
  ymuint n = end - start;
  if ( n <= 1 ) {
    return;
  }
  if ( n == 2 ) {
    // (0, 1) と (1, 0) の2通りだけ
    if ( cube_compare(bv, 0, bv, 1) < 0 ) {
      // (1, 0) だったので交換する．
      _resize_buff(1);
      cube_swap(bv, 0, bv, 1);
    }
    return;
  }
  if ( n == 3 ) {
    // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
    // の6通りなので虱潰し
    if ( cube_compare(bv, 0, bv, 1) < 0 ) {
      // (1, 0, 2), (1, 2, 0), (2, 1, 0)
      if ( cube_compare(bv, 0, bv, 2) < 0 ) {
	// (1, 2, 0), (2, 1, 0)
	if ( cube_compare(bv, 1, bv, 2) < 0 ) {
	  // (2, 1, 0)
	  // 0 と 2 を交換
	  _resize_buff(1);
	  cube_swap(bv, 0, bv, 2);
	}
	else {
	  // (1, 2, 0)
	  // 0 <- 1, 1 <- 2, 2 <- 0
	  _resize_buff(1);
	  cube_rotate3(bv, 0, bv, 1, bv, 2);
	}
      }
      else {
	// (1, 0, 2)
	// 0 <-> 1
	_resize_buff(0);
	cube_swap(bv, 0, bv, 1);
      }
    }
    else {
      // (0, 1, 2), (0, 2, 1), (2, 0, 1)
      if ( cube_compare(bv, 0, bv, 2) < 0 ) {
	// (2, 0, 1)
	// 0 <- 2, 2 <- 1, 1 <- 0
	_resize_buff(1);
	cube_rotate3(bv, 0, bv, 2, bv, 1);
      }
      else {
	// (0, 1, 2), (0, 2, 1)
	if ( cube_compare(bv, 1, bv, 2) < 0 ) {
	  // (0, 2, 1)
	  // 1 <-> 2
	  _resize_buff(1);
	  cube_swap(bv, 1, bv, 2);
	}
	else {
	  // (0, 1, 2)
	  // そのまま
	}
      }
    }
    return;
  }
  if ( n == 4 ) {
    _resize_buff(1);
    // 0 と 1 を整列
    if ( cube_compare(bv, 0, bv, 1) < 0 ) {
      cube_swap(bv, 0, bv, 1);
    }
    // 2 と 3 を整列
    if ( cube_compare(bv, 2, bv, 3) < 0 ) {
      cube_swap(bv, 2, bv, 3);
    }
    // 0 と 2 を比較
    if ( cube_compare(bv, 0, bv, 2) < 0 ) {
      if ( cube_compare(bv, 0, bv, 3) < 0 ) {
	// (0, 1) と (2, 3) を交換
	cube_swap(bv, 0, bv, 2);
	cube_swap(bv, 1, bv, 3);
      }
      else if ( cube_compare(bv, 1, bv, 3) < 0 ) {
	// 0 <- 2, 2 <- 3, 3 <- 1, 1 <- 0
	cube_rotate4(bv, 0, bv, 2, bv, 3, bv, 1);
      }
      else {
	// 0 <- 2, 2 <- 1, 1 <- 0
	cube_rotate3(bv, 0, bv, 2, bv, 1);
      }
    }
    else if ( cube_compare(bv, 1, bv, 2) < 0 ) {
      if ( cube_compare(bv, 1, bv, 3) < 0 ) {
	// 1 <- 2, 2 <- 3, 3 <- 1
	cube_rotate3(bv, 1, bv, 2, bv, 3);
      }
      else {
	// 1 <- 2, 2 <- 1
	cube_swap(bv, 1, bv, 2);
      }
    }
    else {
      // そのまま
    }
    return;
  }

  // 半分に分割してそれぞれソートする．
  ymuint hn = (n + 1) / 2;
  ymuint start1 = start;
  ymuint end1 = start + hn;
  ymuint start2 = end1;
  ymuint end2 = end;
  _sort(bv, start1, end1);
  _sort(bv, start2, end2);

  // trivial case
  // 前半部分の末尾が後半部分の先頭より大きければ
  // すでに整列している．
  if ( cube_compare(bv, end1 - 1, bv, start2) > 0 ) {
    return;
  }

  // マージする．
  // 前半部分を一旦 mTmpBuff にコピーする．
  _resize_buff(hn);
  copy(hn, mTmpBuff, 0, bv, start1);
  ymuint rpos1 = 0;
  ymuint rpos2 = start2;
  ymuint wpos = start1;
  while ( rpos1 < hn && rpos2 < end2 ) {
    int comp_res = cube_compare(mTmpBuff, rpos1, bv, rpos2);
    if ( comp_res > 0 ) {
      cube_copy(bv, wpos, mTmpBuff, rpos1);
      ++ wpos;
      ++ rpos1;
    }
    else if ( comp_res < 0 ) {
      cube_copy(bv, wpos, bv, rpos2);
      ++ wpos;
      ++ rpos2;
    }
    else {
      // 重複したキューブはエラー
      ASSERT_NOT_REACHED;
    }
  }
  for ( ; rpos1 < hn; ++ rpos1, ++ wpos) {
    cube_copy(bv, wpos, mTmpBuff, rpos1);
  }
  // 後半部分が残っている時はそのままでいいはず．
  ASSERT_COND( rpos2 == wpos );
}

// @brief カバー(を表すビットベクタ)の比較を行う．
// @param[in] nc1 1つめのカバーのキューブ数
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] nc2 2つめカバーのキューブ数
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @retval -1 bv1 <  bv2
// @retval  0 bv1 == bv2
// @retval  1 bv1 >  bv2
//
// 比較はキューブごとの辞書式順序で行う．
int
AlgMgr::compare(ymuint nc1,
		const ymuint64* bv1,
		ymuint nc2,
		const ymuint64* bv2)
{
  ymuint rpos1 = 0;
  ymuint rpos2 = 0;
  for ( ; rpos1 < nc1 && rpos2 < nc2; ++ rpos1, ++ rpos2 ) {
    int res = cube_compare(bv1, rpos1, bv2, rpos2);
    if ( res != 0 ) {
      return res;
    }
  }
  if ( rpos1 < nc1 ) {
    // rpos2 == nc2 なので bv1 が大きい
    return 1;
  }
  if ( rpos2 < nc2 ) {
    // rpos1 == nc1 なので bv2 が大きい
    return -1;
  }
  // 等しい
  return 0;
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
AlgMgr::cube_compare(const ymuint64* bv1,
		     ymuint pos1,
		     const ymuint64* bv2,
		     ymuint pos2)
{
  ymuint nb = _cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ _bv1, ++ _bv2) {
    ymuint64 pat1 = *_bv1;
    ymuint64 pat2 = *_bv2;
    if ( pat1 < pat2 ) {
      return -1;
    }
    else if ( pat1 > pat2 ) {
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
AlgMgr::cube_check_product(const ymuint64* bv1,
			   ymuint pos1,
			   const ymuint64* bv2,
			   ymuint pos2)
{
  ymuint nb = _cube_size();
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
AlgMgr::cube_check_containment(const ymuint64* bv1,
			       ymuint pos1,
			       const ymuint64* bv2,
			       ymuint pos2)
{
  ymuint nb = _cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ _bv1, ++ _bv2) {
    if ( (~(*_bv1) & *_bv2) != 0ULL ) {
      return false;
    }
  }
  return true;
}

// @brief ２つのキューブに共通なリテラルがあれば true を返す．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
// @return ２つのキューブに共通なリテラルがあれば true を返す．
bool
AlgMgr::cube_check_intersect(const ymuint64* bv1,
			     ymuint pos1,
			     const ymuint64* bv2,
			     ymuint pos2)
{
  ymuint nb = _cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ _bv1, ++ _bv2) {
    if ( (*_bv1 & *_bv2) != 0ULL ) {
      return true;
    }
  }
  return false;
}

#if 0
// @brief 文字列からキューブを表すビットベクタにセットする．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ番号
// @param[in] str 内容を表す文字列
// @return 正しい文字列なら true を返す．
bool
AlgMgr::cube_set(ymuint64* dst_bv,
		 ymuint dst_pos,
		 const char* str)
{
  // まず空に初期化する．
  ymuint nb = _cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  for (ymuint i = 0; i < nb; ++ i) {
    dst[i] = 0ULL;
  }

  // str から変数を一つづつ読み出して設定する．
  for ( ; ; ) {
    // 次の非空白文字までスキップする
    for ( ; ; ++ str) {
      if ( !isspace(*str) ) {
	break;
      }
    }
    // [a-z]*'? とのマッチを行う．
    string name;
    bool error = false;
    bool inverted = false;
    for ( ; ; ++ str) {
      char c = *str;
      if ( c == '\0' ) {
	break;
      }
      if ( isspace(c) ) {
	break;
      }
      if ( c == '\'' ) {
	inverted = true;
	++ str;
	break;
      }
      if ( c < 'a' || c > 'z' ) {
	error = true;
      }
      name += c;
    }
    if ( error || name == string() ) {
      return false;
    }
    ymuint var;
    if ( !mVarNameMap.find(name, var) ) {
      // 名前が見つからなかった．
      return false;
    }
    set_literal(dst_bv, dst_pos, var, inverted);
  }

  return true;
}
#endif

// @brief キューブ(を表すビットベクタ)をクリアする．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] dst_pos 対象のキューブ番号
void
AlgMgr::cube_clear(ymuint64* dst_bv,
		   ymuint dst_pos)
{
  ymuint nb = _cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  for (ymuint i = 0; i < nb; ++ i, ++ dst) {
    *dst = 0ULL;
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
AlgMgr::cube_product(ymuint64* dst_bv,
		     ymuint dst_pos,
		     const ymuint64* bv1,
		     ymuint pos1,
		     const ymuint64* bv2,
		     ymuint pos2)
{
  ymuint nb = _cube_size();
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
AlgMgr::cube_cofactor(ymuint64* dst_bv,
		      ymuint dst_pos,
		      const ymuint64* bv1,
		      ymuint pos1,
		      const ymuint64* bv2,
		      ymuint pos2)
{
  ymuint nb = _cube_size();
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
AlgMgr::print(ostream& s,
	      ymuint nc,
	      const ymuint64* bv)
{
  const char* plus = "";
  for (ymuint i = 0; i < nc; ++ i) {
    s << plus;
    plus = " + ";
    const char* spc = "";
    for (ymuint j = 0; j < variable_num(); ++ j) {
      AlgPol pol = literal(bv, i, j);
      if ( pol == kAlgPolP ) {
	s << spc << varname(j);
	spc = " ";
      }
      else if ( pol == kAlgPolN ) {
	s << spc << varname(j) << "'";
	spc = " ";
      }
    }
  }
}

// @brief mTmpBuff を初期化する．
void
AlgMgr::_init_buff()
{
  // とりあえず 128 を初期値とする．
  mTmpBuffSize = 128;
  mTmpBuff = new_body(mTmpBuffSize);
}

// @brief mTmpBuff に必要な領域を確保する．
// @param[in] req_size 要求するキューブ数
void
AlgMgr::_resize_buff(ymuint req_size)
{
  ymuint old_size = mTmpBuffSize;
  while ( mTmpBuffSize < req_size ) {
    mTmpBuffSize <<= 1;
  }

  if ( old_size < mTmpBuffSize ) {
    delete_body(mTmpBuff, old_size);
  }
  mTmpBuff = new_body(mTmpBuffSize);
}

END_NAMESPACE_YM_BFO
