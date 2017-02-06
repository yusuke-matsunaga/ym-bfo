#ifndef ALGKERNELMGR_H
#define ALGKERNELMGR_H

/// @file AlgKernelMgr.h
/// @brief AlgKernelMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/AlgCover.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM

/// @breif AlgCover* をキーにしたハッシュ関数クラスの定義
template <>
struct HashFunc<const AlgCover*>
{
  ymuint
  operator()(const AlgCover* cover) const
  {
    return cover->hash();
  }
};

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class AlgKernelMgr AlgKernelMgr.h "AlgKernelMgr.h"
/// @brief カーネルとコカーネルの情報を保持するクラス
//////////////////////////////////////////////////////////////////////
class AlgKernelMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  AlgKernelMgr(AlgMgr& mgr);

  /// @brief デストラクタ
  ~AlgKernelMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを求める．
  /// @param[in] cover 対象のカバー
  void
  find_kernels(const AlgCover& cover);

  /// @brief 直前の find_kernels() で求めたカーネルの個数を返す．
  ymuint
  kernel_num() const;

  /// @brief 直前の find_kernels() で求めたカーネルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < kernel_num() )
  const AlgCover&
  kernel(ymuint pos) const;

  /// @brief 直前の find_kernels() で求めたコカーネルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < kernel_num() )
  ///
  /// コカーネルはキューブだが一つのカーネルに複数のコカーネル
  /// が対応する場合があるので全体としてカバーで表す．
  const AlgCover&
  cokernel(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mKernelList と mCokernelList をクリアする．
  void
  _clear();

  /// @brief カーネルを求める再帰関数
  /// @param[in] cover 対象のカバー
  /// @param[in] lit_list リテラル(と出現頻度)のリスト
  /// @param[in] pos 次に処理するリテラルリストの先頭位置
  /// @param[in] ckcube 現在のコカーネル
  /// @param[in] skip_cube すでに試したリテラルの集合を表すキューブ
  void
  _kernel_sub(const AlgCover& cover,
	      vector<pair<AlgLiteral, ymuint> >& lit_list,
	      ymuint pos,
	      const AlgCube& ckcube,
	      const AlgCube& skip_cube);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  AlgMgr& mMgr;

  // カーネルのリスト
  vector<AlgCover*> mKernelList;

  // コカーネルのリスト
  vector<AlgCover*> mCokernelList;

  // カーネルをキーにして番号を保持するハッシュ表
  HashMap<const AlgCover*, ymuint> mKernelHash;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 直前の find_kernels() で求めたカーネルの個数を返す．
inline
ymuint
AlgKernelMgr::kernel_num() const
{
  return mKernelList.size();
}

// @brief 直前の find_kernels() で求めたカーネルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < kernel_num() )
inline
const AlgCover&
AlgKernelMgr::kernel(ymuint pos) const
{
  ASSERT_COND( pos < kernel_num() );

  return *mKernelList[pos];
}

// @brief 直前の find_kernels() で求めたコカーネルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < kernel_num() )
//
// コカーネルはキューブだが一つのカーネルに複数のコカーネル
// が対応する場合があるので全体としてカバーで表す．
inline
const AlgCover&
AlgKernelMgr::cokernel(ymuint pos) const
{
  ASSERT_COND( pos < kernel_num() );

  return *mCokernelList[pos];
}

END_NAMESPACE_YM_BFO

#endif // ALGKERNELMGR_H
