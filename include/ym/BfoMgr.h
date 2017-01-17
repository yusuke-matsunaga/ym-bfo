#ifndef BFOMGR_H
#define BFOMGR_H

/// @file BfoMgr.h
/// @brief BfoMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class BfoMgr BfoMgr.h "BfoMgr.h"
/// @brief BfoCube, BfoCover を管理するクラス
///
/// といっても実際の役割は入力数を覚えておくことと
/// 変数名のリストを持っておくことだけ．
//////////////////////////////////////////////////////////////////////
class BfoMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// 変数名はデフォルトのものが使用される．
  BfoMgr(ymuint variable_num);

  /// @brief コンストラクタ
  /// @param[in] varname_list 変数名のリスト
  ///
  /// varname_list のサイズが変数の数になる．
  BfoMgr(const vector<string>& varname_list);

  /// @brief デストラクタ
  ~BfoMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  ymuint
  variable_num() const;

  /// @brief 変数名を返す．
  /// @param[in] varid 変数番号 ( 0 <= varid < variable_num() )
  string
  varname(ymuint varid) const;


public:
  //////////////////////////////////////////////////////////////////////
  // BfoCube/BfoCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ/カバー用の領域を確保する．
  /// @param[in] cube_num キューブ数
  ///
  /// キューブの時は cube_num = 1 とする．
  ymuint64*
  new_body(ymuint cube_num = 1);

  /// @brief キューブ/カバー用の領域を削除する．
  /// @param[in] p 領域を指すポインタ
  /// @param[in] cube_num キューブ数
  ///
  /// キューブの時は cube_num = 1 とする．
  void
  delete_body(ymuint64* p,
	      ymuint cube_num = 1);

  /// @brief ブロック位置を計算する．
  /// @param[in] var_id 変数番号
  static
  ymuint
  block_pos(ymuint var_id);

  /// @brief シフト量を計算する．
  /// @param[in] var_id 変数番号
  static
  ymuint
  shift_num(ymuint var_id);

  /// @brief キューブ1つ分のブロックサイズを計算する．
  ymuint
  cube_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  ymuint mVarNum;

  // 変数名のリスト
  vector<string> mVarNameList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
ymuint
BfoMgr::variable_num() const
{
  return mVarNum;
}

// @brief 変数名を返す．
// @param[in] varid 変数番号 ( 0 <= varid < variable_num() )
inline
string
BfoMgr::varname(ymuint varid) const
{
  ASSERT_COND( varid < variable_num() );
  return mVarNameList[varid];
}

// @brief ブロック位置を計算する．
// @param[in] var_id 変数番号
inline
ymuint
BfoMgr::block_pos(ymuint var_id)
{
  return var_id / 32;
}

// @brief シフト量を計算する．
// @param[in] var_id 変数番号
inline
ymuint
BfoMgr::shift_num(ymuint var_id)
{
  // ソートしたときの見栄えの問題で左(MSB)から始める．
  return (32 - (var_id % 32)) * 2;
}

// @brief キューブ1つ分のブロックサイズを計算する．
inline
ymuint
BfoMgr::cube_size() const
{
  return (variable_num() + 31) / 32;
}

END_NAMESPACE_YM_BFO

#endif // BFOCUBEMGR_H
