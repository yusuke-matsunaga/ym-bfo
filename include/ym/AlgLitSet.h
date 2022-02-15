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
  AlgLitSet(
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

  /// @brief 一般的なコンストラクタ
  ///
  /// 空集合を作るコンストラクタでもある．
  explicit
  AlgLitSet(
    AlgMgr& mgr,                            ///< [in] マネージャ
    const vector<AlgLiteral>& lit_list = {} ///< [in] リテラルのリスト
  ) : mMgr{&mgr},
      mBody{mMgr->new_body()}
  {
    mMgr->set_literal(mBody, 0, lit_list);
  }

  /// @brief コピーコンストラクタ
  AlgLitSet(
    const AlgLitSet& src ///< [in] コピー元のオブジェクト
  ) : mMgr{src.mMgr},
      mBody{mMgr->new_body()}
  {
    mMgr->cube_copy(mBody, 0, src.mBody, 0);
  }

  /// @brief 代入演算子
  /// @return 代入後の自身への参照を返す．
  AlgLitSet&
  operator=(
    const AlgLitSet& src ///< [in] コピー元のオブジェクト
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
  ~AlgLitSet()
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

  /// @brief 該当するリテラルが含まれているか調べる．
  /// @retval true 含まれている．
  /// @retval false 含まれていない．
  bool
  is_in(
    AlgLiteral lit ///< [in] 対象のリテラル
  ) const
  {
    return mgr().is_in(mBody, lit);
  }

  /// @brief 要素を足す．
  /// @return 演算結果を返す．
  AlgLitSet
  operator+(
    AlgLiteral lit ///< [in] 追加するリテラル
  ) const
  {
    return AlgLitSet(*this).operator+=(lit);
  }

  /// @brief 要素を足す．
  /// @return 自身への参照を返す．
  AlgLitSet&
  operator+=(
    AlgLiteral lit ///< [in] 追加するリテラル
  )
  {
    mgr().add_lit(mBody, lit);

    return *this;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AlgMgr* mMgr;

  // 内容を表すビットベクタ
  ymuint64* mBody;

};

END_NAMESPACE_YM_BFO

#endif // ALGLITSET_H
