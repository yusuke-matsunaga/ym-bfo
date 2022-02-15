#ifndef YM_BFOLITERAL_H
#define YM_BFOLITERAL_H

/// @file ym/AlgLiteral.h
/// @brief AlgLiteral のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bfo_nsdef.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @ingroup AlgGroup
/// @class AlgLiteral AlgLiteral.h "ym/AlgLiteral.h"
/// @brief リテラル(変数番号＋極性)を表すクラス
///
/// ただの int だが整数型の変数と暗黙の型変換ができないようにクラスとして
/// 定義している．
/// だから int を引数に持つコンストラクタの explicit 宣言は重要
//////////////////////////////////////////////////////////////////////
class AlgLiteral
{
public:

  /// @brief デフォルトコンストラクタ
  ///
  /// AlgLiteralUndef に初期化される．
  AlgLiteral(
  ) : mBody(0xfffffffe)
  {
    // というかこれが ALgLiteralUndef の定義
  }

  /// @brief 変数番号と極性を指定したコンストラクタ
  AlgLiteral(
    int varid,  ///< [in] 変数番号
    bool inv    ///< [in] 極性
                ///<  - false: 反転なし (正極性)
                ///<  - true:  反転あり (負極性)
  )
  {
    set(varid, inv);
  }

  /// @brief index からの変換関数
  static
  AlgLiteral
  index2literal(
    ymuint32 index ///< [in] 変数番号を極性をエンコードしたもの
  )
  {
    return AlgLiteral(index);
  }

  // コピーコンストラクタ,代入演算子,デストラクタはデフォルト
  // のものがそのまま使える．


public:
  //////////////////////////////////////////////////////////////////////
  // 内容をセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    int varid,       ///< [in] 変数番号
    bool inv = false ///< [in] 極性
                     ///<  - false: 反転なし (正極性)
                     ///<  - true:  反転あり (負極性)
  )
  {
    mBody = (varid << 1) + static_cast<ymuint32>(inv);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号を得る．
  /// @return 変数番号
  int
  varid() const
  {
    return static_cast<int>(mBody >> 1);
  }

  /// @brief 正極性のリテラルの時 true を返す．
  bool
  is_positive() const
  {
    return !is_negative();
  }

  /// @brief 負極性のリテラルの時 true を返す．
  bool
  is_negative() const
  {
    return static_cast<bool>(mBody & 1U);
  }

  /// @brief 極性の反転
  /// @return 極性を反転させたリテラルを返す．
  AlgLiteral
  operator~() const
  {
    return AlgLiteral(mBody ^ 1U);
  }

  /// @brief 同じ変数の正極性リテラルを返す．
  AlgLiteral
  make_positive() const
  {
    return AlgLiteral(mBody & ~1U);
  }

  /// @brief 同じ変数の負極性リテラルを返す．
  AlgLiteral
  make_negative() const
  {
    return AlgLiteral(mBody | 1U);
  }

  /// @brief ハッシュ用の関数
  SizeType
  hash() const
  {
    return static_cast<SizeType>(mBody);
  }

  /// @brief 配列のインデックスとして使用可能な数を返す．
  ymuint32
  index() const
  {
    return mBody;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部でのみ用いるコンストラクタ
  explicit
  AlgLiteral(
    ymuint32 body
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号と極性をパックしたもの
  ymuint32 mBody;

};

/// @relates AlgLiteral
/// @brief 未定義リテラル
extern
const AlgLiteral AlgLiteralUndef;

/// @relates AlgLiteral
/// @brief 比較関数
/// @retval -1 lit1 < lit2
/// @retval  0 lit1 = lit2
/// @retval  1 lit1 > lit2
inline
int
compare(
  AlgLiteral lit1, ///< [in] 第1オペランド
  AlgLiteral lit2  ///< [in] 第2オペランド
)
{
  if ( lit1.index() < lit2.index() ) {
    return -1;
  }
  if ( lit1.index() > lit2.index() ) {
    return 1;
  }
  return 0;
}

/// @relates AlgLiteral
/// @brief 等価比較
/// @return lit1 と lit2 が等しいリテラルの時 true を返す．
inline
bool
operator==(
  AlgLiteral lit1, ///< [in] 第1オペランド
  AlgLiteral lit2  ///< [in] 第2オペランド
)
{
  return compare(lit1, lit2) == 0;
}

/// @relates AlgLiteral
/// @brief 非等価比較
/// @return lit1 と lit2 が等しいリテラルでないとき true を返す．
inline
bool
operator!=(
  AlgLiteral lit1, ///< [in] 第1オペランド
  AlgLiteral lit2  ///< [in] 第2オペランド
)
{
  return !operator==(lit1, lit2);
}

/// @relates AlgLiteral
/// @brief 小なり比較
/// @return lit1 が lit2 より小さいとき true を返す．
inline
bool
operator<(
  AlgLiteral lit1, ///< [in] 第1オペランド
  AlgLiteral lit2  ///< [in] 第2オペランド
)
{
  return compare(lit1, lit2) == -1;
}

/// @relates AlgLiteral
/// @brief 大なり比較
/// @return lit1 が lit2 より大きいとき true を返す．
inline
bool
operator>(
  AlgLiteral lit1, ///< [in] 第1オペランド
  AlgLiteral lit2  ///< [in] 第2オペランド
)
{
  return operator<(lit2, lit1);
}

/// @relates AlgLiteral
/// @brief 小なりイコール比較
/// @return lit1 が lit2 より小さいか等しいとき true を返す．
inline
bool
operator<=(
  AlgLiteral lit1, ///< [in] 第1オペランド
  AlgLiteral lit2  ///< [in] 第2オペランド
)
{
  return !operator<(lit2, lit1);
}

/// @relates AlgLiteral
/// @brief 大なりイコール比較
/// @return lit1 が lit2 より大きいか等しいとき true を返す．
inline
bool
operator>=(
  AlgLiteral lit1, ///< [in] 第1オペランド
  AlgLiteral lit2  ///< [in] 第2オペランド
)
{
  return !operator<(lit1, lit2);
}

/// @relates AlgLiteral
/// @brief AlgLiteral の内容を ostream に出力する関数
/// @return ストリームを返す．
inline
ostream&
operator<<(
  ostream& s,           ///< [in] 出力ストリーム
  const AlgLiteral& lit ///< [in] 出力対象のリテラル
)
{
  if ( lit == AlgLiteralUndef ) {
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

BEGIN_NAMESPACE_STD

// AlgLiteral をキーにしたハッシュ関数クラス
template <>
struct hash<YM_NAMESPACE::AlgLiteral>
{
  SizeType
  operator()(
    YM_NAMESPACE::AlgLiteral lit
  ) const
  {
    return lit.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_BFOLITERAL_H
