#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Usamos unordered_map para búsqueda en tiempo constante O(1)
// Necesitamos un hash para el pair de ints
struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> & v) const {
        return v.first * 31 + v.second;
    }
};

typedef std::pair<int, int> Pair;
typedef std::unordered_map<Pair, int, pair_hash> Stats;

void get_stats(const std::vector<int>& ids, Stats& stats) {
    stats.clear();
    for (size_t i = 0; i < ids.size() - 1; ++i) {
        stats[{ids[i], ids[i+1]}]++;
    }
}

// Merge "in-place" para evitar copias masivas de memoria
void perform_merge(std::vector<int>& ids, Pair pair, int replacement_id) {
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < ids.size(); ++read_idx) {
        if (read_idx < ids.size() - 1 && ids[read_idx] == pair.first && ids[read_idx+1] == pair.second) {
            ids[write_idx++] = replacement_id;
            read_idx++; // saltar el segundo elemento
        } else {
            ids[write_idx++] = ids[read_idx];
        }
    }
    ids.resize(write_idx); // Ajustar el tamaño final sin reasignar memoria
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: bpe_core <archivo_corpus> <num_merges>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file) return 1;

    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::vector<int> ids(buffer.begin(), buffer.end());
    file.close();

    int num_merges = std::stoi(argv[2]);
    std::ofstream merges_file("merges.txt");
    Stats stats;

    for (int i = 0; i < num_merges; ++i) {
        get_stats(ids, stats);
        if (stats.empty()) break;

        Pair best_pair;
        int max_freq = -1;
        
        for (auto const& it : stats) {
            const Pair& pair = it.first;
            int freq = it.second;

            if (freq > max_freq) {
                max_freq = freq;
                best_pair = pair;
            }
        }

        if (max_freq <= 0) break;

        merges_file << best_pair.first << " " << best_pair.second << "\n";
        perform_merge(ids, best_pair, 256 + i);

        if ((i + 1) % 100 == 0) {
            std::cout << "Fusion " << (i + 1) << " completada. Tamanio actual: " << ids.size() << std::endl;
        }
    }

    merges_file.close();
    return 0;
}