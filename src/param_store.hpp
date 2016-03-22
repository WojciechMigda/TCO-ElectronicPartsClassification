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
    {"booster", "gbtree"},
    {"reg_alpha", "0"},
    {"colsample_bytree", "0.5525129874711902"},
    {"silent", "1"},
    {"colsample_bylevel", "1"},
    {"scale_pos_weight", "0.38570090801277784"},
    {"learning_rate", "0.045"},
    {"missing", "nan"},
    {"max_delta_step", "0"},
    {"base_score", "0.5"},
    {"n_estimators", "51"},
    {"cache_opt", "0"},
    {"subsample", "0.7168114425646201"},
    {"reg_lambda", "1"},
    {"seed", "0"},
    {"min_child_weight", "175"},

    {"objective", "rank:pairwise"},
    {"max_depth", "6"},
    {"gamma", "1.676750571566339"}
};


}  // namespace params


#endif /* PARAM_STORE_HPP_ */
