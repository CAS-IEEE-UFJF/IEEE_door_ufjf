![](https://img.shields.io/badge/build-passing-rgb(76%2C198%2C31).svg)

## [ 2º Versão](https://github.com/wesley-cantarino/IEEE_door_ufjf/tree/master/version2)
### Apresentação

O Ramo Estudantil IEEE devido ao seu grande número de membros, necessitava de um melhor controle de acesso ao seu domínio. Porém, as trancas convencionais demandam cópias de chave e não havia como saber quem entrou e em qual horário. A partir disso fizemos um levantamento de qual seria a melhor proposta para resolver o controle de acesso ao tempo que pudéssemos gerenciar as chaves. Em nosso levantamento descobrimos que as carteirinhas estudantis possuíam um dispositivo interno para identificação chamado de radiofrequência chamado RFID, elas por sua vez, são chaves únicas e podem ser obtidas através de leitura.

### Objetivo

O projeto Porta Tunada 2.0 consiste em um equipamento eficiente e de baixo custo para controlar a entrada e saída de pessoas na sala do Ramo Estudantil IEEE UFJF, estando restrito o acesso somente aos membros que tenham suas carteirinhas estudantis cadastradas. Nessa 2º versão fomentamos a equipe o uso do nodeMCU (ESP8266) e para desenvolver a capacidade de programação em IOT.


### Descrição do funcionamento:

Quando uma pessoa aproxima sua carteirinha do equipamento, o RFID - RC522 é o sensor que faz a leitura da tag da carteirinha aproximada. O ESP8266 recebe essa informação e permite que o motor abra a porta, somente se, a mesma for reconhecida. E, então, o LED verde acende e o motor é acionado abrindo a porta. Caso contrário, o buzzer emite um som e o LED vermelho acende. A fonte é utilizada para alimentar o circuito, a ponte H controla o movimento do motor e os jumpers fazem a ligação entre os componentes.

### Componentes utilizados:

* Fonte de 12V;
* Fonte 5V;
* Microcontrolador ESP8266;
* Sensor RFDI - RC522;
* Ponte H - l290n;
* Motor DC;
* Dois LED’s (um vermelho e um verde);
* Buzzer;
* Jumpers.

### Files

```
.
|--- item_adicionais
     |--- contador
          |--- contador.ino
          |--- esquematico_IMG.jpg
          |--- esquematico_fritzing.fzz
          |--- sn74hc595-datasheet.pdf
          |--- video_01.mp4
          |--- video_02.mp4
     |--- matriz_numerica
          |--- library
               |--- keypad.zip
          |--- ZRX_543.jpg
          |--- matriz_numerica.ino
     |--- readme.md
|--- version1
     |--- main.ino
|--- version2
     |--- CODE
          |--- library
               |--- firebase-arduino-master.rar
               |--- rfid-master.rar
          |--- testes
               |--- test_RFID_only.ino
               |--- test_simples.ino
               |--- textHTML_exemple.html
          |--- main.ino
     |--- circuito-porta.fzz
     |--- circuito-porta_IMG.png
     |--- circuito-porta_IMG_in_esp.png
     |--- circuito-porta_IMG_in_rfid.png
     |--- componentes.txt
     |--- flowchart_IMG.jpeg
|--- .gitignore
|--- README.md
|--- this_is_nodeMCU.pdf
```


##### * foto:
![](https://github.com/CAS-IEEE-UFJF/IEEE_door_ufjf/blob/master/version2/circuito-porta_IMG.png)

![](https://github.com/CAS-IEEE-UFJF/IEEE_door_ufjf/blob/master/version2/circuito-porta_IMG_in_rfid.png)

![](https://github.com/CAS-IEEE-UFJF/IEEE_door_ufjf/blob/master/version2/circuito-porta_IMG_in_esp.png)

##### * flowchart:
![](https://github.com/wesley-cantarino/IEEE_door_ufjf/blob/master/version2/flowchart_IMG.jpeg)

## [ 1º Versão](https://github.com/wesley-cantarino/IEEE_door_ufjf/tree/master/version1)
Objetivo da 1º versão é apresentar para a equipe o Arduino e desenvolver a capacidade de programação.
