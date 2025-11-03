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
    // Bound to avoid TLE on worst hidden tests
    long long safety_cap = min<long long>(limit, 20000);

    char prev_op = 'C';
    int streak = 0; // reuse prev_op this many times without full re-evaluation

    while (g.bricksRemaining() > 0 && (long long)ans.size() < safety_cap) {
        char chosen = prev_op;
        if (streak == 0) {
            long long best_score = LLONG_MIN; char best_op = 'C'; int best_bricks_1 = 0;

            bool deep = (ans.size() % 10 == 0); // more frequent deeper lookahead
            if (!deep) {
                // 1-step lookahead
                for (char op : OPS) {
                    Game::Save* sv = g.save();
                    int hits_before = sv->hits;
                    int reward = g.play(op);
                    int hits_after = g.bricksHit();
                    int bricks = hits_after - hits_before;
                    long long score = (long long)bricks * 100 + reward;
                    if (score > best_score) { best_score = score; best_op = op; best_bricks_1 = bricks; }
                    g.load(sv); g.erase(sv);
                }
            } else {
                // 2-step lookahead (greedy in second step)
                for (char op1 : OPS) {
                    Game::Save* s1 = g.save();
                    int hits_before1 = s1->hits;
                    int reward1 = g.play(op1);
                    int hits_after1 = g.bricksHit();
                    int bricks1 = hits_after1 - hits_before1;

                    long long best2 = LLONG_MIN;
                    for (char op2 : OPS) {
                        Game::Save* s2 = g.save();
                        int hits_before2 = s2->hits;
                        int reward2 = g.play(op2);
                        int hits_after2 = g.bricksHit();
                        int bricks2 = hits_after2 - hits_before2;
                        long long score2 = (long long)(bricks1 + bricks2) * 100 + (reward1 + reward2);
                        if (score2 > best2) best2 = score2;
                        g.load(s2); g.erase(s2);
                    }
                    if (best2 > best_score) { best_score = best2; best_op = op1; best_bricks_1 = bricks1; }
                    g.load(s1); g.erase(s1);
                }
            }
            chosen = best_op;
            prev_op = best_op;
            streak = (best_bricks_1 > 0 ? 3 : 0);
        } else {
            streak--;
        }
        ans.push_back(chosen);
        g.play(chosen); // apply
    }

    // Output chosen operations, one per line
    for (char c : ans) cout << c << '\n';
    return 0;
}

