#if !defined(INC_FIND_MAX_PROFIT)
#define INC_FIND_MAX_PROFIT


#include <vector>

/** Given a non-empty vector of stock @p prices (elements represent price on the `i`th day), 
    and a @p fee for a transaction, finds the maximum profit that can be achieved. Fee is 
    the same for buying and selling at any day.

    It observes the following trading rules:
   
    1. After a stock is sold, stock cannot be bought on the next day (i.e., cooldown one day). 
    2. There can be only one transaction simultaneously (i.e., one must sell stock before 
       buying it again).

    If no profit can be achieved, returns 0 (since it essentially chooses not to trade in
    such a market).
    */
double find_max_profit(std::vector<double> const& prices, int fee);


#endif // !defined(INC_FIND_MAX_PROFIT)
