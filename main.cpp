#include "board.h"
#include "move.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

int count;

unordered_map<unsigned long long, vector<vector<Move>>> hashtable;

void perft(Board board, int depth) {
    if (depth == 0) {
        count++;
    } else {
        vector<vector<Move>> moves;
        if (hashtable.find(board.zobristKey) == hashtable.end()) {
            moves = board.getMoves();
            hashtable.emplace(board.zobristKey, moves);
        } else {
            moves = hashtable[board.zobristKey];
        }
        for (int i = 0; i < moves.size(); ++i) {
            board.makeMove(moves[i], true);
            perft(board, depth - 1);
            board.makeMove(moves[i], false);
        }
    }
}

int main(int argc, char **argv)
{
    clock_t start, end;
    Board test(argv[1]);
    test.display();
    count = 0;
    start = clock();
    cout << "fen : " << argv[1] << endl;
    cout << "depth : " << atoi(argv[2]) << endl;
    perft(test, atoi(argv[2]));
    end = clock();
    cout << "count : " << count << endl;
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "duration : " << time_taken << endl;
    cout << "c/s : " << count / time_taken << endl;
    cout << "hashtable : " << hashtable.size() << endl;
    test.display();
    return 0;
}