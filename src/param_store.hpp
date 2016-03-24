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

namespace yes
{

const std::map<const std::string, const std::string> CURRENT
{
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},

    {"n_estimators", "5"},
    {"reg_alpha", "0"},
    {"colsample_bytree", "0.46035362112038597"},
    {"colsample_bylevel", "1"},
    {"learning_rate", "0.045"},
    {"max_delta_step", "0"},
    {"base_score", "0.5"},
    {"subsample", "0.8723920700026842"},
    {"reg_lambda", "1"},
    {"min_child_weight", "1"},
    {"num_pairsample", "11"},

    {"max_depth", "10"},
};

}  // namespace yes

namespace no
{

const std::map<const std::string, const std::string> CURRENT
{
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},

    {"n_estimators", "22"},
    {"reg_alpha", "0"},
    {"colsample_bytree", "0.4942777486217037"},
    {"colsample_bylevel", "1"},
    {"learning_rate", "0.045"},
    {"max_delta_step", "0"},
    {"base_score", "0.5"},
    {"subsample", "0.9775739770609148"},
    {"reg_lambda", "1"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},

    {"max_depth", "17"},
};


}  // namespace no


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
    {"colsample_bytree", "0.5519898087273335"},
    {"colsample_bylevel", "1"},
    {"scale_pos_weight", "0.6362125677898036"},
    {"learning_rate", "0.045"},
    {"max_delta_step", "0"},
    {"base_score", "0.5"},
    {"subsample", "0.9739728027243106"},
    {"reg_lambda", "1"},
    {"min_child_weight", "5"},

    {"max_depth", "15"},
    //{"gamma", "1.676750571566339"}
};


const std::map<const std::string, const std::string> softprob
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
