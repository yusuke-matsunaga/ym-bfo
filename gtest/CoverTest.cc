
/// @file CoverTest.cc
/// @brief CoverTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/AlgCube.h"
#include "ym/AlgCover.h"
#include "ym/AlgMgr.h"


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
  AlgMgr&
  mgr() { return mMgr; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AlgMgr mMgr;

};


TEST_F(CoverTest, nullcover)
{
  AlgCover cover(mgr());

  EXPECT_EQ( mgr().variable_num(), cover.variable_num() );
  EXPECT_EQ( 0, cover.cube_num() );
  EXPECT_EQ( 0, cover.literal_num() );
}

TEST_F(CoverTest, constructor1)
{
  // キューブのリストからのコンストラクタのテスト
  vector<AlgCube> cube_list;
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, false));
    tmp_list.push_back(AlgLiteral(1, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, true));
    tmp_list.push_back(AlgLiteral(2, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }

  AlgCover cover(mgr(), cube_list);

  EXPECT_EQ( 2, cover.cube_num() );
  EXPECT_EQ( mgr().variable_num(), cover.variable_num() );
  EXPECT_EQ( 4, cover.literal_num() );

  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }

  EXPECT_EQ( kAlgPolN, cover.literal(1, 0) );
  EXPECT_EQ( kAlgPolX, cover.literal(1, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(1, 2) );
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(1, i) );
  }
}

TEST_F(CoverTest, constructor1r)
{
  // キューブのリストからのコンストラクタのテスト
  vector<AlgCube> cube_list;
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, true));
    tmp_list.push_back(AlgLiteral(2, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, false));
    tmp_list.push_back(AlgLiteral(1, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }

  AlgCover cover(mgr(), cube_list);

  EXPECT_EQ( 2, cover.cube_num() );
  EXPECT_EQ( mgr().variable_num(), cover.variable_num() );
  EXPECT_EQ( 4, cover.literal_num() );

  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }

  EXPECT_EQ( kAlgPolN, cover.literal(1, 0) );
  EXPECT_EQ( kAlgPolX, cover.literal(1, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(1, 2) );
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(1, i) );
  }
}

TEST_F(CoverTest, constructor2)
{
  // コピーコンストラクタのテスト
  vector<AlgCube> cube_list;
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, false));
    tmp_list.push_back(AlgLiteral(1, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, true));
    tmp_list.push_back(AlgLiteral(2, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }
  AlgCover src_cover(mgr(), cube_list);

  AlgCover cover(src_cover);

  EXPECT_EQ( 2, cover.cube_num() );
  EXPECT_EQ( mgr().variable_num(), cover.variable_num() );
  EXPECT_EQ( 4, cover.literal_num() );

  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }

  EXPECT_EQ( kAlgPolN, cover.literal(1, 0) );
  EXPECT_EQ( kAlgPolX, cover.literal(1, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(1, 2) );
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(1, i) );
  }
}

TEST_F(CoverTest, constructor3)
{
  // キューブからの変換コンストラクタのテスト
  vector<AlgLiteral> lit_list;
  lit_list.push_back(AlgLiteral(0, false));
  lit_list.push_back(AlgLiteral(1, true));
  AlgCube src_cube(mgr(), lit_list);

  AlgCover cover(src_cube);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 2, cover.literal_num() );

  // cover.cube(0) が今設定した src_cube と等しいことを確認する．
  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolN, cover.literal(0, 1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }
}

TEST_F(CoverTest, from_cstr1)
{
  // 文字列からの変換コンストラクタのテスト
  const char* str = "a b c";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 3, cover.literal_num() );
  ymuint n = cover.variable_num();
  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 2) );
  for (ymuint i = 3; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }
}

TEST_F(CoverTest, from_cstr2)
{
  // 文字列からの変換コンストラクタのテスト
  const char* str = "c b a";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 3, cover.literal_num() );
  ymuint n = cover.variable_num();
  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 2) );
  for (ymuint i = 3; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }
}

TEST_F(CoverTest, from_cstr3)
{
  // 文字列からの変換コンストラクタのテスト
  const char* str = "c+ b  + a";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 3, cover.cube_num() );
  EXPECT_EQ( 3, cover.literal_num() );
  ymuint n = cover.variable_num();
  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(1, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(2, 2) );
  for (ymuint j = 0; j < cover.cube_num(); ++ j) {
    for (ymuint i = 0; i < n; ++ i) {
      if ( i != j ) {
	EXPECT_EQ( kAlgPolX, cover.literal(j, i) );
      }
    }
  }
}

TEST_F(CoverTest, from_bad_cstr)
{
  // 文字列からの変換コンストラクタのテスト
  const char* str = "c_a_b";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 0, cover.cube_num() );
  EXPECT_EQ( 0, cover.literal_num() );
}

TEST_F(CoverTest, from_string1)
{
  // string からの変換コンストラクタのテスト
  string str("a b c");
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 3, cover.literal_num() );
  ymuint n = cover.variable_num();
  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 2) );
  for (ymuint i = 3; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }
}

TEST_F(CoverTest, from_string2)
{
  // string からの変換コンストラクタのテスト
  string str("c b a");
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 3, cover.literal_num() );
  ymuint n = cover.variable_num();
  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(0, 2) );
  for (ymuint i = 3; i < n; ++ i) {
    EXPECT_EQ( kAlgPolX, cover.literal(0, i) );
  }
}

TEST_F(CoverTest, from_string3)
{
  // string からの変換コンストラクタのテスト
  string str("c+ b  + a");
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 3, cover.cube_num() );
  EXPECT_EQ( 3, cover.literal_num() );
  ymuint n = cover.variable_num();
  EXPECT_EQ( kAlgPolP, cover.literal(0, 0) );
  EXPECT_EQ( kAlgPolP, cover.literal(1, 1) );
  EXPECT_EQ( kAlgPolP, cover.literal(2, 2) );
  for (ymuint j = 0; j < cover.cube_num(); ++ j) {
    for (ymuint i = 0; i < n; ++ i) {
      if ( i != j ) {
	EXPECT_EQ( kAlgPolX, cover.literal(j, i) );
      }
    }
  }
}

TEST_F(CoverTest, from_bad_string)
{
  // 文字列からの変換コンストラクタのテスト
  string str("c_a_b");
  AlgCover cover(mgr(), str);

  EXPECT_EQ( 0, cover.cube_num() );
  EXPECT_EQ( 0, cover.literal_num() );
}

TEST_F(CoverTest, sort2_1)
{
  const char* str = "a + b";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b"), cover );
}

TEST_F(CoverTest, sort2_2)
{
  const char* str = "b + a";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b"), cover );
}

TEST_F(CoverTest, sort3_1)
{
  const char* str = "a + b + c";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c"), cover );
}

TEST_F(CoverTest, sort3_2)
{
  const char* str = "a + c + b";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c"), cover );
}

TEST_F(CoverTest, sort3_3)
{
  const char* str = "b + a + c";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c"), cover );
}

TEST_F(CoverTest, sort3_4)
{
  const char* str = "b + c + a";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c"), cover );
}

TEST_F(CoverTest, sort3_5)
{
  const char* str = "c + a + b";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c"), cover );
}

TEST_F(CoverTest, sort3_6)
{
  const char* str = "c + b + a";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c"), cover );
}

TEST_F(CoverTest, sort4_1)
{
  const char* str = "a + b + c + d";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d"), cover );
}

TEST_F(CoverTest, sort4_2)
{
  const char* str = "b + a  + d + c";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d"), cover );
}

TEST_F(CoverTest, sort4_3)
{
  const char* str = "a + c  + b + d";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d"), cover );
}

TEST_F(CoverTest, sort4_4)
{
  const char* str = "c + d  + b + a";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d"), cover );
}

TEST_F(CoverTest, sort4_5)
{
  const char* str = "b + d  + a + c";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d"), cover );
}

TEST_F(CoverTest, sort4_6)
{
  const char* str = "b + c  + a + d";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d"), cover );
}

TEST_F(CoverTest, sort4_7)
{
  const char* str = "a + d  + b + c";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d"), cover );
}

TEST_F(CoverTest, sort5_1)
{
  const char* str = "a + b + c + d + e";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d + e"), cover );
}

TEST_F(CoverTest, sort5_2)
{
  const char* str = "e + d + c + b + a";
  AlgCover cover(mgr(), str);

  EXPECT_EQ( AlgCover(mgr(), "a + b + c + d + e"), cover );
}

TEST_F(CoverTest, literal_num1)
{
  AlgCover cover(mgr(), "a b + a c + a' d + b c'");

  AlgLiteral a_lit(0, false);
  AlgLiteral an_lit(0, true);

  EXPECT_EQ( 8, cover.literal_num() );
  EXPECT_EQ( 2, cover.literal_num(a_lit) );
  EXPECT_EQ( 1, cover.literal_num(an_lit) );
}

TEST_F(CoverTest, int_sum1)
{
  AlgCover cover1(mgr(), "a");
  AlgCover cover2(mgr(), "b");

  cover1 += cover2;

  EXPECT_EQ( AlgCover(mgr(), "a + b"), cover1 );
}

TEST_F(CoverTest, bin_sum1)
{
  AlgCover cover1(mgr(), "a");
  AlgCover cover2(mgr(), "b");

  AlgCover cover3 = cover1 + cover2;

  EXPECT_EQ( AlgCover(mgr(), "a + b"), cover3 );
}

TEST_F(CoverTest, int_sum2)
{
  AlgCover cover1(mgr(), "a d");
  AlgCover cover2(mgr(), "a b");

  cover1 += cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a d"), cover1 );
}

TEST_F(CoverTest, bin_sum2)
{
  AlgCover cover1(mgr(), "a d");
  AlgCover cover2(mgr(), "a b");

  AlgCover cover3 = cover1 + cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a d"), cover3 );
}

TEST_F(CoverTest, int_sum3)
{
  AlgCover cover1(mgr(), "a d + b c");
  AlgCover cover2(mgr(), "a b + b d");

  cover1 += cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a d + b c + b d"), cover1 );
}

TEST_F(CoverTest, bin_sum3)
{
  AlgCover cover1(mgr(), "a d + b c");
  AlgCover cover2(mgr(), "a b + b d");

  AlgCover cover3 = cover1 + cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a d + b c + b d"), cover3 );
}

TEST_F(CoverTest, int_sum_c1)
{
  AlgCover cover1(mgr(), "a");
  AlgCube cube2(mgr(), "b");

  cover1 += cube2;

  EXPECT_EQ( AlgCover(mgr(), "a + b"), cover1 );
}

TEST_F(CoverTest, bin_sum_c1)
{
  AlgCover cover1(mgr(), "a");
  AlgCube cube2(mgr(), "b");

  AlgCover cover3 = cover1 + cube2;

  EXPECT_EQ( AlgCover(mgr(), "a + b"), cover3 );
}

TEST_F(CoverTest, int_sum_c2)
{
  AlgCover cover1(mgr(), "a d");
  AlgCube cube2(mgr(), "a b");

  cover1 += cube2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a d"), cover1 );
}

TEST_F(CoverTest, bin_sum_c2)
{
  AlgCover cover1(mgr(), "a d");
  AlgCube cube2(mgr(), "a b");

  AlgCover cover3 = cover1 + cube2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a d"), cover3 );
}

TEST_F(CoverTest, int_diff1)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCover cover2(mgr(), "a c");

  cover1 -= cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + b d"), cover1 );
}

TEST_F(CoverTest, bin_diff1)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCover cover2(mgr(), "a c");

  AlgCover cover3 = cover1 - cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + b d"), cover3 );
}

TEST_F(CoverTest, int_diff2)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCover cover2(mgr(), "a' c");

  cover1 -= cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a c + b d"), cover1 );
}

TEST_F(CoverTest, bin_diff2)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCover cover2(mgr(), "a' c");

  AlgCover cover3 = cover1 - cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a c + b d"), cover1 );
}

TEST_F(CoverTest, int_diff3)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCover cover2(mgr(), "a c + a b");

  cover1 -= cover2;

  EXPECT_EQ( AlgCover(mgr(), "b d"), cover1 );
}

TEST_F(CoverTest, bin_diff3)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCover cover2(mgr(), "a c + a b");

  AlgCover cover3 = cover1 - cover2;

  EXPECT_EQ( AlgCover(mgr(), "b d"), cover3 );
}

TEST_F(CoverTest, int_diff_c1)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCube cube2(mgr(), "a c");

  cover1 -= cube2;

  EXPECT_EQ( AlgCover(mgr(), "a b + b d"), cover1 );
}

TEST_F(CoverTest, bin_diff_c1)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCube cube2(mgr(), "a c");

  AlgCover cover3 = cover1 - cube2;

  EXPECT_EQ( AlgCover(mgr(), "a b + b d"), cover3 );
}

TEST_F(CoverTest, int_diff_c2)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCube cube2(mgr(), "a' c");

  cover1 -= cube2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a c + b d"), cover1 );
}

TEST_F(CoverTest, bin_diff_c2)
{
  AlgCover cover1(mgr(), "a b + a c + b d");
  AlgCube cube2(mgr(), "a' c");

  AlgCover cover3 = cover1 - cube2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a c + b d"), cover1 );
}

TEST_F(CoverTest, int_product1)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCover cover2(mgr(), "c");

  cover1 *= cover2;

  EXPECT_EQ( AlgCover(mgr(), "a c + b c"), cover1 );
}

TEST_F(CoverTest, bin_product1)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCover cover2(mgr(), "c");

  AlgCover cover3 = cover1 * cover2;

  EXPECT_EQ( AlgCover(mgr(), "a c + b c"), cover3 );
}

TEST_F(CoverTest, int_product2)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCover cover2(mgr(), "a'");

  cover1 *= cover2;

  EXPECT_EQ( AlgCover(mgr(), "a' b"), cover1 );
}

TEST_F(CoverTest, bin_product2)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCover cover2(mgr(), "a'");

  AlgCover cover3 = cover1 * cover2;

  EXPECT_EQ( AlgCover(mgr(), "a' b"), cover3 );
}

TEST_F(CoverTest, int_product3)
{
  AlgCover cover1(mgr(), "a + d");
  AlgCover cover2(mgr(), "b + c");

  cover1 *= cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a c + b d + c d"), cover1 );
}

TEST_F(CoverTest, bin_product3)
{
  AlgCover cover1(mgr(), "a + d");
  AlgCover cover2(mgr(), "b + c");

  AlgCover cover3 = cover1 * cover2;

  EXPECT_EQ( AlgCover(mgr(), "a b + a c + b d + c d"), cover3 );
}

TEST_F(CoverTest, int_product4)
{
  AlgCover cover1(mgr(), "a + d");
  AlgCover cover2(mgr(), "a' + c");

  cover1 *= cover2;

  EXPECT_EQ( AlgCover(mgr(), "a c + a' d + c d"), cover1 );
}

TEST_F(CoverTest, bin_product4)
{
  AlgCover cover1(mgr(), "a + d");
  AlgCover cover2(mgr(), "a' + c");

  AlgCover cover3 = cover1 * cover2;

  EXPECT_EQ( AlgCover(mgr(), "a c + a' d + c d"), cover3 );
}

TEST_F(CoverTest, int_product_c1)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCube cube2(mgr(), "c");

  cover1 *= cube2;

  EXPECT_EQ( AlgCover(mgr(), "a c + b c"), cover1 );
}

TEST_F(CoverTest, bin_product_c1)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCube cube2(mgr(), "c");

  AlgCover cover3 = cover1 * cube2;

  EXPECT_EQ( AlgCover(mgr(), "a c + b c"), cover3 );
}

TEST_F(CoverTest, int_product_c2)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCube cube2(mgr(), "a'");

  cover1 *= cube2;

  EXPECT_EQ( AlgCover(mgr(), "a' b"), cover1 );
}

TEST_F(CoverTest, bin_product_c2)
{
  AlgCover cover1(mgr(), "a + b");
  AlgCube cube2(mgr(), "a'");

  AlgCover cover3 = cover1 * cube2;

  EXPECT_EQ( AlgCover(mgr(), "a' b"), cover3 );
}

TEST_F(CoverTest, int_division1)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCover cover2(mgr(), "b + c");

  cover1 /= cover2;

  EXPECT_EQ( AlgCover(mgr(), "a"), cover1 );
}

TEST_F(CoverTest, bin_division1)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCover cover2(mgr(), "b + c");

  AlgCover cover3 = cover1 / cover2;

  EXPECT_EQ( AlgCover(mgr(), "a"), cover3 );
}

TEST_F(CoverTest, int_division2)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCover cover2(mgr(), "b + c'");

  cover1 /= cover2;

  EXPECT_EQ( AlgCover(mgr(), ""), cover1 );
}

TEST_F(CoverTest, bin_division2)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCover cover2(mgr(), "b + c'");

  AlgCover cover3 = cover1 / cover2;

  EXPECT_EQ( AlgCover(mgr(), ""), cover3 );
}

TEST_F(CoverTest, int_division3)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCover cover2(mgr(), "a b + b c");

  cover1 /= cover2;

  EXPECT_EQ( AlgCover(mgr(), 1), cover1);
}

TEST_F(CoverTest, bin_division3)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCover cover2(mgr(), "a b + b c");

  AlgCover cover3 = cover1 / cover2;

  EXPECT_EQ( AlgCover(mgr(), 1), cover3);

  AlgCover cover4 = cover3 * cover2;
  AlgCover cover5 = cover1 - cover4;
  EXPECT_EQ( AlgCover(mgr(), "a c"), cover5);
}

TEST_F(CoverTest, int_division_c1)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCube cube2(mgr(), "a");

  cover1 /= cube2;

  EXPECT_EQ( AlgCover(mgr(), "b + c"), cover1 );
}

TEST_F(CoverTest, bin_division_c1)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCube cube2(mgr(), "a");

  AlgCover cover3 = cover1 / cube2;

  EXPECT_EQ( AlgCover(mgr(), "b + c"), cover3 );
}

TEST_F(CoverTest, int_division_c2)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCube cube2(mgr(), "a'");

  cover1 /= cube2;

  EXPECT_EQ( AlgCover(mgr(), ""), cover1 );
}

TEST_F(CoverTest, bin_division_c2)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCube cube2(mgr(), "a'");

  AlgCover cover3 = cover1 / cube2;

  EXPECT_EQ( AlgCover(mgr(), ""), cover3 );
}

TEST_F(CoverTest, int_division_c3)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCube cube2(mgr(), "a c");

  cover1 /= cube2;

  EXPECT_EQ( AlgCover(mgr(), 1), cover1);
}

TEST_F(CoverTest, bin_division_c3)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgCube cube2(mgr(), "a c");

  AlgCover cover3 = cover1 / cube2;

  EXPECT_EQ( AlgCover(mgr(), 1), cover3);
}

TEST_F(CoverTest, int_division_l1)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgLiteral a(0, false);

  cover1 /= a;

  EXPECT_EQ( AlgCover(mgr(), "b + c"), cover1 );
}

TEST_F(CoverTest, bin_division_l1)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgLiteral a(0, false);

  AlgCover cover3 = cover1 / a;

  EXPECT_EQ( AlgCover(mgr(), "b + c"), cover3 );
}

TEST_F(CoverTest, int_division_l2)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgLiteral a(0, true);

  cover1 /= a;

  EXPECT_EQ( AlgCover(mgr(), ""), cover1 );
}

TEST_F(CoverTest, bin_division_l2)
{
  AlgCover cover1(mgr(), "a b + a c + b c");
  AlgLiteral a(0, true);

  AlgCover cover3 = cover1 / a;

  EXPECT_EQ( AlgCover(mgr(), ""), cover3 );
}

TEST_F(CoverTest, common_cube1)
{
  AlgCover cover1(mgr(), "a b + a c + a d");

  AlgCube ccube = cover1.common_cube();

  EXPECT_EQ( AlgCube(mgr(), "a"), ccube );
}

TEST_F(CoverTest, common_cube2)
{
  AlgCover cover1(mgr(), "a b + a c + b d");

  AlgCube ccube = cover1.common_cube();

  EXPECT_EQ( AlgCube(mgr(), ""), ccube );
}

TEST_F(CoverTest, print1)
{
  AlgCover cover(mgr());

  ostringstream tmp;
  tmp << cover;

  EXPECT_EQ( string(), tmp.str() );
}

TEST_F(CoverTest, print2)
{
  vector<AlgCube> cube_list;
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, false));
    tmp_list.push_back(AlgLiteral(1, true));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, true));
    tmp_list.push_back(AlgLiteral(2, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }

  AlgCover cover(mgr(), cube_list);

  ostringstream tmp;
  tmp << cover;

  EXPECT_EQ( string("a b' + a' c"), tmp.str() );
}

TEST_F(CoverTest, print3)
{
  vector<AlgCube> cube_list;
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(26, false));
    tmp_list.push_back(AlgLiteral(27, true));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(26, true));
    tmp_list.push_back(AlgLiteral(28, false));
    cube_list.push_back(AlgCube(mgr(), tmp_list));
  }

  AlgCover cover(mgr(), cube_list);

  ostringstream tmp;
  tmp << cover;

  EXPECT_EQ( string("ba bb' + ba' bc"), tmp.str() );
}

TEST(CoverTest2, print4)
{
  vector<string> varname_list(30);
  for (ymuint i = 0; i < 30; ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }
  AlgMgr mgr(varname_list);

  vector<AlgCube> cube_list;
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, false));
    tmp_list.push_back(AlgLiteral(1, true));
    cube_list.push_back(AlgCube(mgr, tmp_list));
  }
  {
    vector<AlgLiteral> tmp_list;
    tmp_list.push_back(AlgLiteral(0, true));
    tmp_list.push_back(AlgLiteral(2, false));
    cube_list.push_back(AlgCube(mgr, tmp_list));
  }

  AlgCover cover(mgr, cube_list);

  ostringstream tmp;
  cover.print(tmp);

  EXPECT_EQ( string("x0 x1' + x0' x2"), tmp.str() );
}

END_NAMESPACE_YM_BFO
