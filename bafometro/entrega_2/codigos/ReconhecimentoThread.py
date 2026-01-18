import cv2
import serial
import threading
import time

# Configurando a porta serial (substitua 'COM3' pela porta onde seu Arduino está conectado)
arduino = serial.Serial(port='COM3', baudrate=9600, timeout=1)

# Variável compartilhada para nível de álcool
nivel_alcool = 0

# Variável compartilhada para liberar o acesso
libera_acesso = 0

# Função para ler o serial vindo do arduino
def ler_serial():
    global nivel_alcool 
    global flag

    while True:
        try:
            data = arduino.readline().decode('utf-8').strip()  # Lê e decodifica os dados
            if data.startswith("FLAG:"):
                flag = int(data.split(":")[1])
            elif data.startswith("ALC:"):
                nivel_alcool = float(data.split(":")[1])
        except ValueError:
            pass  # Ignora leituras inválidas

# Thread para leitura serial, o que aumenta o desempenho
thread_serial = threading.Thread(target=ler_serial, daemon=True)
thread_serial.start()

# Definindo confiança como global
confianca = 0

# Função para verificar confiança
def verifica_confianca():
    while True:
        if confianca > 90:
            print("Pessoa nao confiavel")
        time.sleep(1)
            
# Thread para verificar confiança
thread_confianca = threading.Thread(target=verifica_confianca, daemon=True)
thread_confianca.start()

# Carregar o modelo treinado
reconhecedor = cv2.face.LBPHFaceRecognizer_create()
reconhecedor.read("modelo_reconhecimento.yml")

# Mapeamento de IDs para nomes
nomes = {1: "Pedro", 2: "Estefany", 3: "Luiz"}  # Nomes do reconhecimento.

# Inicializar a câmera
camera = cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FRAME_WIDTH, 640)  # Reduz a resolução
camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cascata_facial = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

print("Pressione 'q' para sair.")
try:
    while True:
        ret, frame = camera.read()
        if not ret:
            print("Erro ao capturar frame da câmera.")
            break 
        
        # Processamento da imagem
        cinza = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        rostos = cascata_facial.detectMultiScale(cinza, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

        for (x, y, w, h) in rostos:
            rosto = cinza[y:y + h, x:x + w]
            id_pessoa, confianca = reconhecedor.predict(rosto)
            nome = nomes.get(id_pessoa, "Desconhecido")
                

            # Exibir nome e confiança
            cv2.putText(frame, f"{nome} ({confianca:.2f})", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (255, 255, 255), 2)
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

            
            # Verifica o nível de álcool
            if nivel_alcool > 0.04 and flag == 1:
                print(f"{nome} bloqueado de dirigir\nConcentração de álcool: {nivel_alcool}mg/L")
                nivel_alcool = 0
            elif nivel_alcool == 0.04 and flag == 1 and confianca < 90:
                print(f"{nome} liberado para dirigir\nConcentração de álcool: {nivel_alcool}mg/L")
                nivel_alcool = 0
                time.sleep(3)

        cv2.imshow("Reconhecimento Facial", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    print("\nEncerrando o programa...")

finally:
    camera.release()
    arduino.close()
    cv2.destroyAllWindows()
    print("Câmera e conexão serial encerradas.")





