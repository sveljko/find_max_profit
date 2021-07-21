#include "find_max_profit.hpp"

#include <algorithm>
#include <cassert>


double find_max_profit(std::vector<double> const& prices, int fee)
{
    assert(!prices.empty());
    for (auto const& p : prices) {
        assert(p > 0);
    }

    if (prices.size() == 1) {
        return 0;
    }
    const auto          n = prices.size();
    std::vector<double> cache(n, -1.0);
    for (int i = n - 2; i >= 0; --i) {
        double max_profit = 0.0;
        for (int j = i + 1; j < n; ++j) {
            const double transaction_profit = -prices[i] + prices[j] - 2 * fee;
            double       max_future_profit  = 0.0;
            if (n - j > 3) {
                assert(cache[j + 2] >= 0);
                max_future_profit = cache[j + 2];
            }
            if (transaction_profit + max_future_profit > max_profit) {
                max_profit = transaction_profit + max_future_profit;
            }
        }
        cache[i] = std::max(max_profit, cache[i + 1]);
    }

    assert(cache[0] >= 0);
    return cache[0];
}