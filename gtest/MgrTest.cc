
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

TEST(MgrTest, constructor1)
{
  ymuint variable_num = 10;

  BfoMgr mgr(variable_num);

  EXPECT_EQ( variable_num, mgr.variable_num() );
  EXPECT_EQ( string("a"), mgr.varname(0) );
}

TEST(MgrTest, constructor1big)
{
  ymuint variable_num = 100;

  BfoMgr mgr(variable_num);

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

  BfoMgr mgr(varname_list);

  EXPECT_EQ( varname_list.size(), mgr.variable_num() );
  for (ymuint i = 0; i < varname_list.size(); ++ i) {
    string varname = varname_list[i];
    EXPECT_EQ( varname, mgr.varname(i) );
  }
}

TEST(MgrTest, new_delete1)
{
  ymuint variable_num = 10;

  BfoMgr mgr(variable_num);

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

  BfoMgr mgr(variable_num);

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

  BfoMgr mgr(variable_num);

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

  BfoMgr mgr(variable_num);

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

END_NAMESPACE_YM_BFO
