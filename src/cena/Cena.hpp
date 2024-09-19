#ifndef CENA_CENA_HPP
#define CENA_CENA_HPP

#include <memory>
#include <vector>
#include <tuple>
#include "geometria/Solido.hpp"
#include "geometria/Raio.hpp"
#include "geometria/malha/Malha.hpp"
#include "geometria/BoundingVolume.hpp"
#include "luz/LuzPontual.hpp"
#include "utils/tipos.hpp"
#include "luz/IntensidadeLuz.hpp"
#include "cena/Camera.hpp"

class Cena {

    private:
        // Vetor com ponteiros para os sólidos presentes na cena.
        std::vector<std::shared_ptr<Solido>> solidos;
        // Vetor com malhas presentes na cena.
        std::vector<std::shared_ptr<Malha>> malhas;
        // Vetor com bounding volumes presentes na cena.
        std::vector<std::shared_ptr<BoundingVolume>> bounding_volumes;
        // Luz pontual presente na cena.
        std::vector<std::unique_ptr<FonteLuz>> fontes_luz;
        // Intensidade da luz ambiente.
        IntensidadeLuz I_A;

        Camera camera;
        
    public:

        // --- CONSTRUTORES ---

        // I: intensidade da luz ambiente
        Cena(Camera camera = Camera(), IntensidadeLuz I_A = IntensidadeLuz(0.0));

        // --- GETTERS E SETTERS ---

        // Retorna uma referência para a câmera da cena.
        Camera& get_camera();
        void set_camera(Camera c);

        IntensidadeLuz get_I_A() const;
        void set_I_A(IntensidadeLuz i);

        // --- MÉTODOS ---

        // Insere um novo sólido na cena.
        void inserir_solido(std::shared_ptr<Solido> solido);

        // Insere uma malha na cena.
        void inserir_malha(std::shared_ptr<Malha> malha);

        // Insere um bounding volume na cena.
        void inserir_bounding_volume(std::shared_ptr<BoundingVolume> bounding_volume);

        // Insere uma nova fonte de luz na cena.
        void inserir_fonte_luz(std::unique_ptr<FonteLuz> luz);
        
        // Checa se o raio intersectou algum sólido ou malha, calcula a interação da luz com os sólidos e retorna as informações da intersecção (cor captada, ponteiro para o sólido intersectado e ponteiro para a malha intersectada).
        // Se o raio não intersectar nada, retorna a cor padrão e dois ponteiros nulos.
        hit_info retorna_interseccao(Raio& raio, rgb cor_padrao = rgb{0, 0, 0});

};

#endif