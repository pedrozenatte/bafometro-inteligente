import cv2
import numpy as np
import os

# Caminho do dataset
caminho_dataset = "imagem"

# Inicializar o reconhecedor facial
reconhecedor = cv2.face.LBPHFaceRecognizer_create()

# Lista para armazenar dados
ids = []
rostos = []

# Tamanho padrão para as imagens
tamanho_padrao = (100, 100)

# Carregar imagens e IDs
for arquivo in os.listdir(caminho_dataset):
    if arquivo.endswith(".jpg"):
        caminho = os.path.join(caminho_dataset, arquivo)

        # Carregar a imagem em escala de cinza para facilitar o reconhecimento 
        imagem = cv2.imread(caminho, cv2.IMREAD_GRAYSCALE)
        if imagem is None:
            print(f"[ERRO] Não foi possível carregar a imagem: {arquivo}")
            continue

        # Redimensionar a imagem para o tamanho padrão
        try:
            imagem_redimensionada = cv2.resize(imagem, tamanho_padrao)
        except Exception as e:
            print(f"[ERRO] Não foi possível redimensionar a imagem {arquivo}: {e}")
            continue

        # Obter o ID da pessoa
        try:
            id_pessoa = int(arquivo.split("_")[1])
        except ValueError:
            print(f"[ERRO] ID inválido no nome do arquivo: {arquivo}")
            continue

        # Adicionar a imagem e o ID às listas
        rostos.append(imagem_redimensionada)
        ids.append(id_pessoa)

# Verificar se existem imagens válidas
if not rostos:
    print("[ERRO] Nenhuma imagem válida encontrada para treinamento.")
    exit()

# Converter listas para arrays
rostos = np.array(rostos, dtype="uint8")
ids = np.array(ids)

# Treinar o modelo
print("Treinando o modelo... Aguarde.")
reconhecedor.train(rostos, ids)
reconhecedor.save("modelo_reconhecimento.yml")
print("Modelo treinado e salvo com sucesso!")
