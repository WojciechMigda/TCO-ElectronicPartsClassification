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


array_type
get_dummies(std::valarray<real_type> && what)
{
    auto unique = std::unordered_set<real_type>();

    std::copy_if(std::begin(what), std::end(what), std::inserter(unique, unique.end()),
        [](real_type v)
        {
            return !is_missing(v);
        }
    );

//    std::cout << what.size() << " " << unique.size() << std::endl;

    array_type newmat({what.size(), unique.size()}, 0.0);

    int index{0};

    for (const auto v : unique)
    {
        const std::valarray<bool> bool_mask = (what == v);
        std::valarray<real_type> mask(bool_mask.size());

        std::copy(std::begin(bool_mask), std::end(bool_mask), std::begin(mask));

        newmat[newmat.column(index)] = mask;

        ++index;
    }

    return newmat;
}

array_type
one_hot(const array_type & what, std::vector<std::string> & colnames, std::vector<std::size_t> && columns)
{
    array_type newmat(what);

    for (const auto col : columns)
    {
        const auto dummies = get_dummies(what[what.column(col)]);

        newmat = num::add_columns<real_type>(newmat, dummies);

        const std::string colname = colnames[col];
        for (std::size_t dummy{0}; dummy < dummies.shape().second; ++dummy)
        {
            colnames.emplace_back(colname + "_" + std::to_string(dummy + 1));
        }

        {
            const std::size_t ndummies = dummies.shape().second;

            const std::valarray<real_type> first_dummy = dummies[dummies.column(0)];
            const std::valarray<real_type> nth_df = newmat[newmat.column(-ndummies)];
            assert(std::equal(std::begin(first_dummy), std::end(first_dummy), std::begin(nth_df)));

            const std::valarray<real_type> last_dummy = dummies[dummies.column(-1)];
            const std::valarray<real_type> last_df = newmat[newmat.column(-1)];
            assert(std::equal(std::begin(last_dummy), std::end(last_dummy), std::begin(last_df)));
        }
    }

    // we will only delete columns right-to-left
    std::sort(columns.begin(), columns.end(), std::greater<std::size_t>());
    for (const auto col : columns)
    {
        newmat = num::del_column<real_type>(newmat, col);

        colnames.erase(colnames.begin() + col);
    }

    return newmat;
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
            const int prod_id = df[df.row(index)][0];
            const char segment = df[df.row(index)][8];

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

    const varray_type special_part = gen_attribute(gb_train, i_train_data, "SPECIAL_PART");
    colnames.push_back("SPECIAL_PART");
    train_data = num::add_column(train_data, special_part);

    return std::make_tuple(colnames, train_data, test_data);
}


std::vector<std::string>
ElectronicPartsClassification::classifyParts(
    std::vector<std::string> & i_training,
    std::vector<std::string> & i_testing) const
{

    std::vector<std::string> colnames{"PRODUCT_NUMBER", "CUSTOMER_NUMBER", "TRANSACTION_DATE",
        "PRODUCT_PRICE", "GROSS_SALES", "REGION", "WAREHOUSE", "CUSTOMER_ZIP", "CUSTOMER_SEGMENT1",
        "CUSTOMER_SEGMENT2", "CUSTOMER_TYPE1", "CUSTOMER_TYPE2", "CUSTOMER_MANAGED_LEVEL",
        "CUSTOMER_ACCOUNT_TYPE", "CUSTOMER_FIRST_ORDER_DATE", "PRODUCT_CLASS_ID1",
        "PRODUCT_CLASS_ID2", "PRODUCT_CLASS_ID3", "PRODUCT_CLASS_ID4","BRAND",
        "PRODUCT_ATTRIBUTE_X", "PRODUCT_SALES_UNIT", "SHIPPING_WEIGHT", "TOTAL_BOXES_SOLD",
        "PRODUCT_COST1", "PRODUCT_UNIT_OF_MEASURE", "ORDER_SOURCE", "PRICE_METHOD", "SPECIAL_PART"};


    const auto time0 = timestamp();

    const num::loadtxtCfg<real_type>::converters_type converters_train =
        {
            {colidx(colnames, "TRANSACTION_DATE"), date_xlt},
            {colidx(colnames, "CUSTOMER_SEGMENT1"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
            {colidx(colnames, "CUSTOMER_TYPE2"), [](const char * str){return from_list_xlt({"A", "B", "C"}, str);}},
            {colidx(colnames, "CUSTOMER_MANAGED_LEVEL"), [](const char * str){return from_list_xlt({"N", "L"}, str);}},
            {colidx(colnames, "CUSTOMER_ACCOUNT_TYPE"), [](const char * str){return from_list_xlt({"ST", "DM"}, str);}},
            {colidx(colnames, "CUSTOMER_FIRST_ORDER_DATE"), date_xlt},
            {colidx(colnames, "BRAND"), [](const char * str){return from_list_xlt({"IN_HOUSE", "NOT_IN_HOUSE"}, str);}},
            {colidx(colnames, "PRODUCT_SALES_UNIT"), [](const char * str){return from_list_xlt({"Y", "N"}, str);}},
            {colidx(colnames, "PRODUCT_UNIT_OF_MEASURE"), [](const char * str){return from_list_xlt({"B", "LB", "EA"}, str);}},
            {colidx(colnames, "ORDER_SOURCE"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
            {colidx(colnames, "SPECIAL_PART"), [](const char * str){return from_list_xlt({"No", "Maybe", "Yes"}, str);}},
        };
    const num::loadtxtCfg<real_type>::converters_type converters_test =
        {
            {colidx(colnames, "TRANSACTION_DATE"), date_xlt},
            {colidx(colnames, "CUSTOMER_SEGMENT1"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
            {colidx(colnames, "CUSTOMER_TYPE2"), [](const char * str){return from_list_xlt({"A", "B", "C"}, str);}},
            {colidx(colnames, "CUSTOMER_MANAGED_LEVEL"), [](const char * str){return from_list_xlt({"N", "L"}, str);}},
            {colidx(colnames, "CUSTOMER_ACCOUNT_TYPE"), [](const char * str){return from_list_xlt({"ST", "DM"}, str);}},
            {colidx(colnames, "CUSTOMER_FIRST_ORDER_DATE"), date_xlt},
            {colidx(colnames, "BRAND"), [](const char * str){return from_list_xlt({"IN_HOUSE", "NOT_IN_HOUSE"}, str);}},
            {colidx(colnames, "PRODUCT_SALES_UNIT"), [](const char * str){return from_list_xlt({"Y", "N"}, str);}},
            {colidx(colnames, "PRODUCT_UNIT_OF_MEASURE"), [](const char * str){return from_list_xlt({"B", "LB", "EA"}, str);}},
            {colidx(colnames, "ORDER_SOURCE"), [](const char * str){return from_list_xlt({"A", "B"}, str);}},
        };

    ////////////////////////////////////////////////////////////////////////////

    array_type i_train_data =
        num::loadtxt(
            std::move(i_training),
            std::move(
                num::loadtxtCfg<real_type>()
                .delimiter(',')
                .converters(num::loadtxtCfg<real_type>::converters_type{converters_train})
            )
        );

    const
    array_type i_test_data =
        num::loadtxt(
            std::move(i_testing),
            std::move(
                num::loadtxtCfg<real_type>()
                .delimiter(',')
                .converters(num::loadtxtCfg<real_type>::converters_type{converters_test})
            )
        );

    gen_features(colnames, i_train_data, i_test_data);

    return {};

    // retrieve response vector
    const array_type::varray_type train_y_valarr = i_train_data[i_train_data.column(-1)];
    const std::vector<float> train_y(std::begin(train_y_valarr), std::end(train_y_valarr));

    std::cerr << "train_y size: " << train_y.size() << std::endl;


    // drop the CONSUMER_ID column
    array_type test_data({i_test_data.shape().first, i_test_data.shape().second - 1}, i_test_data[i_test_data.columns(1, -1)]);
    // drop the CONSUMER_ID and DEMO_X columns
    array_type train_data({i_train_data.shape().first, i_train_data.shape().second - 2}, i_train_data[i_train_data.columns(1, -2)]);

    colnames.erase(colnames.end() - 1);
    colnames.erase(colnames.begin());
    assert(colnames.size() == train_data.shape().second);

    std::cerr << "train_data shape: " << train_data.shape() << std::endl;
    std::cerr << "test_data shape: " << test_data.shape() << std::endl;

    {
        array_type full_data({train_data.shape().first + test_data.shape().first, train_data.shape().second}, 0);
        full_data[full_data.rows(0, train_data.shape().first - 1)] = train_data[train_data.rows(0, -1)];
        full_data[full_data.rows(train_data.shape().first, -1)] = test_data[test_data.rows(0, -1)];

        const auto & c_full_data(full_data);

        full_data = one_hot(full_data, colnames,
            {
                colidx(colnames, "GENDER"),
                colidx(colnames, "REGISTRATION_ROUTE"),
                colidx(colnames, "REGISTRATION_CONTEXT"),
                colidx(colnames, "MIGRATED_USER_TYPE"),
                colidx(colnames, "PLATFORM_CENTRE"),
                colidx(colnames, "TOD_CENTRE"),
                colidx(colnames, "CONTENT_CENTRE"),
            });

        assert(colnames.size() == full_data.shape().second);

        train_data = array_type(
            {train_data.shape().first, full_data.shape().second},
            c_full_data[full_data.rows(0, train_data.shape().first - 1)]);
        test_data = array_type(
            {test_data.shape().first, full_data.shape().second},
            c_full_data[full_data.rows(train_data.shape().first, - 1)]);
    }

    std::cerr << "OneHot/FE train_data shape: " << train_data.shape() << std::endl;
    std::cerr << "OneHot/FE test_data shape: " << test_data.shape() << std::endl;


    constexpr int   TIME_MARGIN{60};
    const int       MAX_TIMESTAMP = time0 + 600 - TIME_MARGIN;

    const std::map<const std::string, const std::string> * PARAMS_SET[] = {&params::CURRENT};


    std::cerr << std::endl << "Training " << std::distance(std::begin(PARAMS_SET), std::end(PARAMS_SET)) << " estimator(s)" << std::endl;
    std::cerr << "Total time limit: " << 600 << " secs" << std::endl;


    // collection of probabilities predicted by each estimator
    std::vector<std::vector<float>> y_hat_proba_set;

    for (const auto & PARAMS_p : PARAMS_SET)
    {
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

        if (y_hat_proba_set.size() == 0)
        {
            // double first estimator's score
//            y_hat_proba_set.push_back(proba);
        }
        y_hat_proba_set.push_back(proba);

        std::cerr << "Elapsed time: " << timestamp() - time0 << std::endl;
    }

    // array of propabilities accumulated from completed estimators
    std::vector<float> y_hat_proba_cumm(test_data.shape().first, 0.);

    for (int idx{0}; idx < y_hat_proba_set.size(); ++idx)
    {
        std::transform(y_hat_proba_set[idx].cbegin(), y_hat_proba_set[idx].cend(), y_hat_proba_cumm.begin(),
            y_hat_proba_cumm.begin(),
            [](const float x, const float a)
            {
//                return a + (x > 0.5);
                return a + x;
            });
    }

    // quantized prediction
    std::vector<int> y_hat(test_data.shape().first);

    std::transform(y_hat_proba_cumm.cbegin(), y_hat_proba_cumm.cend(), y_hat.begin(),
        [&y_hat_proba_set](const float what)
        {
            const float mean = what / y_hat_proba_set.size();

            return mean > 0.50;
        }
    );

//    return y_hat;
    return {};
}

#endif /* ELECTRONICPARTS_HPP_ */
