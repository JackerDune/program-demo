#include <vector>
#include <iostream>

/*
给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。 
示例 1：

输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
示例 2：

输入：nums = [0,1]
输出：[[0,1],[1,0]]
示例 3：

输入：nums = [1]
输出：[[1]]
 

提示：

1 <= nums.length <= 6
-10 <= nums[i] <= 10
nums 中的所有整数 互不相同
*/

using namespace std;

class Solution {
public:
    void dfs(int i,vector<int> &nums,
            vector<int> &t, vector<bool> &vis,
            vector<vector <int>> &ans, int n) {
        if (i == n) {
            ans.emplace_back(t);
            return;
        }
        for (int j = 0; j < n; ++j) {
            if (!vis[j]) {
                vis[j] = true;
                t[i] = nums[j];
                dfs(i + 1, nums, t, vis, ans, n);
                //这里是回溯的关键
                vis[j] = false;
            }
        }
    }
    vector<vector<int>> permute(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> ans;
        vector<int> t(n);
        vector<bool> vis(n);
        dfs(0, nums, t, vis, ans, n);
        return ans;
    }
};

int main(char *argv[], int argc)
{
    //vector<int> nums = {1, 2, 3};
    vector<int> nums = {1, 2, 3, 5, 6, 7, 9};
    vector<vector<int>> ret;
    Solution s;
     
    ret = s.permute(nums);
    for (auto v : ret) {
        char buf[256];
        int j = 0;
        for (auto i: v) {
            j += sprintf(buf + j, "%d ", i);
        }
        cout << buf << endl;
    }
}
