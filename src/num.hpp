/*******************************************************************************
 * Copyright (c) 2015 Wojciech Migda
 * All rights reserved
 * Distributed under the terms of the GNU LGPL v3
 *******************************************************************************
 *
 * Filename: num.hpp
 *
 * Description:
 *      description
 *
 * Authors:
 *          Wojciech Migda (wm)
 *
 *******************************************************************************
 * History:
 * --------
 * Date         Who  Ticket     Description
 * ----------   ---  ---------  ------------------------------------------------
 * 2015-02-08   wm              Initial version
 *
 ******************************************************************************/

#ifndef NUM_HPP_
#define NUM_HPP_

#include <valarray>
#include <cmath>

namespace num
{

typedef std::size_t size_type;

template<typename _ValueType>
_ValueType mean(const std::valarray<_ValueType> & vector)
{
    typedef _ValueType value_type;

    const value_type result = vector.size() != 0 ? vector.sum() / vector.size() : value_type{};

    return result;
}

template<typename _ValueType>
_ValueType mean(std::valarray<_ValueType> && vector)
{
    return mean(vector);
}

template<typename _ValueType>
_ValueType std(const std::valarray<_ValueType> & vector, const size_type ddof = 1)
{
    typedef _ValueType value_type;

    const value_type mu = mean(vector);

    const value_type result = vector.size() != 0 ?
        std::sqrt(((vector - mu) * (vector - mu)).sum() / (vector.size() - ddof)) :
        value_type{};

    return result;
}

}  // namespace num

#endif /* NUM_HPP_ */
