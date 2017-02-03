#ifndef YM_ALGCUBE_H
#define YM_ALGCUBE_H

/// @file AlgCube.h
/// @brief AlgCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/AlgLiteral.h"
#include "ym/AlgMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class AlgCube AlgCube.h "ym/AlgCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// 常に固定サイズのビット配列として実装する．<br>
/// 1つの変数につき2ビットを使用する．<br>
//////////////////////////////////////////////////////////////////////
class AlgCube
{
  friend class AlgCover;

public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] lit リテラル
  ///
  /// 単一のリテラルからなるキューブを作る．
  AlgCube(AlgMgr& mgr,
	  AlgLiteral lit);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] lit_list キューブを表すリテラルのリスト
  ///
  /// lit_list が省略された時には空のキューブを作る．
  explicit
  AlgCube(AlgMgr& mgr,
	  const vector<AlgLiteral>& lit_list = vector<AlgLiteral>());

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] str 内容を表す文字列
  ///
  /// 文字列は mgr の varname() を空白で区切ったもの<br>
  /// 否定の場合は ' を変数名の直後につける．空白は認めない<br>
  /// 文字列が不正だった場合には空のキューブとなる．
  AlgCube(AlgMgr& mgr,
	  const char* str);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] str 内容を表す文字列
  ///
  /// 文字列は mgr の varname() を空白で区切ったもの<br>
  /// 否定の場合は ' を変数名の直後につける．空白は認めない<br>
  /// 文字列が不正だった場合には空のキューブとなる．
  AlgCube(AlgMgr& mgr,
	  const string& str);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  AlgCube(const AlgCube& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  const AlgCube&
  operator=(const AlgCube& src);

  /// @brief デストラクタ
  ~AlgCube();


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

  /// @brief リテラル数を返す．
  ymuint
  literal_num() const;

  /// @brief 内容を読み出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
  AlgPol
  literal(ymuint pos) const;

  /// @brief オペランドのキューブに含まれていたら true を返す．
  /// @param[in] right オペランドのキューブ
  ///
  /// ここではキューブの表す論理関数の含意を考える<br>
  /// だからリテラル集合としてはオペランドのキューブを含むことになる．
  bool
  check_containment(const AlgCube& right) const;

  /// @brief 2つのキューブに共通なリテラルがあれば true を返す．
  /// @param[in] right オペランドのキューブ
  bool
  check_intersect(const AlgCube& right) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  const AlgCube&
  operator&=(const AlgCube& right);

  /// @brief コファクターを計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  const AlgCube&
  operator/=(const AlgCube& right);

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
  AlgCube(AlgMgr& mgr,
	  ymuint64* body);

  // friend 関数の宣言
  friend
  int
  compare(const AlgCube& left,
	  const AlgCube& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AlgMgr* mMgr;

  // 内容を表すビットベクタ
  ymuint64* mBody;

};

/// @relates AlgCube
/// @brief AlgCube の論理積を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
AlgCube
operator&(const AlgCube& left,
	  const AlgCube& right);

/// @relates AlgCube
/// @brief コファクターを計算する
/// @param[in] left, right オペランド
AlgCube
operator/(const AlgCube& left,
	  const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子
/// @param[in] left, right オペランド
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
int
compare(const AlgCube& left,
	const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(EQ)
/// @param[in] left, right オペランド
/// @retval true  left == right
/// @retval false left != right
bool
operator==(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(NE)
/// @param[in] left, right オペランド
/// @retval true  left != right
/// @retval false left == right
bool
operator!=(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(LT)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<(const AlgCube& left,
	  const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(GT)
/// @param[in] left, right オペランド
/// @retval true  left > right
/// @retval false left <= right
bool
operator>(const AlgCube& left,
	  const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(LE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<=(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(GE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator>=(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCube の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cube 対象のキューブ(のポインタ)
///
/// cube->print(s) と等価
ostream&
operator<<(ostream& s,
	   const AlgCube& cube);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] lit リテラル
//
// 単一のリテラルからなるキューブを作る．
inline
AlgCube::AlgCube(AlgMgr& mgr,
		 AlgLiteral lit) :
  mMgr(&mgr),
  mBody(mMgr->new_body())
{
  // わざわざ vector<AlgLiteral> を作っているので
  // あまり効率はよくなけど，AlgMgr に別の関数を
  // 作るほどのことではないと判断した．
  mMgr->set_literal(mBody, 0, vector<AlgLiteral>(1, lit));
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] lit_list キューブを表すリテラルのリスト
inline
AlgCube::AlgCube(AlgMgr& mgr,
		 const vector<AlgLiteral>& lit_list) :
  mMgr(&mgr),
  mBody(mMgr->new_body())
{
  mMgr->set_literal(mBody, 0, lit_list);
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] str 内容を表す文字列
//
// 文字列は mgr の varname() を空白で区切ったもの<br>
// 否定の場合は ' を変数名の直後につける．空白は認めない<br>
// 文字列が不正だった場合には空のキューブとなる．
inline
AlgCube::AlgCube(AlgMgr& mgr,
		 const char* str) :
  mMgr(&mgr),
  mBody(mMgr->new_body())
{
  vector<AlgLiteral> lit_list;
  ymuint n = mMgr->parse(str, lit_list);
  if ( n == 1 ) {
    mMgr->set_literal(mBody, 0, lit_list);
  }
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] str 内容を表す文字列
//
// 文字列は mgr の varname() を空白で区切ったもの<br>
// 否定の場合は ' を変数名の直後につける．空白は認めない<br>
// 文字列が不正だった場合には空のキューブとなる．
inline
AlgCube::AlgCube(AlgMgr& mgr,
		 const string& str) :
  mMgr(&mgr),
  mBody(mMgr->new_body())
{
  vector<AlgLiteral> lit_list;
  ymuint n = mMgr->parse(str.c_str(), lit_list);
  if ( n == 1 ) {
    mMgr->set_literal(mBody, 0, lit_list);
  }
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
AlgCube::AlgCube(const AlgCube& src) :
  mMgr(src.mMgr)
{
  mBody = mMgr->new_body();
  mMgr->cube_copy(mBody, 0, src.mBody, 0);
}

// @brief 内容を指定するコンストラクタ
// @param[in] mgr マネージャ
// @param[in] body キューブのパタンを表す本体
//
// 危険なので普通は使わないように
inline
AlgCube::AlgCube(AlgMgr& mgr,
		 ymuint64* body) :
  mMgr(&mgr),
  mBody(body)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身への参照を返す．
inline
const AlgCube&
AlgCube::operator=(const AlgCube& src)
{
  if ( &src != this ) {
    if ( &mMgr != &src.mMgr ) {
      // マネージャが異なっていたら mBody を作り直す．
      mMgr->delete_body(mBody);
      mMgr = src.mMgr;
      mBody = mMgr->new_body();
    }
    else {
      // マネージャが同じなら mBody は使いまわす．
    }
    mMgr->cube_copy(mBody, 0, src.mBody, 0);
  }

  return *this;
}

// @brief デストラクタ
inline
AlgCube::~AlgCube()
{
  mMgr->delete_body(mBody);
}

// @brief マネージャを返す．
inline
AlgMgr&
AlgCube::mgr() const
{
  return *mMgr;
}

// @brief 変数の数を返す．
inline
ymuint
AlgCube::variable_num() const
{
  return mgr().variable_num();
}

// @brief 内容を読み出す．
// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
inline
AlgPol
AlgCube::literal(ymuint pos) const
{
  return mgr().literal(mBody, 0, pos);
}

// @brief リテラル数を返す．
inline
ymuint
AlgCube::literal_num() const
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
AlgCube::check_containment(const AlgCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  return mgr().cube_check_containment(mBody, 0, right.mBody, 0);
}

// @brief 2つのキューブに共通なリテラルがあれば true を返す．
// @param[in] right オペランドのキューブ
inline
bool
AlgCube::check_intersect(const AlgCube& right) const
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
const AlgCube&
AlgCube::operator&=(const AlgCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  bool res = mgr().cube_product(mBody, 0, mBody, 0, right.mBody, 0);
  if ( !res ) {
    mgr().cube_clear(mBody, 0);
  }

  return *this;
}

// @brief AlgCube の論理積を計算する
// @param[in] left, right オペランド
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
AlgCube
operator&(const AlgCube& left,
	  const AlgCube& right)
{
  return AlgCube(left).operator&=(right);
}

// @brief コファクターを計算し自身に代入する．
// @param[in] right オペランドのキューブ
// @return 演算後の自身の参照を返す．
inline
const AlgCube&
AlgCube::operator/=(const AlgCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  bool res = mgr().cube_cofactor(mBody, 0, mBody, 0, right.mBody, 0);
  if ( !res ) {
    mgr().cube_clear(mBody, 0);
  }

  return *this;
}

// @brief コファクターを計算する
// @param[in] left, right オペランド
inline
AlgCube
operator/(const AlgCube& left,
	  const AlgCube& right)
{
  return AlgCube(left).operator/=(right);
}

// @brief AlgCubeの比較演算子
// @param[in] left, right オペランド
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
inline
int
compare(const AlgCube& left,
	const AlgCube& right)
{
  ASSERT_COND( left.variable_num() == right.variable_num() );

  return left.mgr().cube_compare(left.mBody, 0, right.mBody, 0);
}

// @relates AlgCube
// @brief AlgCubeの比較演算子(EQ)
// @param[in] left, right オペランド
// @retval true  left == right
// @retval false left != right
inline
bool
operator==(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) == 0;
}

// @relates AlgCube
// @brief AlgCubeの比較演算子(NE)
// @param[in] left, right オペランド
// @retval true  left != right
// @retval false left == right
inline
bool
operator!=(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) != 0;
}

// @brief AlgCubeの比較演算子(LT)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<(const AlgCube& left,
	  const AlgCube& right)
{
  return compare(left, right) < 0;
}

// @brief AlgCubeの比較演算子(GT)
// @param[in] left, right オペランド
// @retval true  left > right
// @retval false left <= right
inline
bool
operator>(const AlgCube& left,
	  const AlgCube& right)
{
  return compare(left, right) > 0;
}

// @brief AlgCubeの比較演算子(LE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<=(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) <= 0;
}

// @brief AlgCubeの比較演算子(GE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator>=(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) >= 0;
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
inline
void
AlgCube::print(ostream& s) const
{
  mgr().print(s, mBody, 0, 1);
}

// @brief AlgCube の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cube 対象のキューブ
//
// cube.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const AlgCube& cube)
{
  cube.print(s);
  return s;
}

END_NAMESPACE_YM_BFO

#endif // YM_ALGCUBE_H
