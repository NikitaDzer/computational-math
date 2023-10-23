#pragma once

#include <Eigen/Dense>

#include <vector>
#include <functional>

namespace snae
{

template <int M>
using Function = std::function<double(const Eigen::Vector<double, M>& vec)>;

template <int M>
using FunctionsVector = std::array<Function<M>, M>;

template <int M>
using FunctionsMatrix = std::array<FunctionsVector<M>, M>;

} // namespace snae
