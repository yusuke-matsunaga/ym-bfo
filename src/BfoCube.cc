
/// @file BfoCube.cc
/// @brief BfoCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BfoCube.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoCube
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
BfoCube::BfoCube(ymuint variable_num) :
  mVarNum(variable_num)
{
  ymuint n = _block_num();
  for (ymuint i = 0; i < n; ++ i) {
    // kBfoLitX は 0 なので all-0 は 0ULL
    mBody[i] = 0ULL;
  }
}

/// @brief リテラル数を返す．
ymuint
BfoCube::literal_num() const
{
#warning "TODO: 効率のよい実装に変える．"
  ymuint ans = 0;
  ymuint n = variable_num();
  for (ymuint i = 0; i < n; ++ i) {
    if ( literal(i) != kBfoLitX ) {
      ++ ans;
    }
  }
  return ans;
}

// @brief 内容をコピーする．
// @param[in] src コピー元のキューブ
void
BfoCube::copy(const BfoCube* src)
{
  ASSERT_COND( variable_num() == src->variable_num() );
  ymuint n = _block_num();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] = src->mBody[i];
  }
}

// @brief オペランドのキューブに含まれていたら true を返す．
// @param[in] src オペランドのキューブ
//
// ここではキューブの表す論理関数の含意を考える<br>
// だからリテラル集合としては真逆になる．
bool
BfoCube::check_contain(const BfoCube* src) const
{
  ASSERT_COND( variable_num() == src->variable_num() );
  ymuint n = _block_num();
  for (ymuint i = 0; i < n; ++ i) {
    if ( (~mBody[i] & src->mBody[i]) != 0ULL ) {
      return false;
    }
  }
  return true;
}

// @brief 論理積を計算し自身に代入する．
// @param[in] src オペランドのキューブ
// @return 結果が NULL Cube になったら false を返す．
//
// false を返した場合の内容は不定
bool
BfoCube::make_product(const BfoCube* src)
{
  ASSERT_COND( variable_num() == src->variable_num() );
  ymuint n = _block_num();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] |= src->mBody[i];
    // x・x' のチェックを行う．
    ymuint64 tmp = mBody[i];
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

// @brief オペランドに含まれているリテラルを取り除く
// @param[in] src オペランドのキューブ
void
BfoCube::make_diff(const BfoCube* src)
{
  ASSERT_COND( variable_num() == src->variable_num() );
  ymuint n = _block_num();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] &= ~src->mBody[i];
  }
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
// @param[in] varname_list 変数名のリスト
//
// varname_list が省略された時には適当な名前を作る．<br>
// varname_list のサイズは variable_num() 以上でなければならない．
void
BfoCube::print(ostream& s,
	       const vector<string>& varname_list) const
{
  ASSERT_COND( varname_list.empty() || varname_list.size() >= variable_num() );
  ymuint n = variable_num();
  const char* spc = "";
  for (ymuint i = 0; i < n; ++ i) {
    BfoLiteral lit = literal(i);
    if ( lit == kBfoLitP ) {
      s << spc << varname_list[i];
      spc = " ";
    }
    else if ( lit == kBfoLitN ) {
      s << spc << varname_list[i] << "'";
      spc = " ";
    }
  }
}

// @brief print内で変数名を作る関数
// @param[in] pos 変数番号 ( 0 <= pos < variable_num() )
// @param[in] varname_list 変数名のリスト
//
// varname_list が空の時には適当な名前を作る．
string
BfoCube::_varname(ymuint pos,
		  const vector<string>& varname_list) const
{
  if ( varname_list.empty() ) {
    // variable_num() に応じて変数名の長さが変わる．
    // 26 個以内なら a - z に割り当てる．
    if ( variable_num() <= 26 ) {

  }
  else {
    return varname_list[pos];
  }
}

// @relates BfoCube
// @brief BfoCube(のポインタ)の比較演算子
// @param[in] left, right オペランド
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
int
compare(const BfoCube* left,
	const BfoCube* right)
{
  ASSERT_COND( left->variable_num() == right->variable_num() );
  ymuint nb = left->_block_num();
  for (ymuint i = 0; i < nb; ++ i) {
    ymuint64 p1 = left->mBody[i];
    ymuint64 p2 = right->mBody[i];
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
