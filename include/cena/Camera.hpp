#ifndef CENA_CAMERA_HPP
#define CENA_CAMERA_HPP

#include "algebra/Ponto3.hpp"
#include "algebra/Vetor3.hpp"
#include "algebra/Matriz4.hpp"

class Camera {

    private:

        // Posição da câmera em coordenadas de mundo.
        Ponto3 posicao;
        // Ponto para o qual a câmera está olhando.
        Ponto3 ponto_visada;
        // Ponto que amarra o plano sargital da câmera.
        Ponto3 ponto_amarra;
        // Limites da janela do campo de visão em coordenadas de câmera.
        double x_max, x_min, y_max, y_min;
        // Distância focal atual da câmera.
        double distancia_focal;

        // Distância focal padrão da câmera.
        double distancia_focal_padrao;

        // Matriz para mudar coordenadas de câmera para mundo.
        Matriz4 matriz_camera_mundo;
        // Matriz para mudar coordenadas de mundo para câmera.
        Matriz4 matriz_mundo_camera;
        // Método privado para fazer o cálculo das matrizes de mudança de coordenadas.
        void recalcular_matrizes();

    public:

        // --- CONSTRUTORES ---

            Camera(Ponto3 posicao = Ponto3(0.0), Ponto3 ponto_visada = Ponto3(0.0, 0.0, 1.0), Ponto3 ponto_amarra = Ponto3(0.0, 1.0, 0.0), double x_max = 30.0, double x_min = -30.0, double y_max = 30.0, double y_min = -30.0, double distancia_focal = 30.0);

        // --- GETTERS E SETTERS ---

            Ponto3 get_posicao() const;
            void set_posicao(Ponto3 p, bool atualizar_matrizes = true);

            Ponto3 get_ponto_visada() const;
            void set_ponto_visada(Ponto3 pv, bool atualizar_matrizes = true);

            Ponto3 get_ponto_amarra() const;
            void set_ponto_amarra(Ponto3 pa, bool atualizar_matrizes = true);

            double get_x_max() const;
            void set_x_max(double x);

            double get_x_min() const;
            void set_x_min(double x);

            double get_y_max() const;
            void set_y_max(double y);

            double get_y_min() const;
            void set_y_min(double y);

            double get_distancia_focal() const;
            void set_distancia_focal(double d);

            Matriz4 const& get_matriz_camera_mundo() const;

            Matriz4 const& get_matriz_mundo_camera() const;

        // --- OUTROS MÉTODOS ---

            // Aumenta o zoom em n vezes.
            void aumentar_zoom(double fator);

            // Diminui o zoom em n vezes.
            void diminuir_zoom(double divisor);

};

#endif
