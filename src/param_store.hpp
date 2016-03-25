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

// zz_real01_yes_pairsample_1000_redux2.log
//
// best score: 951648.35165  best params: {'colsample_bytree': 0.34509756057561064, 'min_child_weight': 1, 'num_pairsample': 12.0, 'n_estimators': 3, 'subsample': 0.8899402461087893, 'objective': 'rank:pairwise', 'max_depth': 13}
// best score: 951648.35165  best params: {'colsample_bytree': 0.46035362112038597, 'min_child_weight': 1, 'num_pairsample': 11.0, 'n_estimators': 5, 'subsample': 0.8723920700026842, 'objective': 'rank:pairwise', 'max_depth': 10}

// zz_real01_yes_pairsample_1000.log
//
// best score: 952747.25275  best params: {'colsample_bytree': 0.466528864637806, 'min_child_weight': 1, 'num_pairsample': 14.0, 'n_estimators': 4, 'subsample': 0.8722970886136255, 'objective': 'rank:pairwise', 'max_depth': 13}

const std::map<const std::string, const std::string> sub01
{
    // zz_real01_yes_pairsample_1000_redux2.log
    //
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

// zz_real01_no_pairsample.log
//
// best score: 961538.46154  best params: {'colsample_bytree': 0.39023114912939827, 'min_child_weight': 5, 'num_pairsample': 7.0, 'n_estimators': 15, 'subsample': 0.9908460260283289, 'objective': 'rank:pairwise', 'max_depth': 6}
// best score: 961538.46154  best params: {'colsample_bytree': 0.4424634281170999, 'min_child_weight': 6, 'num_pairsample': 8.0, 'n_estimators': 20, 'subsample': 0.9812356001094796, 'objective': 'rank:pairwise', 'max_depth': 3}
// best score: 961538.46154  best params: {'colsample_bytree': 0.47379656789499525, 'min_child_weight': 7, 'num_pairsample': 7.0, 'n_estimators': 20, 'subsample': 0.9790758797613377, 'objective': 'rank:pairwise', 'max_depth': 6}
// best score: 961538.46154  best params: {'colsample_bytree': 0.4738641534121585, 'min_child_weight': 7, 'num_pairsample': 7.0, 'n_estimators': 20, 'subsample': 0.9995658906783058, 'objective': 'rank:pairwise', 'max_depth': 5}

// zz_real01_no_pairsample_1000.log
//
// best score: 963736.26374  best params: {'colsample_bytree': 0.40257140602899394, 'min_child_weight': 6, 'num_pairsample': 12.0, 'n_estimators': 16, 'subsample': 0.983545156246625, 'objective': 'rank:pairwise', 'max_depth': 19}
// best score: 963736.26374  best params: {'colsample_bytree': 0.40883522617482504, 'min_child_weight': 8, 'num_pairsample': 15.0, 'n_estimators': 22, 'subsample': 0.9952266507630004, 'objective': 'rank:pairwise', 'max_depth': 22}
// best score: 963736.26374  best params: {'colsample_bytree': 0.41299627909545644, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 18, 'subsample': 0.9783718656070242, 'objective': 'rank:pairwise', 'max_depth': 21}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4159461398136117, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 22, 'subsample': 0.9693862928174324, 'objective': 'rank:pairwise', 'max_depth': 14}
// best score: 963736.26374  best params: {'colsample_bytree': 0.41612516613533074, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 18, 'subsample': 0.9721128759080947, 'objective': 'rank:pairwise', 'max_depth': 14}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4191749018083908, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 22, 'subsample': 0.9774053312125621, 'objective': 'rank:pairwise', 'max_depth': 15}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4210115672233792, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9998829695594603, 'objective': 'rank:pairwise', 'max_depth': 24}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4224970166487822, 'min_child_weight': 6, 'num_pairsample': 14.0, 'n_estimators': 18, 'subsample': 0.9998142573274683, 'objective': 'rank:pairwise', 'max_depth': 24}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4254906918714264, 'min_child_weight': 5, 'num_pairsample': 15.0, 'n_estimators': 16, 'subsample': 0.9996251139963696, 'objective': 'rank:pairwise', 'max_depth': 21}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4263256879630181, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9931755936519263, 'objective': 'rank:pairwise', 'max_depth': 17}
// best score: 963736.26374  best params: {'colsample_bytree': 0.42674197124246305, 'min_child_weight': 6, 'num_pairsample': 10.0, 'n_estimators': 18, 'subsample': 0.9706340401648974, 'objective': 'rank:pairwise', 'max_depth': 14}
// best score: 963736.26374  best params: {'colsample_bytree': 0.42994873010079765, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 22, 'subsample': 0.9864489484583033, 'objective': 'rank:pairwise', 'max_depth': 17}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4321009234295135, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9885315314986682, 'objective': 'rank:pairwise', 'max_depth': 13}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4481345429579818, 'min_child_weight': 5, 'num_pairsample': 14.0, 'n_estimators': 18, 'subsample': 0.959197441004323, 'objective': 'rank:pairwise', 'max_depth': 8}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4497723491549631, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9451752245208219, 'objective': 'rank:pairwise', 'max_depth': 11}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4522591097747013, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9694113975224392, 'objective': 'rank:pairwise', 'max_depth': 12}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4523124789653089, 'min_child_weight': 5, 'num_pairsample': 15.0, 'n_estimators': 16, 'subsample': 0.9610971837647183, 'objective': 'rank:pairwise', 'max_depth': 10}
// best score: 963736.26374  best params: {'colsample_bytree': 0.45349675073083223, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 18, 'subsample': 0.9579966344627809, 'objective': 'rank:pairwise', 'max_depth': 11}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4545709058980515, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9730172954891433, 'objective': 'rank:pairwise', 'max_depth': 13}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4553807076115144, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.976426000702353, 'objective': 'rank:pairwise', 'max_depth': 17}
// best score: 963736.26374  best params: {'colsample_bytree': 0.45540856705915905, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 22, 'subsample': 0.9773730986266238, 'objective': 'rank:pairwise', 'max_depth': 14}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4561272715439755, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9763028349384633, 'objective': 'rank:pairwise', 'max_depth': 14}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4580721529184133, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 22, 'subsample': 0.9700479935552055, 'objective': 'rank:pairwise', 'max_depth': 12}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4588200175861937, 'min_child_weight': 5, 'num_pairsample': 15.0, 'n_estimators': 18, 'subsample': 0.9620816155285523, 'objective': 'rank:pairwise', 'max_depth': 10}
// best score: 963736.26374  best params: {'colsample_bytree': 0.46138378668305846, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9735268325314345, 'objective': 'rank:pairwise', 'max_depth': 18}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4664068889050296, 'min_child_weight': 5, 'num_pairsample': 15.0, 'n_estimators': 18, 'subsample': 0.963739782240584, 'objective': 'rank:pairwise', 'max_depth': 21}
// best score: 963736.26374  best params: {'colsample_bytree': 0.46664011781726844, 'min_child_weight': 5, 'num_pairsample': 14.0, 'n_estimators': 14, 'subsample': 0.9995176058691917, 'objective': 'rank:pairwise', 'max_depth': 21}
// best score: 963736.26374  best params: {'colsample_bytree': 0.47301262990780135, 'min_child_weight': 6, 'num_pairsample': 15.0, 'n_estimators': 18, 'subsample': 0.94815859798008, 'objective': 'rank:pairwise', 'max_depth': 11}
// best score: 963736.26374  best params: {'colsample_bytree': 0.47357315918715825, 'min_child_weight': 6, 'num_pairsample': 15.0, 'n_estimators': 20, 'subsample': 0.987323819491395, 'objective': 'rank:pairwise', 'max_depth': 20}
// best score: 963736.26374  best params: {'colsample_bytree': 0.47429601710432967, 'min_child_weight': 5, 'num_pairsample': 15.0, 'n_estimators': 16, 'subsample': 0.966192849439185, 'objective': 'rank:pairwise', 'max_depth': 21}
// best score: 963736.26374  best params: {'colsample_bytree': 0.47588785639818587, 'min_child_weight': 5, 'num_pairsample': 14.0, 'n_estimators': 18, 'subsample': 0.9442795377770282, 'objective': 'rank:pairwise', 'max_depth': 19}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4778607080648671, 'min_child_weight': 6, 'num_pairsample': 14.0, 'n_estimators': 18, 'subsample': 0.98716025014739, 'objective': 'rank:pairwise', 'max_depth': 19}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4811255591508305, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9761422055674039, 'objective': 'rank:pairwise', 'max_depth': 16}
// best score: 963736.26374  best params: {'colsample_bytree': 0.48136611811968055, 'min_child_weight': 6, 'num_pairsample': 14.0, 'n_estimators': 18, 'subsample': 0.9681655750288356, 'objective': 'rank:pairwise', 'max_depth': 22}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4868142216554417, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 20, 'subsample': 0.9629562290984364, 'objective': 'rank:pairwise', 'max_depth': 20}
// best score: 963736.26374  best params: {'colsample_bytree': 0.4942777486217037, 'min_child_weight': 7, 'num_pairsample': 16.0, 'n_estimators': 22, 'subsample': 0.9775739770609148, 'objective': 'rank:pairwise', 'max_depth': 17}


const std::map<const std::string, const std::string> prov37
{
    // no: 949375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.40257140602899394"},
    {"min_child_weight", "6"},
    {"num_pairsample", "12"},
    {"n_estimators", "16"},
    {"subsample", "0.983545156246625"},
    {"objective", "rank:pairwise"},
    {"max_depth", "19"},
};

const std::map<const std::string, const std::string> prov36
{
    // no: 953125.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.40883522617482504"},
    {"min_child_weight", "8"},
    {"num_pairsample", "15"},
    {"n_estimators", "22"},
    {"subsample", "0.9952266507630004"},
    {"objective", "rank:pairwise"},
    {"max_depth", "22"},
};

const std::map<const std::string, const std::string> prov35
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.41299627909545644"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "18"},
    {"subsample", "0.9783718656070242"},
    {"objective", "rank:pairwise"},
    {"max_depth", "21"},
};

const std::map<const std::string, const std::string> prov34
{
    // no: 953125.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4159461398136117"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "22"},
    {"subsample", "0.9693862928174324"},
    {"objective", "rank:pairwise"},
    {"max_depth", "14"},
};

const std::map<const std::string, const std::string> prov33
{
    // no: 953125.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.41612516613533074"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "18"},
    {"subsample", "0.9721128759080947"},
    {"objective", "rank:pairwise"},
    {"max_depth", "14"},
};

const std::map<const std::string, const std::string> prov32
{
    // no: 953125.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4191749018083908"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "22"},
    {"subsample", "0.9774053312125621"},
    {"objective", "rank:pairwise"},
    {"max_depth", "15"},
};

const std::map<const std::string, const std::string> prov31
{
    // no: 948812.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4210115672233792"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9998829695594603"},
    {"objective", "rank:pairwise"},
    {"max_depth", "24"},
};

const std::map<const std::string, const std::string> prov30
{
    // no: 964375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4224970166487822"},
    {"min_child_weight", "6"},
    {"num_pairsample", "14"},
    {"n_estimators", "18"},
    {"subsample", "0.9998142573274683"},
    {"objective", "rank:pairwise"},
    {"max_depth", "24"},
};

const std::map<const std::string, const std::string> prov29
{
    // no: 975625.000000
    // LB: 926746.46 (sub# 02)
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4254906918714264"},
    {"min_child_weight", "5"},
    {"num_pairsample", "15"},
    {"n_estimators", "16"},
    {"subsample", "0.9996251139963696"},
    {"objective", "rank:pairwise"},
    {"max_depth", "21"},
};

const std::map<const std::string, const std::string> prov28
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4263256879630181"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9931755936519263"},
    {"objective", "rank:pairwise"},
    {"max_depth", "17"},
};

const std::map<const std::string, const std::string> prov27
{
    // no: 955625.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.42674197124246305"},
    {"min_child_weight", "6"},
    {"num_pairsample", "10"},
    {"n_estimators", "18"},
    {"subsample", "0.9706340401648974"},
    {"objective", "rank:pairwise"},
    {"max_depth", "14"},
};

const std::map<const std::string, const std::string> prov26
{
    // no: 953125.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.42994873010079765"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "22"},
    {"subsample", "0.9864489484583033"},
    {"objective", "rank:pairwise"},
    {"max_depth", "17"},
};

const std::map<const std::string, const std::string> prov25
{
    // no: 953125.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4321009234295135"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9885315314986682"},
    {"objective", "rank:pairwise"},
    {"max_depth", "13"},
};

const std::map<const std::string, const std::string> prov24
{
    // 960062.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4481345429579818"},
    {"min_child_weight", "5"},
    {"num_pairsample", "14"},
    {"n_estimators", "18"},
    {"subsample", "0.959197441004323"},
    {"objective", "rank:pairwise"},
    {"max_depth", "8"},
};

const std::map<const std::string, const std::string> prov23
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4497723491549631"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9451752245208219"},
    {"objective", "rank:pairwise"},
    {"max_depth", "11"},
};

const std::map<const std::string, const std::string> prov22
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4522591097747013"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9694113975224392"},
    {"objective", "rank:pairwise"},
    {"max_depth", "12"},
};

const std::map<const std::string, const std::string> prov21
{
    // no: 970625.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4523124789653089"},
    {"min_child_weight", "5"},
    {"num_pairsample", "15"},
    {"n_estimators", "16"},
    {"subsample", "0.9610971837647183"},
    {"objective", "rank:pairwise"},
    {"max_depth", "10"},
};

const std::map<const std::string, const std::string> prov20
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.45349675073083223"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "18"},
    {"subsample", "0.9579966344627809"},
    {"objective", "rank:pairwise"},
    {"max_depth", "11"},
};

const std::map<const std::string, const std::string> prov19
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4545709058980515"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9730172954891433"},
    {"objective", "rank:pairwise"},
    {"max_depth", "13"},
};

const std::map<const std::string, const std::string> prov18
{
    // no: 962562.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4553807076115144"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.976426000702353"},
    {"objective", "rank:pairwise"},
    {"max_depth", "17"},
};

const std::map<const std::string, const std::string> prov17
{
    // no: 956312.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.45540856705915905"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "22"},
    {"subsample", "0.9773730986266238"},
    {"objective", "rank:pairwise"},
    {"max_depth", "14"},
};

const std::map<const std::string, const std::string> prov16
{
    // no: 962562.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4561272715439755"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9763028349384633"},
    {"objective", "rank:pairwise"},
    {"max_depth", "14"},
};

const std::map<const std::string, const std::string> prov15
{
    // no: 954375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4580721529184133"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "22"},
    {"subsample", "0.9700479935552055"},
    {"objective", "rank:pairwise"},
    {"max_depth", "12"},
};

const std::map<const std::string, const std::string> prov14
{
    // no: 970625.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4588200175861937"},
    {"min_child_weight", "5"},
    {"num_pairsample", "15"},
    {"n_estimators", "18"},
    {"subsample", "0.9620816155285523"},
    {"objective", "rank:pairwise"},
    {"max_depth", "10"},
};

const std::map<const std::string, const std::string> prov13
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.46138378668305846"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9735268325314345"},
    {"objective", "rank:pairwise"},
    {"max_depth", "18"},
};

const std::map<const std::string, const std::string> prov12
{
    // no: 970625.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4664068889050296"},
    {"min_child_weight", "5"},
    {"num_pairsample", "15"},
    {"n_estimators", "18"},
    {"subsample", "0.963739782240584"},
    {"objective", "rank:pairwise"},
    {"max_depth", "21"},
};

const std::map<const std::string, const std::string> prov11
{
    // no: 970625.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.46664011781726844"},
    {"min_child_weight", "5"},
    {"num_pairsample", "14"},
    {"n_estimators", "14"},
    {"subsample", "0.9995176058691917"},
    {"objective", "rank:pairwise"},
    {"max_depth", "21"},
};

const std::map<const std::string, const std::string> prov10
{
    // no: 965625.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.47301262990780135"},
    {"min_child_weight", "6"},
    {"num_pairsample", "15"},
    {"n_estimators", "18"},
    {"subsample", "0.94815859798008"},
    {"objective", "rank:pairwise"},
    {"max_depth", "11"},
};

const std::map<const std::string, const std::string> prov09
{
    // no: 959375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.47357315918715825"},
    {"min_child_weight", "6"},
    {"num_pairsample", "15"},
    {"n_estimators", "20"},
    {"subsample", "0.987323819491395"},
    {"objective", "rank:pairwise"},
    {"max_depth", "20"},
};

const std::map<const std::string, const std::string> prov08
{
    // no: 966312.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.47429601710432967"},
    {"min_child_weight", "5"},
    {"num_pairsample", "15"},
    {"n_estimators", "16"},
    {"subsample", "0.966192849439185"},
    {"objective", "rank:pairwise"},
    {"max_depth", "21"},
};

const std::map<const std::string, const std::string> prov07
{
    // no: 963812.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.47588785639818587"},
    {"min_child_weight", "5"},
    {"num_pairsample", "14"},
    {"n_estimators", "18"},
    {"subsample", "0.9442795377770282"},
    {"objective", "rank:pairwise"},
    {"max_depth", "19"},
};

const std::map<const std::string, const std::string> prov06
{
    // no: 964375.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4778607080648671"},
    {"min_child_weight", "6"},
    {"num_pairsample", "14"},
    {"n_estimators", "18"},
    {"subsample", "0.98716025014739"},
    {"objective", "rank:pairwise"},
    {"max_depth", "19"},
};

const std::map<const std::string, const std::string> prov05
{
    // no: 962562.500000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4811255591508305"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9761422055674039"},
    {"objective", "rank:pairwise"},
    {"max_depth", "16"},
};

const std::map<const std::string, const std::string> prov04
{
    // no: 968125.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.48136611811968055"},
    {"min_child_weight", "6"},
    {"num_pairsample", "14"},
    {"n_estimators", "18"},
    {"subsample", "0.9681655750288356"},
    {"objective", "rank:pairwise"},
    {"max_depth", "22"},
};

const std::map<const std::string, const std::string> prov03
{
    // no: 960625.000000
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4868142216554417"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "20"},
    {"subsample", "0.9629562290984364"},
    {"objective", "rank:pairwise"},
    {"max_depth", "20"},
};

const std::map<const std::string, const std::string> prov02 // =sub01
{
    {"missing", "nan"},
    {"seed", "0"},
    {"silent", "1"},
    {"cache_opt", "0"},

    {"booster", "gbtree"},
    {"objective", "rank:pairwise"},
    {"learning_rate", "0.045"},

    {"colsample_bytree", "0.4942777486217037"},
    {"min_child_weight", "7"},
    {"num_pairsample", "16"},
    {"n_estimators", "22"},
    {"subsample", "0.9775739770609148"},
    {"objective", "rank:pairwise"},
    {"max_depth", "17"},
};

const std::map<const std::string, const std::string> sub01
{
    // no: 960625.000000
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
