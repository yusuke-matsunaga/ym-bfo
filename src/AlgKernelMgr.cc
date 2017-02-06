
/// @file AlgKernelMgr.cc
/// @brief AlgKernelMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/AlgKernelMgr.h"
#include "ym/AlgMgr.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_BFO

// @brief コンストラクタ
// @param[in] mgr マネージャ
AlgKernelMgr::AlgKernelMgr(AlgMgr& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
AlgKernelMgr::~AlgKernelMgr()
{
  _clear();
}

// リテラルのソート用の比較関数
struct LitLt
{
  bool
  operator()(const pair<AlgLiteral, ymuint>& left,
	     const pair<AlgLiteral, ymuint>& right)
  {
    return left.second < right.second;
  }
};

// @brief カーネルとコカーネルを求める．
// @param[in] cover 対象のカバー
void
AlgKernelMgr::find_kernels(const AlgCover& cover)
{
  _clear();

  // リテラルを出現頻度の昇順に整列させる．
  ymuint n = cover.variable_num();
  vector<pair<AlgLiteral, ymuint> > tmp_list;
  tmp_list.reserve(n * 2);
  for (ymuint i = 0; i < n; ++ i) {
    AlgLiteral litp(i, false);
    ymuint np = cover.literal_num(litp);
    if ( np >= 2 ) {
      tmp_list.push_back(make_pair(litp, np));
    }
    AlgLiteral litn(i, false);
    ymuint nn = cover.literal_num(litn);
    if ( nn >= 2 ) {
      tmp_list.push_back(make_pair(litn, nn));
    }
  }
  sort(tmp_list.begin(), tmp_list.end(), LitLt());

  // 実際にカーネルを求める再帰関数を呼ぶ．
  AlgCube ckcube0(mMgr);
  AlgCube skip_cube0(mMgr);
  _kernel_sub(cover, tmp_list, 0, ckcube0, skip_cube0);
}

// @brief カーネルを求める再帰関数
// @param[in] cover 対象のカバー
// @param[in] lit_list リテラル(と出現頻度)のリスト
// @param[in] pos 次に処理するリテラルリストの先頭位置
// @param[in] ckcube 現在のコカーネル
// @param[in] skip_cube すでに試したリテラルの集合を表すキューブ
void
AlgKernelMgr::_kernel_sub(const AlgCover& cover,
			  vector<pair<AlgLiteral, ymuint> >& lit_list,
			  ymuint pos,
			  const AlgCube& ckcube,
			  const AlgCube& skip_cube)
{
  ymuint n = lit_list.size();
  AlgCube skip_cube1(skip_cube);
  for (ymuint i = pos; i < n; ++ i) {
    AlgLiteral lit = lit_list[i].first;

    // lit のコファクターを計算する．
    AlgCover cover1 = cover / lit;
    if ( cover1.cube_num() == 0 ) {
      // 空ならスキップ
      continue;
    }

    // 共通のキューブを求める．
    AlgCube ccube = cover1.common_cube();
    // ccube に lit_list の pos 以前のリテラルが含まれている場合はスキップする．
    if ( ccube.check_intersect(skip_cube) ) {
      continue;
    }

    // キューブフリーにする．
    cover1 /= ccube;

    // 定義により cover1 はカーネル
    // 対応するコカーネルは ckcube & ccube
    ccube &= ckcube;

    ymuint kid;
    if ( mKernelHash.find(&cover1, kid) ) {
      (*mCokernelList[kid]) += ccube;
    }
    else {
      AlgCover* kernel = new AlgCover(cover1);
      kid = mKernelList.size();
      mKernelHash.add(kernel, kid);
      mKernelList.push_back(kernel);
      AlgCover* ckcube = new AlgCover(ccube);
      mCokernelList.push_back(ckcube);
    }

    skip_cube1 &= AlgCube(mMgr, lit);
    _kernel_sub(cover1, lit_list, pos + 1, ccube, skip_cube1);
  }
}

// @brief mKernelList と mCokernelList をクリアする．
void
AlgKernelMgr::_clear()
{
  ymuint n = kernel_num();
  for (ymuint i = 0; i < n; ++ i) {
    delete mKernelList[i];
    delete mCokernelList[i];
  }

  mKernelHash.clear();
}

END_NAMESPACE_YM_BFO
