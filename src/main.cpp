/*******************************************************************************
 * Copyright (c) 2016 Wojciech Migda
 * All rights reserved
 * Distributed under the terms of the MIT License
 *******************************************************************************
 *
 * Filename: main.cpp
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

#include "ElectronicParts.hpp"

#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <utility>
#include <cstdio>
#include <map>
#include <unordered_set>
#include <iterator>


std::vector<std::string>
read_file(std::string && fname)
{
    std::ifstream fcsv(fname);
    std::vector<std::string> vcsv;

    for (std::string line; std::getline(fcsv, line); /* nop */)
    {
        vcsv.push_back(line);
    }
    fcsv.close();

    return vcsv;
}


const std::vector<std::pair<int, char>> descriptors =
{
    {1249, 'B'},
    {2309, 'A'},
    {2619, 'A'},
    {6068, 'B'},
    {7085, 'A'},
    {16418, 'B'},
    {16542, 'B'},
    {16603, 'B'},
    {16700, 'B'},
    {19169, 'A'},
    {50218, 'A'},
    {50218, 'B'},
    {100586, 'A'},
    {100586, 'B'},
    {101813, 'A'},
    {102122, 'A'},
    {102122, 'B'},
    {105147, 'B'},
    {107075, 'A'},
    {252135, 'B'},
    {304243, 'B'},
    {304270, 'A'},
    {304270, 'B'},
    {306445, 'A'},
    {338806, 'A'},
    {338806, 'B'},
    {441191, 'A'},
    {445285, 'B'},
    {446798, 'B'},
    {504357, 'A'},
    {504357, 'B'},
    {518493, 'B'},
    {533085, 'B'},
    {536879, 'A'},
    {536879, 'B'},
    {543620, 'A'},
    {715519, 'B'},
    {740773, 'A'},
    {740773, 'B'},
    {754095, 'A'},
    {808642, 'B'},
    {853253, 'B'},
    {866646, 'A'},
    {882866, 'A'},
    {916686, 'B'},
    {918815, 'B'},
    {920032, 'A'},
    {921925, 'B'},
    {943291, 'A'},
    {1088032, 'A'},
    {1088032, 'B'},
    {1121993, 'B'},
    {1138850, 'B'},
    {1142552, 'A'},
    {1292352, 'A'},
    {1298815, 'B'},
    {1327889, 'B'},
    {1331013, 'B'},
    {1331262, 'B'},
    {1360121, 'B'},
    {1466902, 'B'},
    {1672734, 'B'},
    {1680256, 'A'},
    {1734274, 'A'},
    {1790395, 'A'},
    {2270296, 'A'},
    {2270296, 'B'},
    {2414368, 'B'},
    {3574685, 'A'},
    {3668805, 'B'},
    {3872125, 'B'},
    {4067012, 'A'},
    {4223131, 'B'},
    {4361972, 'B'},
    {4554022, 'B'},
    {4673406, 'B'},
    {4674654, 'B'},
    {5205877, 'A'},
    {5215069, 'A'},
    {5253050, 'A'},
    {5465699, 'B'},
    {6482923, 'A'},
    {6482923, 'B'},
    {6790929, 'B'},
    {7996764, 'B'},
    {8133205, 'B'},
    {8156853, 'B'},
    {8536822, 'B'},
    {9594241, 'A'},
    {9790681, 'B'},
    {10283064, 'A'}
};


double avnet_score(const std::vector<std::string> & y_hat, const std::vector<std::string> & y_true)
{
    typedef enum Answer
    {
        No,
        Maybe,
        Yes,
        NA
    } answer_type;

    auto answer_from = [](const char * str) -> answer_type
        {
            if (strcmp("No", str) == 0)
            {
                return No;
            }
            else if (strcmp("Maybe", str) == 0)
            {
                return Maybe;
            }
            else if (strcmp("Yes", str) == 0)
            {
                return Yes;
            }
            else if (strcmp("NA", str) == 0)
            {
                return NA;
            }
            else
            {
                assert(false);
            }
        };

    assert(y_hat.size() == y_true.size());

    // negative key for segment A, positive for segment B,
    // value is a pair of truth/prediction values
    std::map<int, std::pair<answer_type, answer_type>> truth_pred;

    for (const auto & s : y_true)
    {
        int prod_id = 0;
        char segA_answer[6];
        char segB_answer[6];

        sscanf(s.c_str(), "%d,%[^,],%[^,]", &prod_id, segA_answer, segB_answer);

        if (strcmp("NA", segA_answer) != 0)
        {
            truth_pred[-prod_id] = {answer_from(segA_answer), NA};
        }
        if (strcmp("NA", segB_answer) != 0)
        {
            truth_pred[+prod_id] = {answer_from(segB_answer), NA};
        }
    }

    for (const auto & s : y_hat)
    {
        int prod_id = 0;
        char segA_answer[6];
        char segB_answer[6];

        sscanf(s.c_str(), "%d,%[^,],%[^,]", &prod_id, segA_answer, segB_answer);

        if (strcmp("NA", segA_answer) != 0)
        {
            truth_pred[-prod_id].second = answer_from(segA_answer);
        }
        if (strcmp("NA", segB_answer) != 0)
        {
            truth_pred[+prod_id].second = answer_from(segB_answer);
        }
    }

    const double score_matrix[3][3] = {{0.00, 0.20, 0.70}, {0.50, 0.00, 0.01}, {1.00, 0.01, 0.00}};
    double score = 0.0;

    for (const auto & kv : truth_pred)
    {
        assert(kv.second.first != NA);
        assert(kv.second.second != NA);

        score += score_matrix[kv.second.second][kv.second.first];
    }

    return score;
}


struct LOOTestIndices
{
    typedef std::pair<int, char> key_type;

    LOOTestIndices(const std::vector<std::string> & df)
    {
        for (std::size_t ix{0}; ix < df.size(); ++ix)
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

    key_type key(const std::vector<std::string> & df, const std::size_t index) const
    {
        if (df.size() == 0)
        {
            return {0, 0};
        }
        else
        {
            char segment = '*';
            int prod_id = -1;

            sscanf(df[index].c_str(), "%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%c", &prod_id, &segment);

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

    std::map<key_type, std::vector<std::size_t>> m_groups;
    std::map<key_type, std::vector<std::size_t>>::const_iterator m_current;
};


std::vector<std::string> test_y_from(const std::vector<std::string> & test_data)
{
    std::map<int, std::pair<std::string, std::string>> responses;

    for (const auto & s : test_data)
    {
        char segment = 0;
        int prod_id = -1;

        sscanf(s.c_str(), "%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%c", &prod_id, &segment);

        const auto last_comma = s.rfind(',');
        const auto last_alpha = s.find_last_not_of("\r\n");
        const std::string yes_no_maybe(s.cbegin() + last_comma + 1, s.cbegin() + last_alpha + 1);

        if (responses.count(prod_id))
        {
            if (segment == 'A')
            {
                responses[prod_id].first = yes_no_maybe;
            }
            else
            {
                responses[prod_id].second = yes_no_maybe;
            }
        }
        else
        {
            if (segment == 'A')
            {
                responses[prod_id] = std::make_pair(yes_no_maybe, "NA");
            }
            else
            {
                responses[prod_id] = std::make_pair("NA", yes_no_maybe);
            }
        }
    }

    std::vector<std::string> result;

    std::transform(responses.cbegin(), responses.cend(), std::back_inserter(result),
        [](const std::pair<int, std::pair<std::string, std::string>> & kv)
        {
            return std::to_string(kv.first) + ',' + kv.second.first + ',' + kv.second.second;
        }
    );

    return result;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    const int SEED = (argc == 2 ? std::atoi(argv[1]) : 1);
    const char * FNAME = (argc == 3 ? argv[2] : "../data/example_data.csv");

    std::cerr << "SEED: " << SEED << ", CSV: " << FNAME << std::endl;


    std::vector<std::string> vcsv = read_file(std::string(FNAME));
    std::cerr << "Read " << vcsv.size() << " lines" << std::endl;

    // for train data skip first row with feature names
    vcsv.erase(vcsv.begin());


    ////////////////////////////////////////////////////////////////////////////

    const ElectronicPartsClassification solver;

    std::vector<double> CVscores;
    CVscores.reserve(descriptors.size());


    LOOTestIndices test_indices_gen(vcsv);

    for (auto test_indices_v = test_indices_gen.yield(); test_indices_v.size() != 0; test_indices_v = test_indices_gen.yield())
    {
//        std::cerr << test_indices_v.size() << ", " << test_indices_v.front() << ", " << test_indices_v.back() << std::endl;
        std::vector<std::string> train_data;
        std::vector<std::string> test_data;

        const std::unordered_set<std::size_t> test_indices(test_indices_v.cbegin(), test_indices_v.cend());
        for (std::size_t ix{0}; ix < vcsv.size(); ++ix)
        {
            if (test_indices.find(ix) != test_indices.cend())
            {
                test_data.push_back(vcsv[ix]);
            }
            else
            {
                train_data.push_back(vcsv[ix]);
            }
        }

        const std::vector<std::string> test_y = test_y_from(test_data);
//        std::copy(test_y.cbegin(), test_y.cend(), std::ostream_iterator<std::string>(std::cerr, "\n"));

        // remove response from test data
        for (auto  & s : test_data)
        {
            const auto pos = s.rfind(',');

            s.resize(pos);
        }

//        const std::vector<std::string> yhat = test_y;
        const auto yhat = solver.classifyParts(train_data, test_data);

        // score
        const double fold_score = avnet_score(yhat, test_y);
        CVscores.push_back(fold_score);

    }


    const double final_score = 1e6 * (1. - (std::accumulate(CVscores.cbegin(), CVscores.cend(), 0) / CVscores.size()));
    std::cerr << "  mean score: " << final_score << std::endl;

    return 0;
}
