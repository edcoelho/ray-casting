#ifndef GEOMETRIA_MALHA_RETANGULOXY_HPP
#define GEOMETRIA_MALHA_RETANGULOXY_HPP

#include "algebra/Ponto3.hpp"
#include "geometria/malha/MalhaEspecifica.hpp"

// Retângulo no plano XY.
class RetanguloXY : public MalhaEspecifica {

    private:
        Ponto3 centro_base;
        double largura;
        double altura;

        // Indica se o vetor normal aponta para o Z positivo.
        // Se for true, o vetor normal aponta para o Z positivo, e, em caso contrário, aponta para o Z negativo.
        bool sentido_vetor_normal;

        // Recalcula os atributos da malha específica.
        void recalcular_atributos() override;

    protected:

        // --- MÉTODOS PROTEGIDOS ---

            // Calcula e inseres os vértices, arestas e faces da malha específica.
            void calcular_estrutura() override;

    public:

        // --- CONSTRUTORES ---

            RetanguloXY(Ponto3 centro_base = Ponto3(), double largura = 30.0, double altura = 60.0, Material material = Material(), bool sentido_vetor_normal = true);

        // --- GETTERS E SETTERS ---

            Ponto3 get_centro_base();

            double get_largura();

            double get_altura();

            bool get_sentido_vetor_normal();
            
        // --- OUTROS MÉTODOS ---

            // Aplica uma matriz de transformação qualquer.
            void transformar(Matriz4 const& matriz) override;

            // Aplica uma translação.
            void transladar(double x, double y, double z) override;

            // Aplica uma rotação em um dos eixos canônicos.
            void rotacionar(double angulo, EixoCanonico eixo) override;
            // Aplica uma rotação em um eixo arbitrário.
            void rotacionar(double angulo, Ponto3 ponto_eixo, Vetor3 direcao_eixo) override;

            // Aplica uma escala.
            void escalar(double fator_x, double fator_y, double fator_z, Ponto3 ponto_amarra = Ponto3(0.0)) override;

            // Aplica um cisalhamento num plano formado pelo eixo1 e eixo2 cisalhando no eixo2.
            void cisalhar(double angulo, EixoCanonico eixo1, EixoCanonico eixo2, Ponto3 ponto_amarra = Ponto3(0.0)) override;

            // Aplica uma reflexão em um plano formado por dois dos eixos canônicos.
            void refletir(EixoCanonico eixo1, EixoCanonico eixo2) override;
            // Aplica uma reflexão em um plano arbitrário.
            void refletir(Vetor3 vetor_normal_plano, Ponto3 ponto_plano = Ponto3(0.0)) override;

};

#endif