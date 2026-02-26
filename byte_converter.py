

# convert_text_to_bytes debe recibir el corpus en formato de texto y devolver su version en bytes
def convert_text_to_bytes(input_file, output_file):
    try:
        # abrir corpus ingresado
        with open(input_file, 'r',encoding='utf-8') as f:
            text = f.read()
        
        print(f"Archivo cargado: {input_file}")
        print(f"Cantidad de caracteres: {len(text)}")

        # convertir a bytes
        byte_data = text.encode('utf-8')

        print(f"Tamaño en bytes: {len(byte_data)}")

        # guardar binario
        with open(output_file, 'wb') as f:
            f.write(byte_data)

        print(f"Guardado de bytes exitoso en: {output_file}")

        # debug
        print("\nPrimeros 20 bytes del corpus:")
        print(list(byte_data[:20]))

    except FileNotFoundError:
        print(f"Error: no se encontro el archivo {input_file}")
    except Exception as e:
        print(f"Ocurrio un error inesperado: {e}")