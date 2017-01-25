#ifndef BFOCOVER_H
#define BFOCOVER_H

/// @file BfoCover.h
/// @brief BfoCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/BfoCube.h"
#include "ym/BfoLiteral.h"
#include "ym/BfoMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class BfoCover BfoCover.h "BfoCover.h"
/// @brief カバー(積和形論理式)を表すクラス
//////////////////////////////////////////////////////////////////////
class BfoCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr ネージャ
  /// @param[in] cube_list キューブを表すリテラルのリストのリスト
  explicit
  BfoCover(BfoMgr& mgr,
	   const vector<vector<BfoLiteral> >& cube_list = vector<vector<BfoLiteral> >());

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BfoCover(const BfoCover& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身の参照を返す．
  const BfoCover&
  operator=(const BfoCover& src);

  /// @brief キューブからの変換コンストラクタ
  /// @param[in] cube 対象のキューブ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  BfoCover(const BfoCube& cube);

  /// @brief 内容をしていたコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] cube_num キューブ数
  /// @param[in] body 内容のパタンを表す本体
  ///
  /// この関数は危険ならの普通は使わないこと
  BfoCover(BfoMgr& mgr,
	   ymuint cube_num,
	   ymuint64* body);

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~BfoCover();


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

  /// @brief キューブの数を返す．
  ymuint
  cube_num() const;

  /// @brief リテラル数を返す．
  ymuint
  literal_num() const;

  /// @brief 指定されたリテラルの出現回数を返す．
  /// @param[in] lit 対象のリテラル
  ymuint
  literal_num(BfoLiteral lit) const;

  /// @brief 内容を返す．
  /// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
  /// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
  BfoPol
  literal(ymuint cube_id,
	  ymuint var_id) const;

#if 0
  /// @brief 論理和を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator+=(const BfoCover& right);

  /// @brief 差分を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator-=(const BfoCover& right);

  /// @brief 論理積を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator*=(const BfoCover& right);

  /// @brief algebraic division を行って代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator/=(const BfoCover& right);

  /// @brief キューブのコファクターを計算して代入する．
  /// @param[in] cube 対象のキューブ
  /// @return 演算後の自身への参照を返す．
  BfoCover
  operator/=(const BfoCube& cube);

  /// @brief リテラルのコファクターを計算して代入する．
  /// @param[in] lit 対象のリテラル
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator/=(BfoLiteral lit);
#endif

  /// @brief 論理和を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator+(const BfoCover& right) const;

  /// @brief 差分を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator-(const BfoCover& right) const;

  /// @brief 論理積を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator*(const BfoCover& right) const;

  /// @brief algebraic division を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator/(const BfoCover& right) const;

  /// @brief キューブのコファクターを計算する．
  /// @param[in] cube 対象のキューブ
  /// @return 計算結果を返す．
  BfoCover
  operator/(const BfoCube& cube) const;

  /// @brief リテラルのコファクターを計算する．
  /// @param[in] lit 対象のリテラル
  /// @return 計算結果を返す．
  BfoCover
  operator/(BfoLiteral lit) const;

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  BfoCube
  common_cube() const;

  /// @brief 内容をわかりやすい形で出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cube をソートする．
  void
  _sort_cubes();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブマネージャ
  BfoMgr& mMgr;

  // キューブ数
  ymuint mCubeNum;

  // 内容を表すビットベクタ
  ymuint64* mBody;

};

/// @relates BfoCover
/// @brief BfoCover の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cover 対象のカバー
///
/// cover.print(s) と等価
ostream&
operator<<(ostream& s,
	   const BfoCover& cover);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 内容をしていたコンストラクタ
// @param[in] mgr マネージャ
// @param[in] cube_num キューブ数
// @param[in] body 内容のパタンを表す本体
//
// この関数は危険ならの普通は使わないこと
inline
BfoCover::BfoCover(BfoMgr& mgr,
		   ymuint cube_num,
		   ymuint64* body) :
  mMgr(mgr),
  mCubeNum(cube_num),
  mBody(body)
{
}

// @brief マネージャを返す．
inline
BfoMgr&
BfoCover::mgr() const
{
  return mMgr;
}

// @brief 変数の数を返す．
inline
ymuint
BfoCover::variable_num() const
{
  return mMgr.variable_num();
}

// @brief キューブの数を返す．
inline
ymuint
BfoCover::cube_num() const
{
  return mCubeNum;
}

// @brief 内容を返す．
// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
inline
BfoPol
BfoCover::literal(ymuint cube_id,
		  ymuint var_id) const
{
  ASSERT_COND( cube_id < cube_num() );

  return mgr().literal(mBody, cube_id, var_id);
}

// @brief 論理和を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator+(const BfoCover& right) const
{
  return mgr().make_sum(cube_num(), mBody, right.cube_num(), right.mBody);
}

// @brief 差分を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator-(const BfoCover& right) const
{
  return mgr().make_diff(cube_num(), mBody, right.cube_num(), right.mBody);
}

// @brief 論理積を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator*(const BfoCover& right) const
{
  return mgr().make_product(cube_num(), mBody, right.cube_num(), right.mBody);
}

// @brief algebraic division を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator/(const BfoCover& right) const
{
  return mgr().make_division(cube_num(), mBody, right.cube_num(), right.mBody);
}

// @brief キューブのコファクターを計算する．
// @param[in] cube 対象のキューブ
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator/(const BfoCube& cube) const
{
  return mgr().make_division(cube_num(), mBody, cube.mBody);
}

// @brief リテラルのコファクターを計算する．
// @param[in] lit 対象のリテラル
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator/(BfoLiteral lit) const
{
  return mgr().make_division(cube_num(), mBody, lit);
}

// @brief 共通なキューブを返す．
//
// 共通なキューブがない場合には空のキューブを返す．
inline
BfoCube
BfoCover::common_cube() const
{
  return mgr().common_cube(cube_num(), mBody);
}

// @brief BfoCover の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cover 対象のカバー
//
// cover.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const BfoCover& cover)
{
  cover.print(s);
  return s;
}

END_NAMESPACE_YM_BFO

#endif // BFOCOVER_H
