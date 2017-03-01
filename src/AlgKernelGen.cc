
/// @file AlgKernelGen.cc
/// @brief AlgKernelGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/AlgKernelGen.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
// クラス AlgKernelGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AlgKernelGen::AlgKernelGen()
{
}

// @brief デストラクタ
AlgKernelGen::~AlgKernelGen()
{
}

// @brief カーネルとコカーネルを列挙する．
// @param[in] cover 対象のカバー
// @param[out] kernel_list 結果を格納するリスト
void
AlgKernelGen::generate(const AlgCover& cover,
		       vector<AlgKernelInfo>& kernel_list)
{
  // cover に現れるリテラルの出現頻度の昇順のリストを作る．


  AlgCube ccube0(cover.mgr());
  AlgLitSet plits0(cover.mgr());
  kern_sub(cover, 0, ccube0, plits0);

  // kernel_list に答を入れる．
}

// @brief カーネルを求める下請け関数
// @param[in] cover 対象のカバー
// @param[in] pos mLitList 上の位置
// @param[in] ccube 今までに括りだされた共通のキューブ
// @param[in] plits mLitList[0]〜mLitList[pos - 1] までをまとめたリテラル集合
ymuint
AlgKernelGen::kern_sub(const AlgCover& cover,
		       ymuint pos,
		       const AlgCube& ccube,
		       const AlgLitSet& plits)
{
  ymuint max_level = 0;
  AlgLitSet plits1(plits);
  for (ymuint i = pos; i < mLitList.size(); ++ i) {
    AlgLiteral lit = mLitList[i];
    if ( cover.literal_num(lit) <= 1 ) {
      continue;
    }

    // まず lit で割る．
    AlgCover cover1 = cover / lit;
    // 共通なキューブを求める．
    AlgCube ccube1 = cover1.common_cube();
    if ( ccube1.contains(plits1) ) {
      // これはすでに処理されている．
      continue;
    }

    cover1 /= ccube1;
    ccube1 *= ccube;
    plits1 += lit;

    ymuint level = kern_sub(cover1, pos + 1, ccube1, plits1);

    // cover1/ccube1/level を記録．

    if ( max_level < level + 1 ) {
      max_level = level + 1;
    }
  }

  return max_level;
}

END_NAMESPACE_YM_BFO
