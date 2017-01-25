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
  /// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
  string
  varname(ymuint var_id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // BfoCube/BfoCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのカバーの論理和を計算する．
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバーのキューブ数
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @return 結果のカバーを返す．
  BfoCover
  make_sum(ymuint nc1,
	   const ymuint64* bv1,
	   ymuint nc2,
	   const ymuint64* bv2);

  /// @brief 2つのカバーの差分を計算する．
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバーのキューブ数
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @return 結果のカバーを返す．
  BfoCover
  make_diff(ymuint nc1,
	    const ymuint64* bv1,
	    ymuint nc2,
	    const ymuint64* bv2);

  /// @brief 2つのカバーの論理積を計算する．
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバーのキューブ数
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @return 結果のカバーを返す．
  BfoCover
  make_product(ymuint nc1,
	       const ymuint64* bv1,
	       ymuint nc2,
	       const ymuint64* bv2);

  /// @brief カバーの代数的除算を行う．
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバー(除数)のキューブ数
  /// @param[in] bv2 2つめのカバー(除数)を表すビットベクタ
  /// @return 結果のカバーを返す．
  BfoCover
  make_division(ymuint nc1,
		const ymuint64* bv1,
		ymuint nc2,
		const ymuint64* bv2);

  /// @brief カバーをキューブで割る(コファクター演算)．
  /// @param[in] nc1 カバーのキューブ数
  /// @param[in] bv1 カバーを表すビットベクタ
  /// @param[in] bv2 キューブを表すビットベクタ
  /// @return 結果のカバーを返す．
  BfoCover
  make_division(ymuint nc1,
		const ymuint64* bv1,
		const ymuint64* bv2);

  /// @brief カバーをリテラルで割る(コファクター演算)．
  /// @param[in] nc1 カバーのキューブ数
  /// @param[in] bv1 カバーを表すビットベクタ
  /// @param[in] lit 対象のリテラル
  BfoCover
  make_division(ymuint nc1,
		const ymuint64* bv1,
		BfoLiteral lit);

  /// @brief カバー中のすべてのキューブの共通部分を求める．
  /// @param[in] nc1 カバーのキューブ数
  /// @param[in] bv1 カバーを表すビットベクタ
  /// @return 結果のキューブを返す．
  ///
  /// 共通部分がないときは空のキューブを返す．
  BfoCube
  common_cube(ymuint nc1,
	      const ymuint64* bv1);

  /// @brief ビットベクタからリテラルを取り出す．
  /// @param[in] bv ビットベクタ
  /// @param[in] cube_id キューブ番号
  /// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
  BfoPol
  literal(const ymuint64* bv,
	  ymuint cube_id,
	  ymuint var_id);

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

  /// @brief カバー/キューブの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] nc キューブ数
  /// @param[in] bv カバー/キューブを表すビットベクタ
  ///
  /// キューブの場合は nc を 1 とする．
  void
  print(ostream& s,
	ymuint nc,
	const ymuint64* bv);

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
// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
inline
string
BfoMgr::varname(ymuint var_id) const
{
  ASSERT_COND( var_id < variable_num() );
  return mVarNameList[var_id];
}

// @brief ビットベクタからリテラルを取り出す．
// @param[in] bv ビットベクタ
// @param[in] cube_id キューブ番号
// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
inline
BfoPol
BfoMgr::literal(const ymuint64* bv,
		ymuint cube_id,
		ymuint var_id)
{
  ASSERT_COND( var_id < variable_num() );

  ymuint blk = block_pos(var_id) + cube_size() * cube_id;
  ymuint sft = shift_num(var_id);
  return static_cast<BfoPol>((bv[blk] >> sft) & 3ULL);
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
