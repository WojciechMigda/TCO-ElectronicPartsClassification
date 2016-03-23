/*******************************************************************************
 * Copyright (c) 2016 Wojciech Migda
 * All rights reserved
 * Distributed under the terms of the MIT License
 *******************************************************************************
 *
 * Filename: param_store.hpp
 *
 * Description:
 *      Database of parameter sets of the XGBoost estimators
 *
 * Authors:
 *          Wojciech Migda (wm)
 *
 *******************************************************************************
 * History:
 * --------
 * Date         Who  Ticket     Description
 * ----------   ---  ---------  ------------------------------------------------
 * 2016-03-22   wm              Initial version
 *
 ******************************************************************************/


#ifndef PARAM_STORE_HPP_
#define PARAM_STORE_HPP_

#include <map>
#include <string>

namespace params
{


const std::map<const std::string, const std::string> CURRENT
{
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
//    {"objective", "rank:pairwise"},
    {"objective", "multi:softprob"},
    {"num_class", "3"},

    {"n_estimators", "20"},
    {"reg_alpha", "0"},
    {"colsample_bytree", "0.6142072845246039"},
    {"colsample_bylevel", "1"},
    {"scale_pos_weight", "0.6362125677898036"},
    {"learning_rate", "0.045"},
    {"max_delta_step", "0"},
    {"base_score", "0.5"},
    {"subsample", "0.9960842849583936"},
    {"reg_lambda", "1"},
    {"min_child_weight", "2"},

    {"max_depth", "10"},
    //{"gamma", "1.676750571566339"}
};


}  // namespace params


#endif /* PARAM_STORE_HPP_ */
