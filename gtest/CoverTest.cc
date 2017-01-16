
/// @file CoverTest.cc
/// @brief CoverTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/BfoCube.h"
#include "ym/BfoCover.h"
#include "ym/BfoMgr.h"


BEGIN_NAMESPACE_YM_BFO

class CoverTest :
  public ::testing::Test
{
public:

  /// @brief コンストラクタ
  CoverTest() : mMgr(30) { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  BfoMgr&
  mgr() { return mMgr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BfoMgr mMgr;

};


TEST_F(CubeTest,

END_NAMESPACE_YM_BFO
