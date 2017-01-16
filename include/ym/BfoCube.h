#ifndef BFOCUBE_H
#define BFOCUBE_H

/// @file BfoCube.h
/// @brief BfoCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class BfoCube BfoCube.h "BfoCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// 常に固定サイズのビット配列として実装する．<br>
/// 1つの変数につき2ビットを使用する．<br>
/// 少し特殊なことをしているので普通にアロケートすることはできない．
//////////////////////////////////////////////////////////////////////
class BfoCube
{
  friend class BfoMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  BfoCube(ymuint variable_num);

  /// @brief デストラクタ
  ~BfoCube();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  ymuint
  variable_num() const;

  /// @brief 内容を読み出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
  BfoLiteral
  literal(ymuint pos) const;

  /// @brief 内容を設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
  /// @param [in] lit リテラルの値
  void
  set_literal(ymuint pos,
	      BfoLiteral lit);

  /// @brief リテラル数を返す．
  ymuint
  literal_num() const;

  /// @brief 内容をコピーする．
  /// @param[in] src コピー元のキューブ
  void
  copy(const BfoCube* src);

  /// @brief オペランドのキューブに含まれていたら true を返す．
  /// @param[in] src オペランドのキューブ
  ///
  /// ここではキューブの表す論理関数の含意を考える<br>
  /// だからリテラル集合としては真逆になる．
  bool
  check_contain(const BfoCube* src) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @param[in] src オペランドのキューブ
  /// @return 結果が NULL Cube になったら false を返す．
  ///
  /// false を返した場合の内容は不定
  bool
  make_product(const BfoCube* src);

  /// @brief オペランドに含まれているリテラルを取り除く
  /// @param[in] src オペランドのキューブ
  void
  make_diff(const BfoCube* src);

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

  /// @brief ブロック数を得る．
  ymuint
  _block_num() const;

  /// @brief print内で変数名を作る関数
  /// @param[in] pos 変数番号 ( 0 <= pos < variable_num() )
  /// @param[in] varname_list 変数名のリスト
  ///
  /// varname_list が空の時には適当な名前を作る．
  string
  _varname(ymuint pos,
	   const vector<string>& varname_list) const;

  // friend 関数の宣言
  friend
  int
  compare(const BfoCube* left,
	  const BfoCube* right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  ymuint mVarNum;

  // 内容を表すビットベクタ
  // 実際には本当の要素数に応じたサイズが確保される．
  ymuint64 mBody[1];

};

/// @relates BfoCube
/// @brief BfoCube の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cube 対象のキューブ(のポインタ)
///
/// cube->print(s) と等価
ostream&
operator<<(ostream& s,
	   const BfoCube* cube);

/// @relates BfoCube
/// @brief BfoCube(のポインタ)の比較演算子
/// @param[in] left, right オペランド
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
int
compare(const BfoCube* left,
	const BfoCube* right);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
ymuint
BfoCube::variable_num() const
{
  return mVarNum;
}

// @brief 内容を読み出す．
// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
inline
BfoLiteral
BfoCube::literal(ymuint pos) const
{
  ASSERT_COND( pos < variable_num() );
  ymuint blk = pos / 32;
  // ソートしたときの見栄えの問題で左(MSB)から始める．
  ymuint sft = (32 - (pos % 32)) * 2;
  return static_cast<BfoLiteral>((mBody[blk] >> sft) & 3ULL);
}

// @brief 内容を設定する．
// @param[in] pos 位置番号 ( 0 <= pos < variable_num() )
// @param [in] lit リテラルの値
inline
void
BfoCube::set_literal(ymuint pos,
		     BfoLiteral lit)
{
  ASSERT_COND( pos < variable_num() );
  ymuint blk = pos / 32;
  // ソートしたときの見栄えの問題で左(MSB)から始める．
  ymuint sft = (32 - (pos % 32)) * 2;
  mBody[blk] &= ~(3ULL << sft);
  mBody[blk] |= (static_cast<ymuint64>(lit) << sft);
}

// @brief ブロック数を得る．
inline
ymuint
BfoCube::_block_num() const
{
  return (mVarNum + 31) / 32;
}

// @brief BfoCube の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cube 対象のキューブ(のポインタ)
//
// cube->print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const BfoCube* cube)
{
  cube->print(s);
  return s;
}

END_NAMESPACE_YM_BFO

#endif // BFOCUBE_H
