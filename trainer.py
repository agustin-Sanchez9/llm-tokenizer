import byte_converter
import os


# cuenta la frecuencia de los pares adyacentes de ids
def get_stast(ids):
    counts = {}
    for pair in zip(ids, ids[1:]):
        counts[pair] = counts.get(pair, 0) + 1
    return counts


# reemplaza las ocurrencias del par por el nuevo id
def merge(ids, pair, replacement_id):
    new_ids = []
    i = 0
    while i < len(ids):
        if i < len(ids) - 1 and ids[i] == pair[0] and ids[i+1] == pair[1]:
            new_ids.append(replacement_id)
            i += 2
        else:
            new_ids.append(ids[i])
            i += 1
    return new_ids


# funcion principal del flujo
def train_bpe(raw_bytes, num_merges):
    ids = list(raw_bytes)
    merges = [] # (int, int) -> int

    for i in range(num_merges):
        stats = get_stast(ids)
        if not stats:
            break
    
        most_frequent_pair = max(stats, key=stats.get)
        new_token_id = 256 + i

        ids = merge(ids, most_frequent_pair, new_token_id) # aplicar la fusion en la secuencia
        merges.append(most_frequent_pair)
    
    return merges

if __name__ == "__main__":

    byte_converter.convert_text_to_bytes("don_quijote.txt", "corpus.bin")


    if not os.path.exists("corpus.bin"):
        print("Error: No se encuentra 'corpus.bin'. Ejecuta primero 'byte_converter.py'")
    else:
        with open("corpus.bin", "rb") as f:
            data = f.read()
        
        NUM_MERGES = 5000
        all_merges = train_bpe(data, NUM_MERGES)

        with open("merges.txt", "w", encoding="utf-8") as f:
            for pair in all_merges:
                f.write(f"{pair[0]} {pair[1]}\n")
        
        print("\nArchivo 'merges.txt' generado con éxito.")