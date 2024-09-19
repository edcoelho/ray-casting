#include "cena/Canvas.hpp"
#include <iostream>
#include <string>

Canvas::Canvas(std::size_t altura, std::size_t largura, rgb cor_padrao) {

    this->altura = altura;
    this->largura = largura;
    this->cor_padrao = cor_padrao;

    this->inicializar_matrizes();

}

void Canvas::inicializar_matrizes() {

    this->pixel_buffer.clear();
    this->matriz_solidos.clear();
    this->matriz_solidos.clear();

    for (std::size_t i = 0; i < this->altura; i++) {

        this->pixel_buffer.push_back(std::vector<rgb>());
        this->matriz_solidos.push_back(std::vector<std::shared_ptr<Solido>>());
        this->matriz_malhas.push_back(std::vector<std::shared_ptr<Malha>>());

        for (std::size_t j = 0; j < this->largura; j++) {

            this->pixel_buffer[i].push_back(cor_padrao);
            this->matriz_solidos[i].push_back(nullptr);
            this->matriz_malhas[i].push_back(nullptr);

        }

    }

}

std::size_t Canvas::get_altura() const {

    return this->altura;

}
void Canvas::set_altura(double a, bool reiniciar_matrizes) {

    this->altura = a;

    if (reiniciar_matrizes) this->inicializar_matrizes();

}

std::size_t Canvas::get_largura() const {

    return this->largura;

}
void Canvas::set_largura(double l, bool reiniciar_matrizes) {

    this->largura = l;
    
    if (reiniciar_matrizes) this->inicializar_matrizes();

}

rgb Canvas::get_cor_padrao() const {

    return this->cor_padrao;

}
void Canvas::set_cor_padrao(rgb c) {

    this->cor_padrao = c;

}

void Canvas::calcular_cores(Cena& cena) {

    // Raio para o ray casting.
    Raio raio;

    // Largura do campo de visão.
    double largura_fov = cena.get_camera().get_x_max() - cena.get_camera().get_x_min();
    // Altura do campo de visão.
    double altura_fov = cena.get_camera().get_y_max() - cena.get_camera().get_y_min();

    // Dimensões dos pixels no canvas.

    double D_x = largura_fov/this->get_largura(),
           D_y = altura_fov/this->get_altura();

    // Posição na janela do campo de visão por onde passará um raio.
    Ponto3 centro_pixel;

    hit_info interseccao;

    // Iterando no campo de visão.
    for (std::size_t l = 0; l < this->get_altura(); l++) {
        
        for (std::size_t c = 0; c < this->get_largura(); c++) {

            centro_pixel[0] = cena.get_camera().get_x_min() + D_x/2.0 + c*D_x;
            centro_pixel[1] = cena.get_camera().get_y_max() - D_y/2.0 - l*D_y;
            centro_pixel[2] = -cena.get_camera().get_distancia_focal();

            // Convertendo o ponto do centro do pixel para coordenadas de mundo.
            centro_pixel = cena.get_camera().get_matriz_camera_mundo() * centro_pixel;

            // Convertendo o raio em coordenadas de mundo.
            raio = Raio(cena.get_camera().get_posicao(), centro_pixel);

            // Lançando o raio e guardando informações da intersecção.
            interseccao = cena.retorna_interseccao(raio, this->get_cor_padrao());
            this->pixel_buffer[l][c] = interseccao.cor;
            this->matriz_solidos[l][c] = interseccao.solido;
            this->matriz_malhas[l][c] = interseccao.malha;

        }

    }

}

void Canvas::desenhar_pixels(SDL_Renderer* renderer) const {

    // Limpando o renderer antes de desenhar os pixels.
    SDL_RenderClear(renderer);

    for (std::size_t l = 0; l < this->get_altura(); l++) {
        
        for (std::size_t c = 0; c < this->get_largura(); c++) {

            // Definindo a cor que será pintada. Essa função segue o padrão RGBA, mas o canal alpha está sendo ignorado.
            SDL_SetRenderDrawColor(renderer, this->pixel_buffer[l][c][0], this->pixel_buffer[l][c][1], this->pixel_buffer[l][c][2], 255);

            // Pintando o pixel.
            SDL_RenderDrawPoint(renderer, c, l);

        }

    }
    
    // Atualizando a janela com o renderer pintado.
    SDL_RenderPresent(renderer);

}

int Canvas::loop(Cena& cena, const char* titulo) {

    // Ponteiro para uma janela do SDL.
    SDL_Window* window;
    // Ponteiro para um contexto de renderização.
    SDL_Renderer* renderer;

    this->repetir_loop = true;
    this->repetir_calculos = true;
    this->renderizar_pixel_buffer = true;

    // Inicializando o subsistema de vídeo e de input da SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        SDL_Log("Erro ao inicializar o SDL! SDL Error: %s", SDL_GetError());
        return EXIT_FAILURE;

    }

    // Criando uma janela.
    window = SDL_CreateWindow(
        titulo, // Título da janela.
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Posição inicial da janela (x, y).
        this->get_largura(), this->get_altura(), // Tamanho em pixels da janela (x, y).
        SDL_WINDOW_OPENGL // Flags
    );

     // Checando se a janela foi criada com sucesso.
    if (window == NULL) {

        SDL_Log("Erro ao criar a janela! SDL Error: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;

    }

    // Criando o contexto de renderização para a janela.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {

        SDL_Log("Erro ao criar o renderer! SDL Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;

    }

    // Loop principal onde é feita as interações com a janela.
    while (this->repetir_loop) {

        this->processar_eventos();

        // Calcula e desenha a matriz de cores apenas se for necessário para otimizar o programa.
        if (this->repetir_calculos) {

            // Chamando a função para calcular a matriz de cores que será pintada.
            this->calcular_cores(cena);

            this->renderizar_pixel_buffer = true;
            this->repetir_calculos = false;
        
        }

        if (this->renderizar_pixel_buffer) {

            // Chamando a função para pintar os pixels.
            this->desenhar_pixels(renderer);

            this->renderizar_pixel_buffer = false;

        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;

}

void Canvas::processar_eventos() {

    // Objeto do SDL para lidar com eventos.
    SDL_Event event;

    // Lidando com eventos.
    while (SDL_PollEvent(&event)) {
        
        // Para o loop principal se o botão de fechar da janela for clicado.
        if (event.type == SDL_QUIT) {

            this->repetir_loop = false;

        } else if (event.type == SDL_KEYUP) {

            // Para o loop principal se a tecla "Q" for pressionada.
            if (!strcmp(SDL_GetKeyName(event.key.keysym.sym), "Q")) {
                
                this->repetir_loop = false;

            // Força uma atualização da janela se a tecla "F5" for pressionada.
            } else if (!strcmp(SDL_GetKeyName(event.key.keysym.sym), "F5")) {
                
                this->repetir_calculos = true;
                std::cout << std::endl << "Recalculando cena..." << std::endl << std::endl;

            }

        } else if (event.type == SDL_WINDOWEVENT) {

            if (event.window.event == SDL_WINDOWEVENT_SHOWN) {

                // Força o pixel buffer a ser renderizado novamente quando a tela é maximizada para evitar o bug da tela transparente.
                this->renderizar_pixel_buffer = true;

            }

        } else if (event.type == SDL_MOUSEBUTTONUP) {

            if (event.button.button == SDL_BUTTON_LEFT) {

                if (this->matriz_malhas[event.button.y][event.button.x] != nullptr) {

                    this->menu_interativo(this->matriz_malhas[event.button.y][event.button.x]);

                }

            }

        }

    }

}

void Canvas::menu_interativo(std::shared_ptr<Malha> malha) {

    std::string nome_malha = malha->get_nome();
    int escolha_usuario;

    double x, y, z, angulo;
    EixoCanonico eixo, eixo1, eixo2;
    Ponto3 ponto_eixo, ponto_plano;
    Vetor3 direcao_eixo, vetor_normal_plano;

    std::system("clear");
    std::cout << "Você selecionou " << nome_malha << "!" << std::endl << std::endl;
    std::cout << "Deseja modificar malha?" << std::endl << std::endl;
    std::cout << "0 - Sim" << std::endl << "1 - Não" << std::endl << std::endl << "R: ";
    std::cin >> escolha_usuario;

    while (escolha_usuario != 0 && escolha_usuario != 1) {

        std::system("clear");

        std::cout << std::endl << "Opção inválida!" << std::endl << std::endl;
        std::cout << "Deseja modificar malha?" << std::endl << std::endl;
        std::cout << "0 - Sim" << std::endl << "1 - Não" << std::endl << std::endl << "R: ";
        std::cin >> escolha_usuario;

    }

    if (escolha_usuario == 0) {

        std::system("clear");

        std::cout << "O que deseja fazer?" << std::endl << std::endl;
        std::cout << "0 - Translação" << std::endl << "1 - Rotação em eixo canônico." << std::endl << "2 - Rotação em eixo arbitrário" << std::endl << "3 - Escala" << std::endl << "4 - Cisalhamento" << std::endl << "5 - Reflexão em plano canônico" << std::endl << "6 - Reflexão em plano arbitrário." << std::endl << "7 - Cancelar ação" << std::endl << std::endl << "R: ";
        std::cin >> escolha_usuario;

        while (escolha_usuario < 0 || escolha_usuario > 7) {

            std::system("clear");

            std::cout << std::endl << "Opção inválida!" << std::endl << std::endl;
            std::cout << "O que deseja fazer?" << std::endl << std::endl;
            std::cout << "0 - Translação" << std::endl << "1 - Rotação em eixo canônico." << std::endl << "2 - Rotação em eixo arbitrário" << std::endl << "3 - Escala" << std::endl << "4 - Cisalhamento" << std::endl << "5 - Reflexão em plano canônico" << std::endl << "6 - Reflexão em plano arbitrário." << std::endl << "7 - Cancelar ação" << std::endl << std::endl << "R: ";
            std::cin >> escolha_usuario;

        }

        std::system("clear");

        switch (escolha_usuario) {

            case 0:

                std::cout << std::endl  << std::endl << "Digite, separadas por espaço, as componentes x, y e z da translação: ";
                std::cin >> x >> y >> z;

                malha->transladar(x, y, z);

                std::system("clear");
                std::cout << std::endl << std::endl << "Translação aplicada! Não esqueça de pressionar F5 para recarregar o cenário." << std::endl;

                break;

            case 1:

                std::cout << std::endl << std::endl << "Digite, em radianos, o ângulo da rotação: ";
                std::cin >> angulo;

                std::cout << std::endl << std::endl << "Ao redor de qual eixo deseja rotacionar?" << std::endl << std::endl << "0 - x" << std::endl << "1 - y" << std::endl << "2 - z" << std::endl << std::endl << "R:";
                std::cin >> escolha_usuario;

                if (escolha_usuario == 0) eixo = EIXO_X;
                else if (escolha_usuario == 1) eixo = EIXO_Y;
                else eixo = EIXO_Z;

                malha->rotacionar(angulo, eixo);

                std::system("clear");
                std::cout << std::endl << std::endl << "Rotação aplicada! Não esqueça de pressionar F5 para recarregar o cenário." << std::endl;

                break;

            case 2:

                std::cout << std::endl << std::endl << "Digite, em radianos, o ângulo da rotação: ";
                std::cin >> angulo;

                std::cout << std::endl << std::endl << "Digite, separadas por espaço, as coordenadas de um ponto do eixo: ";
                std::cin >> ponto_eixo[0] >> ponto_eixo[1] >> ponto_eixo[2];

                std::cout << std::endl << std::endl << "Digite, separadas por espaço, as componentes do vetor direção do eixo: ";
                std::cin >> direcao_eixo[0] >> direcao_eixo[1] >> direcao_eixo[2];

                malha->rotacionar(angulo, ponto_eixo, direcao_eixo);

                std::system("clear");
                std::cout << std::endl << std::endl << "Rotação aplicada! Não esqueça de pressionar F5 para recarregar o cenário." << std::endl;

                break;

            case 3:

                std::cout << std::endl << std::endl << "Digite, separadas por espaço, os fatores x, y e z da escala: ";
                std::cin >> x >> y >> z;

                malha->escalar(x, y, z, malha->buscar_vertice_por_id(1));

                std::system("clear");
                std::cout << std::endl << std::endl << "Escala aplicada! Não esqueça de pressionar F5 para recarregar o cenário." << std::endl;

                break;

            case 4:

                std::cout << std::endl << std::endl << "Digite, em radianos, o ângulo do cisalhamento: ";
                std::cin >> angulo;

                std::cout << std::endl << std::endl << "Qual é o primeiro eixo do plano?" << std::endl << std::endl << "0 - x" << std::endl << "1 - y" << std::endl << "2 - z" << std::endl << std::endl << "R:";
                std::cin >> escolha_usuario;

                if (escolha_usuario == 0) eixo1 = EIXO_X;
                else if (escolha_usuario == 1) eixo1 = EIXO_Y;
                else eixo1 = EIXO_Z;

                std::cout << std::endl << std::endl << "Em qual eixo será cisalhado?" << std::endl << std::endl << "0 - x" << std::endl << "1 - y" << std::endl << "2 - z" << std::endl << std::endl << "R:";
                std::cin >> escolha_usuario;

                if (escolha_usuario == 0) eixo2 = EIXO_X;
                else if (escolha_usuario == 1) eixo2 = EIXO_Y;
                else eixo2 = EIXO_Z;

                malha->cisalhar(angulo, eixo1, eixo2, malha->buscar_vertice_por_id(1));

                std::system("clear");
                std::cout << std::endl << std::endl << "Cisalhamento aplicado! Não esqueça de pressionar F5 para recarregar o cenário." << std::endl;

                break;

            case 5:

                std::cout << std::endl << std::endl << "Qual é o primeiro eixo do plano?" << std::endl << std::endl << "0 - x" << std::endl << "1 - y" << std::endl << "2 - z" << std::endl << std::endl << "R:";
                std::cin >> escolha_usuario;

                if (escolha_usuario == 0) eixo1 = EIXO_X;
                else if (escolha_usuario == 1) eixo1 = EIXO_Y;
                else eixo1 = EIXO_Z;

                std::cout << std::endl << std::endl << "Qual é o segundo eixo do plano?" << std::endl << std::endl << "0 - x" << std::endl << "1 - y" << std::endl << "2 - z" << std::endl << std::endl << "R:";
                std::cin >> escolha_usuario;

                if (escolha_usuario == 0) eixo2 = EIXO_X;
                else if (escolha_usuario == 1) eixo2 = EIXO_Y;
                else eixo2 = EIXO_Z;

                malha->refletir(eixo1, eixo2);

                std::system("clear");
                std::cout << std::endl << std::endl << "Reflexão aplicada! Não esqueça de pressionar F5 para recarregar o cenário." << std::endl;

                break;

            case 6:

                std::cout << std::endl << std::endl << "Digite, separadas por espaço, as componentes do vetor normal ao plano: ";
                std::cin >> vetor_normal_plano[0] >> vetor_normal_plano[1] >> vetor_normal_plano[2];

                std::cout << std::endl << std::endl << "Digite, separadas por espaço, as coordenadas de um ponto do plano: ";
                std::cin >> ponto_plano[0] >> ponto_plano[1] >> ponto_plano[2];

                malha->refletir(vetor_normal_plano, ponto_plano);

                std::system("clear");
                std::cout << std::endl << std::endl << "Reflexão aplicada! Não esqueça de pressionar F5 para recarregar o cenário." << std::endl;

                break;

        }

    }

}