#include "cena/Cena.hpp"
#include <cmath>

Cena::Cena(Camera camera, IntensidadeLuz I_A) {

    this->set_I_A(I_A);
    this->set_camera(camera);

}

IntensidadeLuz Cena::get_I_A() const {

    return this->I_A;

}
void Cena::set_I_A(IntensidadeLuz i) {

    this->I_A = i;

}

Camera& Cena::get_camera() {

    return this->camera;

}
void Cena::set_camera(Camera c) {

    this->camera = c;

}

void Cena::inserir_solido(std::shared_ptr<Solido> solido) {

    this->solidos.push_back(std::move(solido));

}

void Cena::inserir_malha(std::shared_ptr<Malha> malha) {

    this->malhas.push_back(std::move(malha));

}

void Cena::inserir_bounding_volume(std::shared_ptr<BoundingVolume> bounding_volume) {

    this->bounding_volumes.push_back(std::move(bounding_volume));

}

void Cena::inserir_fonte_luz(std::unique_ptr<FonteLuz> luz) {

    this->fontes_luz.push_back(std::move(luz));

}

hit_info Cena::retorna_interseccao(Raio& raio, rgb cor_padrao) {

    // Índice do sólido intersectado primeiro pelo raio da câmera.
    std::size_t indice_solido;
    // Distância da câmera ao ponto de intersecção com um sólido.
    double t_int;
    // Distância da câmera ao ponto de intersecção mais próximo com o sólido mais próximo.
    double min_t_int = INFINITY;
    // Indica se um raio intersectou um sólido.
    bool raio_intersectou = false;
    // Indica se um raio intersectou uma malha primeiro.
    bool raio_intersectou_malha = false;
    // Indica se um raio intersectou um bounding_volume primeiro.
    bool raio_intersectou_bounding_volume = false;

    // Indica se um raio de luz foi obstruído por um outro sólido.
    bool raio_luz_obstruido = false;
    // Distância do ponto de intersecção até o ponto que obstrui um raio de luz.
    double t_int_obstrucao;

    // Variável para auxiliar nos cálculos.
    double aux;
    // Expoente de espelhamento de um sólido.
    double espelhamento;

    // Vetores para auxiliar nos cálculos.

    Vetor3 l, n, v, r;

    // Ponto da intersecção.
    Ponto3 p_int;

    // Ponteiro para o raio da fonte de luz pontual.
    Raio raio_luz;

    // Intensidades ambiente, difusa e especular da energia luminosa que vem do ponto intersectado.
    IntensidadeLuz I_A(0.0, 0.0, 0.0), I_D(0.0, 0.0, 0.0), I_E(0.0, 0.0, 0.0);
    // Intensidade final da energia luminosa que vai para a câmera.
    IntensidadeLuz I_final(0.0, 0.0, 0.0);

    // K ambiente do sólido, K difusão do sólido e K especulamento do sólido.
    IntensidadeLuz k_A, k_D, k_E;
    // K do pixel da textura correspondente ao ponto intersectado.
    IntensidadeLuz k_textura;

    // Índice da malha intersectada primeiro pelo raio da câmera.
    std::size_t indice_malha;
    // Ponteiro para cópia do solido intersectado.
    std::shared_ptr<Solido> solido(nullptr);

    // Informações da intersecção retornada.
    hit_info interseccao = {I_final.cor_rgb(), nullptr, nullptr, nullptr, 0.0, false};
    // Informações da intersecção que está em análise.
    hit_info interseccao_atual;

    // -------------------------------------------------------------
    // --- CÁLCULO DA INTERSECÇÃO MAIS PRÓXIMA DO RAIO DA CÂMERA ---
    // -------------------------------------------------------------

    // Checar intersecção de sólidos apenas se houver algum na cena.
    if (this->solidos.size() >= 1) {

        // Checando os sólidos.
        for (std::size_t i = 0; i < this->solidos.size(); i++) {

            t_int = this->solidos.at(i)->escalar_interseccao(raio);

            if (t_int >= 0.0 && t_int < min_t_int) {

                min_t_int = t_int;
                indice_solido = i;
                raio_intersectou = true;
                raio_intersectou_malha = false;

            }

        }

    }

    // Checar intersecção de malhas apenas se houver alguma na cena.
    if (this->malhas.size() >= 1) {

        // Checando malhas.
        for (std::size_t i = 0; i < this->malhas.size(); i++) {

            t_int = this->malhas.at(i)->escalar_interseccao(raio);

            if (t_int >= 0.0 && t_int < min_t_int) {

                min_t_int = t_int;
                indice_malha = i;
                raio_intersectou = true;
                raio_intersectou_malha = true;

            }

        }

    }

    // Checando bounding volumes.
    for (std::size_t i = 0; i < this->bounding_volumes.size(); i++) {

        interseccao_atual = this->bounding_volumes.at(i)->calcula_interseccao(raio);
        t_int = interseccao_atual.distancia;

        if (t_int >= 0.0 && t_int < min_t_int) {

            min_t_int = t_int;
            raio_intersectou = true;
            raio_intersectou_malha = false;
            raio_intersectou_bounding_volume = true;
            interseccao = interseccao_atual;

        }

    }

    // -----------------------------
    // --- CÁLCULO DA ILUMINAÇÃO ---
    // -----------------------------

    // Antes de calcular as intensidades de luz, checa se houve alguma intersecção e se tem um ponto de luz presente na cena.
    if (raio_intersectou) {

        // Conseguindo o ponto de intersecção.
        p_int = raio.ponto_do_raio(min_t_int);

        if (raio_intersectou_malha) {

            // Conseguindo o triângulo intersectado.
            solido = std::make_shared<Triangulo>(this->malhas.at(indice_malha)->get_ultima_face_intersectada());

            // Checando se a malha tem textura.
            if (solido->tem_textura()) {

                // Conseguindo a cor do pixel da textura correspondente ao ponto intersectado.
                k_textura = this->malhas.at(indice_malha)->cor_textura(p_int);

            }

        } else if (raio_intersectou_bounding_volume) {

            solido = interseccao.solido;
            // Checando se o sólido tem textura.
            if (solido->tem_textura()) {

                // Conseguindo a cor do pixel da textura correspondente ao ponto intersectado.
                k_textura = solido->cor_textura(p_int);

            }

        } else {

            solido = this->solidos.at(indice_solido);

            // Checando se o sólido tem textura.
            if (solido->tem_textura()) {

                // Conseguindo a cor do pixel da textura correspondente ao ponto intersectado.
                k_textura = solido->cor_textura(p_int);

            }

        }

        if (this->fontes_luz.size() > 0) {

            // Itera sobre as fontes de luz.
            for (std::size_t i = 0; i < this->fontes_luz.size(); i++) {

                if (this->fontes_luz.at(i)->ponto_valido(p_int)) {

                    // Instanciando o raio da fonte de luz.
                    raio_luz = Raio(p_int, this->fontes_luz.at(i)->direcao_ponto_luz(p_int));

                    raio_luz_obstruido = false;
                    std::size_t indice = 0;

                    // Checando se o raio de luz intersectou algum dos sólidos antes.
                    while (!raio_luz_obstruido && indice < this->solidos.size()) {

                        t_int_obstrucao = this->solidos.at(indice)->escalar_interseccao(raio_luz);

                        if (this->fontes_luz.at(i)->distancia_ponto_luz(p_int) >= t_int_obstrucao && t_int_obstrucao >= 1e-12) {

                            raio_luz_obstruido = true;

                        }

                        indice++;

                    }

                    indice = 0;

                    // Checando se o raio de luz intersectou alguma das malhas antes.
                    while (!raio_luz_obstruido && indice < this->malhas.size()) {

                        t_int_obstrucao = this->malhas.at(indice)->escalar_interseccao(raio_luz);

                        if (this->fontes_luz.at(i)->distancia_ponto_luz(p_int) >= t_int_obstrucao && t_int_obstrucao >= 1e-12) {

                            raio_luz_obstruido = true;

                        }

                        indice++;

                    }

                    // Checando se o raio da fonte de luz não intersecta nenhum outro objeto, o que bloquearia a chegada da luz no ponto de intersecção.
                    if (!raio_luz_obstruido) {

                        // Se o sólido tiver textura, atribui o k da textura ao k difuso. Em caso contrário, pega o k difuso do material do objeto.
                        k_D = solido->tem_textura() ? k_textura : solido->get_material().get_k_D();

                        // Vetor que vai do ponto de intersecção até a posição da fonte de luz normalizado.
                        l = this->fontes_luz.at(i)->direcao_ponto_luz(p_int);

                        // Vetor normal ao sólido no ponto de intersecção.
                        n = solido->vetor_normal_ponto(p_int);

                        // I_D = I @ Kd
                        I_D = this->fontes_luz.at(i)->get_intensidade() * k_D;

                        // aux = (l . n)
                        aux = l.escalar(n);

                        // Se o produto escalar for negativo, ou seja, se o ângulo entre l e n está no intervalo (90º, 270º), então a intensidade difusa é zerada.
                        aux = aux < 0.0 ? 0.0 : aux;
                        
                        // I_D = I_D * (l . n)
                        I_D = I_D * aux;

                        // Conseguindo o K especular do sólido.
                        k_E = solido->get_material().get_k_E();

                        // I_E = I @ Ke
                        I_E = this->fontes_luz.at(i)->get_intensidade() * k_E;

                        // Vetor que sai do sólido e vai em direção ao olho do câmera.
                        v = (raio.get_ponto_inicial() - p_int).unitario();
                        // Vetor "reflexo" da luz no sólido.
                        r = l.reflexo(n);

                        // aux = v . r
                        aux = v.escalar(r);

                        // Se o produto escalar for negativo, ou seja, se o ângulo entre v e r está no intervalo (90º, 270º), então a intensidade especular é zerada.
                        aux = aux < 0.0 ? 0.0 : aux;

                        espelhamento = solido->get_material().get_espelhamento();

                        // I_E = I_E * (v . r)^espelhamento
                        I_E = I_E * std::pow(aux, espelhamento);

                        // Aplicando as componentes difusa e especular atenuadas na intensidade final que vai para a câmera.
                        I_final = I_final + (I_D + I_E) * this->fontes_luz.at(i)->fator_atenuacao(p_int);

                    }

                }

            }

        }

        // Se o sólido tiver textura, atribui o k da textura ao k ambiente. Em caso contrário, pega o k ambiente do material do objeto.
        k_A = solido->tem_textura() ? k_textura : solido->get_material().get_k_A();

        // I_A = Ia @ k_A
        // Ia: intensidade da luz ambiente.
        I_A = this->get_I_A() * k_A;

        I_final = I_final + I_A;

        interseccao.cor = I_final.cor_rgb();
        if (raio_intersectou_malha) {

            interseccao.malha = this->malhas.at(indice_malha);

        } else {

            interseccao.solido = solido;

        }

    } else {

        interseccao.cor = cor_padrao;

        return interseccao;

    }

    return interseccao;

}