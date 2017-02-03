
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
