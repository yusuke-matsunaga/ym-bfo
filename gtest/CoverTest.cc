
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

TEST_F(CoverTest, constructor1)
{
  // キューブのリスト(リテラルのリストのリスト)からのコンストラクタのテスト
  vector<vector<BfoLiteral> > cube_list(2);
  cube_list[0].push_back(BfoLiteral(0, false));
  cube_list[0].push_back(BfoLiteral(1, false));
  cube_list[1].push_back(BfoLiteral(0, true));
  cube_list[1].push_back(BfoLiteral(2, false));

  BfoCover cover(mgr(), cube_list);

  EXPECT_EQ( 2, cover.cube_num() );
  EXPECT_EQ( mgr().variable_num(), cover.variable_num() );
  EXPECT_EQ( 4, cover.literal_num() );

  EXPECT_EQ( kBfoPolP, cover.literal(0, 0) );
  EXPECT_EQ( kBfoPolP, cover.literal(0, 1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cover.literal(0, i) );
  }

  EXPECT_EQ( kBfoPolN, cover.literal(1, 0) );
  EXPECT_EQ( kBfoPolX, cover.literal(1, 1) );
  EXPECT_EQ( kBfoPolP, cover.literal(1, 2) );
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cover.literal(1, i) );
  }
}

TEST_F(CoverTest, constructor2)
{
  // コピーコンストラクタのテスト
  vector<vector<BfoLiteral> > cube_list(2);
  cube_list[0].push_back(BfoLiteral(0, false));
  cube_list[0].push_back(BfoLiteral(1, false));
  cube_list[1].push_back(BfoLiteral(0, true));
  cube_list[1].push_back(BfoLiteral(2, false));

  BfoCover src_cover(mgr(), cube_list);

  BfoCover cover(src_cover);

  EXPECT_EQ( 2, cover.cube_num() );
  EXPECT_EQ( mgr().variable_num(), cover.variable_num() );
  EXPECT_EQ( 4, cover.literal_num() );

  EXPECT_EQ( kBfoPolP, cover.literal(0, 0) );
  EXPECT_EQ( kBfoPolP, cover.literal(0, 1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cover.literal(0, i) );
  }

  EXPECT_EQ( kBfoPolN, cover.literal(1, 0) );
  EXPECT_EQ( kBfoPolX, cover.literal(1, 1) );
  EXPECT_EQ( kBfoPolP, cover.literal(1, 2) );
  for (ymuint i = 3; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cover.literal(1, i) );
  }
}

TEST_F(CoverTest, constructor3)
{
  // キューブからの変換コンストラクタのテスト
  vector<BfoLiteral> lit_list;
  lit_list.push_back(BfoLiteral(0, false));
  lit_list.push_back(BfoLiteral(1, true));
  BfoCube src_cube(mgr(), lit_list);

  BfoCover cover(src_cube);

  EXPECT_EQ( 1, cover.cube_num() );
  EXPECT_EQ( 2, cover.literal_num() );

  // cover.cube(0) が今設定した src_cube と等しいことを確認する．
  EXPECT_EQ( kBfoPolP, cover.literal(0, 0) );
  EXPECT_EQ( kBfoPolN, cover.literal(0, 1) );
  for (ymuint i = 2; i < mgr().variable_num(); ++ i) {
    EXPECT_EQ( kBfoPolX, cover.literal(0, i) );
  }
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
  vector<vector<BfoLiteral> > cube_list(2);
  cube_list[0].push_back(BfoLiteral(0, false));
  cube_list[0].push_back(BfoLiteral(1, true));
  cube_list[1].push_back(BfoLiteral(0, true));
  cube_list[1].push_back(BfoLiteral(2, false));

  BfoCover cover(mgr(), cube_list);

  ostringstream tmp;
  tmp << cover;

  EXPECT_EQ( string("a b' + a' c"), tmp.str() );
}

TEST_F(CoverTest, print3)
{
  vector<vector<BfoLiteral> > cube_list(2);
  cube_list[0].push_back(BfoLiteral(26, false));
  cube_list[0].push_back(BfoLiteral(27, true));
  cube_list[1].push_back(BfoLiteral(26, true));
  cube_list[1].push_back(BfoLiteral(28, false));

  BfoCover cover(mgr(), cube_list);

  ostringstream tmp;
  tmp << cover;

  EXPECT_EQ( string("ba bb' + ba' bc"), tmp.str() );
}

TEST_F(CoverTest, print4)
{
  // これ CoverTest を使う意味がない．
  vector<string> varname_list(30);
  for (ymuint i = 0; i < 30; ++ i) {
    ostringstream buf;
    buf << "x" << i;
    varname_list[i] = buf.str();
  }
  BfoMgr mgr(varname_list);

  vector<vector<BfoLiteral> > cube_list(2);
  cube_list[0].push_back(BfoLiteral(0, false));
  cube_list[0].push_back(BfoLiteral(1, true));
  cube_list[1].push_back(BfoLiteral(0, true));
  cube_list[1].push_back(BfoLiteral(2, false));

  BfoCover cover(mgr, cube_list);

  ostringstream tmp;
  cover.print(tmp);

  EXPECT_EQ( string("x0 x1' + x0' x2"), tmp.str() );
}

END_NAMESPACE_YM_BFO
