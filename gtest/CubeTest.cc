
/// @file CubeTest.cc
/// @brief CubeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/AlgCube.h"
#include "ym/AlgLiteral.h"
#include "ym/AlgMgr.h"


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
  AlgMgr&
  mgr() { return mMgr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AlgMgr mMgr;

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
  AlgMgr&
  mgr() { return mMgr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AlgMgr mMgr;

};

TEST_F(CubeTest, nullcube)
{
  AlgCube cube(mgr());

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  for (ymuint i = 0; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, set_literal)
{
  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(0, false));
  lit_list.push_back(AlgLiteral(2, true));

  AlgCube cube(mgr(), lit_list);

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );
  EXPECT_EQ( 2, cube.literal_num() );
  EXPECT_EQ( kAlgPolP, cube.literal(0) ) << "cube[0] is not 1";
  EXPECT_EQ( kAlgPolX, cube.literal(1) ) << "cube[1] is not X";
  EXPECT_EQ( kAlgPolN, cube.literal(2) ) << "cube[2] is not 0";
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, single_literal)
{
  AlgCube cube(mgr(), AlgLiteral(0, false));

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  EXPECT_EQ( kAlgPolP, cube.literal(0) );
  for (ymuint i = 1; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, from_cstr1)
{
  const char* str = "a b'";
  AlgCube cube(mgr(), str);

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  EXPECT_EQ( kAlgPolP, cube.literal(0) );
  EXPECT_EQ( kAlgPolN, cube.literal(1) );
  for (ymuint i = 2; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX,  cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, from_cstr2)
{
  const char* str = "b' a";
  AlgCube cube(mgr(), str);

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  EXPECT_EQ( kAlgPolP, cube.literal(0) );
  EXPECT_EQ( kAlgPolN, cube.literal(1) );
  for (ymuint i = 2; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX,  cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, from_bad_cstr)
{
  const char* str = "ba b'";
  AlgCube cube(mgr(), str);

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  EXPECT_EQ( 0, cube.literal_num() );
}

TEST_F(CubeTest, from_string1)
{
  string str("a b'");
  AlgCube cube(mgr(), str);

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  EXPECT_EQ( kAlgPolP, cube.literal(0) );
  EXPECT_EQ( kAlgPolN, cube.literal(1) );
  for (ymuint i = 2; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX,  cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, from_string2)
{
  string str("b' a");
  AlgCube cube(mgr(), str);

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( mgr().variable_num(), cube.variable_num() );

  ymuint n = cube.variable_num();
  EXPECT_EQ( kAlgPolP, cube.literal(0) );
  EXPECT_EQ( kAlgPolN, cube.literal(1) );
  for (ymuint i = 2; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX,  cube.literal(i) ) << "cube[" << i << "] is not X";
  }
}

TEST_F(CubeTest, copy)
{
  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(0, false));
  lit_list.push_back(AlgLiteral(10, true));

  AlgCube src_cube(mgr(), lit_list);

  AlgCube cube(src_cube);

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( kAlgPolP, cube.literal(0) ) << "cube[0] is not 1";
  EXPECT_EQ( kAlgPolN, cube.literal(10) ) << "cube[10] is not 0";
  for (ymuint i = 1; i < mgr().variable_num(); ++ i) {
    if ( i == 10 ) {
      continue;
    }
    EXPECT_EQ( kAlgPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }

  EXPECT_EQ( src_cube, cube );
}

TEST_F(CubeTest, assign)
{
  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(0, false));
  lit_list.push_back(AlgLiteral(10, true));

  AlgCube src_cube(mgr(), lit_list);

  AlgMgr another_mgr(100);
  AlgCube cube(another_mgr);

  cube = src_cube;

  EXPECT_EQ( &mgr(), &cube.mgr() );
  EXPECT_EQ( kAlgPolP, cube.literal(0) ) << "cube[0] is not 1";
  EXPECT_EQ( kAlgPolN, cube.literal(10) ) << "cube[10] is not 0";
  for (ymuint i = 1; i < mgr().variable_num(); ++ i) {
    if ( i == 10 ) {
      continue;
    }
    EXPECT_EQ( kAlgPolX, cube.literal(i) ) << "cube[" << i << "] is not X";
  }

  EXPECT_EQ( src_cube, cube );
}

TEST_F(CubeTest, check_containment)
{
  vector<AlgLiteral> lit_list1;
  lit_list1.push_back(AlgLiteral(0, false));
  lit_list1.push_back(AlgLiteral(1, false));
  lit_list1.push_back(AlgLiteral(2, true));
  AlgCube cube1(mgr(), lit_list1);

  vector<AlgLiteral> lit_list2;
  lit_list2.push_back(AlgLiteral(0, false));
  lit_list2.push_back(AlgLiteral(2, true));
  AlgCube cube2(mgr(), lit_list2);

  EXPECT_TRUE( cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );
}

TEST_F(CubeTest, check_intersect)
{
  vector<AlgLiteral> lit_list1;
  lit_list1.push_back(AlgLiteral(0, false));
  lit_list1.push_back(AlgLiteral(1, false));
  AlgCube cube1(mgr(), lit_list1);

  vector<AlgLiteral> lit_list2;
  lit_list2.push_back(AlgLiteral(0, false));
  lit_list2.push_back(AlgLiteral(2, true));
  AlgCube cube2(mgr(), lit_list2);

  EXPECT_TRUE( cube1.check_intersect(cube2) );
  EXPECT_TRUE( cube2.check_intersect(cube1) );

  vector<AlgLiteral> lit_list3;
  lit_list3.push_back(AlgLiteral(0, true));
  lit_list3.push_back(AlgLiteral(3, false));
  AlgCube cube3(mgr(), lit_list3);

  EXPECT_FALSE( cube1.check_intersect(cube3) );
  EXPECT_FALSE( cube3.check_intersect(cube1) );
}

TEST_F(CubeTest, product1)
{
  AlgCube cube1(mgr(), AlgLiteral(0, false));
  AlgCube cube2(mgr(), AlgLiteral(1, true));

  cube1 &= cube2;

  EXPECT_EQ( 2, cube1.literal_num() );
  EXPECT_EQ( kAlgPolP, cube1.literal(0) );
  EXPECT_EQ( kAlgPolN, cube1.literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cube1.literal(i) );
  }
}

TEST_F(CubeTest, product2)
{
  AlgCube cube1(mgr(), AlgLiteral(0, false));
  AlgCube cube2(mgr(), AlgLiteral(1, true));

  AlgCube cube3 = cube1 & cube2;

  EXPECT_EQ( 2, cube3.literal_num() );
  EXPECT_EQ( kAlgPolP, cube3.literal(0) );
  EXPECT_EQ( kAlgPolN, cube3.literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cube3.literal(i) );
  }
}

TEST_F(CubeTest, product3)
{
  vector<AlgLiteral> lit_list1;
  lit_list1.push_back(AlgLiteral(0, false));
  lit_list1.push_back(AlgLiteral(1, false));
  AlgCube cube1(mgr(), lit_list1);

  AlgCube cube2(mgr(), AlgLiteral(1, true));

  cube1 &= cube2;

  EXPECT_EQ( 0, cube1.literal_num() );
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cube1.literal(i) );
  }
}

TEST_F(CubeTest, product4)
{
  vector<AlgLiteral> lit_list1;
  lit_list1.push_back(AlgLiteral(0, false));
  lit_list1.push_back(AlgLiteral(1, false));
  AlgCube cube1(mgr(), lit_list1);

  AlgCube cube2(mgr(), AlgLiteral(1, true));

  AlgCube cube3 = cube1 & cube2;

  EXPECT_EQ( 0, cube3.literal_num() );
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cube3.literal(i) );
  }
}

TEST_F(CubeTest, cofacter1)
{
  vector<AlgLiteral> lit_list1;
  lit_list1.push_back(AlgLiteral(0, false));
  lit_list1.push_back(AlgLiteral(1, false));
  AlgCube cube1(mgr(), lit_list1);

  AlgCube cube2(mgr(), AlgLiteral(1, false));

  cube1 /= cube2;

  EXPECT_EQ( 1, cube1.literal_num() );
  EXPECT_EQ( kAlgPolP, cube1.literal(0) );
  EXPECT_EQ( kAlgPolX, cube1.literal(1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cube1.literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, cofacter2)
{
  vector<AlgLiteral> lit_list1;
  lit_list1.push_back(AlgLiteral(0, false));
  lit_list1.push_back(AlgLiteral(1, false));
  AlgCube cube1(mgr(), lit_list1);

  AlgCube cube2(mgr(), AlgLiteral(2, false));

  cube1 /= cube2;

  EXPECT_EQ( 0, cube1.literal_num() );
  for (ymuint i = 0; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cube1.literal(i) ) << " i = " << i;
  }
}

TEST_F(CubeTest, compare1)
{
  AlgCube cube1(mgr(), "a");
  AlgCube cube2(mgr(), "b");

  EXPECT_FALSE( cube1 == cube2 );
  EXPECT_TRUE(  cube1 != cube2 );
  EXPECT_FALSE( cube1 <  cube2 );
  EXPECT_FALSE( cube1 <= cube2 );
  EXPECT_TRUE(  cube1 >  cube2 );
  EXPECT_TRUE(  cube1 >= cube2 );
}

TEST_F(CubeTest, compare2)
{
  AlgCube cube1(mgr(), "a");
  AlgCube cube2(mgr(), "a b");

  EXPECT_FALSE( cube1 == cube2 );
  EXPECT_TRUE(  cube1 != cube2 );
  EXPECT_TRUE(  cube1 <  cube2 );
  EXPECT_TRUE(  cube1 <= cube2 );
  EXPECT_FALSE( cube1 >  cube2 );
  EXPECT_FALSE( cube1 >= cube2 );
}

TEST_F(CubeTest, compare3)
{
  AlgCube cube1(mgr(), "a");
  AlgCube cube2(mgr(), "a");

  EXPECT_TRUE(  cube1 == cube2 );
  EXPECT_FALSE( cube1 != cube2 );
  EXPECT_FALSE( cube1 <  cube2 );
  EXPECT_TRUE(  cube1 <= cube2 );
  EXPECT_FALSE( cube1 >  cube2 );
  EXPECT_TRUE(  cube1 >= cube2 );
}

TEST_F(CubeTest, check_containment1)
{
  AlgCube cube1(mgr(), "a");
  AlgCube cube2(mgr(), "a b'");

  EXPECT_FALSE( cube1.check_containment(cube2) );
  EXPECT_TRUE(  cube2.check_containment(cube1) );
}

TEST_F(CubeTest, check_containment2)
{
  AlgCube cube1(mgr(), "a");
  AlgCube cube2(mgr(), "a' b'");

  EXPECT_FALSE( cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );
}

TEST_F(CubeTest, check_containment3)
{
  AlgCube cube1(mgr(), "a c");
  AlgCube cube2(mgr(), "a b'");

  EXPECT_FALSE( cube1.check_containment(cube2) );
  EXPECT_FALSE( cube2.check_containment(cube1) );
}

TEST_F(CubeTest, check_intersect1)
{
  AlgCube cube1(mgr(), "a c");
  AlgCube cube2(mgr(), "a b'");

  EXPECT_TRUE(  cube1.check_intersect(cube2) );
  EXPECT_TRUE(  cube2.check_intersect(cube1) );
}

TEST_F(CubeTest, check_intersect2)
{
  AlgCube cube1(mgr(), "a c");
  AlgCube cube2(mgr(), "a' b'");

  EXPECT_FALSE( cube1.check_intersect(cube2) );
  EXPECT_FALSE( cube2.check_intersect(cube1) );
}

TEST_F(CubeTest, int_product1)
{
  AlgCube cube1(mgr(), "a");
  AlgCube cube2(mgr(), "b");

  cube1 &= cube2;

  EXPECT_EQ( AlgCube(mgr(), "a b"), cube1 );
}

TEST_F(CubeTest, int_product2)
{
  AlgCube cube1(mgr(), "a c");
  AlgCube cube2(mgr(), "a b");

  cube1 &= cube2;

  EXPECT_EQ( AlgCube(mgr(), "a b c"), cube1 );
}

TEST_F(CubeTest, int_product3)
{
  AlgCube cube1(mgr(), "a c");
  AlgCube cube2(mgr(), "a' b");

  cube1 &= cube2;

  EXPECT_EQ( AlgCube(mgr()), cube1 );
}

TEST_F(CubeTest, bin_product1)
{
  AlgCube cube1(mgr(), "a");
  AlgCube cube2(mgr(), "b");

  AlgCube cube3 = cube1 & cube2;

  EXPECT_EQ( AlgCube(mgr(), "a b"), cube3 );
}

TEST_F(CubeTest, bin_product2)
{
  AlgCube cube1(mgr(), "a c");
  AlgCube cube2(mgr(), "a b");

  AlgCube cube3 = cube1 & cube2;

  EXPECT_EQ( AlgCube(mgr(), "a b c"), cube3 );
}

TEST_F(CubeTest, bin_product3)
{
  AlgCube cube1(mgr(), "a c");
  AlgCube cube2(mgr(), "a' b");

  AlgCube cube3 = cube1 & cube2;

  EXPECT_EQ( AlgCube(mgr()), cube3 );
}


TEST_F(CubeTest, print1)
{
  AlgCube cube(mgr());

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string(), tmp.str() );
}

TEST_F(CubeTest, print2)
{
  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(0, false));
  lit_list.push_back(AlgLiteral(10, true));
  AlgCube cube(mgr(), lit_list);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("a k'"), tmp.str() );
}

TEST_F(CubeTest2, print2)
{
  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(0, false));
  lit_list.push_back(AlgLiteral(10, true));
  AlgCube cube(mgr(), lit_list);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("a k'"), tmp.str() );
}

TEST_F(CubeTest2, print3)
{
  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(30, false));
  lit_list.push_back(AlgLiteral(60, true));
  AlgCube cube(mgr(), lit_list);

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
  AlgMgr mgr(varname_list);

  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(30, false));
  lit_list.push_back(AlgLiteral(60, true));
  AlgCube cube(mgr, lit_list);

  ostringstream tmp;
  tmp << cube;

  EXPECT_EQ( string("x30 x60'"), tmp.str() );
}

END_NAMESPACE_YM_BFO
