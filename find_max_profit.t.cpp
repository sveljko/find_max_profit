#include "find_max_profit.hpp"
#include <cassert>
#include <unordered_map>

#include <iostream>
#include <random>


/* The elegant recursive implementation, for which one could reasonably easy
    convince oneself that it is correct. 
    */
double find_max_profit_recursive(double const* prices, size_t n, int fee)
{
    assert(fee >= 0);
    if (0 == n) {
        return 0;
    }
    double max_profit = 0.0;
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            const double transaction_profit = -prices[i] + prices[j] - 2 * fee;
            const double max_future_profit =
                (n - j > 3) ? find_max_profit_recursive(prices + j + 2, n - j - 2, fee) : 0;
            if (transaction_profit + max_future_profit > max_profit) {
                max_profit = transaction_profit + max_future_profit;
            }
        }
    }
    return max_profit;
}

/* Slightly less readable, but, still not hard to reason about, yet _much_
    faster because of caching.
    */
double find_max_profit_recursive_cached(double const*                              prices,
                                        size_t                                     n,
                                        int                                        fee,
                                        std::unordered_map<double const*, double>& cache)
{
    assert(fee >= 0);
    if (0 == n) {
        return 0;
    }
    double max_profit = 0.0;
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            const double transaction_profit = -prices[i] + prices[j] - 2 * fee;
            double       max_future_profit  = 0.0;
            if (n - j > 3) {
                double const* const future = prices + j + 2;
                auto                f      = cache.find(future);
                if (f != cache.end()) {
                    max_future_profit = f->second;
                }
                else {
                    max_future_profit = find_max_profit_recursive_cached(future, n - j - 2, fee, cache);
                }
            }
            if (transaction_profit + max_future_profit > max_profit) {
                max_profit = transaction_profit + max_future_profit;
            }
        }
    }
    cache[prices] = max_profit;
    return max_profit;
}


double find_max_profit_recursive(std::vector<double> const& prices, int fee)
{
    assert(!prices.empty());
    for (auto const& p : prices) {
        assert(p > 0);
    }
    return find_max_profit_recursive(prices.data(), prices.size(), fee);
}


double find_max_profit_recursive_cached(std::vector<double> const& prices, int fee)
{
    assert(!prices.empty());
    for (auto const& p : prices) {
        assert(p > 0);
    }
    std::unordered_map<double const*, double> cache;
    return find_max_profit_recursive_cached(prices.data(), prices.size(), fee, cache);
}


static unsigned failed = false;


#define expect(what, expected)                                                                     \
    do {                                                                                           \
        auto got  = what;                                                                          \
        auto exp_ = expected;                                                                      \
        if (got != exp_) {                                                                         \
            std::cout << "\tfor '" << #what << "' expected '" << #expected << "' which is "        \
                      << exp_ << ", but got " << got << "\n";                                      \
            ++failed;                                                                              \
        }                                                                                          \
    } while (0)


static void manual_test(double (*max_profit)(std::vector<double> const& prices, int fee))
{
    /* Use numbers that have exact presentation in floating point to avoid
        rounding errors in comparisons. */

    expect(max_profit({ 3 }, 0), 0);
    expect(max_profit({ 3 }, 3), 0);
    expect(max_profit({ 55.5 }, 0), 0);

    expect(max_profit({ 2, 4 }, 1), 0);
    expect(max_profit({ 2, 4 }, 0), 2);
    expect(max_profit({ 2, 5 }, 1), 1);
    expect(max_profit({ 3, 9 }, 2), 2);
    expect(max_profit({ 3, 8.5 }, 2), 1.5);
    expect(max_profit({ 55, 6 }, 2), 0);
    expect(max_profit({ 44.2, 31.9 }, 2), 0);

    expect(max_profit({ 2, 4, 5 }, 1), 1);
    expect(max_profit({ 2, 5, 4 }, 1), 1);
    expect(max_profit({ 4, 2, 5 }, 1), 1);
    expect(max_profit({ 4, 5, 2 }, 1), 0);
    expect(max_profit({ 5, 2, 4 }, 1), 0);
    expect(max_profit({ 5, 4, 2 }, 1), 0);
    expect(max_profit({ 1, 9.5, 16.25 }, 2), 11.25);
    expect(max_profit({ 1, 16.25, 9.5 }, 2), 11.25);
    expect(max_profit({ 9.5, 1, 16.25 }, 2), 11.25);
    expect(max_profit({ 9.5, 16.25, 1 }, 2), 2.75);
    expect(max_profit({ 16.25, 1, 9.5 }, 2), 4.5);
    expect(max_profit({ 16.25, 9.5, 1 }, 2), 0);

    /* At this point, I'm convinced that `fee` is taken into account, so
        will use the same from now on, to ease manual checking of results.
        */
    expect(max_profit({ 2, 4, 5, 6 }, 1), 2);
    expect(max_profit({ 2, 4, 6, 5 }, 1), 2);
    expect(max_profit({ 2, 5, 4, 6 }, 1), 2);
    expect(max_profit({ 2, 5, 6, 4 }, 1), 2);
    expect(max_profit({ 2, 6, 4, 5 }, 1), 2);
    expect(max_profit({ 2, 6, 5, 4 }, 1), 2);
    expect(max_profit({ 2, 7, 3, 6 }, 1), 3); // try to trick it!
    expect(max_profit({ 4, 2, 5, 6 }, 1), 2);
    expect(max_profit({ 4, 2, 6, 5 }, 1), 2);
    expect(max_profit({ 4, 5, 2, 6 }, 1), 2);
    expect(max_profit({ 4, 5, 6, 2 }, 1), 0);
    expect(max_profit({ 4, 6, 2, 5 }, 1), 1);
    expect(max_profit({ 4, 6, 5, 2 }, 1), 0);
    expect(max_profit({ 5, 2, 4, 6 }, 1), 2);
    expect(max_profit({ 5, 2, 6, 4 }, 1), 2);
    expect(max_profit({ 5, 4, 2, 6 }, 1), 2);
    expect(max_profit({ 5, 4, 6, 2 }, 1), 0);
    expect(max_profit({ 5, 6, 2, 4 }, 1), 0);
    expect(max_profit({ 5, 6, 4, 2 }, 1), 0);
    expect(max_profit({ 6, 2, 4, 5 }, 1), 1);
    expect(max_profit({ 6, 2, 5, 4 }, 1), 1);
    expect(max_profit({ 6, 4, 5, 2 }, 1), 0);
    expect(max_profit({ 6, 4, 2, 5 }, 1), 1);
    expect(max_profit({ 6, 5, 4, 2 }, 1), 0);
    expect(max_profit({ 6, 5, 2, 4 }, 1), 0);

    /* Longer arrays have too many permutations for an exhaustive
       manual check */
    expect(max_profit({ 1, 9, 11.5, 16, 21.25 }, 1), 18.25);
    expect(max_profit({ 1, 9, 11.5, 21.25, 16 }, 1), 18.25);
    expect(max_profit({ 1, 9, 16, 11.5, 21.25 }, 1), 18.25);
    expect(max_profit({ 1, 9, 16, 21.25, 11.5 }, 1), 18.25);
    expect(max_profit({ 1, 9, 21.25, 16, 11.5 }, 1), 18.25);
    expect(max_profit({ 1, 9, 21.25, 11.5, 16 }, 1), 18.25);
    expect(max_profit({ 1, 11.5, 9, 16, 21.25 }, 1), 18.25);
    expect(max_profit({ 1, 11.5, 9, 21.25, 16 }, 1), 18.25);
    expect(max_profit({ 1, 11.5, 16, 9, 21.25 }, 1), 18.75);
    expect(max_profit({ 1, 11.5, 16, 21.25, 9 }, 1), 18.25);
    expect(max_profit({ 1, 11.5, 21.25, 9, 16 }, 1), 18.25);
    expect(max_profit({ 1, 11.5, 21.25, 16, 9 }, 1), 18.25);
    expect(max_profit({ 1, 16, 9, 11.5, 21.25 }, 1), 20.75);
    expect(max_profit({ 1, 16, 9, 21.25, 11.5 }, 1), 18.25);
    expect(max_profit({ 1, 16, 11.5, 9, 21.25 }, 1), 23.25);
    expect(max_profit({ 1, 16, 11.5, 21.25, 9 }, 1), 18.25);
    expect(max_profit({ 1, 16, 21.25, 9, 11.5 }, 1), 18.25);
    expect(max_profit({ 1, 16, 21.25, 11.5, 9 }, 1), 18.25);
    expect(max_profit({ 1, 21.25, 9, 11.5, 16 }, 1), 20.75);
    expect(max_profit({ 1, 21.25, 9, 16, 11.5 }, 1), 18.25);
    expect(max_profit({ 1, 21.25, 11.5, 9, 16 }, 1), 23.25);
    expect(max_profit({ 1, 21.25, 11.5, 16, 9 }, 1), 18.25);
    expect(max_profit({ 1, 21.25, 16, 9, 11.5 }, 1), 18.75);
    expect(max_profit({ 1, 21.25, 16, 11.5, 9 }, 1), 18.25);

    /* Try some interesting cases for 8 members, which can have
       3 transactions.
       */
    expect(max_profit({ 1, 9, 11.5, 16, 21.25, 24, 30, 33.5 }, 1), 30.5);
    expect(max_profit({ 1, 9, 11.5, 16, 21.25, 24, 33.5, 30 }, 1), 30.5);
    expect(max_profit({ 1, 9, 11.5, 30, 33.5, 24, 16, 21.25 }, 1), 33.75);
    expect(max_profit({ 1, 9, 11.5, 33.5, 30, 24, 16, 21.25 }, 1), 33.75);
    expect(max_profit({ 1, 9, 33.5, 30, 11.5, 24, 16, 21.25 }, 1), 41);
    expect(max_profit({ 1, 33.5, 9, 30, 11.5, 24, 16, 21.25 }, 1), 41);
    expect(max_profit({ 1, 33.5, 9, 11.5, 30, 24, 16, 21.25 }, 1), 50.25);

    /* If one thinks of any other interesting cases for manual check,
       add here... */
}


static void random_reference_test(size_t price_count)
{
    /* Here we will generate random integers, to avoid dealing with
       rounding errors. The manual test should do checks that involve
       non-integers.
       */
    std::random_device  rd;
    std::mt19937        mt(rd());
    std::vector<double> prices(price_count);

    for (size_t i = 0; i < 20; ++i) {
        for (size_t j = 0; j < price_count; ++j) {
            prices[j] = mt() + 1;
        }
        expect(find_max_profit(prices, i), find_max_profit_recursive_cached(prices, i));
    }
}


static void random_slow_reference_test(size_t price_count)
{
    /* Here we will generate random integers, to avoid dealing with
       rounding errors. The manual test should do checks that involve
       non-integers.
       */
    std::random_device  rd;
    std::mt19937        mt(rd());
    std::vector<double> prices(price_count);

    for (size_t i = 0; i < 20; ++i) {
        for (size_t j = 0; j < price_count; ++j) {
            prices[j] = mt() + 1;
        }
        expect(find_max_profit(prices, i), find_max_profit_recursive(prices, i));
    }
}


int main()
{
    manual_test(find_max_profit_recursive);
    manual_test(find_max_profit_recursive_cached);
    manual_test(find_max_profit);

    // To be on the safe side, check against the unoptimized reference
    random_slow_reference_test(20);
    random_reference_test(20);

    // this should be the largest number for which the execution time makes sense
    // (say, a few seconds).
    random_reference_test(500);

    if (0 == failed) {
        std::cout << "\n\tAll tests pass.\n";
    }
    else {
        std::cout << "\n\tNumber of failed tests: " << failed << " !!\n";
        return -1;
    }

    return 0;
}