#include <bits/stdc++.h>
#include "game.h"

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Heap-allocate Game to avoid large stack frame
    auto gp = std::make_unique<Game>(cin);
    Game& g = *gp;

    const array<char,5> OPS = {'A','B','C','D','E'};
    vector<char> ans;

    // Upper bound to avoid excessive runtime on worst cases
    long long limit = g.m; // OJ will cap by m anyway
    // But protect against pathological loops
    long long safety_cap = min<long long>(limit, 50000);

    while (g.bricksRemaining() > 0 && (long long)ans.size() < safety_cap) {
        long long best_score = LLONG_MIN;
        char best_op = 'C';

        // Try all operations with 1-step lookahead using save/load
        for (char op : OPS) {
            Game::Save* sv = g.save();
            int hits_before = sv->hits;
            int reward = g.play(op);
            int hits_after = g.bricksHit();
            int bricks = hits_after - hits_before; // bricks hit in this step

            long long score = (long long)bricks * 100 + reward; // prefer clearing bricks, then combos

            if (score > best_score) {
                best_score = score;
                best_op = op;
            }
            g.load(sv);
            g.erase(sv);
        }

        ans.push_back(best_op);
        g.play(best_op); // apply best op
    }

    // Output chosen operations, one per line
    for (char c : ans) {
        cout << c << '\n';
    }
    return 0;
}

