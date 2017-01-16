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

/// @brief リテラルを表す列挙型
enum BfoLiteral {
  /// @brief なし
  kBfoLitX = 0,
  /// @brief 否定のリテラル
  kBfoLitN = 1,
  /// @brief 肯定のリテラル
  kBfoLitP = 2,
};

// クラスの前方宣言
class BfoCube;
class BfoCover;
class BfoMgr;

END_NAMESPACE_YM_BFO

BEGIN_NAMESPACE_YM

// 親の名前空間にインポートする．
using nsBfo::BfoLiteral;
using nsBfo::BfoCube;
using nsBfo::BfoCover;
using nsBfo::BfoMgr;

END_NAMESPACE_YM

#endif // YM_BFO_NSDEF_H
