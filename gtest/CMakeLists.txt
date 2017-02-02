
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${GTEST_INCLUDE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================



# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

add_executable ( CubeTest
  CubeTest.cc
  $<TARGET_OBJECTS:ym_common_d>
  $<TARGET_OBJECTS:ym_bfo_d>
  )

target_compile_options ( CubeTest
  PRIVATE "-g"
  )

target_link_libraries ( CubeTest
  ${YM_LIB_DEPENDS}
  pthread
  ${GTEST_BOTH_LIBRARIES}
  )

add_test ( BfoTest
  CubeTest
  )


add_executable ( CoverTest
  CoverTest.cc
  $<TARGET_OBJECTS:ym_common_d>
  $<TARGET_OBJECTS:ym_bfo_d>
  )

target_compile_options ( CoverTest
  PRIVATE "-g"
  )

target_link_libraries ( CoverTest
  ${YM_LIB_DEPENDS}
  pthread
  ${GTEST_BOTH_LIBRARIES}
  )

add_test ( BfoTest
  CoverTest
  )


add_executable ( MgrTest
  MgrTest.cc
  $<TARGET_OBJECTS:ym_common_d>
  $<TARGET_OBJECTS:ym_bfo_d>
  )

target_compile_options ( MgrTest
  PRIVATE "-g"
  )

target_link_libraries ( MgrTest
  ${YM_LIB_DEPENDS}
  pthread
  ${GTEST_BOTH_LIBRARIES}
  )

add_test ( BfoTest
  MgrTest
  )
