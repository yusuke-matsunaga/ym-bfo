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
  ///
  /// lit_list が省略された時には空のキューブを作る．
  explicit
  BfoCube(BfoMgr& mgr,
	  const vector<BfoLiteral>& lit_list = vector<BfoLiteral>());

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BfoCube(const BfoCube& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  const BfoCube&
  operator=(const BfoCube& src);

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
  check_intersect(const BfoCube& right) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  const BfoCube&
  operator&=(const BfoCube& right);

  /// @brief コファクターを計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  const BfoCube&
  operator/=(const BfoCube& right);

  /// @brief 内容をわかりやすい形で出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定するコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] body キューブのパタンを表す本体
  ///
  /// 危険なので普通は使わないように
  BfoCube(BfoMgr& mgr,
	  ymuint64* body);

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
/// @brief BfoCube の論理積を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
BfoCube
operator&(const BfoCube& left,
	  const BfoCube& right);

/// @relates BfoCube
/// @brief コファクターを計算する
/// @param[in] left, right オペランド
BfoCube
operator/(const BfoCube& left,
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

/// @relates BfoCube
/// @brief BfoCube の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cube 対象のキューブ(のポインタ)
///
/// cube->print(s) と等価
ostream&
operator<<(ostream& s,
	   const BfoCube& cube);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ネージャ
// @param[in] lit_list キューブを表すリテラルのリスト
inline
BfoCube::BfoCube(BfoMgr& mgr,
		 const vector<BfoLiteral>& lit_list) :
  mMgr(mgr),
  mBody(mMgr.new_body())
{
  mMgr.cube_set(mBody, 0, lit_list);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
BfoCube::BfoCube(const BfoCube& src) :
  mMgr(src.mMgr)
{
  mBody = mMgr.new_body();
  mMgr.cube_copy(mBody, 0, src.mBody, 0);
}

// @brief 内容を指定するコンストラクタ
// @param[in] mgr マネージャ
// @param[in] body キューブのパタンを表す本体
//
// 危険なので普通は使わないように
inline
BfoCube::BfoCube(BfoMgr& mgr,
		 ymuint64* body) :
  mMgr(mgr),
  mBody(body)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身への参照を返す．
inline
const BfoCube&
BfoCube::operator=(const BfoCube& src)
{
  if ( &src != this ) {
    if ( &mMgr != &src.mMgr ) {
      // マネージャが異なっていたら mBody を作り直す．
      mMgr.delete_body(mBody);
      mMgr = src.mMgr;
      mBody = mMgr.new_body();
    }
    else {
      // マネージャが同じなら mBody は使いまわす．
    }
    mMgr.cube_copy(mBody, 0, src.mBody, 0);
  }

  return *this;
}

// @brief デストラクタ
inline
BfoCube::~BfoCube()
{
  mMgr.delete_body(mBody);
}

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

  return mgr().cube_check_containment(mBody, 0, right.mBody, 0);
}

// @brief 2つのキューブに共通なリテラルがあれば true を返す．
// @param[in] right オペランドのキューブ
inline
bool
BfoCube::check_intersect(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  return mgr().cube_check_intersect(mBody, 0, right.mBody, 0);
}

// @brief 論理積を計算し自身に代入する．
// @param[in] right オペランドのキューブ
// @return 演算後の自身の参照を返す．
//
// 相反するリテラルとの積があったら答は空のキューブとなる．
inline
const BfoCube&
BfoCube::operator&=(const BfoCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  mgr().cube_product(mBody, 0, mBody, 0, right.mBody, 0);

  return *this;
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

// @brief コファクターを計算し自身に代入する．
// @param[in] right オペランドのキューブ
// @return 演算後の自身の参照を返す．
inline
const BfoCube&
BfoCube::operator/=(const BfoCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  mgr().cube_cofactor(mBody, 0, mBody, 0, right.mBody, 0);

  return *this;
}

// @brief コファクターを計算する
// @param[in] left, right オペランド
inline
BfoCube
operator/(const BfoCube& left,
	  const BfoCube& right)
{
  return BfoCube(left).operator/=(right);
}

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

  return left.mgr().cube_compare(left.mBody, 0, right.mBody, 0);
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

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
inline
void
BfoCube::print(ostream& s) const
{
  mgr().print(s, 1, mBody);
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
