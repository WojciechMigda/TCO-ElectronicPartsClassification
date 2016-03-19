################################################################################
project( "TCO-ElectronicParts" C CXX )

cmake_minimum_required( VERSION 2.8.3 )

set( CMAKE_ECLIPSE_VERSION 3.8 )
set( _ECLIPSE_VERSION 3.8 )

set( CMAKE_CXX_COMPILER_ARG1 -std=c++11 ) ### for Eclipse's discovery extra arguments


add_definitions( -DDMLC_ENABLE_STD_THREAD=0 )

#add_definitions( -O0 -ggdb -std=c++11 -Wall -pedantic )
#add_definitions( -O2 -ggdb -std=c++11 -Wall -pedantic -mmmx -msse -msse2 -msse3 -s -pipe -Wno-sign-compare -march=native )
add_definitions( -O2 -g -unroll-loops -std=c++11 -Wall -pedantic -mmmx -msse -msse2 -msse3 -s -pipe -Wno-sign-compare -march=native )
#add_definitions( -O2 -ffast-math -fno-finite-math-only  -msse2 -march=native -std=c++11 -Wall -g -pedantic )
#add_definitions( -O2 -ftree-vectorize -ffast-math -fno-finite-math-only  -msse2 -march=native -std=c++11 -Wall -g -pedantic )
#add_definitions( -O3 -ffast-math -msse2 -march=native -std=c++11 -Wall -g -pedantic )

################################################################################

include_directories(
    src
    xgboost/include
    xgboost/dmlc-core/include
    xgboost/rabit/include
)

################################################################################

add_executable( main
    src/main.cpp

    xgboost/src/c_api/c_api.cc
    xgboost/src/data/simple_csr_source.cc
    xgboost/src/data/data.cc
    xgboost/src/logging.cc
    xgboost/rabit/src/c_api.cc
    xgboost/rabit/src/engine.cc
    xgboost/rabit/src/allreduce_base.cc
    xgboost/rabit/src/allreduce_robust.cc
    xgboost/src/c_api/c_api_error.cc
    xgboost/dmlc-core/src/io.cc
    xgboost/dmlc-core/src/io/local_filesys.cc
    xgboost/src/learner.cc
    xgboost/src/objective/objective.cc
    xgboost/dmlc-core/src/io/input_split_base.cc
    xgboost/src/common/common.cc
    xgboost/src/gbm/gbm.cc
    xgboost/dmlc-core/src/io/recordio_split.cc
    xgboost/src/metric/metric.cc
    xgboost/src/data/simple_dmatrix.cc
    xgboost/dmlc-core/src/data.cc
    xgboost/dmlc-core/src/io/line_split.cc
    xgboost/src/objective/regression_obj.cc
    xgboost/src/objective/multiclass_obj.cc
    xgboost/src/metric/elementwise_metric.cc
    xgboost/src/metric/multiclass_metric.cc
    xgboost/src/metric/rank_metric.cc
    xgboost/src/gbm/gbtree.cc
    xgboost/src/gbm/gblinear.cc
    xgboost/src/data/sparse_page_raw_format.cc
    xgboost/src/objective/rank_obj.cc
    xgboost/src/tree/tree_updater.cc
    xgboost/src/tree/updater_histmaker.cc
    xgboost/src/tree/updater_colmaker.cc
    xgboost/src/tree/updater_skmaker.cc
    xgboost/src/tree/updater_prune.cc
    xgboost/src/tree/updater_refresh.cc
    xgboost/src/tree/updater_sync.cc
    xgboost/src/tree/tree_model.cc
)

################################################################################
