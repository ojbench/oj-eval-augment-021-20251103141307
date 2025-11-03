#include <bits/stdc++.h>
#include "game.h"

using namespace std;

static inline int op_to_delta(char op) {
    switch (op) {
        case 'A': return -2;
        case 'B': return -1;
        case 'C': return 0;
        case 'D': return 1;
        case 'E': return 2;
    }
    return 0;
}
static inline char delta_to_op(int d) {
    if (d <= -2) return 'A';
    if (d == -1) return 'B';
    if (d == 0) return 'C';
    if (d == 1) return 'D';
    return 'E';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Heap-allocate Game to avoid large stack frame
    auto gp = std::make_unique<Game>(cin);
    Game& g = *gp;

    vector<char> ans;

    // Upper bound to avoid excessive runtime on worst cases
    long long limit = g.m; // OJ will cap by m anyway
    // But protect against pathological loops
    long long safety_cap = min<long long>(limit, 10000); // tighter cap to avoid TLE on worst cases

    int prev_delta = 0; // last chosen delta in [-2..2]
    int streak = 0;     // reuse previous choice this many times without re-evaluating

    while (g.bricksRemaining() > 0 && (long long)ans.size() < safety_cap) {
        char chosen;
        if (streak > 0) {
            chosen = delta_to_op(prev_delta);
            streak--;
        } else {
            // Build small candidate set around previous decision
            int candidates_raw[3] = { prev_delta, max(prev_delta-1, -2), min(prev_delta+1, 2) };
            // Unique and map to ops
            array<char,3> cand_ops;
            int csz = 0;
            for (int i = 0; i < 3; ++i) {
                char opi = delta_to_op(candidates_raw[i]);
                bool dup = false;
                for (int j = 0; j < csz; ++j) if (cand_ops[j] == opi) { dup = true; break; }
                if (!dup) cand_ops[csz++] = opi;
            }
            long long best_score = LLONG_MIN; char best_op = 'C'; int best_bricks = 0; int best_delta = 0;
            for (int i = 0; i < csz; ++i) {
                char op = cand_ops[i];
                Game::Save* sv = g.save();
                int hits_before = sv->hits;
                int reward = g.play(op);
                int hits_after = g.bricksHit();
                int bricks = hits_after - hits_before;
                long long score = (long long)bricks * 100 + reward;
                if (score > best_score) {
                    best_score = score; best_op = op; best_bricks = bricks; best_delta = op_to_delta(op);
                }
                g.load(sv); g.erase(sv);
            }
            chosen = best_op;
            prev_delta = best_delta;
            // If this op clears bricks, reuse it a few times to cut planning cost
            streak = (best_bricks > 0) ? 3 : 0;
        }
        ans.push_back(chosen);
        g.play(chosen); // apply
    }

    // Output chosen operations, one per line
    for (char c : ans) cout << c << '\n';
    return 0;
}

