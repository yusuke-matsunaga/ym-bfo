
/// @file BfoMgr.cc
/// @brief BfoMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "BfoMgr.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
BfoMgr::BfoMgr(ymuint variable_num) :
  mVarNum(variable_num),
  mAlloc(_cube_size())
{
}

// @brief デストラクタ
BfoMgr::~BfoMgr()
{
  // このインスタンスが確保した BfoCube は mAlloc の
  // デストラクタで自動的に破壊される．
}

// @brief キューブを生成する．
//
// 内容はすべて kBfoLitX で初期化される．
BfoCube*
BfoMgr::new_cube()
{
  void* p = mAlloc.get_memory(_cube_size());
  return new (p) BfoCube(variable_num());
}

// @brief キューブを削除する．
// @param[in] cube 対象のキューブ
void
BfoMgr::delete_cube(BfoCube* cube)
{
  void* p = static_cast<void*>(cube);
  mAlloc.put_memory(_cube_size(), p);
}

// @brief カバーの和を計算する．
BfoMgr::make_sum(const BfoCover& left,
		 const BfoCover& right,
		 BfoCover& ans)
{
  ASSERT_COND( left.m
}

// @brief カバーの差を計算する．
make_diff(const BfoCover& left,
	    const BfoCover& right,
	    BfoCover& ans);

  /// @brief カバーの積を計算する．
  make_product(const BfoCover& left,
	       const BfoCover& right,
	       BfoCover& ans);

  /// @brief カバーの商を計算する．
  make_division(const BfoCover& left,
		const BfoCover& right,
		BfoCover& ans);

// @brief BfoCube の本当のサイズ
ymuint
BfuCubeMgr::_cube_size() const
{
  ymuint block_num = (mVarNum + 31) / 32;
  return sizeof(BfuCube) + sizeof(ymuint64) * (block_num - 1);
}

END_NAMESPACE_YM_BFO
