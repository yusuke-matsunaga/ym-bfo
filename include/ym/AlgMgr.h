#ifndef YM_ALGMGR_H
#define YM_ALGMGR_H

/// @file AlgMgr.h
/// @brief AlgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/bfo_nsdef.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM_BFO

//////////////////////////////////////////////////////////////////////
/// @class AlgMgr AlgMgr.h "ym/AlgMgr.h"
/// @brief AlgCube, AlgCover を管理するクラス
///
/// といっても実際の役割は入力数を覚えておくことと
/// 変数名のリストを持っておくことだけ．
//////////////////////////////////////////////////////////////////////
class AlgMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// 変数名はデフォルトのものが使用される．
  AlgMgr(ymuint variable_num);

  /// @brief コンストラクタ
  /// @param[in] varname_list 変数名のリスト
  ///
  /// varname_list のサイズが変数の数になる．
  AlgMgr(const vector<string>& varname_list);

  /// @brief デストラクタ
  ~AlgMgr();


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
  // AlgCube/AlgCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからリテラルを取り出す．
  /// @param[in] bv ビットベクタ
  /// @param[in] cube_id キューブ番号
  /// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
  AlgPol
  literal(const ymuint64* bv,
	  ymuint cube_id,
	  ymuint var_id);

  /// @brief ビットベクタ上のリテラル数を数える．
  /// @param[in] nc キューブ数
  /// @param[in] bv カバーを表すビットベクタ
  ymuint
  literal_num(ymuint nc,
	      const ymuint64* bv);

  /// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
  /// @param[in] nc キューブ数
  /// @param[in] bv カバーを表すビットベクタ
  /// @param[in] lit 対象のリテラル
  ymuint
  literal_num(ymuint nc,
	      const ymuint64* bv,
	      AlgLiteral lit);

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


public:
  //////////////////////////////////////////////////////////////////////
  // カバーを表すビットベクタに対する処理
  // 基本的に最初に結果を格納するビットベクタのポインタを引数にとる．
  // 結果のキューブ数は事前にはわからないので最大のキューブ数を計算し
  // 領域を確保しておく必要がある．
  // オペランドのカバーはビットベクタとキューブ数の組で表す．
  // 結果のキューブ数を関数の戻り値として返す．
  //////////////////////////////////////////////////////////////////////

  /// @brief カバー/キューブを表す文字列をパーズする．
  /// @param[in] str 対象の文字列
  /// @param[out] lit_list パーズ結果のリテラルのリスト
  /// @return キューブ数を返す．
  ///
  /// lit_list 中の kAlgLiteralUndef はキューブの区切りとみなす．
  ymuint
  parse(const char* str,
	vector<AlgLiteral>& lit_list);

  /// @brief リテラルをセットする．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] dst_pos 対象のキューブ位置
  /// @param[in] lit_list リテラルのリスト
  ///
  /// lit_list 中の kAlgLiteralUndef はキューブの区切りとみなす．
  void
  set_literal(ymuint64* dst_bv,
	      ymuint dst_pos,
	      const vector<AlgLiteral>& lit_list);

  /// @brief 2つのカバーの論理和を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバーのキューブ数
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @return 結果のキューブ数を返す．
  ///
  /// dst_bv には十分な容量があると仮定する．<br>
  /// dst_bv == bv1 の場合もあり得る．<br>
  ymuint
  sum(ymuint64* dst_bv,
      ymuint nc1,
      const ymuint64* bv1,
      ymuint nc2,
      const ymuint64* bv2);

  /// @brief 2つのカバーの差分を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバーのキューブ数
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @return 結果のキューブ数を返す．
  ymuint
  diff(ymuint64* dst_bv,
       ymuint nc1,
       const ymuint64* bv1,
       ymuint nc2,
       const ymuint64* bv2);

  /// @brief 2つのカバーの論理積を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバーのキューブ数
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @return 結果のキューブ数を返す．
  ymuint
  product(ymuint64* dst_bv,
	  ymuint nc1,
	  const ymuint64* bv1,
	  ymuint nc2,
	  const ymuint64* bv2);

  /// @brief カバーとリテラルとの論理積を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] lit 対象のリテラル
  /// @return 結果のキューブ数を返す．
  ymuint
  product(ymuint64* dst_bv,
	  ymuint nc1,
	  const ymuint64* bv1,
	  AlgLiteral lit);

  /// @brief カバーの代数的除算を行う．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめのカバー(除数)のキューブ数
  /// @param[in] bv2 2つめのカバー(除数)を表すビットベクタ
  /// @return 結果のキューブ数を返す．
  ymuint
  division(ymuint64* dst_bv,
	   ymuint nc1,
	   const ymuint64* bv1,
	   ymuint nc2,
	   const ymuint64* bv2);

  /// @brief カバーをリテラルで割る．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 カバーのキューブ数
  /// @param[in] bv1 カバーを表すビットベクタ
  /// @param[in] lit 対象のリテラル
  /// @return 結果のキューブ数を返す．
  ymuint
  division(ymuint64* dst_bv,
	   ymuint nc1,
	   const ymuint64* bv1,
	   AlgLiteral lit);

  /// @brief カバー中のすべてのキューブの共通部分を求める．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 カバーのキューブ数
  /// @param[in] bv1 カバーを表すビットベクタ
  ///
  /// 共通部分がないときは空のキューブとなる．
  void
  common_cube(ymuint64* dst_bv,
	      ymuint nc1,
	      const ymuint64* bv1);

  /// @brief カバー(を表すビットベクタ)のコピーを行う．
  /// @param[in] cube_num キューブ数
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] dst_pos コピー先のキューブ位置
  /// @param[in] src_bv ソースのビットベクタ
  /// @param[in] src_pos ソースのキューブ位置
  void
  copy(ymuint cube_num,
       ymuint64* dst_bv,
       ymuint dst_pos,
       const ymuint64* src_bv,
       ymuint src_pos);

  /// @brief カバー(を表すビットベクタ)を整列する．
  /// @param[in] cube_num キューブ数
  /// @param[in] bv ビットベクタ
  void
  sort(ymuint cube_num,
       ymuint64* bv);

  /// @brief カバー(を表すビットベクタ)の比較を行う．
  /// @param[in] nc1 1つめのカバーのキューブ数
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] nc2 2つめカバーのキューブ数
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  ///
  /// 比較はキューブごとの辞書式順序で行う．
  int
  compare(ymuint nc1,
	  const ymuint64* bv1,
	  ymuint nc2,
	  const ymuint64* bv2);

  /// @brief ビットベクタからハッシュ値を計算する．
  /// @param[in] nc キューブ数
  /// @param[in] bv ビットベクタ
  ymuint
  hash(ymuint nc,
       const ymuint64* bv);


public:
  //////////////////////////////////////////////////////////////////////
  // キューブに対する処理
  // 基本的にはキューブはビットベクタの先頭アドレスとキューブ位置で表す．
  // キューブのサイズ(変数の個数に依存)によってキューブの開始アドレスは変化する．
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(const ymuint64* bv1,
	       ymuint pos1,
	       const ymuint64* bv2,
	       ymuint pos2);

  /// @brief 2つのキューブの積が空でない時 true を返す．
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  bool
  cube_check_product(const ymuint64* bv1,
		     ymuint pos1,
		     const ymuint64* bv2,
		     ymuint pos2);

  /// @brief 一方のキューブが他方のキューブに含まれているか調べる．
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
  bool
  cube_check_containment(const ymuint64* bv1,
			 ymuint pos1,
			 const ymuint64* bv2,
			 ymuint pos2);

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @return ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  cube_check_intersect(const ymuint64* bv1,
		       ymuint pos1,
		       const ymuint64* bv2,
		       ymuint pos2);

  /// @brief キューブ(を表すビットベクタ)のコピーを行う．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] dst_pos コピー先のキューブ番号
  /// @param[in] src_bv ソースのビットベクタ
  /// @param[in] src_pos ソースのキューブ番号
  void
  cube_copy(ymuint64* dst_bv,
	    ymuint dst_pos,
	    const ymuint64* src_bv,
	    ymuint src_pos);

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] dst_pos 対象のキューブ番号
  void
  cube_clear(ymuint64* dst_bv,
	     ymuint dst_pos);

  /// @brief 2つのキューブの積を計算する．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] dst_pos コピー先のキューブ番号
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product(ymuint64* dst_bv,
	       ymuint dst_pos,
	       const ymuint64* bv1,
	       ymuint pos1,
	       const ymuint64* bv2,
	       ymuint pos2);

  /// @brief キューブによる商を求める．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] dst_pos コピー先のキューブ番号
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_division(ymuint64* dst_bv,
		ymuint dst_pos,
		const ymuint64* bv1,
		ymuint pos1,
		const ymuint64* bv2,
		ymuint pos2);

  /// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  void
  cube_swap(ymuint64* bv1,
	    ymuint pos1,
	    ymuint64* bv2,
	    ymuint pos2);

  /// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @param[in] bv3 3つめのカバーを表すビットベクタ
  /// @param[in] pos3 3つめのキューブ番号
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv1 となる．
  void
  cube_rotate3(ymuint64* bv1,
	       ymuint pos1,
	       ymuint64* bv2,
	       ymuint pos2,
	       ymuint64* bv3,
	       ymuint pos3);

  /// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @param[in] bv3 3つめのカバーを表すビットベクタ
  /// @param[in] pos3 3つめのキューブ番号
  /// @param[in] bv4 4つめのカバーを表すビットベクタ
  /// @param[in] pos4 4つめのキューブ番号
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv4, bv4 <- bv1 となる．
  void
  cube_rotate4(ymuint64* bv1,
	       ymuint pos1,
	       ymuint64* bv2,
	       ymuint pos2,
	       ymuint64* bv3,
	       ymuint pos3,
	       ymuint64* bv4,
	       ymuint pos4);

  /// @brief カバー/キューブの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] bv カバー/キューブを表すビットベクタ
  /// @param[in] start キューブの開始位置
  /// @param[in] end キューブの終了位置
  ///
  /// end は実際の末尾 + 1 を指す．
  void
  print(ostream& s,
	const ymuint64* bv,
	ymuint start,
	ymuint end);


public:
  //////////////////////////////////////////////////////////////////////
  // リテラル集合に対する演算
  // 基本キューブと同じだが相反するリテラルがあってもOK
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のチェック
  /// @param[in] bv ビットベクタ
  /// @param[in] lit 対象のリテラル
  /// @return lit を含んでいたら true を返す．
  bool
  is_in(ymuint64* bv,
	AlgLiteral lit);

  /// @brief 要素の追加
  /// @param[in] bv ビットベクタ
  /// @param[in] lit 対象のリテラル
  void
  add_lit(ymuint64* bv,
	  AlgLiteral lit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マージソートを行う下請け関数
  /// @param[in] bv 対象のビットベクタ
  /// @param[in] start 開始位置
  /// @param[in] end 終了位置
  ///
  /// bv[start] - bv[end - 1] の領域をソートする．
  void
  _sort(ymuint64* bv,
	ymuint start,
	ymuint end);

  /// @brief mTmpBuff を初期化する．
  void
  _init_buff();

  /// @brief mTmpBuff に必要な領域を確保する．
  /// @param[in] req_size 要求するキューブ数
  void
  _resize_buff(ymuint req_size);

  /// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
  /// @param[in] src_bv ソースのビットベクタ
  /// @param[in] src_pos ソースのキューブ番号
  void
  _cube_save(const ymuint64* src_bv,
	     ymuint src_pos);

  /// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] dst_pos コピー先のキューブ番号
  void
  _cube_restore(ymuint64* dst_bv,
	       ymuint dst_pos);

  /// @brief ブロック位置を計算する．
  /// @param[in] var_id 変数番号
  static
  ymuint
  _block_pos(ymuint var_id);

  /// @brief シフト量を計算する．
  /// @param[in] var_id 変数番号
  static
  ymuint
  _shift_num(ymuint var_id);

  /// @brief キューブ1つ分のブロックサイズを計算する．
  ymuint
  _cube_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  ymuint mVarNum;

  // 変数名のリスト
  vector<string> mVarNameList;

  // 変数名と変数番号のハッシュ表
  HashMap<string, ymuint> mVarNameMap;

  // 作業用に用いられるビットベクタ
  ymuint64* mTmpBuff;

  // mTmpBuff のキューブ数
  ymuint mTmpBuffSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
ymuint
AlgMgr::variable_num() const
{
  return mVarNum;
}

// @brief 変数名を返す．
// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
inline
string
AlgMgr::varname(ymuint var_id) const
{
  ASSERT_COND( var_id < variable_num() );
  return mVarNameList[var_id];
}

// @brief ビットベクタからリテラルを取り出す．
// @param[in] bv ビットベクタ
// @param[in] cube_id キューブ番号
// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
inline
AlgPol
AlgMgr::literal(const ymuint64* bv,
		ymuint cube_id,
		ymuint var_id)
{
  ASSERT_COND( var_id < variable_num() );

  ymuint blk = _block_pos(var_id) + _cube_size() * cube_id;
  ymuint sft = _shift_num(var_id);
  return static_cast<AlgPol>((bv[blk] >> sft) & 3ULL);
}

// @brief カバー(を表すビットベクタ)を整列する．
// @param[in] cube_num キューブ数
// @param[in] bv ビットベクタ
inline
void
AlgMgr::sort(ymuint cube_num,
	     ymuint64* bv)
{
  // 下請け関数を呼ぶだけ
  _sort(bv, 0, cube_num);
}

// @brief キューブ(を表すビットベクタ)のコピーを行う．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ番号
// @param[in] src_bv ソースのビットベクタ
// @param[in] src_pos ソースのキューブ番号
inline
void
AlgMgr::cube_copy(ymuint64* dst_bv,
		  ymuint dst_pos,
		  const ymuint64* src_bv,
		  ymuint src_pos)
{
  // キューブ数が1の場合のコピー
  copy(1, dst_bv, dst_pos, src_bv, src_pos);
}

// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
inline
void
AlgMgr::cube_swap(ymuint64* bv1,
		  ymuint pos1,
		  ymuint64* bv2,
		  ymuint pos2)
{
  _cube_save(bv1, pos1);
  cube_copy(bv1, pos1, bv2, pos2);
  _cube_restore(bv2, pos2);
}

// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
// @param[in] bv3 3つめのカバーを表すビットベクタ
// @param[in] pos3 3つめのキューブ番号
//
// bv1 <- bv2, bv2 <- bv3, bv3 <- bv1 となる．
inline
void
AlgMgr::cube_rotate3(ymuint64* bv1,
		     ymuint pos1,
		     ymuint64* bv2,
		     ymuint pos2,
		     ymuint64* bv3,
		     ymuint pos3)
{
  _cube_save(bv1, pos1);
  cube_copy(bv1, pos1, bv2, pos2);
  cube_copy(bv2, pos2, bv3, pos3);
  _cube_restore(bv3, pos3);
}

// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] pos1 1つめのキューブ番号
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] pos2 2つめのキューブ番号
// @param[in] bv3 3つめのカバーを表すビットベクタ
// @param[in] pos3 3つめのキューブ番号
// @param[in] bv4 4つめのカバーを表すビットベクタ
// @param[in] pos4 4つめのキューブ番号
//
// bv1 <- bv2, bv2 <- bv3, bv3 <- bv4, bv4 <- bv1 となる．
inline
void
AlgMgr::cube_rotate4(ymuint64* bv1,
		     ymuint pos1,
		     ymuint64* bv2,
		     ymuint pos2,
		     ymuint64* bv3,
		     ymuint pos3,
		     ymuint64* bv4,
		     ymuint pos4)
{
  _cube_save(bv1, pos1);
  cube_copy(bv1, pos1, bv2, pos2);
  cube_copy(bv2, pos2, bv3, pos3);
  cube_copy(bv3, pos3, bv4, pos4);
  _cube_restore(bv4, pos4);
}

// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
// @param[in] src_bv ソースのビットベクタ
// @param[in] src_pos ソースのキューブ番号
inline
void
AlgMgr::_cube_save(const ymuint64* src_bv,
		   ymuint src_pos)
{
  cube_copy(mTmpBuff, 0, src_bv, src_pos);
}

// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] dst_pos コピー先のキューブ番号
inline
void
AlgMgr::_cube_restore(ymuint64* dst_bv,
		      ymuint dst_pos)
{
  cube_copy(dst_bv, dst_pos, mTmpBuff, 0);
}

// @brief ブロック位置を計算する．
// @param[in] var_id 変数番号
inline
ymuint
AlgMgr::_block_pos(ymuint var_id)
{
  return var_id / 32;
}

// @brief シフト量を計算する．
// @param[in] var_id 変数番号
inline
ymuint
AlgMgr::_shift_num(ymuint var_id)
{
  // ソートしたときの見栄えの問題で左(MSB)から始める．
  return (31 - (var_id % 32)) * 2;
}

// @brief キューブ1つ分のブロックサイズを計算する．
inline
ymuint
AlgMgr::_cube_size() const
{
  return (variable_num() + 31) / 32;
}

END_NAMESPACE_YM_BFO

#endif // YM_ALGCUBEMGR_H
