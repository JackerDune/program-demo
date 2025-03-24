#include <vector>
#include <iostream>

/*
假如有一排房子，共 n 个，每个房子可以被粉刷成红色、蓝色或者绿色这三种颜色中的一种，你需要粉刷所有的房子并且使其相邻的两个房子颜色不能相同。
当然，因为市场上不同颜色油漆的价格不同，所以房子粉刷成不同颜色的花费成本也是不同的。每个房子粉刷成不同颜色的花费是以一个 n x 3 的正整数矩阵 costs 来表示的。
例如，costs[0][0] 表示第 0 号房子粉刷成红色的成本花费；costs[1][2] 表示第 1 号房子粉刷成绿色的花费，以此类推。
请计算出粉刷完所有房子最少的花费成本。

示例 1：

输入: costs = [[17,2,17],[16,16,5],[14,3,19]]
输出: 10
解释: 将 0 号房子粉刷成蓝色，1 号房子粉刷成绿色，2 号房子粉刷成蓝色。
     最少花费: 2 + 5 + 3 = 10。
示例 2：

输入: costs = [[7,6,2]]
输出: 2
 

提示:

costs.length == n
costs[i].length == 3
1 <= n <= 100
1 <= costs[i][j] <= 20



==== 思路1 === 
用 dp[i][j] 表示粉刷第 0 号房子到第 i 号房子且第 i 号房子被粉刷成第 j 种颜色时的最小花费成本。由于一共有 n 个房子和 3 种颜色，因此 0≤i<n，0≤j<3。
对于 1≤i<n，第 i 号房子和第 i−1 号房子的颜色必须不同，因此当第 i 号房子被粉刷成某一种颜色时，第 i−1 号房子只能被粉刷成另外两种颜色之一。当第 i 号房子分别被粉刷成三种颜色时，粉刷第 0 号房子到第 i 号房子的最小花费成本计算如下：

dp[i][0] = min(dp[i−1][1],dp[i−1][2])+costs[i][0]
dp[i][1] = min(dp[i−1][0],dp[i−1][2])+costs[i][1]
dp[i][2] = min(dp[i−1][0],dp[i−1][1])+costs[i][2]

三种颜色的情况可以合并为一个状态转移方程，对于 1≤i<n 和 0≤j<3，状态转移方程如下：

dp[i][j]=min(dp[i−1][(j+1)mod3],dp[i−1][(j+2)mod3])+costs[i][j]
计算结束时，dp[n−1] 中的最小值即为粉刷所有房子的最小花费成本。
当 i≥1 时，由于 dp[i] 的计算只和 dp[i−1] 有关，因此可以使用滚动数组优化空间，将空间复杂度降低到 O(1)。

*/

using namespace std;

class Solution {
public:
    int minCost(vector<vector<int>>& costs) {
        int r = 0, g = 0, b = 0;
        for (auto& cost : costs) {
            int _r = r, _g = g, _b = b;
            r = min(_g, _b) + cost[0];
            g = min(_r, _b) + cost[1];
            b = min(_r, _g) + cost[2];
        }
        return min(r, min(g, b)); 
	}
    int minCost1(vector<vector<int>>& costs) {
		int n = costs.size();
		vector<vector<int>> dp(n, vector<int>(3, 0));
		for (int i = 0; i < 3; i++) {
			dp[0][i] = costs[0][i];
		}

		for (int i = 1; i < n; i++) {
			for (int j = 0; j < 3; j++) {
				dp[i][j] = min(dp[i-1][(j+1)%3], dp[i-1][(j+2)%3]) + costs[i][j];
			}
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < 3; j++) {
				cout << dp[i][j] << " ";
			}
			cout << endl;
		}
		return min(dp[n-1][2], min(dp[n-1][0], dp[n-1][1]));
	}
};

int main(char *argv[], int argc)
{
    vector<vector<int>> a = {{17, 2, 17}, {16, 16, 5}, {14,3,19}};
    Solution s;
	cout << s.minCost(a) << endl;     
	cout << s.minCost1(a) << endl;     
}
