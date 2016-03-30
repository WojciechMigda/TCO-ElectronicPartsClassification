/*******************************************************************************
 * Copyright (c) 2016 Wojciech Migda
 * All rights reserved
 * Distributed under the terms of the MIT License
 *******************************************************************************
 *
 * Filename: ElectronicParts.hpp
 *
 * Description:
 *      TCO-ElectronicPartsClassification
 *
 * Authors:
 *          Wojciech Migda (wm)
 *
 *******************************************************************************
 * History:
 * --------
 * Date         Who  Ticket     Description
 * ----------   ---  ---------  ------------------------------------------------
 * 2016-03-19   wm              Initial version
 *
 ******************************************************************************/


#ifndef ELECTRONICPARTS_HPP_
#define ELECTRONICPARTS_HPP_

#include "num.hpp"
#include "array2d.hpp"

#include "param_store.hpp"

#include "xgboost/c_api.h"

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <cmath>
#include <cstring>
#include <iterator>
#include <unordered_map>
#include <chrono>
#include <array>
#include <tuple>

#include <ctime>

typedef float real_type;

typedef num::array2d<real_type> array_type;

/*
 * Wrapper which returns DMatrixHandle directly and not through out param
 */
DMatrixHandle XGDMatrixCreateFromMat(
    const float *data,
    bst_ulong nrow,
    bst_ulong ncol,
    float missing)
{
    DMatrixHandle dmat{nullptr};

    XGDMatrixCreateFromMat(data, nrow, ncol, missing, &dmat);

    return dmat;
}

/*
 * Wrapper which returns BoosterHandle directly and not through out param
 */
BoosterHandle XGBoosterCreate(const DMatrixHandle dmats[], bst_ulong len)
{
    BoosterHandle booster{nullptr};

    XGBoosterCreate(dmats, len, &booster);

    return booster;
}

/*
 * Competition mandated class
 */
struct ElectronicPartsClassification
{
    std::vector<std::string>
    classifyParts(
        std::vector<std::string> & i_training,
        std::vector<std::string> & i_testing) const;
};


constexpr float MISSING{NAN};
constexpr float XGB_MISSING{NAN};

bool is_missing(float v)
{
    if (std::isnan(MISSING))
    {
        return std::isnan(v);
    }
    else
    {
        return v == MISSING;
    }
}


auto date_xlt = [](const char * str) -> real_type
{
    std::tm parsed = {0};

    strptime(str, "%Y-%m-%d %T", &parsed);

    const auto tp = std::chrono::system_clock::from_time_t(std::mktime(&parsed));
    const auto days_since_1900 = std::chrono::duration_cast<std::chrono::hours>(tp.time_since_epoch()).count() / 24;

    return days_since_1900;
};

/*
 * generic pattern converter for loadtxt
 */
auto from_list_xlt = [](const std::vector<std::string> & patterns, const char * str) -> real_type
{
    auto matched_it = std::find_if(patterns.cbegin(), patterns.cend(),
        [&str](const std::string & what)
        {
            return strcmp(what.c_str(), str) == 0;
        }
    );

    if (matched_it != patterns.cend())
    {
        return std::distance(patterns.cbegin(), matched_it);
    }
    else if (strcmp(str, "NA") == 0)
    {
        return MISSING;
    }
    else
    {
        assert(false);
        return NAN;
    }
};

namespace XGB
{

template<typename _StopCondition>
std::unique_ptr<void, int (*)(BoosterHandle)>
fit(const array_type & train_data,
    const std::vector<float> & train_y,
    const std::map<const std::string, const std::string> & params,
    _StopCondition stop_condition)
{
    // prepare placeholder for raw matrix later used by xgboost
    std::vector<float> train_vec = train_data.tovector();
    std::cerr << "train_vec size: " << train_vec.size() << std::endl;
//    assert(std::none_of(train_vec.cbegin(), train_vec.cend(), [](float x){return std::isnan(x);}));

    std::unique_ptr<void, int (*)(DMatrixHandle)> tr_dmat(
        XGDMatrixCreateFromMat(
            train_vec.data(),
            train_data.shape().first,
            train_data.shape().second, XGB_MISSING),
        XGDMatrixFree);

    // attach response vector to tr_dmat
    XGDMatrixSetFloatInfo(tr_dmat.get(), "label", train_y.data(), train_y.size());

    const DMatrixHandle cache[] = {tr_dmat.get()};

    // create Booster with attached tr_dmat
    std::unique_ptr<void, int (*)(BoosterHandle)> booster(
            XGBoosterCreate(cache, 1UL),
            XGBoosterFree);

    for (const auto & kv : params)
    {
        std::cerr << kv.first << " => " << kv.second << std::endl;
        XGBoosterSetParam(booster.get(), kv.first.c_str(), kv.second.c_str());
    }


    for (int iter{0}; stop_condition() == false; ++iter)
    {
        XGBoosterUpdateOneIter(booster.get(), iter, tr_dmat.get());
    }

    return booster;
}


std::vector<float>
predict(
    BoosterHandle booster,
    const array_type & test_data)
{
    std::vector<float> test_vec = test_data.tovector();
    std::cerr << "test_vec size: " << test_vec.size() << std::endl;

    std::unique_ptr<void, int (*)(DMatrixHandle)> te_dmat(
        XGDMatrixCreateFromMat(
            test_vec.data(),
            test_data.shape().first,
            test_data.shape().second, XGB_MISSING),
        XGDMatrixFree);

    bst_ulong y_hat_len{0};
    const float * y_hat_proba{nullptr};
    XGBoosterPredict(booster, te_dmat.get(), 0, 0, &y_hat_len, &y_hat_proba);
    std::cerr << "Got y_hat_proba of length " << y_hat_len << std::endl;

    std::vector<float> y_hat(y_hat_proba, y_hat_proba + y_hat_len);

    return y_hat;
}

}


std::size_t
colidx(const std::vector<std::string> & colnames, const std::string & name)
{
    std::vector<std::string>::const_iterator found = std::find(colnames.cbegin(), colnames.cend(), name);
    if (found == colnames.cend())
    {
        std::cerr << "Bad column name: " << name << std::endl;
    }
    assert(found != colnames.cend());
    return found - colnames.cbegin();
}



#include <sys/time.h>
long int timestamp()
{
//    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec;// + tv.tv_usec / 1e6;
}


struct GroupBy
{
    typedef std::pair<int, char> key_type;

    GroupBy(const array_type & df)
    {
        for (std::size_t ix{0}; ix < df.shape().first; ++ix)
        {
            const key_type k = key(df, ix);

            if (m_groups.count(k))
            {
                m_groups.at(k).push_back(ix);
            }
            else
            {
                m_groups[k] = {ix};
            }
        }

        m_current = m_groups.begin();
    }

    key_type key(const array_type & df, const std::size_t index) const
    {
        if (df.shape().second == 0)
        {
            return {0, 0};
        }
        else
        {
            const int prod_id = df[df.row(index)][0]; // TODO, hardcoded
            const char segment = df[df.row(index)][8]; // TODO, hardcoded

            return {prod_id, segment};
        }
    }

    std::vector<std::size_t> yield()
    {
        if (m_current == m_groups.cend())
        {
            return {};
        }
        else
        {
            const auto & result = m_current->second;
            ++m_current;
            return result;
        }
    }

    std::size_t size() const
    {
        return m_groups.size();
    }

    void rewind()
    {
        m_current = m_groups.begin();
    }

    std::map<key_type, std::vector<std::size_t>> m_groups;
    std::map<key_type, std::vector<std::size_t>>::const_iterator m_current;
};


std::tuple<std::vector<std::string>, array_type, array_type>
gen_features(
    const std::vector<std::string> & i_colnames,
    const array_type & i_train_data,
    const array_type & i_test_data)
{
    typedef std::valarray<real_type> varray_type;

    /*
     * generate feature vector across all groups for a single attribute column
     */
    auto gen_attribute = [&i_colnames](GroupBy & gb, const array_type & arr, const std::string & colname) -> varray_type
    {
        const std::size_t cix = colidx(i_colnames, colname);
        varray_type vec(NAN, gb.size());

        std::size_t vix{0};
        for (auto group = gb.yield(); group.size() != 0; group = gb.yield())
        {
            const varray_type row = arr[arr.row(group.front())];
            vec[vix++] = row[cix];
        }

        gb.rewind();

        return vec;
    };

    auto gen_attributes = [&i_colnames, &gen_attribute](GroupBy & gb, const array_type & arr, const std::vector<std::string> & att_names) -> array_type
    {
        const varray_type att1 = gen_attribute(gb, arr, att_names.front());
        array_type result({att1.size(), 1}, att1);

        for (auto it = std::next(att_names.cbegin()); it != att_names.cend(); ++it)
        {
            const varray_type att = gen_attribute(gb, arr, *it);
            result = num::add_column(result, att);
        }
        return result;
    };


    const std::vector<std::string> att_names{"PRODUCT_CLASS_ID1", "BRAND", "PRODUCT_SALES_UNIT", "PRODUCT_UNIT_OF_MEASURE"};

    ////////////////////////////////////////////////////////////////////////////

    GroupBy gb_train(i_train_data);

    array_type train_data = gen_attributes(gb_train, i_train_data, att_names);

    std::vector<std::string> colnames;
    std::copy(att_names.cbegin(), att_names.cend(), std::back_inserter(colnames));

    ////////////////////////////////////////////////////////////////////////////

    GroupBy gb_test(i_test_data);

    array_type test_data = gen_attributes(gb_test, i_test_data, att_names);

    ////////////////////////////////////////////////////////////////////////////

    auto gen_distribution = [&i_colnames](GroupBy & gb, const array_type & arr, const std::string & colname, const int dict_sz, const int offset) -> array_type
    {
        const std::size_t cix = colidx(i_colnames, colname);

        array_type result({gb.size(), dict_sz}, 0.);

        std::size_t gbix{0};
        for (auto group = gb.yield(); group.size() != 0; group = gb.yield())
        {
            varray_type dict(0., dict_sz);

            for (const auto rix : group)
            {
                const int item = arr[arr.row(rix)][cix] - offset;
                assert(0 <= item && item < dict_sz);

                ++dict[item];
            }

            dict /= dict.sum();
            result[result.row(gbix++)] = dict;
        }

        gb.rewind();

        return result;
    };

    const std::tuple<std::string, int, int> dist_db[] =
        {
            std::make_tuple("PRICE_METHOD", 5, 1),
            std::make_tuple("ORDER_SOURCE", 2, 0),
            std::make_tuple("CUSTOMER_ACCOUNT_TYPE", 2, 0),
            std::make_tuple("CUSTOMER_MANAGED_LEVEL", 2, 0),
            std::make_tuple("CUSTOMER_TYPE2", 3, 0),
            std::make_tuple("CUSTOMER_TYPE1", 3, 1),
        };

    for (const auto & descriptor : dist_db)
    {
        const auto dist = gen_distribution(gb_train, i_train_data,
            std::get<0>(descriptor),
            std::get<1>(descriptor),
            std::get<2>(descriptor));
        train_data = num::add_columns(train_data, dist);
    }

    for (const auto & descriptor : dist_db)
    {
        const auto dist = gen_distribution(gb_test, i_test_data,
            std::get<0>(descriptor),
            std::get<1>(descriptor),
            std::get<2>(descriptor));
        test_data = num::add_columns(test_data, dist);
    }

    for (const auto & descriptor : dist_db)
    {
        for (int ix{0}; ix < std::get<1>(descriptor); ++ix)
        {
            colnames.push_back(std::get<0>(descriptor) + '_' + std::to_string(ix + 1));
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    auto min_max_std = [&i_colnames](GroupBy & gb, const array_type & arr) -> array_type
    {
        const std::size_t pcost1_cix = colidx(i_colnames, "PRODUCT_COST1");
        const std::size_t boxes_cix = colidx(i_colnames, "TOTAL_BOXES_SOLD");
        const std::size_t price_cix = colidx(i_colnames, "PRODUCT_PRICE");
        const std::size_t gross_cix = colidx(i_colnames, "GROSS_SALES");
        const std::size_t unit_cix = colidx(i_colnames, "PRODUCT_UNIT_OF_MEASURE");

        constexpr std::size_t NFEAT = 3 + 3 + 4;

        array_type result({gb.size(), NFEAT}, 0.);

        std::size_t gbix{0};
        for (auto group = gb.yield(); group.size() != 0; group = gb.yield())
        {
            varray_type row(0., NFEAT);
            std::size_t rix{0};

            const std::valarray<std::size_t> indirect(group.data(), group.size());

            const varray_type boxes_sold = arr[arr.column(boxes_cix)][indirect];
            const varray_type signed_pcost = arr[arr.column(pcost1_cix)][indirect];
            const varray_type pcost1 = std::abs(signed_pcost);
            const varray_type pcost1_per_item = pcost1 / boxes_sold;
            const real_type pcost1_mean = num::mean(pcost1_per_item);
            const real_type pcost1_std = num::std(pcost1_per_item);

            row[rix++] = pcost1_per_item.min() / pcost1_mean;
            row[rix++] = pcost1_per_item.max() / pcost1_mean;
            row[rix++] = pcost1_std / pcost1_mean;

            const varray_type signed_price = arr[arr.column(price_cix)][indirect];
            const varray_type price = std::abs(signed_price);
            const real_type price_mean = num::mean(price);
            const real_type price_std = num::std(price);

            row[rix++] = price.min() / price_mean;
            row[rix++] = price.max() / price_mean;
            row[rix++] = price_std / price_mean;

            const varray_type signed_gross_sales = arr[arr.column(gross_cix)][indirect];
            const varray_type commision = arr[arr.row(group.front())][unit_cix] < 2 ?
                (varray_type)(price / pcost1_per_item) :
                (varray_type)(std::abs(signed_gross_sales) / pcost1);
            const real_type commision_mean = num::mean(commision);
            const real_type commision_std = num::std(commision);

            row[rix++] = commision_mean;
            row[rix++] = commision.min() / commision_mean;
            row[rix++] = commision.max() / commision_mean;
            row[rix++] = commision_std / commision_mean;

            result[result.row(gbix)] = row;
        }

        gb.rewind();

        return result;
    };

    {
        const auto stat = min_max_std(gb_train, i_train_data);
        train_data = num::add_columns(train_data, stat);
    }
    {
        const auto stat = min_max_std(gb_test, i_test_data);
        test_data = num::add_columns(test_data, stat);
    }

    const std::string stat_colnames[] = {
        "PCOST1_REL_MIN", "PCOST1_REL_MAX", "PCOST1_REL_STD",
        "PRICE_REL_MIN", "PRICE_REL_MAX", "PRICE_REL_STD",
        "COMMN_MEAN", "COMMN_REL_MIN", "COMMN_REL_MAX", "COMMN_REL_STD"};
    colnames.insert(colnames.end(), std::begin(stat_colnames), std::end(stat_colnames));

    ////////////////////////////////////////////////////////////////////////////


    const varray_type special_part = gen_attribute(gb_train, i_train_data, "SPECIAL_PART");
    colnames.push_back("SPECIAL_PART");
    train_data = num::add_column(train_data, special_part);

    return std::make_tuple(colnames, train_data, test_data);
}


template<typename _Iterator>
std::size_t argmax(_Iterator begin, _Iterator end)
{
    const std::size_t imax = std::distance(begin, std::max_element(begin, end));

    return imax;
}


template<typename Iterator>
std::vector<std::size_t>
run_binary_estimators(
    const Iterator begin,
    const Iterator end,
    const long int time0,
    const array_type & train_data,
    const std::vector<float> & train_y,
    const array_type & test_data)
{
    constexpr int   TIME_MARGIN{60};
    constexpr int   MAX_TIME{600};
    const int       MAX_TIMESTAMP = time0 + MAX_TIME - TIME_MARGIN;

    std::cerr << std::endl << "Training " << std::distance(begin, end) << " estimator(s)" << std::endl;
    std::cerr << "Total time limit: " << MAX_TIME << " secs" << std::endl;

    // collection of probabilities predicted by each estimator
    std::vector<std::vector<float>> y_hat_proba_set;

    for (auto it = begin; it != end; ++it)
    {
        const auto & PARAMS_p = *it;

        const int MAX_ITER = std::stoi(PARAMS_p->at("n_estimators"));
        int iter{0};

        auto booster = XGB::fit(train_data, train_y, *PARAMS_p,
            [&iter, MAX_ITER, MAX_TIMESTAMP]() -> bool
            {
                const bool running = (iter < MAX_ITER) && (timestamp() < MAX_TIMESTAMP);
                ++iter;
                return running == false;
            }
        );

        if (iter <= MAX_ITER)
        {
            // time exceeded
            std::cerr << "Exceeded allocated time limit after iteration " << iter << " of " << MAX_ITER << " for estimator [" << y_hat_proba_set.size() + 1 << "]" << std::endl;

            // but we'll make the prediction anyway if it's our first estimator :)
            if (y_hat_proba_set.size() == 0)
            {
                y_hat_proba_set.push_back(XGB::predict(booster.get(), test_data));
            }
            break;
        }

        auto proba = XGB::predict(booster.get(), test_data);

        y_hat_proba_set.push_back(proba);

        std::cerr << "Elapsed time: " << timestamp() - time0 << std::endl;
    }

    // array of propabilities accumulated from completed estimators
    std::vector<float> y_hat_proba_cumm(y_hat_proba_set.front().size(), 0.);

    for (std::size_t idx{0}; idx < y_hat_proba_set.size(); ++idx)
    {
        std::transform(y_hat_proba_set[idx].cbegin(), y_hat_proba_set[idx].cend(), y_hat_proba_cumm.begin(),
            y_hat_proba_cumm.begin(),
            [](const float x, const float a)
            {
                return a + x;
            });
    }

    // quantized prediction
    std::vector<std::size_t> y_hat(test_data.shape().first);

    for (std::size_t ix{0}; ix < y_hat.size(); ++ix)
    {
        y_hat[ix] = y_hat_proba_cumm[ix] > 0.5;
    }

    return y_hat;
}


std::vector<std::string>
ElectronicPartsClassification::classifyParts(
    std::vector<std::string> & i_training,
    std::vector<std::string> & i_testing) const
{

    const std::vector<std::string> raw_colnames{"PRODUCT_NUMBER", "CUSTOMER_NUMBER", "TRANSACTION_DATE",
        "PRODUCT_PRICE", "GROSS_SALES", "REGION", "WAREHOUSE", "CUSTOMER_ZIP", "CUSTOMER_SEGMENT1",
        "CUSTOMER_SEGMENT2", "CUSTOMER_TYPE1", "CUSTOMER_TYPE2", "CUSTOMER_MANAGED_LEVEL",
        "CUSTOMER_ACCOUNT_TYPE", "CUSTOMER_FIRST_ORDER_DATE", "PRODUCT_CLASS_ID1",
        "PRODUCT_CLASS_ID2", "PRODUCT_CLASS_ID3", "PRODUCT_CLASS_ID4","BRAND",
        "PRODUCT_ATTRIBUTE_X", "PRODUCT_SALES_UNIT", "SHIPPING_WEIGHT", "TOTAL_BOXES_SOLD",
        "PRODUCT_COST1", "PRODUCT_UNIT_OF_MEASURE", "ORDER_SOURCE", "PRICE_METHOD", "SPECIAL_PART"};


    const auto time0 = timestamp();

    const num::loadtxtCfg<real_type>::converters_type converters_train =
        {
            {colidx(raw_colnames, "TRANSACTION_DATE"), date_xlt},
            {colidx(raw_colnames, "CUSTOMER_SEGMENT1"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_TYPE2"), [](const char * str){return from_list_xlt({"A", "B", "C"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_MANAGED_LEVEL"), [](const char * str){return from_list_xlt({"N", "L"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_ACCOUNT_TYPE"), [](const char * str){return from_list_xlt({"ST", "DM"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_FIRST_ORDER_DATE"), date_xlt},
            {colidx(raw_colnames, "BRAND"), [](const char * str){return from_list_xlt({"IN_HOUSE", "NOT_IN_HOUSE"}, str);}},
            {colidx(raw_colnames, "PRODUCT_SALES_UNIT"), [](const char * str){return from_list_xlt({"Y", "N"}, str);}},
            {colidx(raw_colnames, "PRODUCT_UNIT_OF_MEASURE"), [](const char * str){return from_list_xlt({"B", "LB", "EA"}, str);}},
            {colidx(raw_colnames, "ORDER_SOURCE"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
            {colidx(raw_colnames, "SPECIAL_PART"), [](const char * str){return from_list_xlt({"No", "Maybe", "Yes"}, str);}},
        };
    const num::loadtxtCfg<real_type>::converters_type converters_test =
        {
            {colidx(raw_colnames, "TRANSACTION_DATE"), date_xlt},
            {colidx(raw_colnames, "CUSTOMER_SEGMENT1"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_TYPE2"), [](const char * str){return from_list_xlt({"A", "B", "C"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_MANAGED_LEVEL"), [](const char * str){return from_list_xlt({"N", "L"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_ACCOUNT_TYPE"), [](const char * str){return from_list_xlt({"ST", "DM"}, str);}},
            {colidx(raw_colnames, "CUSTOMER_FIRST_ORDER_DATE"), date_xlt},
            {colidx(raw_colnames, "BRAND"), [](const char * str){return from_list_xlt({"IN_HOUSE", "NOT_IN_HOUSE"}, str);}},
            {colidx(raw_colnames, "PRODUCT_SALES_UNIT"), [](const char * str){return from_list_xlt({"Y", "N"}, str);}},
            {colidx(raw_colnames, "PRODUCT_UNIT_OF_MEASURE"), [](const char * str){return from_list_xlt({"B", "LB", "EA"}, str);}},
            {colidx(raw_colnames, "ORDER_SOURCE"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
        };

    ////////////////////////////////////////////////////////////////////////////

    const array_type i_train_data =
        num::loadtxt(
            std::move(i_training),
            std::move(
                num::loadtxtCfg<real_type>()
                .delimiter(',')
                .converters(num::loadtxtCfg<real_type>::converters_type{converters_train})
            )
        );

    const array_type i_test_data =
        num::loadtxt(
            std::move(i_testing),
            std::move(
                num::loadtxtCfg<real_type>()
                .delimiter(',')
                .converters(num::loadtxtCfg<real_type>::converters_type{converters_test})
            )
        );

    std::vector<std::string> colnames;
    array_type train_data;
    array_type test_data;

    std::tie(colnames, train_data, test_data) = gen_features(raw_colnames, i_train_data, i_test_data);

//    std::cerr << train_data.shape() << test_data.shape() << std::endl;
//    std::copy(colnames.cbegin(), colnames.cend(), std::ostream_iterator<std::string>(std::cerr, "\n"));

    assert(train_data.shape().second == test_data.shape().second + 1);

    const array_type::varray_type train_y_valarr = train_data[train_data.column(colidx(colnames, "SPECIAL_PART"))];
    const std::vector<float> train_y(std::begin(train_y_valarr), std::end(train_y_valarr));

    std::cerr << "train_y size: " << train_y.size() << std::endl;

    train_data = num::del_column(train_data, colidx(colnames, "SPECIAL_PART"));
    colnames.erase(std::find(colnames.begin(), colnames.end(), "SPECIAL_PART"));
    assert(colnames.size() == train_data.shape().second);

    std::cerr << "train_data shape: " << train_data.shape() << std::endl;
    std::cerr << "test_data shape: " << test_data.shape() << std::endl;


    const std::map<const std::string, const std::string> * PARAMS_SET__no[] = {&params::no::prov47};
    std::vector<float> train_y__no;
    std::transform(train_y.cbegin(), train_y.cend(), std::back_inserter(train_y__no),
        [](const float what)
        {
            // quantize train y vector into {0,1}
            return what >= 0.5 ? 1. : 0.;
        }
    );

    const auto y_hat_no = run_binary_estimators(
        std::begin(PARAMS_SET__no), std::end(PARAMS_SET__no),
        time0, train_data, train_y__no, test_data);


    ////////////////////////////////////////////////////////////////////////////

    std::vector<std::size_t> y_hat(y_hat_no);

    const std::string yes_no_maybe[] = {"No", "Maybe", "Yes"};
    std::map<int, std::pair<std::string, std::string>> responses;
    GroupBy gb_test(i_test_data);
    int ix{0};
    for (auto group = gb_test.yield(); group.size() != 0; group = gb_test.yield())
    {
        const std::valarray<real_type> row = i_test_data[i_test_data.row(group.front())];
        const int prod_id = row[colidx(raw_colnames, "PRODUCT_NUMBER")];
        const char segment = row[colidx(raw_colnames, "CUSTOMER_SEGMENT1")];
        const auto response = y_hat[ix++];

        if (responses.count(prod_id))
        {
            if (segment == 0)
            {
                responses[prod_id].first = yes_no_maybe[response];
            }
            else
            {
                responses[prod_id].second = yes_no_maybe[response];
            }
        }
        else
        {
            if (segment == 0)
            {
                responses[prod_id] = {yes_no_maybe[response], "NA"};
            }
            else
            {
                responses[prod_id] = {"NA", yes_no_maybe[response]};
            }
        }
    }

    std::vector<std::string> str_y_hat;
    std::transform(responses.cbegin(), responses.cend(), std::back_inserter(str_y_hat),
        [](const std::pair<int, std::pair<std::string, std::string>> & kv)
        {
            return std::to_string(kv.first) + ',' + kv.second.first + ',' + kv.second.second;
        }
    );

    return str_y_hat;
}

#endif /* ELECTRONICPARTS_HPP_ */
