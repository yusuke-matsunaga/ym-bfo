
/// @file CubeTest.cc
/// @brief CubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/BfoCube.h"
#include "ym/BfoLiteral.h"
#include "ym/BfoMgr.h"


BEGIN_NAMESPACE_YM_BFO

class CubeTest :
  public ::testing::Test
{
public:

  /// @brief コンストラクタ
  CubeTest() : mMgr(20) { }


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

// 変数の数が違うだけのクラス
// もっとスマートなやり方があるはず．
class CubeTest2 :
  public ::testing::Test
{
public:

  /// @brief コンストラクタ
  CubeTest2() : mMgr(100) { }


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

TEST_F(CubeTest, nullcube)
{
  BfoCube cube(mgr());

  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  for (ymuint i = 0; i < n; ++ i) {
    EXPECT_EQ( kBfoPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, set_literal)
{
  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(0, false));
  lit_list.push_back(BfoLiteral(2, true));

  BfoCube cube(mgr(), lit_list);

  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );
  EXPECT_EQ( 2, cube.literal_num() );
  EXPECT_EQ( kBfoPolP, cube.literal(0) ) << "cube[0] is not 1";
  EXPECT_EQ( kBfoPolX, cube.literal(1) ) << "cube[1] is not X";
  EXPECT_EQ( kBfoPolN, cube.literal(2) ) << "cube[2] is not 0";
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, copy)
{
  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(0, false));
  lit_list.push_back(BfoLiteral(10, true));

  BfoCube src_cube(mgr(), lit_list);

  BfoCube cube(src_cube);

  EXPECT_EQ( kBfoPolP, cube.literal(0) ) << "cube[0] is not 1";
  EXPECT_EQ( kBfoPolN, cube.literal(10) ) << "cube[10] is not 0";
  for (ymuint i = 1; i < mgr().variable_num(); ++ i) {
    if ( i == 10 ) {
      continue;
    }
    EXPECT_EQ( kBfoPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }

  EXPECT_EQ( src_cube, cube );
}

#if 0
TEST_F(CubeTest, check_contain)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoPolP);
  cube1->set_literal(1, kBfoPolP);
  cube1->set_literal(2, kBfoPolN);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(0, kBfoPolP);
  cube2->set_literal(2, kBfoPolN);

  EXPECT_TRUE( cube1->check_contain(cube2) );
  EXPECT_FALSE( cube2->check_contain(cube1) );
}

TEST_F(CubeTest, make_product1)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoPolP);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(1, kBfoPolN);

  EXPECT_TRUE( cube1->make_product(cube2) );
  EXPECT_EQ( kBfoPolP, cube1->literal(0) );
  EXPECT_EQ( kBfoPolN, cube1->literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube1->literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, make_product2)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoPolP);
  cube1->set_literal(1, kBfoPolP);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(1, kBfoPolN);

  EXPECT_FALSE( cube1->make_product(cube2) );
}

TEST_F(CubeTest, make_diff)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoPolP);
  cube1->set_literal(1, kBfoPolP);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(1, kBfoPolP);

  cube1->make_diff(cube2);
  EXPECT_EQ( kBfoPolP, cube1->literal(0) );
  EXPECT_EQ( kBfoPolX, cube1->literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube1->literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, literal_num)
{
  BfoCube* cube1 = mgr().new_cube();
  EXPECT_EQ( 0, cube1->literal_num() );

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(0, kBfoPolP);
  cube2->set_literal(10, kBfoPolN);
  EXPECT_EQ( 2, cube2->literal_num() );
}

TEST_F(CubeTest, print1)
{
  BfoCube* cube= mgr().new_cube();

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string(), tmp.str() );
}

TEST_F(CubeTest, print2)
{
  BfoCube* cube = mgr().new_cube();
  cube.set_literal(0, kBfoPolP);
  cube.set_literal(10, kBfoPolN);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("a k'"), tmp.str() );
}

TEST_F(CubeTest, print3)
{
  BfoCube* cube = mgr().new_cube();
  cube.set_literal(0, kBfoPolP);
  cube.set_literal(10, kBfoPolN);

  vector<string> varname_list(mgr().variable_num());
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }

  ostringstream tmp;
  cube.print(tmp, varname_list);

  EXPECT_EQ( string("x0 x10'"), tmp.str() );
}

TEST_F(CubeTest2, print1)
{
  BfoCube* cube = mgr().new_cube();

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string(), tmp.str() );
}

TEST_F(CubeTest2, print2)
{
  BfoCube* cube = mgr().new_cube();
  cube.set_literal(0, kBfoPolP);
  cube.set_literal(10, kBfoPolN);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("aa ak'"), tmp.str() );
}

TEST_F(CubeTest2, print3)
{
  BfoCube* cube = mgr().new_cube();
  cube.set_literal(30, kBfoPolP);
  cube.set_literal(60, kBfoPolN);

  ostringstream tmp;
  tmp << cube;
  EXPECT_EQ( string("be ci'"), tmp.str() );
}

TEST_F(CubeTest2, print4)
{
  BfoCube* cube = mgr().new_cube();
  cube.set_literal(30, kBfoPolP);
  cube.set_literal(60, kBfoPolN);

  vector<string> varname_list(mgr().variable_num());
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }

  ostringstream tmp;
  cube.print(tmp, varname_list);

  EXPECT_EQ( string("x30 x60'"), tmp.str() );
}
#endif
END_NAMESPACE_YM_BFO
