#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h> 
#include <chrono>
#include <thread>
#include <cctype> // Para a função isprint()
#include <algorithm> // Para std::remove_if
using namespace std;

// UNIVERSIDADE DE BRASÍLIA
// Gerenciiador de Senhas CERBERUS
// Autor...........: Davi Sousa de Farias
// Matrícula.......: 251007458
// Matéria.........: Computação para Engenharia (CPE) - Turma A 
// Prof............: Cláudia J. Barenco Abbas

// Solução fornecida pelo CLAUDE
// Problema: A criptografia XOR (8-bit ASCII) pode gerar caracteres não imprimíveis (como bytes 0-31) que não podem ser salvos corretamente em arquivos de texto.
// Solução: Dupla camada de proteção: Criptografia em 2 Passos (XOR + Base64)
// 1. XOR - Criptografia dos dados
// 2. Base64 - Codificação que transforma qualquer byte (Nesse caso, a senha criptografada utilizando XOR, em binário) em caracteres imprimíveis (A-Z, a-z, 0-9, +, /)
// Resultado: Todos os dados salvos no arquivo são sempre caracteres "seguros" que podem ser lidos/escritos sem problemas.
// FLUXO:
// Salvar: Texto → XOR → Base64 → Arquivo
// Carregar: Arquivo → Base64 → XOR → Texto

// Função para codificar string em Base64 simples para evitar caracteres não imprimíveis (Feita pelo CLAUDE)
string base64_encode(const string& input) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string result;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

// Função para decodificar Base64 (Feita pelo CLAUDE)
string base64_decode(const string& input) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string result;
    int val = 0, valb = -8;
    for (unsigned char c : input) {
        if (c == '=') break;
        auto pos = chars.find(c);
        if (pos == string::npos) continue;
        val = (val << 6) + pos;
        valb += 6;
        if (valb >= 0) {
            result.push_back((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return result;
}

// Função de criptografia XOR (Feita por mim)
string cripXOR(const string& texto, const string& chave) {
    // & é usado para passar a string por referência.
    // const evita modificações no texto original.
    string saida = texto;
    int chaveLen = chave.length();

    for (int i = 0; i < texto.length(); ++i) {
        saida[i] = texto[i] ^ chave[i % chaveLen];
        // O operador '%' (módulo) garante que a chave se repita caso o texto seja mais longo
        // ^ é o operador XOR em C++
    }
    return saida;
}

// Função combinada: criptografa com XOR e codifica em Base64 (Feita pelo CLAUDE)
string criptografarSeguro(const string& texto, const string& chave) {
    string xorResult = cripXOR(texto, chave);
    return base64_encode(xorResult);
}

// Função combinada: decodifica Base64 e descriptografa com XOR (Feita pelo CLAUDE)
string descriptografarSeguro(const string& textoCodificado, const string& chave) {
    string xorResult = base64_decode(textoCodificado);
    return cripXOR(xorResult, chave);
}

// Estrutura para armazenar os dados
struct Senha {
    string servico;
    string usuario;
    string senha;
};

// Função para desenhar o menu
void desenharMenu(const vector<string>& opcoes, int opcaoSelecionada) {
    system("cls");

    const int larguraMenu = 55; 
    const string bloco = "\u2588"; // Caractere de quadrado

    for (int i = 0; i < larguraMenu + 2; ++i) cout << bloco;
    cout << endl;

    // ==================TITULO==================
    string titulo = "CERBERUS";
    int espacosAntes = (larguraMenu - titulo.length()) / 2;
    int espacosDepois = larguraMenu - titulo.length() - espacosAntes;

    cout << bloco; // Borda esquerda
    cout << "\x1B[0m";
    for (int i = 0; i < espacosAntes; ++i) cout << " ";
    cout << titulo;
    for (int i = 0; i < espacosDepois; ++i) cout << " ";
    cout << "\x1B[0m";
    cout << bloco << endl;

    // Linha separadora
    cout << bloco;
    for (int i = 0; i < larguraMenu; ++i) cout << " ";
    cout << bloco << endl;
    // ===================TITULO====================

    // Desenha as opções
    for (int i = 0; i < opcoes.size(); ++i) {
        cout << bloco; // Borda lateral esquerda
        cout << "\x1B[0m";

        if ((i + 1) == opcaoSelecionada) {
            cout << "\x1B[30;47m";
            
            string textoOpcao = " > " + opcoes[i];
            cout << textoOpcao;
            
            // Preenche o resto da linha
            for (int j = textoOpcao.length(); j < larguraMenu; ++j) cout << " ";

            cout << "\x1B[0m";
            cout << bloco << endl; // Borda lateral direita
            cout << "\x1B[0m";
        } else {
            string textoOpcao = "   " + opcoes[i];
            cout << textoOpcao;
            
            // Preenche o resto da linha
            for (int j = textoOpcao.length(); j < larguraMenu; ++j) cout << " ";
            
            cout << bloco << endl; // Borda lateral direita
            cout << "\x1B[0m";
        }
    }
    
    // Borda inferior
    cout << "\x1B[0m";
    for (int i = 0; i < larguraMenu + 2; ++i) cout << bloco;
    cout << endl;
    
    // Instruções
    cout << "\x1B[1;33m"; // Amarelo para as instruções
    cout << "\n   Use as setas para navegar e Enter para selecionar." << endl;
    cout << "\x1B[0m";
}

void artCerberus() {
    vector<string> C = {
        " █████  ",
        "██   ██ ",
        "██      ",
        "██      ",
        "██      ",
        "██   ██ ",
        " █████  "
    };
    vector<string> E = {
        "███████ ",
        "██      ",
        "██      ",
        "██████  ",
        "██      ",
        "██      ",
        "███████ "
    };
    vector<string> R = {
        "██████  ",
        "██   ██ ",
        "██   ██ ",
        "██████  ",
        "██   ██ ",
        "██   ██ ",
        "██   ██ "
    };
    vector<string> B = {
        "██████  ",
        "██   ██ ",
        "██   ██ ",
        "██████  ",
        "██   ██ ",
        "██   ██ ",
        "██████  "
    };
    vector<string> U = {
        "██   ██ ",
        "██   ██ ",
        "██   ██ ",
        "██   ██ ",
        "██   ██ ",
        "██   ██ ",
        " █████  "
    };
    vector<string> S = {
        " █████  ",
        "██   ██ ",
        "██      ",
        " █████  ",
        "      ██",
        "██   ██ ",
        " █████  "
    };

    for (int i = 0; i < 7; ++i) {
        // Para cada linha i, imprime a parte correspondente de cada letra
        cout << C[i] << E[i] << R[i] << B[i] << E[i] << R[i] << U[i] << S[i] << endl;
    }
    cout << endl;
};

// Essa função é necessária para verificar em tempo real se o usuário deseja cancelar a entrada de dados ou deixou algum campo em branco.
// Não é possível usar cin para isso, pois ele não permite capturar teclas especiais como ESC ou Backspace diretamente.
// cin também só permite verificar por campos em branco após o usuário passar por todo o processo, o que não é ideal.
// cin também não permite esconder a senha digitada.
bool getInputComCancelamento(string& resultado, bool esconderSenha = false) {
    char ch;
    resultado.clear();

    while ((ch = _getch()) != 13) { // 13 = Enter
        
        if (ch == 27) { // 27 = ESC
            return false;
        }

        if (ch == 8) { // 8 = Backspace
            if (!resultado.empty()) {
                resultado.pop_back();
                cout << "\b \b";
            }
        } else {
            resultado += ch;
            if (esconderSenha) {
                cout << '*';
            } else {
                cout << ch;
            }
        }
    }
    cout << endl;
    return true;
}

string getPasswordInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' é Enter
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        } else {
            password += ch;
            cout << '*'; // Mostra * em vez da letra
        }
    }
    cout << endl;
    return password;
}

void adicionarSenha(const string& nomeArquivo, const string& chave) {
    
    Senha novaSenha;
    bool cancelado = false;

    cout << "          -- Adicionar Nova Senha --" << endl;
    cout << " \x1B[93mPressione ESC a qualquer momento para cancelar.\x1B[0m" << endl << endl;

    // Pedir o nome do Serviço
    do {
        cout << "Digite o nome do serviço: ";
        if (!getInputComCancelamento(novaSenha.servico)) { cancelado = true; break; }

        if (novaSenha.servico.empty()) {
            cout << "\x1B[91mO nome do serviço não pode estar vazio. Tente novamente.\x1B[0m" << endl;
        }
    } while (novaSenha.servico.empty());

    if (cancelado) {
        cout << "\n\x1B[93mOperacão cancelada.\x1B[0m" << endl;
        return;
    }

    // Pedir o nome do Utilizador
    do {
        cout << "Digite o nome de usuário ou e-mail: ";
        if (!getInputComCancelamento(novaSenha.usuario)) { cancelado = true; break; }

        if (novaSenha.usuario.empty()) {
            cout << "\x1B[91mO nome de usuário não pode estar vazio. Tente novamente.\x1B[0m" << endl;
        }
    } while (novaSenha.usuario.empty());

    if (cancelado) {
        cout << "\n\x1B[93mOperacão cancelada.\x1B[0m" << endl;
        return;
    }

    // Pedir a Senha
    do {
        cout << "Digite a senha: ";
        // O 'true' aqui ativa o modo de esconder a senha com '*'
        if (!getInputComCancelamento(novaSenha.senha, true)) { cancelado = true; break; }

        if (novaSenha.senha.empty()) {
            cout << "\x1B[91mA senha não pode estar vazia. Tente novamente.\x1B[0m" << endl;
        }
    } while (novaSenha.senha.empty());
    
    if (cancelado) {
        cout << "\n\x1B[93mOperacão cancelada.\x1B[0m" << endl;
        return;
    }

    // Se chegamos aqui, todos os dados foram preenchidos

    // CRIPTOGRAFIA
    string usuarioCriptografado = criptografarSeguro(novaSenha.usuario, chave);
    string senhaCriptografada = criptografarSeguro(novaSenha.senha, chave);

    // ARMAZENAMENTO DOS DADOS
    ofstream arquivo(nomeArquivo, ios::app);

    if (arquivo.is_open()) {
        // FORMATO: servico_em_texto;utilizador_criptografado;senha_criptografada
        arquivo << novaSenha.servico << ";" << usuarioCriptografado << ";" << senhaCriptografada << endl;

        arquivo.close();
        cout << "\n\x1B[92mSenha adicionada com sucesso!\x1B[0m" << endl;
    } else {
        system("cls");
        cout << "\n\x1B[91mErro: Não foi possível abrir o ficheiro para guardar a senha.\x1B[0m" << endl;
        cout << "\x1B[93mCertifique-se que o arquivo que contém as senhas não está aberto.\x1B[0m" << endl;
    }
}

// Função para ler as senhas salvas no arquivo, e as armazena em um vetor.
vector<Senha> carregarSenhasDoArquivo(const string& nomeArquivo, const string& chave) {
    vector<Senha> senhas;
    ifstream arquivo(nomeArquivo);

    if (arquivo.is_open()) {
        string linha;
        while (getline(arquivo, linha)) {
            // Remove espaços em branco e caracteres de controle
            linha.erase(remove_if(linha.begin(), linha.end(), [](char c) {
                return c == '\r' || c == '\n';
            }), linha.end());
            
            if (linha.empty()) {
                continue;
            }

            // Buscar as posições dos separadores
            size_t pos1 = linha.find(';');
            if (pos1 == string::npos) continue;
            
            size_t pos2 = linha.find(';', pos1 + 1);
            if (pos2 == string::npos) continue;

            // Extrair as partes
            string servico = linha.substr(0, pos1);
            string usuarioCriptografado = linha.substr(pos1 + 1, pos2 - pos1 - 1);
            string senhaCriptografada = linha.substr(pos2 + 1);

            // Verificar se todas as partes existem
            if (!servico.empty() && !usuarioCriptografado.empty() && !senhaCriptografada.empty()) {
                Senha s;
                s.servico = servico;
                
                try {
                    s.usuario = descriptografarSeguro(usuarioCriptografado, chave);
                    s.senha = descriptografarSeguro(senhaCriptografada, chave);
                    senhas.push_back(s);
                } catch (...) {
                    // Se houver erro na descriptografia, pula esta entrada
                    continue;
                }
            }
        }
        arquivo.close();
    }
    return senhas;
}

// [FUNÇÕES REUTILIZÁVEIS] - Menu interativo contendo as senhas salvas (mesma lógica do menu principal, mas com as senhas).

// Desenha o menu navegável de senhas, mostrando os serviços disponíveis.
void desenharMenuSenhas(const vector<Senha>& senhas, int selecionadaNaPagina, int paginaAtual, int itensPorPagina) {
    system("cls");
    cout << "--- Selecione um Serviço (ESC para voltar) ---" << endl << endl;

    // O total de páginas é calculado com base no número de senhas e no número de itens por página.
    // Se não houver senhas, totalDePaginas será 1 para evitar divisão por zero.
    int totalDePaginas = (senhas.size() + itensPorPagina - 1) / itensPorPagina; // Arredonda para cima -> (N + D - 1) / D [TETO DE UMA DIVISÃO]
    if (totalDePaginas == 0) totalDePaginas = 1;

    int indiceInicio = paginaAtual * itensPorPagina;
    int indiceFim = min(indiceInicio + itensPorPagina, (int)senhas.size());

    // Loop de desenho simplificado
    for (int i = indiceInicio; i < indiceFim; ++i) {
        if ((i - indiceInicio) == selecionadaNaPagina) {
            // Imprime o item selecionado e VAI para a próxima linha
            cout << "\x1B[30;47m > " << senhas[i].servico << " \x1B[0m" << endl;
        } else {
            // Imprime o item não selecionado e VAI para a próxima linha
            cout << "   " << senhas[i].servico << endl;
        }
    }

    // Informações da página
    cout << "\n\n\x1B[93mUse as setas < > para mudar de pagina.\x1B[0m" << endl;
    cout << "Pagina " << paginaAtual + 1 << " de " << totalDePaginas << endl;
}

// Seleciona uma senha do menu, retornando o índice da senha selecionada ou -1 se o usuário cancelar.
int selecionarSenhaDoMenu(const vector<Senha>& senhas) {
    const int ITENS_POR_PAGINA = 5;
    int paginaAtual = 0;
    int totalDePaginas = (senhas.size() + ITENS_POR_PAGINA - 1) / ITENS_POR_PAGINA;
    int opcaoSelecionadaNaPagina = 0;
    int tecla;

    do {
        desenharMenuSenhas(senhas, opcaoSelecionadaNaPagina, paginaAtual, ITENS_POR_PAGINA);
        tecla = _getch();

        switch (tecla) {
            case 224:
            case -32: { 
                
                tecla = _getch();
                int itensNestaPagina = min(ITENS_POR_PAGINA, (int)senhas.size() - (paginaAtual * ITENS_POR_PAGINA));

                if (tecla == 72) { // Seta para cima
                    opcaoSelecionadaNaPagina = (opcaoSelecionadaNaPagina > 0) ? opcaoSelecionadaNaPagina - 1 : itensNestaPagina - 1;
                } else if (tecla == 80) { // Seta para baixo
                    opcaoSelecionadaNaPagina = (opcaoSelecionadaNaPagina < itensNestaPagina - 1) ? opcaoSelecionadaNaPagina + 1 : 0;
                } else if (tecla == 75) { // Seta para esquerda
                    if (paginaAtual > 0) {
                        paginaAtual--;
                        opcaoSelecionadaNaPagina = 0;
                    }
                } else if (tecla == 77) { // Seta para direita
                    if (paginaAtual < totalDePaginas - 1) {
                        paginaAtual++;
                        opcaoSelecionadaNaPagina = 0;
                    }
                }
                break;

            } 

            case 13: { // Enter

                // Calcula o índice global da senha selecionada
                int indiceGlobal = paginaAtual * ITENS_POR_PAGINA + opcaoSelecionadaNaPagina;
                if (indiceGlobal < senhas.size()) {
                    return indiceGlobal;
                }
                break;
            }
            case 27: // ESC
                return -1;
        }
    } while (true);
}

// Funções para trabalhar com senhas
void mostrarSenhas(const string& nomeArquivo, const string& chave) {
    vector<Senha> senhas = carregarSenhasDoArquivo(nomeArquivo, chave);
    if (senhas.empty()) {
        cout << "\x1B[93mNenhuma senha guardada ainda.\x1B[0m" << endl;
        // Pausa (Sem essa pausa, a mensagem que não há senhas não é vista no menu principal)
        cout << "\nPressione qualquer tecla para voltar ao menu...";
        _getch();
        return;
    }

    // 2. Loop principal desta tela. Só sai quando o utilizador pressionar ESC no menu de seleção.
    while (true) {
        // Chama o menu reutilizável para obter a escolha do utilizador
        int indiceSelecionado = selecionarSenhaDoMenu(senhas);

        // Se o utilizador pressionou ESC (nosso sinal é -1), sai do loop while.
        if (indiceSelecionado == -1) {
            break; 
        }

        // Se o utilizador escolheu uma senha, mostra os detalhes
        system("cls");
        cout << "--- Detalhes da Entrada ---" << endl << endl;
        cout << "  Serviço: " << senhas[indiceSelecionado].servico << endl;
        cout << "  Usuário: " << senhas[indiceSelecionado].usuario << endl;
        cout << "  Senha:   " << "\x1B[92m" << senhas[indiceSelecionado].senha << "\x1B[0m" << endl << endl;
        cout << "\x1B[93mPressione qualquer tecla para voltar a lista...\x1B[0m";
        _getch();
        // Ao pressionar uma tecla, o loop `while(true)` recomeça, redesenhando o menu de seleção.
    }
    // Quando o loop quebra (pelo ESC), a função termina e volta para o menu principal.
}

void excluirSenha(const string& nomeArquivo, const string& chave, int indiceParaExcluir) {
    vector<Senha> senhas = carregarSenhasDoArquivo(nomeArquivo, chave);
    
    if (indiceParaExcluir >= 0 && indiceParaExcluir < senhas.size()) {
        // Remove a senha do vetor
        senhas.erase(senhas.begin() + indiceParaExcluir);
        
        // Reescreve o arquivo sem a senha excluída
        ofstream arquivo(nomeArquivo, ios::trunc); // ios::trunc limpa o arquivo
        
        if (arquivo.is_open()) {
            for (const auto& senha : senhas) {
                string usuarioCriptografado = criptografarSeguro(senha.usuario, chave);
                string senhaCriptografada = criptografarSeguro(senha.senha, chave);
                arquivo << senha.servico << ";" << usuarioCriptografado << ";" << senhaCriptografada << endl;
            }
            arquivo.close();
            cout << "\n\x1B[92mSenha excluída com sucesso!\x1B[0m" << endl;
        } else {
            system("cls");
            cout << "\n\x1B[91mErro: Não foi possível abrir o ficheiro para salvar as alterações.\x1B[0m" << endl;
            cout << "\x1B[93mCertifique-se que o arquivo que contém as senhas não está aberto.\x1B[0m" << endl;
        }
    }
}

void gerenciarSenhas(const string& nomeArquivo, const string& chave) {
    vector<Senha> senhas = carregarSenhasDoArquivo(nomeArquivo, chave);
    if (senhas.empty()) {
        cout << "\x1B[93mNenhuma senha para gerir.\x1B[0m" << endl;
        // Pausa (Sem essa pausa, a mensagem que não há senhas não é vista no menu principal)
        cout << "\nPressione qualquer tecla para voltar ao menu...";
        _getch();
        return;
    }

    int indiceSelecionado = selecionarSenhaDoMenu(senhas);

    if (indiceSelecionado != -1) {
        system("cls");
        cout << "--- Gerenciar Senha ---" << endl << endl;
        cout << "  Serviço: " << senhas[indiceSelecionado].servico << endl;
        cout << "  Usuário: " << senhas[indiceSelecionado].usuario << endl;
        cout << "  Senha:   " << "\x1B[92m" << senhas[indiceSelecionado].senha << "\x1B[0m" << endl << endl;
        
        cout << "\x1B[91mDeseja excluir esta senha? (S/N): \x1B[0m";
        char resposta = _getch();
        cout << resposta << endl;
        
        if (resposta == 'S' || resposta == 's') {
            cout << "\n\x1B[93mConfirma a exclusão? (S/N): \x1B[0m";
            char confirmacao = _getch();
            cout << confirmacao << endl;
            
            if (confirmacao == 'S' || confirmacao == 's') {
                excluirSenha(nomeArquivo, chave, indiceSelecionado);
            } else {
                cout << "\n\x1B[93mExclusão cancelada.\x1B[0m" << endl;
            }
        } else {
            cout << "\n\x1B[93mOperacão cancelada.\x1B[0m" << endl;
        }
    }
}

//
// FUNÇÃO PRINCIPAL
//

int main() {
    system("chcp 65001"); // Configura o console para UTF-8 (Para exibir corretamente os caracteres especiais)
    system("CLS");
    artCerberus();

    string masterKey = "admin";
    const string NOME_ARQUIVO = "senhas.csv";

    cout << "Digite sua senha: ";
    string password = getPasswordInput();
    if (password == masterKey) {
        system("CLS");
        cout << "\x1B[92m"; // ANSI escape code para verde
        artCerberus();
        cout << "\x1B[92mSenha correta. Bem-vindo ao Cerberus!\x1B[0m" << endl;
        cout << "\x1B[0m"; // Reseta a cor do texto
        // ACESSO CONCEDIDO

        // Delay
        this_thread::sleep_for(std::chrono::seconds(2));

    // Caracteres sejam exibidos corretamente
    SetConsoleOutputCP(65001);

    // Desativa o cursor piscando
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    int opcaoSelecionada = 1;
    int tecla;

    // Opções menu
    vector<string> opcoes = {
        "1. Mostrar senhas salvas",
        "2. Adicionar uma nova senha",
        "3. Excluir senha existente",
        "4. Sair"
    };

    do {
        
        desenharMenu(opcoes, opcaoSelecionada);

        // Espera uma tecla
        tecla = _getch();

        // Processa a tecla
        switch (tecla) {
            // Teclas de seta são lidas como dois caracteres
            case -32: 
            case 224: { 
                // Lê o próximo caractere para identificar qual seta
                char teclaSeta = _getch();
                switch (teclaSeta) {
                    case 72: // Seta para cima
                        if (opcaoSelecionada > 1) {
                            opcaoSelecionada--;
                        } else {
                            opcaoSelecionada = opcoes.size(); // Volta para o final
                        }
                        break;
                    case 80: // Seta para baixo
                        if (opcaoSelecionada < opcoes.size()) {
                            opcaoSelecionada++;
                        } else {
                            opcaoSelecionada = 1; // Volta para o início
                        }
                        break;
                }
                break;
            }
            // Teclas numéricas
            case '1': opcaoSelecionada = 1; break;
            case '2': opcaoSelecionada = 2; break;
            case '3': opcaoSelecionada = 3; break;
            case '4': opcaoSelecionada = 4; break;

            // Tecla Enter para confirmar a seleção
            case 13: {

                system("cls");
                cout << "\x1B[0m";

                // -- LOGICA DO MENU --
                switch (opcaoSelecionada) {
                    case 1: // Mostrar senhas salvas
                        mostrarSenhas(NOME_ARQUIVO, masterKey);
                        break;

                    case 2: // Adicionar uma nova senha
                        adicionarSenha(NOME_ARQUIVO, masterKey);
                        cout << "\nPressione qualquer tecla para voltar ao menu...";
                        _getch();
                        break;

                    case 3: // Excluir ou editar senha ja existente
                        gerenciarSenhas(NOME_ARQUIVO, masterKey);
                        cout << "\nPressione qualquer tecla para voltar ao menu...";
                        _getch();
                        break;

                    case 4: // Sair
                        cout << "Encerrando o programa..." << endl;
                        tecla = 27; // ESC para sair do loop
                        break;
                }
                break;
            }
        }

    } while (tecla != 27); // O loop continua ate a tecla ser ESC

    system("cls");
    cout << "Encerrando o programa..." << endl;
    // Restaura a visibilidade do cursor
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(out, &cursorInfo);

    return 0;

    } else {
        // Senha errada
        system("CLS");
        cout << "\x1B[91m"; // ANSI escape code para vermelho
        artCerberus();
        cout << "Senha incorreta - Acesso negado." << endl;
        cout << "\x1B[0m"; // Reseta a cor do texto
        cout << "Pressione qualquer tecla para sair..." << endl;
        _getch(); // Espera por uma tecla
        system("CLS");
        return 1; // Sai do programa com erro
    }   
};