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

  /// @brief ビットベクタからリテラルを取り出す．
  /// @param[in] bv ビットベクタ
  /// @param[in] cube_id キューブ番号
  /// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
  BfoPol
  literal(const ymuint64* bv,
	  ymuint cube_id,
	  ymuint var_id);

  /// @brief ビットベクタ上のリテラル数を数える．
  /// @param[in] nc キューブ数
  /// @param[in] bv カバーを表すビットベクタ
  ymuint
  literal_num(ymuint nc,
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


public:
  //////////////////////////////////////////////////////////////////////
  // カバーを表すビットベクタに対する処理
  // 基本的に最初に結果を格納するビットベクタのポインタを引数にとる．
  // 結果のキューブ数は事前にはわからないので最大のキューブ数を計算し
  // 領域を確保しておく必要がある．
  // オペランドのカバーはビットベクタとキューブ数の組で表す．
  // 結果のキューブ数を関数の戻り値として返す．
  //////////////////////////////////////////////////////////////////////

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

  /// @brief カバーをリテラルで割る(コファクター演算)．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] nc1 カバーのキューブ数
  /// @param[in] bv1 カバーを表すビットベクタ
  /// @param[in] lit 対象のリテラル
  /// @return 結果のキューブ数を返す．
  ymuint
  division(ymuint64* dst_bv,
	   ymuint nc1,
	   const ymuint64* bv1,
	   BfoLiteral lit);

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
  cover_copy(ymuint cube_num,
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

  /// @brief リテラルの集合からキューブを表すビットベクタにセットする．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] dst_pos コピー先のキューブ番号
  /// @param[in] lit_list リテラルのリスト
  void
  cube_set(ymuint64* dst_bv,
	   ymuint dst_pos,
	   const vector<BfoLiteral>& lit_list);

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

  /// @brief キューブによるコファクターを求める．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] dst_pos コピー先のキューブ番号
  /// @param[in] bv1 1つめのカバーを表すビットベクタ
  /// @param[in] pos1 1つめのキューブ番号
  /// @param[in] bv2 2つめのカバーを表すビットベクタ
  /// @param[in] pos2 2つめのキューブ番号
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_cofactor(ymuint64* dst_bv,
		ymuint dst_pos,
		const ymuint64* bv1,
		ymuint pos1,
		const ymuint64* bv2,
		ymuint pos2);

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

  /// @brief mTmpBuff を初期化する．
  void
  init_buff();

  /// @brief mTmpBuff に必要な領域を確保する．
  /// @param[in] req_size 要求するキューブ数
  void
  resize_buff(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  ymuint mVarNum;

  // 変数名のリスト
  vector<string> mVarNameList;

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

// @brief カバー(を表すビットベクタ)を整列する．
// @param[in] cube_num キューブ数
// @param[in] bv ビットベクタ
inline
void
BfoMgr::sort(ymuint cube_num,
	     ymuint64* bv)
{
  // 下請け関数を呼ぶだけ
  _sort(bv, 0, cube_num);
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
  return (31 - (var_id % 32)) * 2;
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
