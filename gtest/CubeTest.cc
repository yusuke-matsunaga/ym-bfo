
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

  EXPECT_EQ( &mgr(), &cube.mgr() );
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

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );
  EXPECT_EQ( 2, cube.literal_num() );
  EXPECT_EQ( kBfoPolP, cube.literal(0) ) << "cube[0] is not 1";
  EXPECT_EQ( kBfoPolX, cube.literal(1) ) << "cube[1] is not X";
  EXPECT_EQ( kBfoPolN, cube.literal(2) ) << "cube[2] is not 0";
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, single_literal)
{
  BfoCube cube(mgr(), BfoLiteral(0, false));

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  EXPECT_EQ( kBfoPolP, cube.literal(0) );
  for (ymuint i = 1; i < n; ++ i) {
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

  EXPECT_EQ( &mgr(), &cube.mgr() );
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

TEST_F(CubeTest, assign)
{
  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(0, false));
  lit_list.push_back(BfoLiteral(10, true));

  BfoCube src_cube(mgr(), lit_list);

  BfoMgr another_mgr(100);
  BfoCube cube(another_mgr);

  cube = src_cube;

  EXPECT_EQ( &mgr(), &cube.mgr() );
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

TEST_F(CubeTest, check_containment)
{
  vector<BfoLiteral> lit_list1;
  lit_list1.push_back(BfoLiteral(0, false));
  lit_list1.push_back(BfoLiteral(1, false));
  lit_list1.push_back(BfoLiteral(2, true));
  BfoCube cube1(mgr(), lit_list1);

  vector<BfoLiteral> lit_list2;
  lit_list2.push_back(BfoLiteral(0, false));
  lit_list2.push_back(BfoLiteral(2, true));
  BfoCube cube2(mgr(), lit_list2);

  EXPECT_TRUE( cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );
}

TEST_F(CubeTest, check_intersect)
{
  vector<BfoLiteral> lit_list1;
  lit_list1.push_back(BfoLiteral(0, false));
  lit_list1.push_back(BfoLiteral(1, false));
  BfoCube cube1(mgr(), lit_list1);

  vector<BfoLiteral> lit_list2;
  lit_list2.push_back(BfoLiteral(0, false));
  lit_list2.push_back(BfoLiteral(2, true));
  BfoCube cube2(mgr(), lit_list2);

  EXPECT_TRUE( cube1.check_intersect(cube2) );
  EXPECT_TRUE( cube2.check_intersect(cube1) );

  vector<BfoLiteral> lit_list3;
  lit_list3.push_back(BfoLiteral(0, true));
  lit_list3.push_back(BfoLiteral(3, false));
  BfoCube cube3(mgr(), lit_list3);

  EXPECT_FALSE( cube1.check_intersect(cube3) );
  EXPECT_FALSE( cube3.check_intersect(cube1) );
}

TEST_F(CubeTest, product1)
{
  BfoCube cube1(mgr(), BfoLiteral(0, false));
  BfoCube cube2(mgr(), BfoLiteral(1, true));

  cube1 &= cube2;

  EXPECT_EQ( 2, cube1.literal_num() );
  EXPECT_EQ( kBfoPolP, cube1.literal(0) );
  EXPECT_EQ( kBfoPolN, cube1.literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube1.literal(i) );
  }
}

TEST_F(CubeTest, product2)
{
  BfoCube cube1(mgr(), BfoLiteral(0, false));
  BfoCube cube2(mgr(), BfoLiteral(1, true));

  BfoCube cube3 = cube1 & cube2;

  EXPECT_EQ( 2, cube3.literal_num() );
  EXPECT_EQ( kBfoPolP, cube3.literal(0) );
  EXPECT_EQ( kBfoPolN, cube3.literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube3.literal(i) );
  }
}

TEST_F(CubeTest, product3)
{
  vector<BfoLiteral> lit_list1;
  lit_list1.push_back(BfoLiteral(0, false));
  lit_list1.push_back(BfoLiteral(1, false));
  BfoCube cube1(mgr(), lit_list1);

  BfoCube cube2(mgr(), BfoLiteral(1, true));

  cube1 &= cube2;

  EXPECT_EQ( 0, cube1.literal_num() );
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube1.literal(i) );
  }
}

TEST_F(CubeTest, product4)
{
  vector<BfoLiteral> lit_list1;
  lit_list1.push_back(BfoLiteral(0, false));
  lit_list1.push_back(BfoLiteral(1, false));
  BfoCube cube1(mgr(), lit_list1);

  BfoCube cube2(mgr(), BfoLiteral(1, true));

  BfoCube cube3 = cube1 & cube2;

  EXPECT_EQ( 0, cube3.literal_num() );
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube3.literal(i) );
  }
}

TEST_F(CubeTest, cofacter1)
{
  vector<BfoLiteral> lit_list1;
  lit_list1.push_back(BfoLiteral(0, false));
  lit_list1.push_back(BfoLiteral(1, false));
  BfoCube cube1(mgr(), lit_list1);

  BfoCube cube2(mgr(), BfoLiteral(1, false));

  cube1 /= cube2;

  EXPECT_EQ( 1, cube1.literal_num() );
  EXPECT_EQ( kBfoPolP, cube1.literal(0) );
  EXPECT_EQ( kBfoPolX, cube1.literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube1.literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, cofacter2)
{
  vector<BfoLiteral> lit_list1;
  lit_list1.push_back(BfoLiteral(0, false));
  lit_list1.push_back(BfoLiteral(1, false));
  BfoCube cube1(mgr(), lit_list1);

  BfoCube cube2(mgr(), BfoLiteral(2, false));

  cube1 /= cube2;

  EXPECT_EQ( 0, cube1.literal_num() );
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cube1.literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, print1)
{
  BfoCube cube(mgr());

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string(), tmp.str() );
}

TEST_F(CubeTest, print2)
{
  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(0, false));
  lit_list.push_back(BfoLiteral(10, true));
  BfoCube cube(mgr(), lit_list);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("a k'"), tmp.str() );
}

TEST_F(CubeTest2, print2)
{
  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(0, false));
  lit_list.push_back(BfoLiteral(10, true));
  BfoCube cube(mgr(), lit_list);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("a k'"), tmp.str() );
}

TEST_F(CubeTest2, print3)
{
  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(30, false));
  lit_list.push_back(BfoLiteral(60, true));
  BfoCube cube(mgr(), lit_list);

  ostringstream tmp;
  tmp << cube;
  EXPECT_EQ( string("be ci'"), tmp.str() );
}

TEST(CubeTest3, print4)
{
  ymuint n = 100;
  vector<string> varname_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }
  BfoMgr mgr(varname_list);

  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(30, false));
  lit_list.push_back(BfoLiteral(60, true));
  BfoCube cube(mgr, lit_list);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("x30 x60'"), tmp.str() );
}

END_NAMESPACE_YM_BFO
