#ifndef ALGLITSET_H
#define ALGLITSET_H

/// @file AlgLitSet.h
/// @brief AlgLitSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/AlgLiteral.h"
#include "ym/AlgMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class AlgLitSet AlgLitSet.h "AlgLitSet.h"
/// @brief AlgLiteral の集合を表すクラス
///
/// AlgCube と異なり，相反するリテラル(x と x')を同時に含んでも良い．
//////////////////////////////////////////////////////////////////////
class AlgLitSet
{
  friend class AlgCube;

public:

  /// @brief シングルトンのコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] lit リテラル
  AlgLitSet(AlgMgr& mgr,
	    AlgLiteral lit);

  /// @brief 一般的なコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] lit_list リテラルのリスト
  ///
  /// 空集合を作るコンストラクタでもある．
  explicit
  AlgLitSet(AlgMgr& mgr,
	    const vector<AlgLiteral>& lit_list = vector<AlgLiteral>());

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  AlgLitSet(const AlgLitSet& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  const AlgLitSet&
  operator=(const AlgLitSet& src);

  /// @brief デストラクタ
  ~AlgLitSet();


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

  /// @brief 該当するリテラルが含まれているか調べる．
  /// @param[in] lit 対象のリテラル
  /// @retval true 含まれている．
  /// @retval false 含まれていない．
  bool
  is_in(AlgLiteral lit) const;

  /// @brief 要素を足す．
  /// @param[in] lit 追加するリテラル
  /// @return 自身への参照を返す．
  const AlgLitSet&
  operator+=(AlgLiteral lit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AlgMgr* mMgr;

  // 内容を表すビットベクタ
  ymuint64* mBody;

};

/// @brief 要素を足す．
/// @param[in] lit_set リテラル集合の本体
/// @param[in] lit 追加するリテラル
/// @return 演算結果を返す．
AlgLitSet
operator+(const AlgLitSet& lit_set,
	  AlgLiteral lit);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] lit リテラル
//
// 単一のリテラルからなるキューブを作る．
inline
AlgLitSet::AlgLitSet(AlgMgr& mgr,
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
AlgLitSet::AlgLitSet(AlgMgr& mgr,
		     const vector<AlgLiteral>& lit_list) :
  mMgr(&mgr),
  mBody(mMgr->new_body())
{
  mMgr->set_literal(mBody, 0, lit_list);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
AlgLitSet::AlgLitSet(const AlgLitSet& src) :
  mMgr(src.mMgr)
{
  mBody = mMgr->new_body();
  mMgr->cube_copy(mBody, 0, src.mBody, 0);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身への参照を返す．
inline
const AlgLitSet&
AlgLitSet::operator=(const AlgLitSet& src)
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
AlgLitSet::~AlgLitSet()
{
  mMgr->delete_body(mBody);
}

// @brief マネージャを返す．
inline
AlgMgr&
AlgLitSet::mgr() const
{
  return *mMgr;
}

// @brief 変数の数を返す．
inline
ymuint
AlgLitSet::variable_num() const
{
  return mgr().variable_num();
}

// @brief 該当するリテラルが含まれているか調べる．
// @param[in] lit 対象のリテラル
// @retval true 含まれている．
// @retval false 含まれていない．
inline
bool
AlgLitSet::is_in(AlgLiteral lit) const
{
  return mgr().is_in(mBody, lit);
}

// @brief 要素を足す．
// @param[in] lit 追加するリテラル
// @return 自身への参照を返す．
inline
const AlgLitSet&
AlgLitSet::operator+=(AlgLiteral lit)
{
  mgr().add_lit(mBody, lit);

  return *this;
}

// @brief 要素を足す．
// @param[in] lit_set リテラル集合の本体
// @param[in] lit 追加するリテラル
// @return 演算結果を返す．
inline
AlgLitSet
operator+(const AlgLitSet& lit_set,
	  AlgLiteral lit)
{
  return AlgLitSet(lit_set).operator+=(lit);
}

END_NAMESPACE_YM_BFO

#endif // ALGLITSET_H
