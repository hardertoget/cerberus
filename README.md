# Trabalho Final - Computação para Engenharia (CPE)
# Universidade de Brasília

Cerberus é um gerenciador de senhas em C++, que armazena o serviço, usuário e a senha em um arquivo .csv (ou .txt) utilizando
uma cifra XOR somada a codificação Base64 para criptografar os dados.

## Como Compilar e Executar

Você precisará de um compilador C++, como o g++.

1.  **Clone o repositório:**
    ```sh
    git clone [https://github.com/hardertoget/cerberus.git](https://github.com/hardertoget/cerberus.git)
    cd cerberus
    ```

2.  **Compile o código:**
    ```sh
    g++ cerberus.cpp -o cerberus.exe
    ```

3.  **Execute o programa:**
    * No Windows: `.\cerberus.exe`
    * No Linux/macOS: `./cerberus`