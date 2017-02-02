#ifndef YM_BFOLITERAL_H
#define YM_BFOLITERAL_H

/// @file ym/BfoLiteral.h
/// @brief BfoLiteral のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/HashFunc.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @ingroup BfoGroup
/// @class BfoLiteral BfoLiteral.h "ym/BfoLiteral.h"
/// @brief リテラル(変数番号＋極性)を表すクラス
///
/// ただの int だが整数型の変数と暗黙の型変換ができないようにクラスとして
/// 定義している．
/// だから ymuint を引数に持つコンストラクタの explicit 宣言は重要
//////////////////////////////////////////////////////////////////////
class BfoLiteral
{
public:

  /// @brief デフォルトコンストラクタ
  /// 内容は不定なのであまり使わない方が良い．
  BfoLiteral();

  /// @brief 変数番号と極性を指定したコンストラクタ
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  BfoLiteral(ymuint varid,
	     bool inv);

  /// @brief index からの変換関数
  /// @param[in] index 変数番号を極性をエンコードしたもの
  static
  BfoLiteral
  index2literal(ymuint index);

  // コピーコンストラクタ,代入演算子,デストラクタはデフォルト
  // のものがそのまま使える．


public:
  //////////////////////////////////////////////////////////////////////
  // 内容をセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set(ymuint varid,
      bool inv = false);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号を得る．
  /// @return 変数番号
  ymuint
  varid() const;

  /// @brief 正極性のリテラルの時 true を返す．
  bool
  is_positive() const;

  /// @brief 負極性のリテラルの時 true を返す．
  bool
  is_negative() const;

  /// @brief 極性の反転
  /// @return 極性を反転させたリテラルを返す．
  BfoLiteral
  operator~() const;

  /// @brief 同じ変数の正極性リテラルを返す．
  BfoLiteral
  make_positive() const;

  /// @brief 同じ変数の負極性リテラルを返す．
  BfoLiteral
  make_negative() const;

  /// @brief ハッシュ用の関数
  ymuint
  hash() const;

  /// @brief 配列のインデックスとして使用可能な数を返す．
  ymuint
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部でのみ用いるコンストラクタ
  explicit
  BfoLiteral(ymuint body);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号と極性をパックしたもの
  ymuint mBody;

};

/// @relates BfoLiteral
/// @brief 未定義リテラル
extern
const BfoLiteral kBfoLiteralUndef;

/// @relates BfoLiteral
/// @brief 比較関数
/// @param[in] lit1, lit2 比較対象のリテラル
/// @retval -1 lit1 < lit2
/// @retval  0 lit1 = lit2
/// @retval  1 lit1 > lit2
int
compare(BfoLiteral lit1,
	BfoLiteral lit2);

/// @relates BfoLiteral
/// @brief 等価比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 と lit2 が等しいリテラルの時 true を返す．
bool
operator==(BfoLiteral lit1,
	   BfoLiteral lit2);

/// @relates BfoLiteral
/// @brief 非等価比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 と lit2 が等しいリテラルでないとき true を返す．
bool
operator!=(BfoLiteral lit1,
	   BfoLiteral lit2);

/// @relates BfoLiteral
/// @brief 小なり比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より小さいとき true を返す．
bool
operator<(BfoLiteral lit1,
	  BfoLiteral lit2);

/// @relates BfoLiteral
/// @brief 大なり比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より大きいとき true を返す．
bool
operator>(BfoLiteral lit1,
	  BfoLiteral lit2);

/// @relates BfoLiteral
/// @brief 小なりイコール比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より小さいか等しいとき true を返す．
bool
operator<=(BfoLiteral lit1,
	   BfoLiteral lit2);

/// @relates BfoLiteral
/// @brief 大なりイコール比較
/// @param[in] lit1, lit2 比較するリテラル
/// @return lit1 が lit2 より大きいか等しいとき true を返す．
bool
operator>=(BfoLiteral lit1,
	   BfoLiteral lit2);

/// @relates BfoLiteral
/// @brief BfoLiteral の内容を ostream に出力する関数
/// @param[in] s 出力ストリーム
/// @param[in] lit 出力対象のリテラル
/// @return s
ostream&
operator<<(ostream& s,
	   const BfoLiteral& lit);


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// 内容を設定する．
inline
void
BfoLiteral::set(ymuint varid,
		bool inv)
{
  mBody = (varid << 1) + static_cast<ymuint>(inv);
}

// デフォルトコンストラクタ
inline
BfoLiteral::BfoLiteral() :
  mBody(0xfffffffe)
{
}

// 変数番号と極性を指定したコンストラクタ
inline
BfoLiteral::BfoLiteral(ymuint varid,
		       bool inv)
{
  set(varid, inv);
}

// 内部でのみ用いるコンストラクタ
inline
BfoLiteral::BfoLiteral(ymuint body) :
  mBody(body)
{
}

// @brief index からの変換関数
inline
BfoLiteral
BfoLiteral::index2literal(ymuint index)
{
  return BfoLiteral(index);
}

// 変数番号を得る．
inline
ymuint
BfoLiteral::varid() const
{
  return ymuint(mBody >> 1);
}

// @brief 正極性のリテラルの時 true を返す．
inline
bool
BfoLiteral::is_positive() const
{
  return !is_negative();
}

// @brief 負極性のリテラルの時 true を返す．
inline
bool
BfoLiteral::is_negative() const
{
  return static_cast<bool>(mBody & 1U);
}

// 極性を反転させたリテラルを返す．
inline
BfoLiteral
BfoLiteral::operator~() const
{
  return BfoLiteral(mBody ^ 1U);
}

// @brief 同じ変数の正極性リテラルを返す．
inline
BfoLiteral
BfoLiteral::make_positive() const
{
  return BfoLiteral(mBody & ~1U);
}

// @brief 同じ変数の負極性リテラルを返す．
inline
BfoLiteral
BfoLiteral::make_negative() const
{
  return BfoLiteral(mBody | 1U);
}

// @brief 比較関数
// @param[in] lit1, lit2 比較対象のリテラル
// @retval -1 lit1 < lit2
// @retval  0 lit1 = lit2
// @retval  1 lit1 > lit2
inline
int
compare(BfoLiteral lit1,
	BfoLiteral lit2)
{
  if ( lit1.index() < lit2.index() ) {
    return -1;
  }
  if ( lit1.index() > lit2.index() ) {
    return 1;
  }
  return 0;
}

// 等価比較
inline
bool
operator==(BfoLiteral lit1,
	   BfoLiteral lit2)
{
  return compare(lit1, lit2) == 0;
}
inline
bool
operator!=(BfoLiteral lit1,
	   BfoLiteral lit2)
{
  return !operator==(lit1, lit2);
}

// @brief 小なり比較
inline
bool
operator<(BfoLiteral lit1,
	  BfoLiteral lit2)
{
  return compare(lit1, lit2) == -1;
}

// @brief 大なり比較
inline
bool
operator>(BfoLiteral lit1,
	  BfoLiteral lit2)
{
  return operator<(lit2, lit1);
}

// @brief 小なりイコール比較
inline
bool
operator<=(BfoLiteral lit1,
	   BfoLiteral lit2)
{
  return !operator<(lit2, lit1);
}

// @brief 大なりイコール比較
inline
bool
operator>=(BfoLiteral lit1,
	   BfoLiteral lit2)
{
  return !operator<(lit1, lit2);
}

// ハッシュ用の関数
inline
ymuint
BfoLiteral::hash() const
{
  return mBody;
}

// @brief 配列のインデックスとして使用可能な数を返す．
inline
ymuint
BfoLiteral::index() const
{
  return mBody;
}

// @relates BfoLiteral
// @brief BfoLiteral の内容を ostream に出力する関数
// @param[in] s 出力ストリーム
// @param[in] lit 出力対象のリテラル
// @return s
inline
ostream&
operator<<(ostream& s,
	   const BfoLiteral& lit)
{
  if ( lit == kBfoLiteralUndef ) {
    s << "-X-";
  }
  else {
    s << "v_" << lit.varid();
    if ( lit.is_negative() ) {
      s << "'";
    }
  }
  return s;
}

END_NAMESPACE_YM_BFO

BEGIN_NAMESPACE_YM

// BfoLiteral をキーにしたハッシュ関数クラス
template <>
struct HashFunc<nsYm::BfoLiteral>
{
  ymuint
  operator()(nsYm::BfoLiteral lit) const
  {
    return lit.hash();
  }
};

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

template <>
struct equal_to<nsYm::BfoLiteral>
{
  bool
  operator()(nsYm::BfoLiteral n1,
	     nsYm::BfoLiteral n2) const
  {
    return n1 == n2;
  }
};

END_NAMESPACE_STD

#endif // YM_BFOLITERAL_H
