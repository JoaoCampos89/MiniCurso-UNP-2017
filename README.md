# MiniCurso-UNP-2017

Códigos do servidor, broker Mqtt e do código arduino

Client
  -- minicurso-ESp.ino --> sketch usado para mandar os dados para o servidor usando o esp

Broker
    instalar mosca
    digitar `mosca -v | pino`  para iniciar broker mqtt

Server
  Requisitos
      node js instalado
      meteor js instalado


  Servidor usado para receber os dados dos Esp
  digitar `git clone https://github.com/JoaoCampos89/MiniCurso-UNP-2017` na linha de comandos para clonar o repositório
  digitar `cd server`
  digitar `meteor npm --install save`
  digitar `meteor` para iniciar o servidor
