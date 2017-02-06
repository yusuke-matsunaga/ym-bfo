#ifndef YM_BFO_NSDEF_H
#define YM_BFO_NSDEF_H

/// @file ym/bfo_nsdef.h
/// @brief ym-bfo の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BFO Boolean Function Optimizer
///
/// 論理関数の簡単化パッケージ<br>
/// 具体的には
/// - 積和形論理式簡単化
/// - ファクタリング
/// を行う．


#include "ym_config.h"


/// @brief bfo 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BFO \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBfo)

/// @brief blif 用の名前空間の終了
#define END_NAMESPACE_YM_BFO \
END_NAMESPACE(nsBfo) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_BFO

// クラスの前方宣言
class AlgLiteral;
class AlgCube;
class AlgCover;
class AlgKernel;
class AlgMgr;

END_NAMESPACE_YM_BFO

BEGIN_NAMESPACE_YM

/// @brief リテラルの極性を表す列挙型
enum AlgPol {
  /// @brief なし
  kAlgPolX = 0,
  /// @brief 否定のリテラル
  kAlgPolN = 1,
  /// @brief 肯定のリテラル
  kAlgPolP = 2,
};

// 親の名前空間にインポートする．
using nsBfo::AlgLiteral;
using nsBfo::AlgCube;
using nsBfo::AlgCover;
using nsBfo::AlgKernel;
using nsBfo::AlgMgr;

END_NAMESPACE_YM

#endif // YM_BFO_NSDEF_H
