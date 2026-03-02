#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <vector>

typedef std::pair<int, int> Pair;

// funcion para contar la frecuencia de pares
std::map<Pair, int> get_stats(const std::vector<int>& ids) {
    std::map<Pair, int> stats;
    for (size_t i = 0; i < ids.size() - 1; ++i) {
        Pair p = {ids[i], ids[i+1]};
        stats[p]++;
    }
    return stats;
}

// funcion para reemplazar el par mas frecuente por el nuevo token
std::vector<int> merge(const std::vector<int>& ids, Pair pair, int replacement_id) {
    std::vector<int> new_ids;
    new_ids.reserve(ids.size()); // optimizacion de memoria
    for (size_t i = 0; i < ids.size(); ++i) {
        if (i < ids.size() - 1 && ids[i] == pair.first && ids[i+1] == pair.second) {
            new_ids.push_back(replacement_id);
            i++;
        } else {
            new_ids.push_back(ids[i]);
        }
    }
    return new_ids;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Uso: bpe_core <archivo_corpus> <num_merges>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    int num_merges = std::stoi(argv[2]);

    // leer el corpus binario
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error al abrir " << filename << std::endl;
        return 1;
    }

    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::vector<int> ids(buffer.begin(), buffer.end());
    file.close();

    std::ofstream merges_file("merges.txt");

    for (int i = 0; i < num_merges; ++i) {
        auto stats = get_stats(ids);
        if (stats.empty()) break;

        // encontrar el par mas frecuente
        auto most_frequent = std::max_element(stats.begin(), stats.end(),
            [](const std::pair<Pair, int>& a, const std::pair<Pair, int>& b) {
                return a.second < b.second;
            });

        Pair best_pair = most_frequent->first;
        int new_token_id = 256 + i;

        // guardar la regla en merges.txt
        merges_file << best_pair.first << " " << best_pair.second << std::endl;

        ids = merge(ids, best_pair, new_token_id);
        
        if ((i + 1) % 100 == 0) {
            std::cout << "Progreso: " << (i + 1) << "/" << num_merges << " fusiones." << std::endl;
        }
    }

    merges_file.close();
    std::cout << "Entrenamiento completado. Reglas guardadas en merges.txt" << std::endl;

    return 0;
}