#ifndef BFOMGR_H
#define BFOMGR_H

/// @file BfoMgr.h
/// @brief BfoMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/UnitAlloc.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class BfoMgr BfoMgr.h "BfoMgr.h"
/// @brief BfoCube, BfoCover を管理するクラス
///
/// BfoCube は特殊な構造を持っているため普通のコンストラクタ/デストラクタ
/// が利用できない．
/// 全てこのクラスを用いて生成/破壊を行う．
//////////////////////////////////////////////////////////////////////
class BfoMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  BfoMgr(ymuint variable_num);

  /// @brief デストラクタ
  ~BfoMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  ymuint
  variable_num() const;

  /// @brief キューブを生成する．
  ///
  /// 内容はすべて kBfoLitX で初期化される．
  BfoCube*
  new_cube();

  /// @brief キューブを削除する．
  /// @param[in] cube 対象のキューブ
  void
  delete_cube(BfoCube* cube);

  /// @brief カバーの和を計算する．
  /// @param[in] left, right オペランド
  /// @param[out] ans 結果を格納するオブジェクト
  void
  make_sum(const BfoCover& left,
	   const BfoCover& right,
	   BfoCover& ans);

  /// @brief カバーの差を計算する．
  /// @param[in] left, right オペランド
  /// @param[out] ans 結果を格納するオブジェクト
  void
  make_diff(const BfoCover& left,
	    const BfoCover& right,
	    BfoCover& ans);

  /// @brief カバーの積を計算する．
  /// @param[in] left, right オペランド
  /// @param[out] ans 結果を格納するオブジェクト
  void
  make_product(const BfoCover& left,
	       const BfoCover& right,
	       BfoCover& ans);

  /// @brief カバーの商を計算する．
  /// @param[in] left, right オペランド
  /// @param[out] ans 結果を格納するオブジェクト
  void
  make_division(const BfoCover& left,
		const BfoCover& right,
		BfoCover& ans);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BfoCube の本当のサイズ
  ymuint
  _cube_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  ymuint mVarNum;

  // キューブ用のメモリアロケータ
  UnitAlloc mAlloc;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
ymuint
BfoMgr::variable_num() const
{
  return mVarNum;
}

END_NAMESPACE_YM_BFO

#endif // BFOCUBEMGR_H
