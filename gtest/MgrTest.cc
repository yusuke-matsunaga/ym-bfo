
/// @file MgrTest.cc
/// @brief BfoMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/BfoMgr.h"
#include "ym/BfoCube.h"
#include "ym/BfoCover.h"


BEGIN_NAMESPACE_YM_BFO

class MgrTest :
  public ::testing::Test
{
public:

  /// @brief コンストラクタ
  MgrTest() : mMgr(20) { }


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

#if 0
TEST_F(MgrTest, new_delete)
{
  for (ymuint i = 0; i < 10000000; ++ i) {
    BfoCube cube;

  }
}

TEST_F(MgrTest, make_sum)
{
  BfoCover cover1(mgr());
  BfoCover cover2(mgr());

  BfoCube* cube1 = cover1.add_cube();
  cube1->set_literal(0, kBfoLitP);

  BfoCube* cube2 = cover1.add_cube();
  cube2->set_literal(1, kBfoLitN);

  BfoCube* cube3 = cover2.add_cube();
  cube3->set_literal(0, kBfoLitP);

  BfoCube* cube4 = cover2.add_cube();
  cube4->set_literal(2, kBfoLitP);

  BfoCover cover3(mgr());
  mgr().make_sum(cover1, cover2, cover3);

  ostringstream tmp;
  tmp << cover3;

  EXPECT_EQ( string("a + b' + c"), tmp.str() );
}
#endif

END_NAMESPACE_YM_BFO
