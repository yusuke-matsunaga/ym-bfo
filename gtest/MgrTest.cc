
/// @file MgrTest.cc
/// @brief AlgMgr のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "ym/AlgMgr.h"
#include "ym/AlgCube.h"
#include "ym/AlgCover.h"


BEGIN_NAMESPACE_YM_BFO

TEST(MgrTest, constructor1)
{
  ymuint variable_num = 10;

  AlgMgr mgr(variable_num);

  EXPECT_EQ( variable_num, mgr.variable_num() );
  EXPECT_EQ( string("a"), mgr.varname(0) );
}

TEST(MgrTest, constructor1big)
{
  ymuint variable_num = 100;

  AlgMgr mgr(variable_num);

  EXPECT_EQ( variable_num, mgr.variable_num() );
  EXPECT_EQ( string("a"), mgr.varname(0) );
  EXPECT_EQ( string("ba"), mgr.varname(26) );
}

TEST(MgrTest, constructor2)
{
  vector<string> varname_list;
  varname_list.push_back("x0");
  varname_list.push_back("x1");
  varname_list.push_back("y0");
  varname_list.push_back("y1");

  AlgMgr mgr(varname_list);

  EXPECT_EQ( varname_list.size(), mgr.variable_num() );
  for (ymuint i = 0; i < varname_list.size(); ++ i) {
    string varname = varname_list[i];
    EXPECT_EQ( varname, mgr.varname(i) );
  }
}

TEST(MgrTest, new_delete1)
{
  ymuint variable_num = 10;

  AlgMgr mgr(variable_num);

  ymuint n = 1000;
  vector<ymuint64*> body_list(1000);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint64* body = mgr.new_body();
    body_list[i] = body;
  }
  for (ymuint i = 0; i < n; ++ i) {
    ymuint64* body = body_list[i];
    mgr.delete_body(body);
  }
}

TEST(MgrTest, new_delete2)
{
  ymuint variable_num = 100;

  AlgMgr mgr(variable_num);

  ymuint n = 1000;
  vector<ymuint64*> body_list(1000);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint64* body = mgr.new_body();
    body_list[i] = body;
  }
  for (ymuint i = 0; i < n; ++ i) {
    ymuint64* body = body_list[i];
    mgr.delete_body(body);
  }
}

TEST(MgrTest, new_delete3)
{
  ymuint variable_num = 10;

  AlgMgr mgr(variable_num);

  ymuint n = 1000;
  vector<ymuint64*> body_list(1000);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint cube_num = i % 10;
    ymuint64* body = mgr.new_body(cube_num);
    body_list[i] = body;
  }
  for (ymuint i = 0; i < n; ++ i) {
    ymuint cube_num = i % 10;
    ymuint64* body = body_list[i];
    mgr.delete_body(body, cube_num);
  }
}

TEST(MgrTest, new_delete4)
{
  ymuint variable_num = 100;

  AlgMgr mgr(variable_num);

  ymuint n = 1000;
  vector<ymuint64*> body_list(1000);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint cube_num = i % 10;
    ymuint64* body = mgr.new_body(cube_num);
    body_list[i] = body;
  }
  for (ymuint i = 0; i < n; ++ i) {
    ymuint cube_num = i % 10;
    ymuint64* body = body_list[i];
    mgr.delete_body(body, cube_num);
  }
}

TEST(MgrTest, parse1)
{
  ymuint variable_num = 100;
  AlgMgr mgr(variable_num);

  const char* str = "a b c";
  vector<AlgLiteral> lit_list;
  ymuint n = mgr.parse(str, lit_list);

  EXPECT_EQ( 1, n );
  EXPECT_EQ( 3, lit_list.size() );
  EXPECT_EQ( AlgLiteral(0, false), lit_list[0] );
  EXPECT_EQ( AlgLiteral(1, false), lit_list[1] );
  EXPECT_EQ( AlgLiteral(2, false), lit_list[2] );
}

TEST(MgrTest, parse2)
{
  ymuint variable_num = 100;
  AlgMgr mgr(variable_num);

  const char* str = "ba bb' cc";
  vector<AlgLiteral> lit_list;
  ymuint n = mgr.parse(str, lit_list);

  EXPECT_EQ( 1, n );
  EXPECT_EQ( 3, lit_list.size() );
  EXPECT_EQ( AlgLiteral(26, false), lit_list[0] );
  EXPECT_EQ( AlgLiteral(27, true), lit_list[1] );
  EXPECT_EQ( AlgLiteral(54, false), lit_list[2] );
}

TEST(MgrTest, parse3)
{
  ymuint variable_num = 100;
  AlgMgr mgr(variable_num);

  const char* str = "a+b+c";
  vector<AlgLiteral> lit_list;
  ymuint n = mgr.parse(str, lit_list);

  EXPECT_EQ( 3, n );
  EXPECT_EQ( 5, lit_list.size() );
  EXPECT_EQ( AlgLiteral(0, false), lit_list[0] );
  EXPECT_EQ( kAlgLiteralUndef,     lit_list[1] );
  EXPECT_EQ( AlgLiteral(1, false), lit_list[2] );
  EXPECT_EQ( kAlgLiteralUndef,     lit_list[3] );
  EXPECT_EQ( AlgLiteral(2, false), lit_list[4] );
}

TEST(MgrTest, parse4)
{
  ymuint variable_num = 100;
  AlgMgr mgr(variable_num);

  const char* str = "a + b + c";
  vector<AlgLiteral> lit_list;
  ymuint n = mgr.parse(str, lit_list);

  EXPECT_EQ( 3, n );
  EXPECT_EQ( 5, lit_list.size() );
  EXPECT_EQ( AlgLiteral(0, false), lit_list[0] );
  EXPECT_EQ( kAlgLiteralUndef,     lit_list[1] );
  EXPECT_EQ( AlgLiteral(1, false), lit_list[2] );
  EXPECT_EQ( kAlgLiteralUndef,     lit_list[3] );
  EXPECT_EQ( AlgLiteral(2, false), lit_list[4] );
}

TEST(MgrTest, parse5)
{
  ymuint variable_num = 100;
  AlgMgr mgr(variable_num);

  const char* str = "a+ b +c";
  vector<AlgLiteral> lit_list;
  ymuint n = mgr.parse(str, lit_list);

  EXPECT_EQ( 3, n );
  EXPECT_EQ( 5, lit_list.size() );
  EXPECT_EQ( AlgLiteral(0, false), lit_list[0] );
  EXPECT_EQ( kAlgLiteralUndef,     lit_list[1] );
  EXPECT_EQ( AlgLiteral(1, false), lit_list[2] );
  EXPECT_EQ( kAlgLiteralUndef,     lit_list[3] );
  EXPECT_EQ( AlgLiteral(2, false), lit_list[4] );
}

END_NAMESPACE_YM_BFO
