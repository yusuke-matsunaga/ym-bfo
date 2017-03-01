#ifndef ALGKERNELGEN_H
#define ALGKERNELGEN_H

/// @file AlgKernelGen.h
/// @brief AlgKernelGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/AlgCover.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class カーネル(とコカーネル)の情報を表す構造体
//////////////////////////////////////////////////////////////////////
struct AlgKernelInfo
{
  /// @brief 空のコンストラクタ
  AlgKernelInfo(AlgMgr& mgr);

  /// @brief カーネル
  AlgCover mKernel;

  /// @brief コカーネルの集合
  ///
  /// カバーの形で表す．
  AlgCover mCoKernel;

  /// @brief カーネルのレベル
  ymuint mLevel;

};


//////////////////////////////////////////////////////////////////////
/// @class AlgKernelGen AlgKernelGen.h "AlgKernelGen.h"
/// @brief カーネルを求めるクラス
//////////////////////////////////////////////////////////////////////
class AlgKernelGen
{
public:

  /// @brief コンストラクタ
  AlgKernelGen();

  /// @brief デストラクタ
  ~AlgKernelGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  /// @param[in] cover 対象のカバー
  /// @param[out] kernel_list 結果を格納するリスト
  void
  generate(const AlgCover& cover,
	   vector<AlgKernelInfo>& kernel_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルを求める下請け関数
  /// @param[in] cover 対象のカバー
  /// @param[in] pos mLitList 上の位置
  /// @param[in] ccube 今までに括りだされた共通のキューブ
  /// @param[in] plits mLitList[0]〜mLitList[pos - 1] までをまとめたリテラル集合
  /// @return cover に含まれるカーネルのレベルの最大値を返す．
  ymuint
  kern_sub(const AlgCover& cover,
	   ymuint pos,
	   const AlgCube& ccube,
	   const AlgLitSet& plits);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在対象としているカバーのリテラルのリスト
  // 出現頻度の昇順になっている．
  vector<AlgLiteral> mLitList;

  // 見つかったカーネルを入れておく作業用のリスト
};

END_NAMESPACE_YM_BFO

#endif // ALGKERNELGEN_H
