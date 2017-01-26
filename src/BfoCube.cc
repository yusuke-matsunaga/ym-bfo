
/// @file BfoCube.cc
/// @brief BfoCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BfoCube.h"
#include "ym/BfoLiteral.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス BfoCube
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr ネージャ
// @param[in] lit_list キューブを表すリテラルのリスト
BfoCube::BfoCube(BfoMgr& mgr,
		 const vector<BfoLiteral>& lit_list) :
  mMgr(mgr),
  mBody(mMgr.new_body())
{
  mMgr.set_cube(mBody, 0, lit_list);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
BfoCube::BfoCube(const BfoCube& src) :
  mMgr(src.mMgr)
{
  mBody = mMgr.new_body();
  mMgr.copy(mBody, 0, src.mBody, 0);
}

// @brief 内容を指定するコンストラクタ
// @param[in] mgr マネージャ
// @param[in] body キューブのパタンを表す本体
//
// 危険なので普通は使わないように
BfoCube::BfoCube(BfoMgr& mgr,
		 ymuint64* body) :
  mMgr(mgr),
  mBody(body)
{
}

// @brief デストラクタ
BfoCube::~BfoCube()
{
  mMgr.delete_body(mBody);
}

#if 0
// @brief 2つのキューブに共通なリテラルがあれば true を返す．
// @param[in] right オペランドのキューブ
bool
BfoCube::operator&&(const BfoCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    if ( mBody[i] & right.mBody[i] ) {
      return true;
    }
  }
  return false;
}
#endif

// @brief 論理積を計算し自身に代入する．
// @param[in] right オペランドのキューブ
// @return 演算後の自身の参照を返す．
//
// 相反するリテラルとの積があったら答は空のキューブとなる．
const BfoCube&
BfoCube::operator&=(const BfoCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  mgr().make_product(mBody, 0, mBody, 0, right.mBody, 0);

  return *this;
}

#if 0
// @brief 共通部分を計算し自身に代入する．
// @param[in] src オペランドのキューブ
// @return 結果が NULL Cube になったら false を返す．
bool
BfoCube::make_intersect(const BfoCube* src)
{
  ASSERT_COND( variable_num() == src->variable_num() );

  ymuint64 tmp = 0ULL;
  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] &= src->mBody[i];
    tmp |= mBody[i];
  }

  return tmp != 0ULL;
}

// @brief オペランドに含まれているリテラルを取り除く
// @param[in] src オペランドのキューブ
void
BfoCube::make_diff(const BfoCube* src)
{
  ASSERT_COND( variable_num() == src->variable_num() );

  ymuint n = mMgr.cube_size();
  for (ymuint i = 0; i < n; ++ i) {
    mBody[i] &= ~src->mBody[i];
  }
}
#endif

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
void
BfoCube::print(ostream& s) const
{
  mgr().print(s, 1, mBody);
}

END_NAMESPACE_YM_BFO
