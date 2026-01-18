# 🚗🫁 Bafômetro Inteligente

Projeto desenvolvido na disciplina de **Sensores Inteligentes**, com foco em integração entre hardware e software para detecção de álcool e validação de usuário por reconhecimento facial.

## 📌 Objetivo
Construir um protótipo de bafômetro que:
- detecta sopro e estima a concentração de álcool;
- sinaliza níveis por LEDs;
- (na entrega 2) integra reconhecimento facial para liberação/bloqueio de acesso.

## 🧩 Estrutura do repositório
- `bafometro/entrega_1/`
  - `codigo/`: versão básica do bafômetro em Arduino.
  - `video/`: vídeo da entrega 1.
  - `Sensor de álcool.pptx`: apresentação da entrega 1.
- `bafometro/entrega_2/`
  - `codigos/`: Arduino + Python (captura, treino e reconhecimento facial).
  - `video/`: vídeo da entrega 2.

## ⚙️ Entrega 1 — Bafômetro básico (Arduino)
- Leitura do sensor no pino analógico.
- Detecção de limiar para iniciar a medição.
- Cálculo do valor máximo em 5 segundos.
- Mapeamento do nível para LEDs (barra de 6 níveis).

## 🤖 Entrega 2 — Bafômetro com reconhecimento facial
### Arduino
- Detecção de sopro por variação de leitura.
- Cálculo da concentração de álcool (mg/L).
- Envio de dados via serial com flags (`FLAG` e `ALC`).
- Bloqueio/liberação de partida via pino digital.

### Python
- `CapturaImagem.py`: captura imagens do rosto para treino.
- `TreinarModelo.py`: treina modelo LBPH e salva em `modelo_reconhecimento.yml`.
- `ReconhecimentoThread.py`: reconhece rosto e decide bloqueio/liberação com base na confiança e no álcool.

## 🧪 Fluxo recomendado
1. Executar `CapturaImagem.py` para criar o dataset.
2. Executar `TreinarModelo.py` para gerar o modelo.
3. Subir o código Arduino da entrega 2.
4. Executar `ReconhecimentoThread.py` para operação integrada.

## 📹 Materiais
- `bafometro/entrega_1/video/`: demonstração da entrega 1.
- `bafometro/entrega_2/video/`: apresentação da entrega 2.

## ✅ Observações
- Ajuste a porta serial em `ReconhecimentoThread.py` conforme o seu Arduino.
- A calibração do sensor pode exigir mudanças em `LIMIAR` e `delta`.
