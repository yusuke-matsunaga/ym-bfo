#ifndef YM_ALGCUBE_H
#define YM_ALGCUBE_H

/// @file AlgCube.h
/// @brief AlgCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2022 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/AlgLiteral.h"
#include "ym/AlgLitSet.h"
#include "ym/AlgMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class AlgCube AlgCube.h "ym/AlgCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// 離散数学的には AlgLiteral の集合だが，相反するリテラル(x と x')は
/// 同時には含まない．<br>
/// 常に固定サイズのビット配列として実装する．<br>
/// 1つの変数につき2ビットを使用する．<br>
//////////////////////////////////////////////////////////////////////
class AlgCube
{
  friend class AlgCover;

public:

  /// @brief コンストラクタ
  ///
  /// 単一のリテラルからなるキューブを作る．
  AlgCube(
    AlgMgr& mgr,   ///< [in] マネージャ
    AlgLiteral lit ///< [in] リテラル
  ) : mMgr{&mgr},
      mBody{mMgr->new_body()}
  {
    // わざわざ vector<AlgLiteral> を作っているので
    // あまり効率はよくなけど，AlgMgr に別の関数を
    // 作るほどのことではないと判断した．
    mMgr->set_literal(mBody, 0, vector<AlgLiteral>{lit});
  }

  /// @brief コンストラクタ
  ///
  /// lit_list が省略された時には空のキューブを作る．
  explicit
  AlgCube(
    AlgMgr& mgr,                            ///< [in] マネージャ
    const vector<AlgLiteral>& lit_list = {} ///< [in] キューブを表すリテラルのリスト
  ) : mMgr{&mgr},
      mBody{mMgr->new_body()}
  {
    mMgr->set_literal(mBody, 0, lit_list);
  }

  /// @brief コンストラクタ
  ///
  /// 文字列は mgr の varname() を空白で区切ったもの<br>
  /// 否定の場合は ' を変数名の直後につける．変数名と'の間の空白は認めない<br>
  /// 文字列が不正だった場合には空のキューブとなる．
  AlgCube(
    AlgMgr& mgr,    ///< [in] マネージャ
    const char* str ///< [in] 内容を表す文字列
  ) : mMgr{&mgr},
      mBody{mMgr->new_body()}
  {
    vector<AlgLiteral> lit_list;
    SizeType n = mMgr->parse(str, lit_list);
    if ( n == 1 ) {
      mMgr->set_literal(mBody, 0, lit_list);
    }
    else {
      // 複数のキューブを表す文字列だった．
      // 無視する．
    }
  }

  /// @brief コンストラクタ
  ///
  /// 文字列は mgr の varname() を空白で区切ったもの<br>
  /// 否定の場合は ' を変数名の直後につける．変数名と'の間の空白は認めない<br>
  /// 文字列が不正だった場合には空のキューブとなる．
  AlgCube(
    AlgMgr& mgr,      ///< [in] マネージャ
    const string& str ///< [in] 内容を表す文字列
  ) : mMgr{&mgr},
      mBody{mMgr->new_body()}
  {
    vector<AlgLiteral> lit_list;
    SizeType n = mMgr->parse(str.c_str(), lit_list);
    if ( n == 1 ) {
      mMgr->set_literal(mBody, 0, lit_list);
    }
    else {
      // 複数のキューブを表す文字列だった．
      // 無視する．
    }
  }

  /// @brief コピーコンストラクタ
  AlgCube(
    const AlgCube& src ///< [in] コピー元のオブジェクト
  ) : mMgr{src.mMgr},
      mBody{mMgr->new_body()}
  {
    mMgr->cube_copy(mBody, 0, src.mBody, 0);
  }

  /// @brief 代入演算子
  /// @return 代入後の自身への参照を返す．
  const AlgCube&
  operator=(
    const AlgCube& src ///< [in] コピー元のオブジェクト
  )
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

  /// @brief デストラクタ
  ~AlgCube()
  {
    mMgr->delete_body(mBody);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  AlgMgr&
  mgr() const
  {
    return *mMgr;
  }

  /// @brief 変数の数を返す．
  SizeType
  variable_num() const
  {
    return mgr().variable_num();
  }

  /// @brief リテラル数を返す．
  SizeType
  literal_num() const
  {
    return mgr().literal_num(1, mBody);
  }

  /// @brief 内容を読み出す．
  AlgPol
  literal(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < variable_num() )
  ) const
  {
    return mgr().literal(mBody, 0, pos);
  }

  /// @brief 指定したリテラルを含んでいたら true を返す．
  bool
  has_literal(
    AlgLiteral lit ///< [in] 対象のリテラル
  ) const
  {
    SizeType varid = lit.varid();
    AlgPol pol = literal(varid);
    AlgPol ref_pol = lit.is_positive() ? kAlgPolP : kAlgPolN;
    return pol == ref_pol;
  }

  /// @brief オペランドのキューブに含まれていたら true を返す．
  ///
  /// ここではキューブの表す論理関数の含意を考える<br>
  /// だからリテラル集合としてはオペランドのキューブを含むことになる．
  bool
  check_containment(
    const AlgCube& right ///< [in] オペランドのキューブ
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );
    return mgr().cube_check_containment(mBody, 0, right.mBody, 0);
  }

  /// @brief 2つのキューブに共通なリテラルがあれば true を返す．
  bool
  check_intersect(
    const AlgCube& right ///< [in] オペランドのキューブ
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );
    return mgr().cube_check_intersect(mBody, 0, right.mBody, 0);
  }

  /// @brief 引数のリテラルをひとつでも含んでいたら true を返す．
  bool
  contains(
    const AlgLitSet& right ///< [in] 対象のリテラル集合
  ) const
  {
    // 中身は check_intersect() と同じ
    ASSERT_COND( variable_num() == right.variable_num() );

    return mgr().cube_check_intersect(mBody, 0, right.mBody, 0);
  }

  /// @brief 論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  const AlgCube&
  operator*=(
    const AlgCube& right ///< [in] オペランドのキューブ
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    bool res = mgr().cube_product(mBody, 0, mBody, 0, right.mBody, 0);
    if ( !res ) {
      mgr().cube_clear(mBody, 0);
    }

    return *this;
  }

  /// @brief リテラルとの論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  const AlgCube&
  operator*=(
    AlgLiteral right ///< [in] オペランドのリテラル
  )
  {
    int res = mgr().product(mBody, 1, mBody, right);
    if ( res == 0 ) {
      mgr().cube_clear(mBody, 0);
    }

    return *this;
  }

  /// @brief キューブによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  const AlgCube&
  operator/=(
    const AlgCube& right ///< [in] オペランドのキューブ
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    bool res = mgr().cube_division(mBody, 0, mBody, 0, right.mBody, 0);
    if ( !res ) {
      mgr().cube_clear(mBody, 0);
    }

    return *this;
  }

  /// @brief リテラルによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  const AlgCube&
  operator/=(
    AlgLiteral right ///< [in] オペランドのリテラル
  )
  {
    int res = mgr().division(mBody, 1, mBody, right);
    if ( res == 0 ) {
      mgr().cube_clear(mBody, 0);
    }

    return *this;
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return mgr().hash(1, mBody);
  }

  /// @brief 内容をわかりやすい形で出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const
  {
    mgr().print(s, mBody, 0, 1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定するコンストラクタ
  ///
  /// 危険なので普通は使わないように
  AlgCube(
    AlgMgr& mgr,   ///< [in] マネージャ
    ymuint64* body ///< [in] キューブのパタンを表す本体
  ) : mMgr{&mgr},
      mBody{body}
  {
  }

  // friend 関数の宣言
  friend
  int
  compare(
    const AlgCube& left,
    const AlgCube& right
  );


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
/// @brief キューブの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
AlgCube
operator*(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return AlgCube(left).operator*=(right);
}

/// @relates AlgCube
/// @brief キューブとリテラルの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
AlgCube
operator*(
  const AlgCube& left, ///< [in] 第1オペランド
  AlgLiteral right     ///< [in] 第2オペランド
)
{
  return AlgCube(left).operator*=(right);
}

/// @relates AlgCube
/// @brief キューブによる商を計算する
inline
AlgCube
operator/(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return AlgCube(left).operator/=(right);
}

/// @relates AlgCube
/// @brief リテラルによる商を計算する
inline
AlgCube
operator/(
  const AlgCube& left, ///< [in] 第1オペランド
  AlgLiteral right     ///< [in] 第2オペランド
)
{
  return AlgCube(left).operator/=(right);
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
inline
int
compare(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  ASSERT_COND( left.variable_num() == right.variable_num() );

  return left.mgr().cube_compare(left.mBody, 0, right.mBody, 0);
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(EQ)
/// @retval true  left == right
/// @retval false left != right
inline
bool
operator==(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == 0;
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(NE)
/// @retval true  left != right
/// @retval false left == right
inline
bool
operator!=(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) != 0;
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(LT)
/// @retval true  left < right
/// @retval false left >= right
inline
bool
operator<(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) < 0;
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(GT)
/// @retval true  left > right
/// @retval false left <= right
inline
bool
operator>(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) > 0;
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(LE)
/// @retval true  left < right
/// @retval false left >= right
inline
bool
operator<=(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) <= 0;
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(GE)
/// @retval true  left < right
/// @retval false left >= right
inline
bool
operator>=(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) >= 0;
}

/// @relates AlgCube
/// @brief AlgCube の内容を出力する．
/// @return ストリームを返す．
///
/// cube->print(s) と等価
inline
ostream&
operator<<(
  ostream& s,         ///< [in] 出力先のストリーム
  const AlgCube& cube ///< [in] 対象のキューブ
)
{
  cube.print(s);
  return s;
}

END_NAMESPACE_YM_BFO

BEGIN_NAMESPACE_STD

/// @breif AlgCube をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::AlgCube>
{
  SizeType
  operator()(
    const YM_NAMESPACE::AlgCube& cube
  ) const
  {
    return cube.hash();
  }
};

END_NAMESPACE_YM

#endif // YM_ALGCUBE_H
