
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


TEST_F(CoverTest, nullcover)
{
  BfoCover cover(mgr());

  EXPECT_EQ( mgr().variable_num(), cover.variable_num() );
  EXPECT_EQ( 0, cover.cube_num() );
  EXPECT_EQ( 0, cover.literal_num() );
}

TEST_F(CoverTest, add_cube1)
{
  BfoCover cover(mgr());

  BfoCube* cube = cover.add_cube();
  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( mgr().variable_num(), cube->variable_num() );
  EXPECT_EQ( 0, cube->literal_num() );

  cube->set_literal(0, kBfoLitP);
  cube->set_literal(1, kBfoLitN);

  // cover.cube(0) が今設定した cube であることを確認する．
  const BfoCube* cube1 = cover.cube(0);
  EXPECT_EQ( kBfoLitP, cube1->literal(0) );
  EXPECT_EQ( kBfoLitN, cube1->literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoLitX, cube1->literal(i) );
  }

  EXPECT_EQ( 2, cover.literal_num() );
}

TEST_F(CoverTest, add_cube2)
{
  BfoCover cover(mgr());

  BfoCube* src_cube = mgr().new_cube();

  src_cube->set_literal(0, kBfoLitP);
  src_cube->set_literal(1, kBfoLitN);

  cover.add_cube(src_cube);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 2, cover.literal_num() );

  // cover.cube(0) が今設定した src_cube と等しいことを確認する．
  const BfoCube* cube1 = cover.cube(0);
  EXPECT_EQ( kBfoLitP, cube1->literal(0) );
  EXPECT_EQ( kBfoLitN, cube1->literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoLitX, cube1->literal(i) );
  }
}

TEST_F(CoverTest, clear)
{
  BfoCover cover(mgr());

  BfoCube* src_cube = mgr().new_cube();

  src_cube->set_literal(0, kBfoLitP);
  src_cube->set_literal(1, kBfoLitN);

  cover.add_cube(src_cube);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 2, cover.literal_num() );

  cover.clear();

  EXPECT_EQ( 0, cover.cube_num() );
  EXPECT_EQ( 0, cover.literal_num() );
}

TEST_F(CoverTest, print1)
{
  BfoCover cover(mgr());

  ostringstream tmp;
  tmp << cover;

  EXPECT_EQ( string(), tmp.str() );
}

TEST_F(CoverTest, print2)
{
  BfoCover cover(mgr());

  BfoCube* cube1 = cover.add_cube();
  cube1->set_literal(0, kBfoLitP);
  cube1->set_literal(1, kBfoLitN);

  BfoCube* cube2 = cover.add_cube();
  cube2->set_literal(0, kBfoLitN);
  cube2->set_literal(2, kBfoLitP);

  ostringstream tmp;
  tmp << cover;

  EXPECT_EQ( string("aa ab' + aa' ac"), tmp.str() );
}

TEST_F(CoverTest, print3)
{
  BfoCover cover(mgr());

  BfoCube* cube1 = cover.add_cube();
  cube1->set_literal(0, kBfoLitP);
  cube1->set_literal(1, kBfoLitN);

  BfoCube* cube2 = cover.add_cube();
  cube2->set_literal(0, kBfoLitN);
  cube2->set_literal(2, kBfoLitP);

  vector<string> varname_list(mgr().variable_num());
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }

  ostringstream tmp;
  cover.print(tmp, varname_list);

  EXPECT_EQ( string("x0 x1' + x0' x2"), tmp.str() );
}

END_NAMESPACE_YM_BFO
