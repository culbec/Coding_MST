#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <map>
#include <algorithm>
#include <queue>

#define INF 1e7
#define NIL (-1)

using namespace std;

class Prufer {
private:
    ifstream fin;
    ofstream fout;

public:
    Prufer(string in, string out) : fin{in}, fout{out} {}

    /* ======== PRUFER CODING ======== */
    /*
     * 1. Identifies the leaf with the lowest value;
     * 2. Extracts it;
     * 3. Puts its parent in the sequence;
     */

    void PruferCoding() {
        vector<int> parent;

        int size;
        this->fin >> size;

        parent.resize(size);

        for (int i = 0; i < size; i++) {
            this->fin >> parent[i];
        }

        vector<bool> exists(size, true);
        vector<int> code;

        for (int i = 0; i < size - 1; i++) {
            int minLeaf = INF;
            for (int j = 0; j < size; j++) {
                if (exists[j] && parent[j] != -1 && j < minLeaf) {
                    minLeaf = j;
                }
            }

            code.push_back(parent[minLeaf]);
            parent[minLeaf] = -1;
            exists[minLeaf] = false;
        }

        this->fout << code.size() << "\n";
        for (int i: code) {
            this->fout << i << " ";
        }
    }

    /* ======== PRUFER CODING ======== */
    /*
     * 1. Loops |V| - 1 times
     * 2. Extracts the first element of the sequence = x;
     * 3. Find the lowest element that doesn't exist in the sequence = y;
     * 4. (x, y) in E -> x parent of y in T
     */

    void PruferDecoding() {
        int size = 0;
        this->fin >> size;

        deque<int> code;
        vector<int> freq(size + 1, 0);
        vector<int> tree(size + 1, -1);

        for (int i = 0; i < size; i++) {
            int val;
            this->fin >> val;
            code.push_back(val);
            freq[val]++;
        }

        for (int i = 0; i < size; i++) {
            int x = code.front();

            int y = INF;
            for (auto j = 0; j < freq.size(); j++) {
                if (freq[j] == 0 && j < y) {
                    y = j;
                }
            }

            freq[y]++;
            freq[x]--;
            code.pop_front();
            code.push_back(y);
            tree[y] = x;
        }

        this->fout << tree.size() << "\n";
        for (int i: tree) {
            this->fout << i << " ";
        }
    }

    ~Prufer() {
        this->fin.close();
        this->fout.close();
    }
};

class Huffmann {
private:
    ifstream fin;
    ofstream fout;

public:
    Huffmann(string in, string out) : fin{in}, fout{out} {}

    /* ======== Huffman CODING ======== */
    /*
     * 1. Computes the frequency of every character in a text;
     * 2. The characters with a higher frequency will be near the root of the tree;
     * 3. The codes for every character will be as follows:
     *      - the first element in the queue -> 0 + code of the character
     *      - the second element in the queue -> 1 + code of the character
     * 4. Will pick the letter that is lower in case the frequencies are equal.
     */

    void HuffmanCoding() {
        struct freqPair {
            int freq = 0;
            char ch = '\0';

            bool operator<(const freqPair &other) const {
                return freq < other.freq || (freq == other.freq && ch < other.ch);
            }
        };

        string text;
        getline(fin, text, '\n');

        if (text.empty()) {
            return;
        }

        int freq[256]{};
        int letters = 0;

        for (auto &ch: text) {
            freq[int(ch)]++;
            if (freq[int(ch)] == 1) {
                ++letters;
            }
        }

        multimap<freqPair, string> Q;

        this->fout << letters << "\n";
        for (int i = 0; i < 256; i++) {
            if (freq[i]) {
                fout << char(i) << " " << freq[i] << "\n";
                Q.insert({{freq[i], char(i)}, string(1, i)});
            }
        }

        string code[256];
        while (--letters) {
            auto x = *Q.begin();
            Q.erase(Q.begin());

            for (auto &chr: x.second) {
                code[int(chr)] = "0" + code[int(chr)];
            }

            auto y = *Q.begin();
            Q.erase(Q.begin());

            for (auto &chr: y.second) {
                code[int(chr)] = "1" + code[int(chr)];
            }

            Q.insert({{x.first.freq + y.first.freq, min(x.first.ch, y.first.ch)}, x.second + y.second});
        }

        for (auto &chr: text) {
            fout << code[int(chr)];
        }
    }

    /* =========== HUFFMAN DECODING ==========*/
    /*
     * Symmetric to Huffman Coding, except we now search for the
     * codes in the encoded sequence to find the letters.
     */

    void HuffmannDecoding() {
        struct freqPair {
            int freq = 0;
            char ch = '\0';

            bool operator<(const freqPair &other) const {
                return freq < other.freq || (freq == other.freq && ch < other.ch);
            }
        };

        string line;
        int freq[256]{}, letters = 0;

        getline(fin, line);
        letters = stoi(line);

        multimap<freqPair, string> Q;

        for (int i = 0; i < letters; i++) {
            getline(fin, line, '\n');
            char ch = line[0];
            freq[int(ch)] = stoi(line.substr(2));
            Q.insert({{freq[int(ch)], ch}, string(1, ch)});
        }


        string encoded;
        getline(fin, encoded);

        string text;
        map<string, char> codes;

        string code[256]{};

        while (--letters) {
            auto x = *Q.begin();
            Q.erase(Q.begin());

            for (auto &chr: x.second) {
                code[int(chr)] = "0" + code[int(chr)];
            }

            auto y = *Q.begin();
            Q.erase(Q.begin());

            for (auto &chr: y.second) {
                code[int(chr)] = "1" + code[int(chr)];
            }

            Q.insert({{x.first.freq + y.first.freq, min(x.first.ch, y.first.ch)}, x.second + y.second});
        }

        for (int i = 0; i < 256; i++) {
            if (code[i] != "\0") {
                codes.insert({code[i], i});
            }
        }

        string currentCode;
        int ind = 0;
        bool found = false;

        while (!encoded.empty()) {
            found = false;
            currentCode += encoded[ind];

            for (auto &code: codes) {
                if (currentCode == code.first) {
                    text.append(string(1, code.second));

                    encoded.erase(encoded.find(currentCode), currentCode.length());
                    currentCode.clear();
                    ind = 0;
                    found = true;
                    break;
                }
            }

            if (found == false) {
                ind++;
            }
        }

        fout << text;
    }
};

class Kruskal {
    class DSU {
    private:
        vector<int> parent;
        vector<int> rank;

    public:
        explicit DSU(int N) {
            this->parent.resize(N);
            this->rank.resize(N);

            for (int i = 0; i < N; i++) {
                this->parent[i] = -1;
                this->rank[i] = 1;
            }
        }

        int findSet(int i) {
            if (parent[i] == -1) {
                return i;
            }

            return parent[i] = findSet(parent[i]);
        }

        void unionSet(int x, int y) {
            int s1 = findSet(x);
            int s2 = findSet(y);

            if (s1 != s2) {
                if (rank[s1] < rank[s2]) {
                    parent[s1] = s2;
                } else if (rank[s2] < rank[s1]) {
                    parent[s2] = s1;
                } else {
                    parent[s2] = s1;
                    rank[s1]++;
                }
            }
        }
    };
    struct Edge{
        int u = 0, v = 0, w = 0;
    };
private:
    ifstream fin; ofstream fout;
    int V, E;
    vector<Edge> edges;

public:
    Kruskal(string in, string out): fin{in}, fout{out} {
        this->fin >> this->V >> this->E;

        for(int i = 0; i < this->E; i++) {
            int u, v, w;
            this->fin >> u >> v >> w;
            this->edges.push_back({u, v, w});
        }
    }

    void KruskalMST() {
        // sortare muchii in functie de ponderi
        sort(this->edges.begin(), this->edges.end(), [](const Edge& e1, const Edge& e2){
            return e1.w < e2.w;
        });

        vector<Edge> tree;
        int minCost = 0;

        DSU S(this->V);

        for(auto& edge: edges) {
            int x = edge.u;
            int y = edge.v;
            int w = edge.w;

            if(S.findSet(x) != S.findSet(y)) {
                S.unionSet(x, y);
                minCost += w;
                tree.push_back(edge);
            }
        }

        this->fout << minCost << "\n";
        this->fout << tree.size() << "\n";

        // sortam iar muchiile din arbore, de data asta dupa varfuri
        sort(tree.begin(), tree.end(), [](const Edge& e1, const Edge& e2){
            if(e1.u == e2.u) {
                return e1.v < e2.v;
            }
            return e1.u < e2.u;
        });

        for(auto& edge : tree) {
            this->fout << edge.u << " " << edge.v << "\n";
        }
    }

};

class Prim {
    struct Edge{
        int u = 0, v = 0, w = 0;
    };
private:
    ifstream fin; ofstream fout;
    int V, E;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adj;

public:
    Prim(string in, string out) : fin{in}, fout{out} {
        this->fin >> this->V >> this->E;
        this->edges.resize(E);
        this->adj.resize(V);

        for(int i = 0; i < this->E; i++) {
            int u, v, w;
            this->fin >> u >> v >> w;

            this->edges.push_back({u, v, w});
            this->adj[u].emplace_back(v, w);
            this->adj[v].emplace_back(u, w);
        }
    }

    void PrimMST() {
        int root = 0;
        int edges = -1;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

        vector<int> key(V);
        vector<int> parent(V);
        vector<bool> visited(V);

        for(int i = 0; i < V; i++) {
            key[i] = INF;
            parent[i] = NIL;
            visited[i] = false;
        }

        pq.emplace(0, root);
        key[root] = 0;

        while(!pq.empty()) {
            int current = pq.top().second;
            pq.pop();

            if(visited[current]) {
                continue;
            }

            visited[current] = true;
            edges++;

            for(auto& u: adj[current]) {
                int v = u.first;
                int w = u.second;

                if(key[v] > w && !visited[v]) {
                    key[v] = w;
                    pq.emplace(w, v);
                    parent[v] = current;
                }
            }
        }

        int minCost = 0;
        for(int i = 0; i < V; i++) {
            minCost += key[i];
        }

        fout << minCost << "\n";
        fout << edges << "\n";

        // afisare MST
        vector<pair<int, int>> tree;
        for(int i = 0; i < V; i++) {
            if(i != root && visited[i] && visited[parent[i]]) {
                tree.emplace_back(parent[i], i);
            }
        }

        sort(tree.begin(), tree.end(), [](const pair<int, int>& e1, const pair<int, int>& e2){
            if(e1.first == e2.first) {
                return e1.second < e2.second;
            }
            return e1.first < e2.first;
        });

        for(auto& e: tree) {
            this->fout << e.first << " " << e.second << "\n";
        }
    }
};

int main(int argc, char **argv) {
    //Prufer prufer{argv[1], argv[2]};
    //prufer.PruferCoding();
    //prufer.PruferDecoding();

    //Huffmann huffmann{argv[1], argv[2]};
    //huffmann.HuffmanCoding();
    //huffmann.HuffmannDecoding();

    Kruskal kruskal{argv[1], argv[2]};
    kruskal.KruskalMST();

    Prim prim{argv[1], argv[2]};
    prim.PrimMST();

    return 0;
}