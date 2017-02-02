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
///
/// 中身は positional cube matrix 表現を用いたビットベクタ<br>
/// ビットベクタの内容は一旦，設定されたら変わることはない<br>
/// キューブはワードレベルの順序で整列しているものと仮定している<br>
/// 実際の操作は BfoMgr が行う．
//////////////////////////////////////////////////////////////////////
class BfoCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] cube_list キューブのリスト
  explicit
  BfoCover(BfoMgr& mgr,
	   const vector<BfoCube>& cube_list = vector<BfoCube>());

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

  /// @brief 論理和を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator+(const BfoCover& right) const;

  /// @brief 論理和を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator+(const BfoCube& right) const;

  /// @brief 差分を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  BfoCover
  operator-(const BfoCover& right) const;

  /// @brief 差分を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  BfoCover
  operator-(const BfoCube& right) const;

  /// @brief 論理積を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator*(const BfoCover& right) const;

  /// @brief 論理積を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  BfoCover
  operator*(const BfoCube& right) const;

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

  /// @brief 論理和を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator+=(const BfoCover& right);

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator+=(const BfoCube& right);

  /// @brief 差分を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator-=(const BfoCover& right);

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator-=(const BfoCube& right);

  /// @brief 論理積を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator*=(const BfoCover& right);

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const BfoCover&
  operator*=(const BfoCube& right);

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

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] cube_num キューブ数
  /// @param[in] cube_cap キューブ容量
  /// @param[in] body 内容のパタンを表す本体
  ///
  /// この関数は危険なので普通は使わないこと
  BfoCover(BfoMgr& mgr,
	   ymuint cube_num,
	   ymuint cube_cap,
	   ymuint64* body);

  /// @brief キューブ容量を変更する．
  /// @param[in] req_cap 要求するキューブ容量
  ///
  /// 現在のキューブ容量が大きかれば変更しない．
  void
  resize(ymuint req_cap);

  /// @brief キューブ容量を計算する．
  static
  ymuint
  get_capacity(ymuint cube_num);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブマネージャ
  BfoMgr* mMgr;

  // キューブ数
  ymuint mCubeNum;

  // mBody の実際に確保されているキューブ容量
  ymuint mCubeCap;

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

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] cube_list キューブのリスト
inline
BfoCover::BfoCover(BfoMgr& mgr,
		   const vector<BfoCube>& cube_list) :
  mMgr(&mgr),
  mCubeNum(cube_list.size()),
  mCubeCap(0)
{
  resize(mCubeNum);
  for (ymuint i = 0; i < mCubeNum; ++ i) {
    const BfoCube& cube = cube_list[i];
    mMgr->cube_copy(mBody, i, cube.mBody, 0);
  }
  mMgr->sort(mCubeNum, mBody);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
BfoCover::BfoCover(const BfoCover& src) :
  mMgr(src.mMgr),
  mCubeNum(src.mCubeNum),
  mCubeCap(0)
{
  resize(mCubeNum);
  mMgr->cover_copy(mCubeNum, mBody, 0, src.mBody, 0);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身の参照を返す．
inline
const BfoCover&
BfoCover::operator=(const BfoCover& src)
{
  if ( &src != this ) {
    ymuint old_cap = mCubeCap;
    ymuint64* old_body = mBody;
    mCubeNum = src.mCubeNum;
    resize(mCubeNum);
    if ( old_body != mBody ) {
      mgr().delete_body(mBody, old_cap);
    }
    mMgr->cover_copy(mCubeNum, mBody, 0, src.mBody, 0);
  }

  return *this;
}

// @brief キューブからの変換コンストラクタ
// @param[in] cube 対象のキューブ
//
// 指定されたキューブのみのカバーとなる．
inline
BfoCover::BfoCover(const BfoCube& cube) :
  mMgr(&cube.mgr()),
  mCubeNum(1),
  mCubeCap(0)
{
  resize(mCubeNum);
  mMgr->cube_copy(mBody, 0, cube.mBody, 0);
}

// @brief デストラクタ
//
// ここに属しているすべてのキューブは削除される．
inline
BfoCover::~BfoCover()
{
  mMgr->delete_body(mBody, mCubeCap);
}

// @brief 内容を指定したコンストラクタ
// @param[in] mgr マネージャ
// @param[in] cube_num キューブ数
// @param[in] cube_cap キューブ容量
// @param[in] body 内容のパタンを表す本体
//
// この関数は危険なので普通は使わないこと
inline
BfoCover::BfoCover(BfoMgr& mgr,
		   ymuint cube_num,
		   ymuint cube_cap,
		   ymuint64* body) :
  mMgr(&mgr),
  mCubeNum(cube_num),
  mCubeCap(cube_cap),
  mBody(body)
{
}

// @brief マネージャを返す．
inline
BfoMgr&
BfoCover::mgr() const
{
  return *mMgr;
}

// @brief 変数の数を返す．
inline
ymuint
BfoCover::variable_num() const
{
  return mMgr->variable_num();
}

// @brief キューブの数を返す．
inline
ymuint
BfoCover::cube_num() const
{
  return mCubeNum;
}

// @brief リテラル数を返す．
inline
ymuint
BfoCover::literal_num() const
{
  return mgr().literal_num(cube_num(), mBody);
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
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1 + nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().sum(body, nc1, mBody, nc2, right.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief 論理和を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator+(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint cap = get_capacity(nc1 + nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().sum(body, nc1, mBody, nc2, right.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief 論理和を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator+=(const BfoCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint old_cap = mCubeCap;
  ymuint64* old_body = mBody;
  resize(nc1 + nc2);
  mCubeNum = mgr().sum(mBody, nc1, old_body, nc2, right.mBody);
  if ( old_body != mBody ) {
    mgr().delete_body(old_body, old_cap);
  }

  return *this;
}

// @brief 論理和を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator+=(const BfoCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint old_cap = mCubeCap;
  ymuint64* old_body = mBody;
  resize(nc1 + nc2);
  mCubeNum = mgr().sum(mBody, nc1, old_body, nc2, right.mBody);
  if ( old_body != mBody ) {
    mgr().delete_body(old_body, old_cap);
  }

  return *this;
}

// @brief 差分を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator-(const BfoCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().diff(body, nc1, mBody, nc2, right.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief 差分を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator-(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().diff(body, nc1, mBody, nc2, right.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief 差分を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator-=(const BfoCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  // 結果のキューブ数は減るだけなのでキューブ容量の変更はしない．
  mCubeNum = mgr().diff(mBody, nc1, mBody, nc2, right.mBody);

  return *this;
}

// @brief 差分を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator-=(const BfoCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  // 結果のキューブ数は減るだけなのでキューブ容量の変更はしない．
  mCubeNum = mgr().diff(mBody, nc1, mBody, nc2, right.mBody);

  return *this;
}

// @brief 論理積を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator*(const BfoCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1 * nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().product(body, nc1, mBody, nc2, right.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator*(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint cap = get_capacity(nc1 * nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().product(body, nc1, mBody, nc2, right.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator*=(const BfoCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint old_cap = mCubeCap;
  ymuint64* old_body = mBody;
  resize(nc1 * nc2);
  mCubeNum = mgr().product(mBody, nc1, old_body, nc2, right.mBody);
  if ( old_body != mBody ) {
    mgr().delete_body(old_body, old_cap);
  }

  return *this;
}

// @brief 論理積を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator*=(const BfoCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint old_cap = mCubeCap;
  ymuint64* old_body = mBody;
  resize(nc1 * nc2);
  mCubeNum = mgr().product(mBody, nc1, old_body, nc2, right.mBody);
  if ( old_body != mBody ) {
    mgr().delete_body(old_body, old_cap);
  }

  return *this;
}

// @brief algebraic division を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator/(const BfoCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1 / nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().division(body, nc1, mBody, nc2, right.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief algebraic division を行って代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator/=(const BfoCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, nc1, mBody, nc2, right.mBody);

  return *this;
}

// @brief キューブのコファクターを計算する．
// @param[in] cube 対象のキューブ
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator/(const BfoCube& cube) const
{
  ASSERT_COND( variable_num() == cube.variable_num() );

  ymuint nc1 = cube_num();
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().division(body, nc1, mBody, 1, cube.mBody);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief キューブのコファクターを計算して代入する．
// @param[in] cube 対象のキューブ
// @return 演算後の自身への参照を返す．
inline
BfoCover
BfoCover::operator/=(const BfoCube& cube)
{
  ASSERT_COND( variable_num() == cube.variable_num() );

  ymuint nc1 = cube_num();
  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, nc1, mBody, 1, cube.mBody);

  return *this;
}

// @brief リテラルのコファクターを計算する．
// @param[in] lit 対象のリテラル
// @return 計算結果を返す．
inline
BfoCover
BfoCover::operator/(BfoLiteral lit) const
{
  ymuint nc1 = cube_num();
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().division(body, nc1, mBody, lit);

  return BfoCover(mgr(), nc, cap, body);
}

// @brief リテラルのコファクターを計算して代入する．
// @param[in] lit 対象のリテラル
// @return 演算後の自身への参照を返す．
inline
const BfoCover&
BfoCover::operator/=(BfoLiteral lit)
{
  ymuint nc1 = cube_num();
  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, nc1, mBody, lit);

  return *this;
}

// @brief 共通なキューブを返す．
//
// 共通なキューブがない場合には空のキューブを返す．
inline
BfoCube
BfoCover::common_cube() const
{
  ymuint64* body = mgr().new_body();
  mgr().common_cube(body, cube_num(), mBody);

  return BfoCube(mgr(), body);
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
inline
void
BfoCover::print(ostream& s) const
{
  mgr().print(s, cube_num(), mBody);
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

// @brief キューブ容量を変更する．
// @param[in] req_cap 要求するキューブ容量
//
// 現在のキューブ容量が大きかれば変更しない．
inline
void
BfoCover::resize(ymuint req_cap)
{
  ymuint new_cap = get_capacity(req_cap);
  if ( new_cap > mCubeCap ) {
    mCubeCap = new_cap;
    mBody = mgr().new_body(mCubeCap);
  }
}

// @brief キューブ容量を計算する．
inline
ymuint
BfoCover::get_capacity(ymuint cube_num)
{
  // 初期値を16としてcube_numを下回らない
  // ２のべき乗の数を求める．
  ymuint ans = 16;
  while ( ans < cube_num ) {
    ans *= 2;
  }
  return ans;
}

END_NAMESPACE_YM_BFO

#endif // BFOCOVER_H
