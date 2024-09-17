#ifndef CENA_CANVAS_HPP
#define CENA_CANVAS_HPP

#include "utils/tipos.hpp"
#include "cena/Cena.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class Canvas {

    private:
        // Altura do canvas
        std::size_t altura;
        // Largura do canvas
        std::size_t largura;
        // Cor padrão do canvas.
        rgb cor_padrao;

        // Matriz de cores para serem pintadas no canvas.
        matriz_cores pixel_buffer;
        // Matriz com ponteiros para sólidos intersectados em cada pixel.
        std::vector<std::vector<std::shared_ptr<Solido>>> matriz_solidos;
        // Matriz com ponteiros para malhas intersectadas em cada pixel.
        std::vector<std::vector<std::shared_ptr<Malha>>> matriz_malhas;

        // Booleano para controlar o loop principal.
        bool repetir_loop;
        // Booleano para controlar o cálculo do pixel buffer e das matrizes de sólidos e cores.
        bool repetir_calculos;
        // Booleano para controlar se o pixel buffer deve ser renderizado.
        bool renderizar_pixel_buffer;

        // --- MÉTODOS PRIVADOS ---

            // Calcula a matriz de cores de acordo com uma certa cena.
            void calcular_cores(Cena& cena);

            // Desenha 
            void desenhar_pixels(SDL_Renderer* renderer) const;

            // Lida com eventos da window.
            void processar_eventos();

            // Menu para alterar sólido.
            void menu_interativo(std::shared_ptr<Solido> solido);

            // Menu para alterar malha.
            void menu_interativo(std::shared_ptr<Malha> malha);

            // Reinicia as matrizes internas.
            void inicializar_matrizes();

    public:

        // --- CONSTRUTORES ---

            Canvas(std::size_t altura = 500, std::size_t largura = 500, rgb cor_padrao = rgb{0, 0, 0});

        // --- GETTERS E SETTERS ---

            std::size_t get_altura() const;
            void set_altura(double a, bool reiniciar_matrizes = true);

            std::size_t get_largura() const;
            void set_largura(double l, bool reiniciar_matrizes = true);

            rgb get_cor_padrao() const;
            void set_cor_padrao(rgb c);

        // --- MÉTODOS ---

            // Loop principal de renderização.
            // Recebe o título da janela.
            int loop(Cena& cena, const char* titulo);

};

#endif