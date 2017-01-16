#ifndef BFOCOVER_H
#define BFOCOVER_H

/// @file BfoCover.h
/// @brief BfoCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class BfoCover BfoCover.h "BfoCover.h"
/// @brief カバー(積和形論理式)を表すクラス
///
/// 中身は BfoCube のリスト<br>
/// BfoCube は常に唯一つの BfoCover に属する<br>
//////////////////////////////////////////////////////////////////////
class BfoCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr ネージャ
  ///
  /// キューブを一つも持たない空のカバーとなる．
  BfoCover(BfoMgr& mgr);

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~BfoCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  ymuint
  variable_num() const;

  /// @brief キューブの数を返す．
  ymuint
  cube_num() const;

  /// @brief リテラル数を返す．
  ymuint
  literal_num() const;

  /// @brief キューブを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < cube_num() )
  const BfoCube*
  cube(ymuint pos) const;

  /// @brief 内容をクリアする．
  ///
  /// ここに属しているすべてのキューブは削除される．
  void
  clear();

  /// @brief 新しいキューブを追加する．
  /// @param[in] cube 追加するキューブ
  ///
  /// 実際には cube のコピーを追加する．
  void
  add_cube(const BfoCube* cube);

  /// @brief 2つのキューブの積を計算して追加する．
  /// @param[in] cube1, cube2 積をとるキューブ
  ///
  /// 積が空キューブになった場合には追加しない．
  void
  add_product(const BfoCube* cube1,
	      const BfoCube* cube2);

  /// @brief 内容をわかりやすい形で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] varname_list 変数名のリスト
  ///
  /// varname_list が省略された時には適当な名前を作る．<br>
  /// varname_list のサイズは variable_num() 以上でなければならない．
  void
  print(ostream& s,
	const vector<string>& varname_list = vector<string>()) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブマネージャ
  BfoMgr& mMgr;

  // キューブのリスト
  vector<BfoCube*> mCubeList;

};

/// @relates BfoCover
/// @brief BfoCover の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cover 対象のカバー
///
/// cover.print(s) と等価
ostream&
operator<<(ostream& s,
	   const BfoCover& cover);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief BfoCover の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cover 対象のカバー
//
// cover.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const BfoCover& cover)
{
  cover.print(s);
  return s;
}

END_NAMESPACE_YM_BFO

#endif // BFOCOVER_H
