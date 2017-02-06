#ifndef YM_ALGCOVER_H
#define YM_ALGCOVER_H

/// @file AlgCover.h
/// @brief AlgCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/AlgCube.h"
#include "ym/AlgLiteral.h"
#include "ym/AlgMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class AlgCover AlgCover.h "ym/AlgCover.h"
/// @brief カバー(積和形論理式)を表すクラス
///
/// 中身は positional cube matrix 表現を用いたビットベクタ<br>
/// ビットベクタの内容は一旦，設定されたら変わることはない<br>
/// キューブはワードレベルの順序で整列しているものと仮定している<br>
/// この式は代数的(algebraic)なので他のキューブに包含されるキューブは
/// 許されない．もちろん重複したキューブも許されない．<br>
/// 通常の操作を行っているかぎり代数的な式しか生成されない．<br>
/// 実際の操作は AlgMgr が行う．
//////////////////////////////////////////////////////////////////////
class AlgCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] cube_list キューブのリスト
  ///
  /// cube_list が空の時は空のカバーとなる．
  explicit
  AlgCover(AlgMgr& mgr,
	   const vector<AlgCube>& cube_list = vector<AlgCube>());

  /// @brief 特殊なコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] dummy ダミーの引数
  ///
  /// 空のキューブを1つ持つカバーとなる．
  /// dummy の値は無視される．
  AlgCover(AlgMgr& mgr,
	   int dummy);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] lit_list カバーを表すリテラルのリスト
  ///
  /// lit_list 中に kAlgLiteralUndef があった場合，キューブの区切り
  /// とみなされる．
  AlgCover(AlgMgr& mgr,
	   const vector<AlgLiteral>& lit_list);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] str カバーを表す文字列
  AlgCover(AlgMgr& mgr,
	   const char* str);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] str カバーを表す文字列
  AlgCover(AlgMgr& mgr,
	   const string& str);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  AlgCover(const AlgCover& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身の参照を返す．
  const AlgCover&
  operator=(const AlgCover& src);

  /// @brief キューブからの変換コンストラクタ
  /// @param[in] cube 対象のキューブ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  AlgCover(const AlgCube& cube);

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~AlgCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  AlgMgr&
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
  literal_num(AlgLiteral lit) const;

  /// @brief 内容を返す．
  /// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
  /// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
  AlgPol
  literal(ymuint cube_id,
	  ymuint var_id) const;

  /// @brief 論理和を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator+(const AlgCover& right) const;

  /// @brief 論理和を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator+(const AlgCube& right) const;

  /// @brief 差分を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(const AlgCover& right) const;

  /// @brief 差分を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(const AlgCube& right) const;

  /// @brief 論理積を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(const AlgCover& right) const;

  /// @brief 論理積を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(const AlgCube& right) const;

  /// @brief 論理積を計算する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(AlgLiteral right) const;

  /// @brief algebraic division を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator/(const AlgCover& right) const;

  /// @brief キューブによる商を計算する．
  /// @param[in] cube 対象のキューブ
  /// @return 計算結果を返す．
  AlgCover
  operator/(const AlgCube& cube) const;

  /// @brief リテラルによる商を計算する．
  /// @param[in] lit 対象のリテラル
  /// @return 計算結果を返す．
  AlgCover
  operator/(AlgLiteral lit) const;

  /// @brief 論理和を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator+=(const AlgCover& right);

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator+=(const AlgCube& right);

  /// @brief 差分を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator-=(const AlgCover& right);

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator-=(const AlgCube& right);

  /// @brief 論理積を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator*=(const AlgCover& right);

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator*=(const AlgCube& right);

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator*=(AlgLiteral right);

  /// @brief algebraic division を行って代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator/=(const AlgCover& right);

  /// @brief キューブによる商を計算して代入する．
  /// @param[in] cube 対象のキューブ
  /// @return 演算後の自身への参照を返す．
  AlgCover
  operator/=(const AlgCube& cube);

  /// @brief リテラルによる商を計算して代入する．
  /// @param[in] lit 対象のリテラル
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator/=(AlgLiteral lit);

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  AlgCube
  common_cube() const;

  /// @param[in]
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
  AlgCover(AlgMgr& mgr,
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

  /// @brief 比較演算子(rich compare)
  /// @param[in] right オペランド
  /// @return 比較結果を返す．
  ///
  /// 比較方法はキューブごとの辞書式順序
  friend
  int
  compare(const AlgCover& left,
	  const AlgCover& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブマネージャ
  AlgMgr* mMgr;

  // キューブ数
  ymuint mCubeNum;

  // mBody の実際に確保されているキューブ容量
  ymuint mCubeCap;

  // 内容を表すビットベクタ
  ymuint64* mBody;

};

/// @relates AlgCover
/// @brief 比較演算子(rich compare)
/// @param[in] left, right オペランド
/// @return 比較結果を返す．
///
/// 比較方法はキューブごとの辞書式順序
int
compare(const AlgCover& left,
	const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (EQ)
/// @param[in] left, right オペランド
/// @return 等しい時に true を返す．
bool
operator==(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (NE)
/// @param[in] left, right オペランド
/// @return 等しくない時に true を返す．
bool
operator!=(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (LT)
/// @param[in] left, right オペランド
/// @return left が right より小さい時に true を返す．
bool
operator<(const AlgCover& left,
	  const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (GT)
/// @param[in] left, right オペランド
/// @return left が right より大きい時に true を返す．
bool
operator>(const AlgCover& left,
	  const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (LE)
/// @param[in] left, right オペランド
/// @return left が right と等しいか小さい時に true を返す．
bool
operator<=(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (GE)
/// @param[in] left, right オペランド
/// @return left が right と等しいか大きい時に true を返す．
bool
operator>=(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief AlgCover の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cover 対象のカバー
///
/// cover.print(s) と等価
ostream&
operator<<(ostream& s,
	   const AlgCover& cover);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] cube_list キューブのリスト
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const vector<AlgCube>& cube_list) :
  mMgr(&mgr),
  mCubeNum(cube_list.size()),
  mCubeCap(0)
{
  resize(mCubeNum);
  for (ymuint i = 0; i < mCubeNum; ++ i) {
    const AlgCube& cube = cube_list[i];
    mMgr->cube_copy(mBody, i, cube.mBody, 0);
  }
  mMgr->sort(mCubeNum, mBody);
}

// @brief 特殊なコンストラクタ
// @param[in] mgr マネージャ
// @param[in] dummy ダミーの引数
//
// 空のキューブを1つ持つカバーとなる．
// dummy の値は無視される．
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   int dummy) :
  mMgr(&mgr),
  mCubeNum(1),
  mCubeCap(0)
{
  resize(mCubeNum);
  mMgr->cube_clear(mBody, 0);
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] lit_list カバーを表すリテラルのリスト
//
// lit_list 中に kAlgLiteralUndef があった場合，キューブの区切り
// とみなされる．
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const vector<AlgLiteral>& lit_list) :
  mMgr(&mgr),
  mCubeCap(0)
{
  ymuint n = 0;
  bool first = true;
  for (ymuint i = 0; i < lit_list.size(); ++ i) {
    if ( lit_list[i] == kAlgLiteralUndef ) {
      first = true;
    }
    else if ( first ) {
      first = false;
      ++ n;
    }
  }
  mCubeNum = n;
  resize(mCubeNum);
  mMgr->set_literal(mBody, 0, lit_list);
  mMgr->sort(mCubeNum, mBody);
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] str カバーを表す文字列
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const char* str) :
  mMgr(&mgr),
  mCubeCap(0)
{
  vector<AlgLiteral> lit_list;
  mCubeNum = mMgr->parse(str, lit_list);
  resize(mCubeNum);
  mMgr->set_literal(mBody, 0, lit_list);
  mMgr->sort(mCubeNum, mBody);
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] str カバーを表す文字列
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const string& str) :
  mMgr(&mgr),
  mCubeCap(0)
{
  vector<AlgLiteral> lit_list;
  mCubeNum = mMgr->parse(str.c_str(), lit_list);
  resize(mCubeNum);
  mMgr->set_literal(mBody, 0, lit_list);
  mMgr->sort(mCubeNum, mBody);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
AlgCover::AlgCover(const AlgCover& src) :
  mMgr(src.mMgr),
  mCubeNum(src.mCubeNum),
  mCubeCap(0)
{
  resize(mCubeNum);
  mMgr->copy(mCubeNum, mBody, 0, src.mBody, 0);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身の参照を返す．
inline
const AlgCover&
AlgCover::operator=(const AlgCover& src)
{
  if ( &src != this ) {
    ymuint old_cap = mCubeCap;
    ymuint64* old_body = mBody;
    mCubeNum = src.mCubeNum;
    resize(mCubeNum);
    if ( old_body != mBody ) {
      mgr().delete_body(mBody, old_cap);
    }
    mMgr->copy(mCubeNum, mBody, 0, src.mBody, 0);
  }

  return *this;
}

// @brief キューブからの変換コンストラクタ
// @param[in] cube 対象のキューブ
//
// 指定されたキューブのみのカバーとなる．
inline
AlgCover::AlgCover(const AlgCube& cube) :
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
AlgCover::~AlgCover()
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
AlgCover::AlgCover(AlgMgr& mgr,
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
AlgMgr&
AlgCover::mgr() const
{
  return *mMgr;
}

// @brief 変数の数を返す．
inline
ymuint
AlgCover::variable_num() const
{
  return mMgr->variable_num();
}

// @brief キューブの数を返す．
inline
ymuint
AlgCover::cube_num() const
{
  return mCubeNum;
}

// @brief リテラル数を返す．
inline
ymuint
AlgCover::literal_num() const
{
  return mgr().literal_num(cube_num(), mBody);
}

// @brief 指定されたリテラルの出現回数を返す．
// @param[in] lit 対象のリテラル
inline
ymuint
AlgCover::literal_num(AlgLiteral lit) const
{
  return mgr().literal_num(cube_num(), mBody, lit);
}

// @brief 内容を返す．
// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
inline
AlgPol
AlgCover::literal(ymuint cube_id,
		  ymuint var_id) const
{
  ASSERT_COND( cube_id < cube_num() );

  return mgr().literal(mBody, cube_id, var_id);
}

// @brief 論理和を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator+(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1 + nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().sum(body, nc1, mBody, nc2, right.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理和を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator+(const AlgCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint cap = get_capacity(nc1 + nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().sum(body, nc1, mBody, nc2, right.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理和を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator+=(const AlgCover& right)
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
const AlgCover&
AlgCover::operator+=(const AlgCube& right)
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
AlgCover
AlgCover::operator-(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().diff(body, nc1, mBody, nc2, right.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 差分を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator-(const AlgCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().diff(body, nc1, mBody, nc2, right.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 差分を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator-=(const AlgCover& right)
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
const AlgCover&
AlgCover::operator-=(const AlgCube& right)
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
AlgCover
AlgCover::operator*(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1 * nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().product(body, nc1, mBody, nc2, right.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator*(const AlgCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = 1;
  ymuint cap = get_capacity(nc1 * nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().product(body, nc1, mBody, nc2, right.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算する(リテラル版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator*(AlgLiteral right) const
{
  ymuint nc1 = cube_num();
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().product(body, nc1, mBody, right);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator*=(const AlgCover& right)
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
const AlgCover&
AlgCover::operator*=(const AlgCube& right)
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

// @brief 論理積を計算して代入する(リテラル版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator*=(AlgLiteral right)
{
  mCubeNum = mgr().product(mBody, mCubeNum, mBody, right);

  return *this;
}

// @brief algebraic division を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator/(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  ymuint cap = get_capacity(nc1 / nc2);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().division(body, nc1, mBody, nc2, right.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief algebraic division を行って代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator/=(const AlgCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint nc1 = cube_num();
  ymuint nc2 = right.cube_num();
  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, nc1, mBody, nc2, right.mBody);

  return *this;
}

// @brief キューブによる商を計算する．
// @param[in] cube 対象のキューブ
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator/(const AlgCube& cube) const
{
  ASSERT_COND( variable_num() == cube.variable_num() );

  ymuint nc1 = cube_num();
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().division(body, nc1, mBody, 1, cube.mBody);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief キューブによる商を計算して代入する．
// @param[in] cube 対象のキューブ
// @return 演算後の自身への参照を返す．
inline
AlgCover
AlgCover::operator/=(const AlgCube& cube)
{
  ASSERT_COND( variable_num() == cube.variable_num() );

  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, mCubeNum, mBody, 1, cube.mBody);

  return *this;
}

// @brief リテラルによる商を計算する．
// @param[in] lit 対象のリテラル
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator/(AlgLiteral lit) const
{
  ymuint nc1 = cube_num();
  ymuint cap = get_capacity(nc1);
  ymuint64* body = mgr().new_body(cap);
  ymuint nc = mgr().division(body, nc1, mBody, lit);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief リテラルによる商を計算して代入する．
// @param[in] lit 対象のリテラル
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator/=(AlgLiteral lit)
{
  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, mCubeNum, mBody, lit);

  return *this;
}

// @brief 共通なキューブを返す．
//
// 共通なキューブがない場合には空のキューブを返す．
inline
AlgCube
AlgCover::common_cube() const
{
  ymuint64* body = mgr().new_body();
  mgr().common_cube(body, cube_num(), mBody);

  return AlgCube(mgr(), body);
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
inline
void
AlgCover::print(ostream& s) const
{
  mgr().print(s, mBody, 0, mCubeNum);
}

// @relates AlgCover
// @brief 比較演算子(rich compare)
// @param[in] left, right オペランド
// @return 比較結果を返す．
//
// 比較方法はキューブごとの辞書式順序
inline
int
compare(const AlgCover& left,
	const AlgCover& right)
{
  ASSERT_COND( &left.mgr() == &right.mgr() );

  return left.mgr().compare(left.cube_num(), left.mBody,
			    right.cube_num(), right.mBody);
}

// @relates AlgCover
// @brief 比較演算子 (EQ)
// @param[in] left, right オペランド
// @return 等しい時に true を返す．
inline
bool
operator==(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) == 0;
}

// @relates AlgCover
// @brief 比較演算子 (NE)
// @param[in] left, right オペランド
// @return 等しくない時に true を返す．
inline
bool
operator!=(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) != 0;
}

// @relates AlgCover
// @brief 比較演算子 (LT)
// @param[in] left, right オペランド
// @return left が right より小さい時に true を返す．
inline
bool
operator<(const AlgCover& left,
	  const AlgCover& right)
{
  return compare(left, right) < 0;
}

// @relates AlgCover
// @brief 比較演算子 (GT)
// @param[in] left, right オペランド
// @return left が right より大きい時に true を返す．
inline
bool
operator>(const AlgCover& left,
	  const AlgCover& right)
{
  return compare(left, right) > 0;
}

// @relates AlgCover
// @brief 比較演算子 (LE)
// @param[in] left, right オペランド
// @return left が right と等しいか小さい時に true を返す．
inline
bool
operator<=(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) <= 0;
}

// @relates AlgCover
// @brief 比較演算子 (GE)
// @param[in] left, right オペランド
// @return left が right と等しいか大きい時に true を返す．
inline
bool
operator>=(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) >= 0;
}

// @brief AlgCover の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cover 対象のカバー
//
// cover.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const AlgCover& cover)
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
AlgCover::resize(ymuint req_cap)
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
AlgCover::get_capacity(ymuint cube_num)
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

#endif // YM_ALGCOVER_H
