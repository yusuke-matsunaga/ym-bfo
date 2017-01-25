
/// @file CubeTest.cc
/// @brief CubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/BfoCube.h"
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
  BfoCube* cube = mgr().new_cube();

  EXPECT_EQ( mgr().variable_num(), cube->variable_num() );

  ymuint n = cube->variable_num();
  for (ymuint i = 0; i < n; ++ i) {
    BfoLiteral lit = cube->literal(i);
    EXPECT_EQ( kBfoLitX, lit ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, set_literal)
{
  BfoCube* cube = mgr().new_cube();

  ymuint n = cube->variable_num();
  for (ymuint i = 0; i < n; ++ i) {
    cube->set_literal(i, kBfoLitP);
  }
  EXPECT_EQ( kBfoLitP, cube->literal(0) );

  cube->set_literal(1, kBfoLitX);
  EXPECT_EQ( kBfoLitX, cube->literal(1) );

  cube->set_literal(2, kBfoLitN);
  EXPECT_EQ( kBfoLitN, cube->literal(2) );
}

TEST_F(CubeTest, copy)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoLitP);
  cube1->set_literal(10, kBfoLitN);

  BfoCube* cube2 = mgr().new_cube();
  cube2->copy(cube1);

  EXPECT_EQ( kBfoLitP, cube2->literal(0) );
  EXPECT_EQ( kBfoLitX, cube2->literal(1) );
  EXPECT_EQ( kBfoLitN, cube2->literal(10) );

  EXPECT_EQ( 0, compare(cube1, cube2) );
}

TEST_F(CubeTest, check_contain)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoLitP);
  cube1->set_literal(1, kBfoLitP);
  cube1->set_literal(2, kBfoLitN);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(0, kBfoLitP);
  cube2->set_literal(2, kBfoLitN);

  EXPECT_TRUE( cube1->check_contain(cube2) );
  EXPECT_FALSE( cube2->check_contain(cube1) );
}

TEST_F(CubeTest, make_product1)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoLitP);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(1, kBfoLitN);

  EXPECT_TRUE( cube1->make_product(cube2) );
  EXPECT_EQ( kBfoLitP, cube1->literal(0) );
  EXPECT_EQ( kBfoLitN, cube1->literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoLitX, cube1->literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, make_product2)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoLitP);
  cube1->set_literal(1, kBfoLitP);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(1, kBfoLitN);

  EXPECT_FALSE( cube1->make_product(cube2) );
}

TEST_F(CubeTest, make_diff)
{
  BfoCube* cube1 = mgr().new_cube();
  cube1->set_literal(0, kBfoLitP);
  cube1->set_literal(1, kBfoLitP);

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(1, kBfoLitP);

  cube1->make_diff(cube2);
  EXPECT_EQ( kBfoLitP, cube1->literal(0) );
  EXPECT_EQ( kBfoLitX, cube1->literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoLitX, cube1->literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, literal_num)
{
  BfoCube* cube1 = mgr().new_cube();
  EXPECT_EQ( 0, cube1->literal_num() );

  BfoCube* cube2 = mgr().new_cube();
  cube2->set_literal(0, kBfoLitP);
  cube2->set_literal(10, kBfoLitN);
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
  cube->set_literal(0, kBfoLitP);
  cube->set_literal(10, kBfoLitN);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("a k'"), tmp.str() );
}

TEST_F(CubeTest, print3)
{
  BfoCube* cube = mgr().new_cube();
  cube->set_literal(0, kBfoLitP);
  cube->set_literal(10, kBfoLitN);

  vector<string> varname_list(mgr().variable_num());
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }

  ostringstream tmp;
  cube->print(tmp, varname_list);

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
  cube->set_literal(0, kBfoLitP);
  cube->set_literal(10, kBfoLitN);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("aa ak'"), tmp.str() );
}

TEST_F(CubeTest2, print3)
{
  BfoCube* cube = mgr().new_cube();
  cube->set_literal(30, kBfoLitP);
  cube->set_literal(60, kBfoLitN);

  ostringstream tmp;
  tmp << cube;
  EXPECT_EQ( string("be ci'"), tmp.str() );
}

TEST_F(CubeTest2, print4)
{
  BfoCube* cube = mgr().new_cube();
  cube->set_literal(30, kBfoLitP);
  cube->set_literal(60, kBfoLitN);

  vector<string> varname_list(mgr().variable_num());
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }

  ostringstream tmp;
  cube->print(tmp, varname_list);

  EXPECT_EQ( string("x30 x60'"), tmp.str() );
}

END_NAMESPACE_YM_BFO