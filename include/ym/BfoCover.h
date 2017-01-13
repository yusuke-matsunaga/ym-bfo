#ifndef BFOCOVER_H
#define BFOCOVER_H

/// @file BfoCover.h
/// @brief BfoCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class BfoCover BfoCover.h "BfoCover.h"
/// @brief カバー(積和形論理式)を表すクラス
///
/// 中身は BfoCube のリスト<br>
/// BfoCube は常に唯一つの BfoCover に属する<br>
//////////////////////////////////////////////////////////////////////
class BfoCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr ネージャ
  ///
  /// キューブを一つも持たない空のカバーとなる．
  BfoCover(BfoMgr& mgr);

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~BfoCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  ymuint
  variable_num() const;

  /// @brief キューブの数を返す．
  ymuint
  cube_num() const;

  /// @brief キューブを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < cube_num() )
  const BfoCube*
  cube(ymuint pos) const;

  /// @brief 内容をクリアする．
  ///
  /// ここに属しているすべてのキューブは削除される．
  void
  clear();

  /// @brief 新しいキューブを追加する．
  /// @param[in] cube 追加するキューブ
  ///
  /// 実際には cube のコピーを追加する．
  void
  add_cube(const BfoCube* cube);

  /// @brief 2つのキューブの積を計算して追加する．
  /// @param[in] cube1, cube2 積をとるキューブ
  ///
  /// 積が空キューブになった場合には追加しない．
  void
  add_product(const BfoCube* cube1,
	      const BfoCube* cube2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブマネージャ
  BfoMgr& mMgr;

  // キューブのリスト
  vector<BfoCube*> mCubeList;

};

END_NAMESPACE_YM_BFO

#endif // BFOCOVER_H
