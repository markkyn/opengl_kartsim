def ppm_p3_to_p2_ascii(input_file, output_file):
    # Abrir arquivo de entrada para leitura
    with open(input_file, 'r') as f:
        lines = f.readlines()

    # Remover comentários
    lines = [line.strip() for line in lines if not line.startswith('#')]

    # Obter largura, altura e valor máximo de cor
    width, height = map(int, lines[1].split())
    max_color = int(lines[2])

    # Converter dados para lista de inteiros
    data = [int(value) for line in lines[3:] for value in line.split()]

    # Abrir arquivo de saída para escrita
    with open(output_file, 'w') as f:
        # Escrever cabeçalho P2
        f.write('P2\n')
        f.write(f"{width} {height}\n")
        f.write(f"{max_color}\n")

        # Escrever dados convertidos
        for i in range(0, len(data), width):
            row = data[i:i+width]
            for value in row:
                # Escrever valor de cor
                f.write(f"{value} ")
            f.write('\n')

if __name__ == "__main__":
    input_file = "input.ppm"  # Nome do arquivo de entrada
    output_file = "output.pgm"  # Nome do arquivo de saída
    ppm_p3_to_p2_ascii(input_file, output_file)
    print("Conversão concluída com sucesso!")