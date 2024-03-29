
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
_varname(
  SizeType id
)
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

const AlgLiteral AlgLiteralUndef;

//////////////////////////////////////////////////////////////////////
// クラス AlgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AlgMgr::AlgMgr(
  SizeType variable_num
) : mVarNum{variable_num},
    mVarNameList(mVarNum)
{
  // 変数名を作る．
  // 変数番号を26進数で表して文字列にする．
  for ( SizeType i = 0; i < mVarNum; ++ i ) {
    string name = _varname(i);
    mVarNameList[i] = name;
    mVarNameMap.emplace(name, i);
  }

  _init_buff();
}

// @brief コンストラクタ
AlgMgr::AlgMgr(
  const vector<string>& varname_list
) : mVarNum{varname_list.size()},
    mVarNameList{varname_list}
{
  // 変数名を登録する．
  for ( SizeType i = 0; i < mVarNum; ++ i ) {
    mVarNameMap.emplace(mVarNameList[i], i);
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
SizeType
_count(ymuint64 pat)
{
  SizeType table[] = {
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
SizeType
AlgMgr::literal_num(
  SizeType nc,
  const ymuint64* bv
)
{
  SizeType n = nc * _cube_size();
  // 8 ビットごとに区切って表引きで計算する．
  SizeType ans = 0;
  for ( SizeType i = 0; i < n; ++ i, ++ bv ) {
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

// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
SizeType
AlgMgr::literal_num(
  SizeType nc,
  const ymuint64* bv,
  AlgLiteral lit
)
{
  SizeType var_id = lit.varid();
  SizeType blk = _block_pos(var_id);
  SizeType sft = _shift_num(var_id);
  ymuint64 pat = lit.is_positive() ? kAlgPolP : kAlgPolN;
  ymuint64 mask = pat << sft;
  SizeType n = 0;
  for ( SizeType i = 0; i < nc; ++ i, blk += _cube_size() ) {
    if ( (bv[blk] & mask) == mask ) {
      ++ n;
    }
  }
  return n;
}

// @brief キューブ/カバー用の領域を確保する．
ymuint64*
AlgMgr::new_body(
  SizeType cube_num
)
{
  SizeType size = _cube_size() * cube_num;
  ymuint64* body = new ymuint64[size];
  for ( SizeType i = 0; i < size; ++ i ) {
    body[i] = 0ULL;
  }
  return body;
}

// @brief キューブ/カバー用の領域を削除する．
void
AlgMgr::delete_body(
  ymuint64* p,
  SizeType cube_num
)
{
  delete [] p;
}


BEGIN_NONAMESPACE

// 変数名として適切な文字なら true を返す．
inline
bool
is_validchar(
  char c
)
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
SizeType
AlgMgr::parse(
  const char* str,
  vector<AlgLiteral>& lit_list
)
{
  lit_list.clear();

  char c;
  string name;
  SizeType var;

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
    lit_list.push_back(AlgLiteralUndef);
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
    if ( mVarNameMap.count(name) == 0 ) {
      goto state_error;
    }
    var = mVarNameMap.at(name);
    lit_list.push_back(AlgLiteral(var, true));
    goto state0;
  }
  // それ以外なら一旦現在の name を登録する．
  if ( mVarNameMap.count(name) == 0 ) {
    goto state_error;
  }
  var = mVarNameMap.at(name);
  lit_list.push_back(AlgLiteral(var, false));
  goto state0;

 state_error:
  lit_list.clear();
  return 0;

 state_end:
  SizeType cube_num = 0;
  bool first = true;
  for ( auto lit: lit_list ) {
    if ( lit == AlgLiteralUndef ) {
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
void
AlgMgr::set_literal(
  ymuint64* dst_bv,
  SizeType dst_pos,
  const vector<AlgLiteral>& lit_list
)
{
  SizeType nb = _cube_size();
  ymuint64* _dst = dst_bv + dst_pos * nb;
  for ( auto lit: lit_list ) {
    if ( lit == AlgLiteralUndef ) {
      _dst += nb;
    }
    else {
      SizeType var = lit.varid();
      SizeType blk = _block_pos(var);
      SizeType sft = _shift_num(var);
      ymuint64 pat = lit.is_negative() ? kAlgPolN : kAlgPolP;
      _dst[blk] |= (pat << sft);
    }
  }
}

// @brief 2つのカバーの論理和を計算する．
SizeType
AlgMgr::sum(
  ymuint64* dst_bv,
  SizeType nc1,
  const ymuint64* bv1,
  SizeType nc2,
  const ymuint64* bv2
)
{
  // dst_bv == bv1 の時は bv1 のコピーを作る．
  _resize_buff(nc1);
  copy(nc1, mTmpBuff, 0, bv1, 0);
  bv1 = mTmpBuff;

  SizeType rpos1 = 0;
  SizeType rpos2 = 0;
  SizeType wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = cube_compare(bv1, rpos1, bv2, rpos2);
    if ( res > 0 ) {
      cube_copy(dst_bv, wpos, bv1, rpos1);
      ++ rpos1;
    }
    else if ( res < 0 ) {
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
SizeType
AlgMgr::diff(
  ymuint64* dst_bv,
  SizeType nc1,
  const ymuint64* bv1,
  SizeType nc2,
  const ymuint64* bv2
)
{
  SizeType rpos1 = 0;
  SizeType rpos2 = 0;
  SizeType wpos = 0;
  while ( rpos1 < nc1 && rpos2 < nc2 ) {
    int res = cube_compare(bv1, rpos1, bv2, rpos2);
    if ( res > 0 ) {
      cube_copy(dst_bv, wpos, bv1, rpos1);
      ++ rpos1;
      ++ wpos;
    }
    else if ( res < 0 ) {
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
SizeType
AlgMgr::product(
  ymuint64* dst_bv,
  SizeType nc1,
  const ymuint64* bv1,
  SizeType nc2,
  const ymuint64* bv2
)
{
  // dst_bv == bv1 の時は bv1 のコピーを作る．
  _resize_buff(nc1);
  copy(nc1, mTmpBuff, 0, bv1, 0);
  bv1 = mTmpBuff;

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType wpos = 0;
  for ( SizeType rpos1 = 0; rpos1 < nc1; ++ rpos1 ) {
    for ( SizeType rpos2 = 0; rpos2 < nc2; ++ rpos2 ) {
      if ( cube_product(dst_bv, wpos, bv1, rpos1, bv2, rpos2) ) {
	++ wpos;
      }
    }
  }

  return wpos;
}

// @brief カバーとリテラルとの論理積を計算する．
SizeType
AlgMgr::product(
  ymuint64* dst_bv,
  SizeType nc1,
  const ymuint64* bv1,
  AlgLiteral lit
)
{
  SizeType var_id = lit.varid();
  SizeType blk = _block_pos(var_id);
  SizeType sft = _shift_num(var_id);
  ymuint64 pat = lit.is_positive() ? kAlgPolP : kAlgPolN;
  ymuint64 pat1 = pat << sft;
  ymuint64 mask = 3UL << sft;
  ymuint64 nmask = ~mask;
  SizeType nb = _cube_size();

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  SizeType wpos = 0;
  for ( SizeType rpos1 = 0; rpos1 < nc1; ++ rpos1 ) {
    SizeType rbase = rpos1 * nb;
    ymuint64 tmp = bv1[rbase + blk] | pat1;
    if ( (tmp & mask) == mask ) {
      // 相反するリテラルがあった．
      continue;
    }
    SizeType wbase = wpos * nb;
    for ( SizeType i = 0; i < nb; ++ i ) {
      dst_bv[wbase] = bv1[rbase];
    }
    dst_bv[wbase + blk] = tmp;
    ++ wpos;
  }

  return wpos;
}

// @brief カバーの代数的除算を行う．
SizeType
AlgMgr::division(
  ymuint64* dst_bv,
  SizeType nc1,
  const ymuint64* bv1,
  SizeType nc2,
  const ymuint64* bv2
)
{
  // 作業領域のビットベクタを確保する．
  // 大きさは nc1
  _resize_buff(nc1);

  // bv1 の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  vector<bool> mark(nc1, false);
  for ( SizeType i = 0; i < nc1; ++ i ) {
    for ( SizeType j = 0; j < nc2; ++ j ) {
      if ( cube_division(mTmpBuff, i, bv1, i, bv2, j) ) {
	mark[i] = true;
	break;
      }
    }
  }

  // 商のキューブは tmp 中に nc2 回現れているはず．
  vector<SizeType> pos_list;
  pos_list.reserve(nc1);
  for ( SizeType i = 0; i < nc1; ++ i ) {
    if ( !mark[i] ) {
      // 対象ではない．
      continue;
    }

    SizeType c = 1;
    vector<SizeType> tmp_list;
    for ( SizeType i2 = i + 1; i2 < nc1; ++ i2 ) {
      if ( mark[i2] && cube_compare(mTmpBuff, i, mTmpBuff, i2) == 0 ) {
	++ c;
	// i 番目のキューブと等しかったキューブ位置を記録する．
	tmp_list.push_back(i2);
      }
    }
    if ( c == nc2 ) {
      // 見つけた
      pos_list.push_back(i);
      // tmp_list に含まれるキューブはもう調べなくて良い．
      for ( SizeType j = 0; j < tmp_list.size(); ++ j ) {
	SizeType pos = tmp_list[j];
	mark[pos] = false;
      }
    }
  }

  SizeType nc = pos_list.size();
  for ( SizeType i = 0; i < nc; ++ i ) {
    SizeType pos = pos_list[i];
    cube_copy(dst_bv, i, mTmpBuff, pos);
  }

  return nc;
}

// @brief カバーをリテラルで割る．
SizeType
AlgMgr::division(
  ymuint64* dst_bv,
  SizeType nc1,
  const ymuint64* bv1,
  AlgLiteral lit
)
{
  SizeType var_id = lit.varid();
  SizeType blk = _block_pos(var_id);
  SizeType sft = _shift_num(var_id);
  ymuint64 pat = lit.is_positive() ? kAlgPolP : kAlgPolN;
  ymuint64 pat1 = pat << sft;
  ymuint64 mask = 3UL << sft;
  ymuint64 nmask = ~mask;
  SizeType nb = _cube_size();
  SizeType wpos = 0;
  for ( SizeType i = 0; i < nc1; ++ i, bv1 += nb ) {
    if ( (bv1[blk] & mask) == pat1 ) {
      cube_copy(dst_bv, 0, bv1, 0);
      dst_bv[blk] &= nmask;
      ++ wpos;
      dst_bv += nb;
    }
  }

  return wpos;
}

// @brief カバー中のすべてのキューブの共通部分を求める．
void
AlgMgr::common_cube(
  ymuint64* dst_bv,
  SizeType nc1,
  const ymuint64* bv1
)
{
  SizeType nb = _cube_size();

  // 最初のキューブをコピーする．
  cube_copy(dst_bv, 0, bv1, 0);

  // 2番目以降のキューブとの共通部分を求める．
  SizeType offset = nb;
  for ( SizeType cube_pos = 1; cube_pos < nc1; ++ cube_pos ) {
    ymuint64 tmp = 0ULL;
    for ( SizeType i = 0; i < nb; ++ i ) {
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
void
AlgMgr::copy(
  SizeType cube_num,
  ymuint64* dst_bv,
  SizeType dst_pos,
  const ymuint64* src_bv,
  SizeType src_pos
)
{
  SizeType nb = _cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  const ymuint64* src = src_bv + src_pos * nb;
  const ymuint64* src_end = src + cube_num * nb;
  for ( ; src != src_end; ++ src, ++ dst ) {
    *dst = *src;
  }
}

// @brief マージソートを行う下請け関数
void
AlgMgr::_sort(
  ymuint64* bv,
  SizeType start,
  SizeType end
)
{
  SizeType n = end - start;
  if ( n <= 1 ) {
    return;
  }
  if ( n == 2 ) {
    // (0, 1) と (1, 0) の2通りだけ
    SizeType pos0 = start;
    SizeType pos1 = pos0 + 1;
    if ( cube_compare(bv, pos0, bv, pos1) < 0 ) {
      // (1, 0) だったので交換する．
      _resize_buff(1);
      cube_swap(bv, pos0, bv, pos1);
    }
    return;
  }
  if ( n == 3 ) {
    // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
    // の6通りなので虱潰し
    SizeType pos0 = start;
    SizeType pos1 = pos0 + 1;
    SizeType pos2 = pos1 + 1;
    if ( cube_compare(bv, pos0, bv, pos1) < 0 ) {
      // (1, 0, 2), (1, 2, 0), (2, 1, 0)
      if ( cube_compare(bv, pos0, bv, pos2) < 0 ) {
	// (1, 2, 0), (2, 1, 0)
	if ( cube_compare(bv, pos1, bv, pos2) < 0 ) {
	  // (2, 1, 0)
	  // 0 と 2 を交換
	  _resize_buff(1);
	  cube_swap(bv, pos0, bv, pos2);
	}
	else {
	  // (1, 2, 0)
	  // 0 <- 1, 1 <- 2, 2 <- 0
	  _resize_buff(1);
	  cube_rotate3(bv, pos0, bv, pos1, bv, pos2);
	}
      }
      else {
	// (1, 0, 2)
	// 0 <-> 1
	_resize_buff(0);
	cube_swap(bv, pos0, bv, pos1);
      }
    }
    else {
      // (0, 1, 2), (0, 2, 1), (2, 0, 1)
      if ( cube_compare(bv, pos0, bv, pos2) < 0 ) {
	// (2, 0, 1)
	// 0 <- 2, 2 <- 1, 1 <- 0
	_resize_buff(1);
	cube_rotate3(bv, pos0, bv, pos2, bv, pos1);
      }
      else {
	// (0, 1, 2), (0, 2, 1)
	if ( cube_compare(bv, pos1, bv, pos2) < 0 ) {
	  // (0, 2, 1)
	  // 1 <-> 2
	  _resize_buff(1);
	  cube_swap(bv, pos1, bv, pos2);
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
    SizeType pos0 = start;
    SizeType pos1 = pos0 + 1;
    SizeType pos2 = pos1 + 1;
    SizeType pos3 = pos2 + 1;
    _resize_buff(1);
    // 0 と 1 を整列
    if ( cube_compare(bv, pos0, bv, pos1) < 0 ) {
      cube_swap(bv, pos0, bv, pos1);
    }
    // 2 と 3 を整列
    if ( cube_compare(bv, pos2, bv, pos3) < 0 ) {
      cube_swap(bv, pos2, bv, pos3);
    }
    // 0 と 2 を比較
    if ( cube_compare(bv, pos0, bv, pos2) < 0 ) {
      if ( cube_compare(bv, pos0, bv, pos3) < 0 ) {
	// (0, 1) と (2, 3) を交換
	cube_swap(bv, pos0, bv, pos2);
	cube_swap(bv, pos1, bv, pos3);
      }
      else if ( cube_compare(bv, pos1, bv, pos3) < 0 ) {
	// 0 <- 2, 2 <- 3, 3 <- 1, 1 <- 0
	cube_rotate4(bv, pos0, bv, pos2, bv, pos3, bv, pos1);
      }
      else {
	// 0 <- 2, 2 <- 1, 1 <- 0
	cube_rotate3(bv, pos0, bv, pos2, bv, pos1);
      }
    }
    else if ( cube_compare(bv, pos1, bv, pos2) < 0 ) {
      if ( cube_compare(bv, pos1, bv, pos3) < 0 ) {
	// 1 <- 2, 2 <- 3, 3 <- 1
	cube_rotate3(bv, pos1, bv, pos2, bv, pos3);
      }
      else {
	// 1 <- 2, 2 <- 1
	cube_swap(bv, pos1, bv, pos2);
      }
    }
    else {
      // そのまま
    }
    return;
  }

  // 半分に分割してそれぞれソートする．
  SizeType hn = (n + 1) / 2;
  SizeType start1 = start;
  SizeType end1 = start + hn;
  SizeType start2 = end1;
  SizeType end2 = end;
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
  SizeType rpos1 = 0;
  SizeType rpos2 = start2;
  SizeType wpos = start1;
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
int
AlgMgr::compare(
  SizeType nc1,
  const ymuint64* bv1,
  SizeType nc2,
  const ymuint64* bv2)
{
  SizeType rpos1 = 0;
  SizeType rpos2 = 0;
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

// @brief ビットベクタからハッシュ値を計算する．
SizeType
AlgMgr::hash(
  SizeType nc,
  const ymuint64* bv
)
{
  // キューブは常にソートされているので
  // 順番は考慮する必要はない．
  // ただおなじキューブの中のビットに関しては
  // 本当は区別しなければならないがどうしようもないので
  // 16ビットに区切って単純に XOR を取る．
  SizeType n = nc * _cube_size();
  const ymuint64* bv_end = bv + n;
  SizeType ans = 0;
  for ( ; bv != bv_end; ++ bv ) {
    ymuint64 pat = *bv;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= pat;
  }
  return ans;
}

// @brief キューブ(を表すビットベクタ)の比較を行う．
int
AlgMgr::cube_compare(
  const ymuint64* bv1,
  SizeType pos1,
  const ymuint64* bv2,
  SizeType pos2
)
{
  SizeType nb = _cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  const ymuint64* _bv1_end = _bv1 + nb;
  for ( ; _bv1 != _bv1_end; ++ _bv1, ++ _bv2 ) {
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
bool
AlgMgr::cube_check_product(
  const ymuint64* bv1,
  SizeType pos1,
  const ymuint64* bv2,
  SizeType pos2
)
{
  SizeType nb = _cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  const ymuint64* _bv1_end = _bv1 + nb;
  for ( ; _bv1 != _bv1_end; ++ _bv1, ++ _bv2 ) {
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
bool
AlgMgr::cube_check_containment(
  const ymuint64* bv1,
  SizeType pos1,
  const ymuint64* bv2,
  SizeType pos2
)
{
  SizeType nb = _cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  const ymuint64* _bv1_end = _bv1 + nb;
  for ( ; _bv1 != _bv1_end; ++ _bv1, ++ _bv2 ) {
    if ( (~(*_bv1) & *_bv2) != 0ULL ) {
      return false;
    }
  }
  return true;
}

// @brief ２つのキューブに共通なリテラルがあれば true を返す．
bool
AlgMgr::cube_check_intersect(
  const ymuint64* bv1,
  SizeType pos1,
  const ymuint64* bv2,
  SizeType pos2
)
{
  SizeType nb = _cube_size();
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  const ymuint64* _bv1_end = _bv1 + nb;
  for ( ; _bv1 != _bv1_end; ++ _bv1, ++ _bv2 ) {
    if ( (*_bv1 & *_bv2) != 0ULL ) {
      return true;
    }
  }
  return false;
}

// @brief キューブ(を表すビットベクタ)をクリアする．
void
AlgMgr::cube_clear(
  ymuint64* dst_bv,
  SizeType dst_pos
)
{
  SizeType nb = _cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  ymuint64* dst_end = dst + nb;
  for ( ; dst != dst_end; ++ dst ) {
    *dst = 0ULL;
  }
}

// @brief 2つのキューブの積を計算する．
bool
AlgMgr::cube_product(
  ymuint64* dst_bv,
  SizeType dst_pos,
  const ymuint64* bv1,
  SizeType pos1,
  const ymuint64* bv2,
  SizeType pos2
)
{
  SizeType nb = _cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  const ymuint64* _bv1_end = _bv1 + nb;
  for ( ; _bv1 != _bv1_end; ++ dst, ++ _bv1, ++ _bv2 ) {
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

// @brief キューブによる商を求める．
bool
AlgMgr::cube_division(
  ymuint64* dst_bv,
  SizeType dst_pos,
  const ymuint64* bv1,
  SizeType pos1,
  const ymuint64* bv2,
  SizeType pos2
)
{
  SizeType nb = _cube_size();
  ymuint64* dst = dst_bv + dst_pos * nb;
  const ymuint64* _bv1 = bv1 + pos1 * nb;
  const ymuint64* _bv2 = bv2 + pos2 * nb;
  const ymuint64* _bv1_end = _bv1 + nb;
  for ( ; _bv1 != _bv1_end; ++ dst, ++ _bv1, ++ _bv2 ) {
    if ( (~(*_bv1) & *_bv2) != 0ULL ) {
      // この場合の dst の値は不定
      return false;
    }
    *dst = *_bv1 & ~(*_bv2);
  }
  return true;
}

// @brief 要素のチェック
bool
AlgMgr::is_in(
  ymuint64* bv,
  AlgLiteral lit
)
{
  SizeType var_id = lit.varid();
  SizeType blk = _block_pos(var_id);
  SizeType sft = _shift_num(var_id);
  ymuint64 pat = lit.is_positive() ? kAlgPolP : kAlgPolN;
  ymuint64 mask = pat << sft;
  if ( bv[blk] & mask ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 要素の追加
void
AlgMgr::add_lit(
  ymuint64* bv,
  AlgLiteral lit
)
{
  SizeType var_id = lit.varid();
  SizeType blk = _block_pos(var_id);
  SizeType sft = _shift_num(var_id);
  ymuint64 pat = lit.is_positive() ? kAlgPolP : kAlgPolN;
  ymuint64 mask = pat << sft;
  bv[blk] |= mask;
}

// @brief カバー/キューブの内容を出力する．
void
AlgMgr::print(
  ostream& s,
  const ymuint64* bv,
  SizeType start,
  SizeType end
)
{
  const char* plus = "";
  for ( SizeType i = start; i < end; ++ i ) {
    s << plus;
    plus = " + ";
    const char* spc = "";
    for ( SizeType j = 0; j < variable_num(); ++ j ) {
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
void
AlgMgr::_resize_buff(
  SizeType req_size
)
{
  SizeType old_size = mTmpBuffSize;
  while ( mTmpBuffSize < req_size ) {
    mTmpBuffSize <<= 1;
  }

  if ( old_size < mTmpBuffSize ) {
    delete_body(mTmpBuff, old_size);
  }
  mTmpBuff = new_body(mTmpBuffSize);
}

END_NAMESPACE_YM_BFO
