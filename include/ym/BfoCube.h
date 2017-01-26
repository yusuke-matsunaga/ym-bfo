#ifndef BFOCUBE_H
#define BFOCUBE_H

/// @file BfoCube.h
/// @brief BfoCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/BfoMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class BfoCube BfoCube.h "BfoCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// 常に固定サイズのビット配列として実装する．<br>
/// 1つの変数につき2ビットを使用する．<br>
//////////////////////////////////////////////////////////////////////
class BfoCube
{
  friend class BfoCover;

public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] lit_list キューブを表すリテラルのリスト
  explicit
  BfoCube(BfoMgr& mgr,
	  const vector<BfoLiteral>& lit_list = vector<BfoLiteral>());

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BfoCube(const BfoCube& src);

  /// @brief 内容を指定するコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] body キューブのパタンを表す本体
  ///
  /// 危険なので普通は使わないように
  BfoCube(BfoMgr& mgr,
	  ymuint64* body);

  /// @brief デストラクタ
  ~BfoCube();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  BfoMgr&
  mgr() const;

  /// @brief 変数の数を返す．
  ymuint
  variable_num() const;

  /// @brief リテラル数を返す．
  ymuint
  literal_num() const;

  /// @brief 内容を読み出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
  BfoPol
  literal(ymuint pos) const;

  /// @brief オペランドのキューブに含まれていたら true を返す．
  /// @param[in] right オペランドのキューブ
  ///
  /// ここではキューブの表す論理関数の含意を考える<br>
  /// だからリテラル集合としてはオペランドのキューブを含むことになる．
  bool
  check_containment(const BfoCube& right) const;

  /// @brief 2つのキューブに共通なリテラルがあれば true を返す．
  /// @param[in] right オペランドのキューブ
  bool
  operator&&(const BfoCube& right) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// 相反するリテラルとの積があったら答はからのキューブとなる．
  const BfoCube&
  operator&=(const BfoCube& right);

#if 0
  /// @brief 共通部分を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 結果が NULL Cube になったら false を返す．
  bool
  make_intersect(const BfoCube* src);

  /// @brief オペランドに含まれているリテラルを取り除く
  /// @param[in] src オペランドのキューブ
  void
  make_diff(const BfoCube* src);
#endif

  /// @brief 内容をわかりやすい形で出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // friend 関数の宣言
  friend
  int
  compare(const BfoCube& left,
	  const BfoCube& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BfoMgr& mMgr;

  // 内容を表すビットベクタ
  ymuint64* mBody;

};

/// @relates BfoCube
/// @brief BfoCube の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cube 対象のキューブ(のポインタ)
///
/// cube->print(s) と等価
ostream&
operator<<(ostream& s,
	   const BfoCube& cube);

/// @relates BfoCube
/// @brief BfoCube の論理積を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
BfoCube
operator&(const BfoCube& left,
	  const BfoCube& right);

/// @relates BfoCube
/// @brief BfoCubeの比較演算子
/// @param[in] left, right オペランド
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
int
compare(const BfoCube& left,
	const BfoCube& right);

/// @relates BfoCube
/// @brief BfoCubeの比較演算子(EQ)
/// @param[in] left, right オペランド
/// @retval true  left == right
/// @retval false left != right
bool
operator==(const BfoCube& left,
	   const BfoCube& right);

/// @relates BfoCube
/// @brief BfoCubeの比較演算子(NE)
/// @param[in] left, right オペランド
/// @retval true  left != right
/// @retval false left == right
bool
operator!=(const BfoCube& left,
	   const BfoCube& right);

/// @relates BfoCube
/// @brief BfoCubeの比較演算子(LT)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<(const BfoCube& left,
	  const BfoCube& right);

/// @relates BfoCube
/// @brief BfoCubeの比較演算子(GT)
/// @param[in] left, right オペランド
/// @retval true  left > right
/// @retval false left <= right
bool
operator>(const BfoCube& left,
	  const BfoCube& right);

/// @relates BfoCube
/// @brief BfoCubeの比較演算子(LE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<=(const BfoCube& left,
	   const BfoCube& right);

/// @relates BfoCube
/// @brief BfoCubeの比較演算子(GE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator>=(const BfoCube& left,
	   const BfoCube& right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief マネージャを返す．
inline
BfoMgr&
BfoCube::mgr() const
{
  return mMgr;
}

// @brief 変数の数を返す．
inline
ymuint
BfoCube::variable_num() const
{
  return mgr().variable_num();
}

// @brief 内容を読み出す．
// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
inline
BfoPol
BfoCube::literal(ymuint pos) const
{
  return mgr().literal(mBody, 0, pos);
}

// @brief リテラル数を返す．
inline
ymuint
BfoCube::literal_num() const
{
  return mgr().literal_num(1, mBody);
}

// @brief オペランドのキューブに含まれていたら true を返す．
// @param[in] src オペランドのキューブ
//
// ここではキューブの表す論理関数の含意を考える<br>
// だからリテラル集合としては真逆になる．
inline
bool
BfoCube::check_containment(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );
  return mgr().check_containment(mBody, 0, right.mBody, 0);
}

// @brief BfoCube の論理積を計算する
// @param[in] left, right オペランド
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
BfoCube
operator&(const BfoCube& left,
	  const BfoCube& right)
{
  return BfoCube(left).operator&=(right);
}

// @relates BfoCube
// @brief BfoCubeの比較演算子
// @param[in] left, right オペランド
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
inline
int
compare(const BfoCube& left,
	const BfoCube& right)
{
  ASSERT_COND( left.variable_num() == right.variable_num() );

  return left.mgr().compare(left.mBody, 0, right.mBody, 0);
}

// @relates BfoCube
// @brief BfoCubeの比較演算子(EQ)
// @param[in] left, right オペランド
// @retval true  left == right
// @retval false left != right
inline
bool
operator==(const BfoCube& left,
	   const BfoCube& right)
{
  return compare(left, right) == 0;
}

// @relates BfoCube
// @brief BfoCubeの比較演算子(NE)
// @param[in] left, right オペランド
// @retval true  left != right
// @retval false left == right
inline
bool
operator!=(const BfoCube& left,
	   const BfoCube& right)
{
  return compare(left, right) != 0;
}

// @brief BfoCubeの比較演算子(LT)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<(const BfoCube& left,
	  const BfoCube& right)
{
  return compare(left, right) < 0;
}

// @brief BfoCubeの比較演算子(GT)
// @param[in] left, right オペランド
// @retval true  left > right
// @retval false left <= right
inline
bool
operator>(const BfoCube& left,
	  const BfoCube& right)
{
  return compare(left, right) > 0;
}

// @brief BfoCubeの比較演算子(LE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<=(const BfoCube& left,
	   const BfoCube& right)
{
  return compare(left, right) <= 0;
}

// @brief BfoCubeの比較演算子(GE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator>=(const BfoCube& left,
	   const BfoCube& right)
{
  return compare(left, right) >= 0;
}

// @brief BfoCube の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cube 対象のキューブ
//
// cube.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const BfoCube& cube)
{
  cube.print(s);
  return s;
}

END_NAMESPACE_YM_BFO

#endif // BFOCUBE_H
