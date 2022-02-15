#ifndef YM_ALGCOVER_H
#define YM_ALGCOVER_H

/// @file AlgCover.h
/// @brief AlgCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2022 Yusuke Matsunaga
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
  ///
  /// cube_list が空の時は空のカバーとなる．
  explicit
  AlgCover(
    AlgMgr& mgr,                          ///< [in] マネージャ
    const vector<AlgCube>& cube_list = {} ///< [in] キューブのリスト
  ) : mMgr{&mgr},
      mCubeNum{cube_list.size()}
  {
    resize(mCubeNum);
    for ( int i = 0; i < mCubeNum; ++ i ) {
      const AlgCube& cube = cube_list[i];
      mMgr->cube_copy(mBody, i, cube.mBody, 0);
    }
    mMgr->sort(mCubeNum, mBody);
  }

  /// @brief 特殊なコンストラクタ
  ///
  /// 空のキューブを1つ持つカバーとなる．
  /// dummy の値は無視される．
  AlgCover(
    AlgMgr& mgr, ///< [in] マネージャ
    int dummy    ///< [in] ダミーの引数
  ) : mMgr{&mgr},
      mCubeNum{1}
  {
    resize(mCubeNum);
    mMgr->cube_clear(mBody, 0);
  }

  /// @brief コンストラクタ
  ///
  /// lit_list 中に AlgLiteralUndef があった場合，キューブの区切り
  /// とみなされる．
  AlgCover(
    AlgMgr& mgr,                       ///< [in] マネージャ
    const vector<AlgLiteral>& lit_list ///< [in] カバーを表すリテラルのリスト
  ) : mMgr{&mgr}
  {
    // キューブ数を数える．
    bool first = true;
    for ( auto lit: lit_list ) {
      if ( lit == AlgLiteralUndef ) {
	first = true;
      }
      else if ( first ) {
	first = false;
	++ mCubeNum;
      }
    }
    resize(mCubeNum);
    // 内容をセットする．
    mMgr->set_literal(mBody, 0, lit_list);
    mMgr->sort(mCubeNum, mBody);
  }

  /// @brief コンストラクタ
  AlgCover(
    AlgMgr& mgr,    ///< [in] マネージャ
    const char* str ///< [in] カバーを表す文字列
  ) : mMgr{&mgr}
  {
    vector<AlgLiteral> lit_list;
    mCubeNum = mMgr->parse(str, lit_list);
    resize(mCubeNum);
    mMgr->set_literal(mBody, 0, lit_list);
    mMgr->sort(mCubeNum, mBody);
  }

  /// @brief コンストラクタ
  AlgCover(
    AlgMgr& mgr,      ///< [in] マネージャ
    const string& str ///< [in] カバーを表す文字列
  ) : mMgr{&mgr}
  {
    vector<AlgLiteral> lit_list;
    mCubeNum = mMgr->parse(str.c_str(), lit_list);
    resize(mCubeNum);
    mMgr->set_literal(mBody, 0, lit_list);
    mMgr->sort(mCubeNum, mBody);
  }

  /// @brief コピーコンストラクタ
  AlgCover(
    const AlgCover& src ///< [in] コピー元のオブジェクト
  ) : mMgr{src.mMgr},
      mCubeNum{src.mCubeNum}
  {
    resize(mCubeNum);
    mMgr->copy(mCubeNum, mBody, 0, src.mBody, 0);
  }

  /// @brief 代入演算子
  /// @return 代入後の自身の参照を返す．
  AlgCover&
  operator=(
    const AlgCover& src ///< [in] コピー元のオブジェクト
  )
  {
    if ( &src != this ) {
      SizeType old_cap = mCubeCap;
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

  /// @brief キューブからの変換コンストラクタ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  AlgCover(
    const AlgCube& cube ///< [in] 対象のキューブ
  ) : mMgr{&cube.mgr()},
      mCubeNum{1}
  {
    resize(mCubeNum);
    mMgr->cube_copy(mBody, 0, cube.mBody, 0);
  }

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~AlgCover()
  {
    mMgr->delete_body(mBody, mCubeCap);
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
    return mMgr->variable_num();
  }

  /// @brief キューブの数を返す．
  SizeType
  cube_num() const
  {
    return mCubeNum;
  }

  /// @brief リテラル数を返す．
  SizeType
  literal_num() const
  {
    return mgr().literal_num(cube_num(), mBody);
  }

  /// @brief 指定されたリテラルの出現回数を返す．
  /// @param[in] lit 対象のリテラル
  SizeType
  literal_num(
    AlgLiteral lit
  ) const
  {
    return mgr().literal_num(cube_num(), mBody, lit);
  }

  /// @brief 指定された位置のリテラルの極性を返す．
  AlgPol
  literal(
    SizeType cube_id, ///< [in] キューブ番号 ( 0 <= cube_id < cube_num() )
    SizeType var_id   ///< [in] 変数番号 ( 0 <= var_id < variable_num() )
  ) const
  {
    ASSERT_COND( cube_id < cube_num() );

    return mgr().literal(mBody, cube_id, var_id);
  }

  /// @brief 論理和を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator+(
    const AlgCover& right ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    SizeType cap = get_capacity(nc1 + nc2);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().sum(body, nc1, mBody, nc2, right.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief 論理和を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator+=(
    const AlgCover& right ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    SizeType old_cap = mCubeCap;
    ymuint64* old_body = mBody;
    resize(nc1 + nc2);
    mCubeNum = mgr().sum(mBody, nc1, old_body, nc2, right.mBody);
    if ( old_body != mBody ) {
      mgr().delete_body(old_body, old_cap);
    }

    return *this;
  }

  /// @brief 論理和を計算する(キューブ版)．
  /// @return 計算結果を返す．
  AlgCover
  operator+(
    const AlgCube& right ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = 1;
    SizeType cap = get_capacity(nc1 + nc2);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().sum(body, nc1, mBody, nc2, right.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator+=(
    const AlgCube& right ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = 1;
    SizeType old_cap = mCubeCap;
    ymuint64* old_body = mBody;
    resize(nc1 + nc2);
    mCubeNum = mgr().sum(mBody, nc1, old_body, nc2, right.mBody);
    if ( old_body != mBody ) {
      mgr().delete_body(old_body, old_cap);
    }

    return *this;
  }

  /// @brief 差分を計算する．
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(
    const AlgCover& right ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    SizeType cap = get_capacity(nc1);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().diff(body, nc1, mBody, nc2, right.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief 差分を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator-=(
    const AlgCover& right ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    // 結果のキューブ数は減るだけなのでキューブ容量の変更はしない．
    mCubeNum = mgr().diff(mBody, nc1, mBody, nc2, right.mBody);

    return *this;
  }

  /// @brief 差分を計算する(キューブ版)．
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(
    const AlgCube& right ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = 1;
    SizeType cap = get_capacity(nc1);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().diff(body, nc1, mBody, nc2, right.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator-=(
    const AlgCube& right ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = 1;
    // 結果のキューブ数は減るだけなのでキューブ容量の変更はしない．
    mCubeNum = mgr().diff(mBody, nc1, mBody, nc2, right.mBody);

    return *this;
  }

  /// @brief 論理積を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator*(
    const AlgCover& right ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    SizeType cap = get_capacity(nc1 * nc2);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().product(body, nc1, mBody, nc2, right.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief 論理積を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(
    const AlgCover& right ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    SizeType old_cap = mCubeCap;
    ymuint64* old_body = mBody;
    SizeType cap = nc1 * nc2;
    resize(cap);
    mCubeNum = mgr().product(mBody, nc1, old_body, nc2, right.mBody);
    if ( old_body != mBody ) {
      mgr().delete_body(old_body, old_cap);
    }

    return *this;
  }

  /// @brief 論理積を計算する(キューブ版)．
  /// @return 計算結果を返す．
  AlgCover
  operator*(
    const AlgCube& right ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = 1;
    SizeType cap = get_capacity(nc1 * nc2);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().product(body, nc1, mBody, nc2, right.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(
    const AlgCube& right ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = 1;
    SizeType old_cap = mCubeCap;
    ymuint64* old_body = mBody;
    resize(nc1 * nc2);
    mCubeNum = mgr().product(mBody, nc1, old_body, nc2, right.mBody);
    if ( old_body != mBody ) {
      mgr().delete_body(old_body, old_cap);
    }

    return *this;
  }

  /// @brief 論理積を計算する(リテラル版)．
  /// @return 計算結果を返す．
  AlgCover
  operator*(
    AlgLiteral right ///< [in] オペランド
  ) const
  {
    SizeType nc1 = cube_num();
    SizeType cap = get_capacity(nc1);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().product(body, nc1, mBody, right);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(
    AlgLiteral right ///< [in] オペランド
  )
  {
    mCubeNum = mgr().product(mBody, mCubeNum, mBody, right);

    return *this;
  }

  /// @brief algebraic division を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator/(
    const AlgCover& right ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    SizeType cap = get_capacity(nc1 / nc2);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().division(body, nc1, mBody, nc2, right.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief algebraic division を行って代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(
    const AlgCover& right ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == right.variable_num() );

    SizeType nc1 = cube_num();
    SizeType nc2 = right.cube_num();
    // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
    mCubeNum = mgr().division(mBody, nc1, mBody, nc2, right.mBody);

    return *this;
  }

  /// @brief キューブによる商を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator/(
    const AlgCube& cube ///< [in] オペランド
  ) const
  {
    ASSERT_COND( variable_num() == cube.variable_num() );

    SizeType nc1 = cube_num();
    SizeType cap = get_capacity(nc1);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().division(body, nc1, mBody, 1, cube.mBody);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief キューブによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(
    const AlgCube& cube ///< [in] オペランド
  )
  {
    ASSERT_COND( variable_num() == cube.variable_num() );

    // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
    mCubeNum = mgr().division(mBody, mCubeNum, mBody, 1, cube.mBody);

    return *this;
  }

  /// @brief リテラルによる商を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator/(
    AlgLiteral lit ///< [in] オペランド
  ) const
  {
    SizeType nc1 = cube_num();
    SizeType cap = get_capacity(nc1);
    ymuint64* body = mgr().new_body(cap);
    SizeType nc = mgr().division(body, nc1, mBody, lit);

    return AlgCover(mgr(), nc, cap, body);
  }

  /// @brief リテラルによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(
    AlgLiteral lit ///< [in] オペランド
  )
  {
    // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
    mCubeNum = mgr().division(mBody, mCubeNum, mBody, lit);

    return *this;
  }

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  AlgCube
  common_cube() const
  {
    ymuint64* body = mgr().new_body();
    mgr().common_cube(body, cube_num(), mBody);

    return AlgCube(mgr(), body);
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return mgr().hash(mCubeNum, mBody);
  }

  /// @brief 内容をわかりやすい形で出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const
  {
    mgr().print(s, mBody, 0, mCubeNum);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// この関数は危険なので普通は使わないこと
  AlgCover(
    AlgMgr& mgr,       ///< [in] マネージャ
    SizeType cube_num, ///< [in] キューブ数
    SizeType cube_cap, ///< [in] キューブ容量
    ymuint64* body     ///< [in] 内容のパタンを表す本体
  ) : mMgr{&mgr},
      mCubeNum{cube_num},
      mCubeCap{cube_cap},
      mBody{body}
  {
  }

  /// @brief キューブ容量を変更する．
  ///
  /// 現在のキューブ容量が大きければ変更しない．
  void
  resize(
    SizeType req_cap ///< [in] 要求するキューブ容量
  )
  {
    SizeType new_cap = get_capacity(req_cap);
    if ( new_cap > mCubeCap ) {
      mCubeCap = new_cap;
      mBody = mgr().new_body(mCubeCap);
    }
  }

  /// @brief キューブ容量を計算する．
  static
  SizeType
  get_capacity(
    SizeType cube_num ///< [in] キューブ数
  )
  {
    // 初期値を16としてcube_numを下回らない
    // ２のべき乗の数を求める．
    SizeType ans = 16;
    while ( ans < cube_num ) {
      ans *= 2;
    }
    return ans;
  }

  /// @relates AlgCover
  /// @brief 比較演算子(rich compare)
  /// @return 比較結果を返す．
  ///
  /// 比較方法はキューブごとの辞書式順序
  friend
  int
  compare(
    const AlgCover& left, ///< [in] 第1オペランド
    const AlgCover& right ///< [in] 第2オペランド
  )
  {
    ASSERT_COND( &left.mgr() == &right.mgr() );

    return left.mgr().compare(left.cube_num(), left.mBody,
			      right.cube_num(), right.mBody);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブマネージャ
  AlgMgr* mMgr{nullptr};

  // キューブ数
  SizeType mCubeNum{0};

  // mBody の実際に確保されているキューブ容量
  SizeType mCubeCap{0};

  // 内容を表すビットベクタ
  ymuint64* mBody{nullptr};

};

/// @relates AlgCover
/// @brief 比較演算子 (EQ)
/// @return 等しい時に true を返す．
inline
bool
operator==(
  const AlgCover& left, ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == 0;
}

/// @relates AlgCover
/// @brief 比較演算子 (NE)
/// @return 等しくない時に true を返す．
inline
bool
operator!=(
  const AlgCover& left, ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) != 0;
}

/// @relates AlgCover
/// @brief 比較演算子 (LT)
//// @return left が right より小さい時に true を返す．
inline
bool
operator<(
  const AlgCover& left, ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) < 0;
}

/// @relates AlgCover
/// @brief 比較演算子 (GT)
/// @return left が right より大きい時に true を返す．
inline
bool
operator>(
  const AlgCover& left, ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) > 0;
}

/// @relates AlgCover
/// @brief 比較演算子 (LE)
/// @return left が right と等しいか小さい時に true を返す．
inline
bool
operator<=(
  const AlgCover& left, ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) <= 0;
}

/// @relates AlgCover
/// @brief 比較演算子 (GE)
/// @return left が right と等しいか大きい時に true を返す．
inline
bool
operator>=(
  const AlgCover& left, ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) >= 0;
}

/// @relates AlgCover
/// @brief AlgCover の内容を出力する．
/// @return ストリームを返す．
///
/// cover.print(s) と等価
inline
ostream&
operator<<(
  ostream& s,           ///< [in] 出力先のストリーム
  const AlgCover& cover ///< [in] 対象のカバー
)
{
  cover.print(s);
  return s;
}

END_NAMESPACE_YM_BFO

BEGIN_NAMESPACE_STD

/// @breif AlgCover をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::AlgCover>
{
  SizeType
  operator()(
    const YM_NAMESPACE::AlgCover& cover
  ) const
  {
    return cover.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_ALGCOVER_H
