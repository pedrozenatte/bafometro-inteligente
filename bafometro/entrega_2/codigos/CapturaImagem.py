import cv2
import os

# Criação do diretório de treinamento
os.makedirs("imagem", exist_ok=True)

# Inicializar a câmera
camera = cv2.VideoCapture(0)
cascata_facial = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

id_pessoa = input("Digite um ID numérico para a pessoa: ")

print("Pressione 'q' para parar a captura de imagens.")

contador = 0
while True:
    ret, frame = camera.read()
    if not ret:
        print("Erro ao capturar frame da câmera.")
        break

    cinza = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    rostos = cascata_facial.detectMultiScale(cinza, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    for (x, y, w, h) in rostos:
        contador += 1
        rosto = cinza[y:y + h, x:x + w]
        cv2.imwrite(f"imagem/pessoa_{id_pessoa}_{contador}.jpg", rosto)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

    cv2.imshow("Captura de Imagens", frame)

    if cv2.waitKey(1) & 0xFF == ord('q') or contador >= 150:  # Limite de 150 imagens por pessoa 
        break

camera.release()
cv2.destroyAllWindows()
print("Imagens capturadas com sucesso!")
