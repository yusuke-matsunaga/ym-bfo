#ifndef YM_ALGMGR_H
#define YM_ALGMGR_H

/// @file AlgMgr.h
/// @brief AlgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bfo_nsdef.h"


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
  ///
  /// 変数名はデフォルトのものが使用される．
  AlgMgr(
    SizeType variable_num ///< <[in] 変数の数
  );

  /// @brief コンストラクタ
  ///
  /// varname_list のサイズが変数の数になる．
  AlgMgr(
    const vector<string>& varname_list ///< [in] 変数名のリスト
  );

  /// @brief デストラクタ
  ~AlgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  SizeType
  variable_num() const
  {
    return mVarNum;
  }

  /// @brief 変数名を返す．
  string
  varname(
    SizeType var_id ///< [in] 変数番号 ( 0 <= var_id < variable_num() )
  ) const
  {
    ASSERT_COND( var_id < variable_num() );
    return mVarNameList[var_id];
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AlgCube/AlgCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからリテラルの極性を取り出す．
  AlgPol
  literal(
    const ymuint64* bv, ///< [in] ビットベクタ
    SizeType cube_id,   ///< [in] キューブ番号
    SizeType var_id     ///< [in] 変数番号 ( 0 <= var_id < variable_num() )
  )
  {
    ASSERT_COND( var_id < variable_num() );
    SizeType blk = _block_pos(var_id) + _cube_size() * cube_id;
    SizeType sft = _shift_num(var_id);
    return static_cast<AlgPol>((bv[blk] >> sft) & 3ULL);
  }

  /// @brief ビットベクタ上のリテラル数を数える．
  SizeType
  literal_num(
    SizeType nc,       ///< [in] キューブ数
    const ymuint64* bv ///< [in] カバーを表すビットベクタ
  );

  /// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
  SizeType
  literal_num(
    SizeType nc,        ///< [in] キューブ数
    const ymuint64* bv, ///< [in] カバーを表すビットベクタ
    AlgLiteral lit      ///< [in] 対象のリテラル
  );

  /// @brief キューブ/カバー用の領域を確保する．
  ///
  /// キューブの時は cube_num = 1 とする．
  ymuint64*
  new_body(
    SizeType cube_num = 1 ///< [in] キューブ数
  );

  /// @brief キューブ/カバー用の領域を削除する．
  ///
  /// キューブの時は cube_num = 1 とする．
  void
  delete_body(
    ymuint64* p,          ///< [in] 領域を指すポインタ
    SizeType cube_num = 1 ///< [in] キューブ数
  );


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
  /// @return キューブ数を返す．
  ///
  /// lit_list 中の AlgLiteralUndef はキューブの区切りとみなす．
  SizeType
  parse(
    const char* str,             ///< [in] 対象の文字列
    vector<AlgLiteral>& lit_list ///< [out] パーズ結果のリテラルのリスト
  );

  /// @brief リテラルをセットする．
  ///
  /// lit_list 中の AlgLiteralUndef はキューブの区切りとみなす．
  void
  set_literal(
    ymuint64* dst_bv,                  ///< [in] 対象のビットベクタ
    SizeType dst_pos,                  ///< [in] 対象のキューブ位置
    const vector<AlgLiteral>& lit_list ///< [in] リテラルのリスト
  );

  /// @brief 2つのカバーの論理和を計算する．
  /// @return 結果のキューブ数を返す．
  ///
  /// dst_bv には十分な容量があると仮定する．<br>
  /// dst_bv == bv1 の場合もあり得る．<br>
  SizeType
  sum(
    ymuint64* dst_bv,    ///< [in] 結果を格納するビットベクタ
    SizeType nc1,        ///< [in] 1つめのカバーのキューブ数
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType nc2,        ///< [in] 2つめのカバーのキューブ数
    const ymuint64* bv2  ///< [in] 2つめのカバーを表すビットベクタ
  );

  /// @brief 2つのカバーの差分を計算する．
  /// @return 結果のキューブ数を返す．
  SizeType
  diff(
    ymuint64* dst_bv,    ///< [in] 結果を格納するビットベクタ
    SizeType nc1,	 ///< [in] 1つめのカバーのキューブ数
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType nc2,	 ///< [in] 2つめのカバーのキューブ数
    const ymuint64* bv2	 ///< [in] 2つめのカバーを表すビットベクタ
  );

  /// @brief 2つのカバーの論理積を計算する．
  /// @return 結果のキューブ数を返す．
  SizeType
  product(
    ymuint64* dst_bv,    ///< [in] 結果を格納するビットベクタ
    SizeType nc1,	 ///< [in] 1つめのカバーのキューブ数
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType nc2,	 ///< [in] 2つめのカバーのキューブ数
    const ymuint64* bv2	 ///< [in] 2つめのカバーを表すビットベクタ
  );

  /// @brief カバーとリテラルとの論理積を計算する．
  /// @return 結果のキューブ数を返す．
  SizeType
  product(
    ymuint64* dst_bv,    ///< [in] 結果を格納するビットベクタ
    SizeType nc1,	 ///< [in] 1つめのカバーのキューブ数
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    AlgLiteral lit       ///< [in] 対象のリテラル
  );

  /// @brief カバーの代数的除算を行う．
  /// @return 結果のキューブ数を返す．
  SizeType
  division(
    ymuint64* dst_bv,    ///< [in] 結果を格納するビットベクタ
    SizeType nc1,	 ///< [in] 1つめのカバーのキューブ数
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType nc2,	 ///< [in] 2つめのカバーのキューブ数
    const ymuint64* bv2	 ///< [in] 2つめのカバーを表すビットベクタ
  );

  /// @brief カバーをリテラルで割る．
  /// @return 結果のキューブ数を返す．
  SizeType
  division(
    ymuint64* dst_bv,    ///< [in] 結果を格納するビットベクタ
    SizeType nc1,	 ///< [in] 1つめのカバーのキューブ数
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    AlgLiteral lit	 ///< [in] 対象のリテラル
  );

  /// @brief カバー中のすべてのキューブの共通部分を求める．
  ///
  /// 共通部分がないときは空のキューブとなる．
  void
  common_cube(
    ymuint64* dst_bv,   ///< [in] 結果のキューブを格納するビットベクタ
    SizeType nc1,       ///< [in] カバーのキューブ数
    const ymuint64* bv1 ///< [in] カバーを表すビットベクタ
  );

  /// @brief カバー(を表すビットベクタ)のコピーを行う．
  void
  copy(
    SizeType cube_num,      ///< [in] キューブ数
    ymuint64* dst_bv,       ///< [in] コピー先のビットベクタ
    SizeType dst_pos,       ///< [in] コピー先のキューブ位置
    const ymuint64* src_bv, ///< [in] ソースのビットベクタ
    SizeType src_pos        ///< [in] ソースのキューブ位置
  );

  /// @brief カバー(を表すビットベクタ)を整列する．
  void
  sort(
    SizeType cube_num, ///< [in] キューブ数
    ymuint64* bv       ///< [in] ビットベクタ
  )
  {
    // 下請け関数を呼ぶだけ
    _sort(bv, 0, cube_num);
  }

  /// @brief カバー(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  ///
  /// 比較はキューブごとの辞書式順序で行う．
  int
  compare(
    SizeType nc1,        ///< [in] 1つめのカバーのキューブ数
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType nc2,        ///< [in] 2つめカバーのキューブ数
    const ymuint64* bv2  ///< [in] 2つめのカバーを表すビットベクタ
  );

  /// @brief ビットベクタからハッシュ値を計算する．
  SizeType
  hash(
    SizeType nc,       ///< [in] キューブ数
    const ymuint64* bv ///< [in] ビットベクタ
  );


public:
  //////////////////////////////////////////////////////////////////////
  // キューブに対する処理
  // 基本的にはキューブはビットベクタの先頭アドレスとキューブ位置で表す．
  // キューブのサイズ(変数の個数に依存)によってキューブの開始アドレスは変化する．
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1,       ///< [in] 1つめのキューブ番号
    const ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2        ///< [in] 2つめのキューブ番号
  );

  /// @brief 2つのキューブの積が空でない時 true を返す．
  bool
  cube_check_product(
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1,	 ///< [in] 1つめのキューブ番号
    const ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2	 ///< [in] 2つめのキューブ番号
  );

  /// @brief 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
  bool
  cube_check_containment(
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1,	 ///< [in] 1つめのキューブ番号
    const ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2	 ///< [in] 2つめのキューブ番号
  );

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  cube_check_intersect(
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1,	 ///< [in] 1つめのキューブ番号
    const ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2	 ///< [in] 2つめのキューブ番号
  );

  /// @brief キューブ(を表すビットベクタ)のコピーを行う．
  void
  cube_copy(
    ymuint64* dst_bv,       ///< [in] コピー先のビットベクタ
    SizeType dst_pos,       ///< [in] コピー先のキューブ番号
    const ymuint64* src_bv, ///< [in] ソースのビットベクタ
    SizeType src_pos        ///< [in] ソースのキューブ番号
  )
  {
    // キューブ数が1の場合のコピー
    copy(1, dst_bv, dst_pos, src_bv, src_pos);
  }

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  cube_clear(
    ymuint64* dst_bv, ///< [in] 対象のビットベクタ
    SizeType dst_pos  ///< [in] 対象のキューブ番号
  );

  /// @brief 2つのキューブの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product(
    ymuint64* dst_bv,    ///< [in] 結果のキューブのビットベクタ
    SizeType dst_pos,    ///< [in] 結果のキューブ番号
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1,       ///< [in] 1つめのキューブ番号
    const ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2        ///< [in] 2つめのキューブ番号
  );

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_division(
    ymuint64* dst_bv,    ///< [in] 結果のキューブのビットベクタ
    SizeType dst_pos,	 ///< [in] 結果のキューブ番号
    const ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1,	 ///< [in] 1つめのキューブ番号
    const ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2	 ///< [in] 2つめのキューブ番号
  );

  /// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
  void
  cube_swap(
    ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1, ///< [in] 1つめのキューブ番号
    ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2  ///< [in] 2つめのキューブ番号
  )
  {
    // mTmpBuff を介して移動する．
    _cube_save(bv1, pos1);
    cube_copy(bv1, pos1, bv2, pos2);
    _cube_restore(bv2, pos2);
  }

  /// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv1 となる．
  void
  cube_rotate3(
    ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1, ///< [in] 1つめのキューブ番号
    ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2, ///< [in] 2つめのキューブ番号
    ymuint64* bv3, ///< [in] 3つめのカバーを表すビットベクタ
    SizeType pos3  ///< [in] 3つめのキューブ番号
  )
  {
    // mTmpBuff を介して移動する．
    _cube_save(bv1, pos1);
    cube_copy(bv1, pos1, bv2, pos2);
    cube_copy(bv2, pos2, bv3, pos3);
    _cube_restore(bv3, pos3);
  }

  /// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv4, bv4 <- bv1 となる．
  void
  cube_rotate4(
    ymuint64* bv1, ///< [in] 1つめのカバーを表すビットベクタ
    SizeType pos1, ///< [in] 1つめのキューブ番号
    ymuint64* bv2, ///< [in] 2つめのカバーを表すビットベクタ
    SizeType pos2, ///< [in] 2つめのキューブ番号
    ymuint64* bv3, ///< [in] 3つめのカバーを表すビットベクタ
    SizeType pos3, ///< [in] 3つめのキューブ番号
    ymuint64* bv4, ///< [in] 4つめのカバーを表すビットベクタ
    SizeType pos4  ///< [in] 4つめのキューブ番号
  )
  {
    // mTmpBuff を介して移動する．
    _cube_save(bv1, pos1);
    cube_copy(bv1, pos1, bv2, pos2);
    cube_copy(bv2, pos2, bv3, pos3);
    cube_copy(bv3, pos3, bv4, pos4);
    _cube_restore(bv4, pos4);
  }

  /// @brief カバー/キューブの内容を出力する．
  ///
  /// end は実際の末尾 + 1 を指す．
  void
  print(
    ostream& s,         ///< [in] 出力先のストリーム
    const ymuint64* bv, ///< [in] カバー/キューブを表すビットベクタ
    SizeType start,     ///< [in] キューブの開始位置
    SizeType end        ///< [in] キューブの終了位置
  );


public:
  //////////////////////////////////////////////////////////////////////
  // リテラル集合に対する演算
  // 基本キューブと同じだが相反するリテラルがあってもOK
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のチェック
  /// @return lit を含んでいたら true を返す．
  bool
  is_in(
    ymuint64* bv,  ///< [in] ビットベクタ
    AlgLiteral lit ///< [in] 対象のリテラル
  );

  /// @brief 要素の追加
  void
  add_lit(
    ymuint64* bv,  ///< [in] ビットベクタ
    AlgLiteral lit ///< [in] 対象のリテラル
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マージソートを行う下請け関数
  ///
  /// bv[start] - bv[end - 1] の領域をソートする．
  void
  _sort(
    ymuint64* bv,   ///< [in] 対象のビットベクタ
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief mTmpBuff を初期化する．
  void
  _init_buff();

  /// @brief mTmpBuff に必要な領域を確保する．
  void
  _resize_buff(
    SizeType req_size ///< [in] 要求するキューブ数
  );

  /// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
  void
  _cube_save(
    const ymuint64* src_bv, ///< [in] ソースのビットベクタ
    SizeType src_pos        ///< [in] ソースのキューブ番号
  )
  {
    cube_copy(mTmpBuff, 0, src_bv, src_pos);
  }

  /// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
  void
  _cube_restore(
    ymuint64* dst_bv, ///< [in] コピー先のビットベクタ
    SizeType dst_pos  ///< [in] コピー先のキューブ番号
  )
  {
    cube_copy(dst_bv, dst_pos, mTmpBuff, 0);
  }

  /// @brief ブロック位置を計算する．
  static
  SizeType
  _block_pos(
    SizeType var_id ///< [in] 変数番号
  )
  {
    return var_id / 32;
  }

  /// @brief シフト量を計算する．
  static
  SizeType
  _shift_num(
    SizeType var_id ///< [in] 変数番号
  )
  {
    // ソートしたときの見栄えの問題で左(MSB)から始める．
    return (31 - (var_id % 32)) * 2;
  }

  /// @brief キューブ1つ分のブロックサイズを計算する．
  SizeType
  _cube_size() const
  {
    return (variable_num() + 31) / 32;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  SizeType mVarNum;

  // 変数名のリスト
  vector<string> mVarNameList;

  // 変数名と変数番号のハッシュ表(mVarNameList の逆写像)
  unordered_map<string, SizeType> mVarNameMap;

  // 作業用に用いられるビットベクタ
  ymuint64* mTmpBuff;

  // mTmpBuff のキューブ数
  SizeType mTmpBuffSize;

};

END_NAMESPACE_YM_BFO

#endif // YM_ALGMGR_H
