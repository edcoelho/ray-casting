#include "include_list.hpp"

using json = nlohmann::json;

// Função para preparar o canvas de acordo com um arquivo JSON.
void preparar_canvas(Canvas& canvas, std::string nome_arquivo_JSON);

// Função para preparar uma cena descrita em um arquivo JSON.
void preparar_cena(Cena& cena, std::string nome_arquivo_JSON);

// Função para carregar para uma cena uma malha descrita em um arquivo JSON.
std::shared_ptr<Malha> carregar_malha(std::string nome_arquivo_JSON);

int main(int argc, char* argv[]) {

    Canvas canvas(0, 0);
    Cena cena;

    preparar_canvas(canvas, "canvas.json");

    if (argc > 1) {

        preparar_cena(cena, argv[1]);

    }

    // Entrando no loop principal do renderizador.
    return canvas.loop(cena, "CG I");

}

void preparar_canvas(Canvas& canvas, std::string nome_arquivo_JSON) {

    // Cor de background do canvas.
    rgb cor_bg{0, 0, 0};

    // Abrindo conexão com o arquivo.
    std::ifstream arquivo_JSON(nome_arquivo_JSON);

    // Checando se o arquivo abriu com sucesso.
    if (!arquivo_JSON.is_open()) {

        throw std::runtime_error("Erro: Não foi possível carregar as informações do canvas!");

    }

    // Coletando informações do arquivo JSON.
    json dados = json::parse(arquivo_JSON);

    // Fechando conexão com o arquivo.
    arquivo_JSON.close();

    if (dados["largura"].empty()) {

        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Largura do canvas não especificado!"));

    }

    if (dados["altura"].empty()) {

        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Altura do canvas não especificado!"));

    }

    if (!dados["cor_background"].empty()) {

        for (int k = 0; k < 3; k++)
            cor_bg[k] = dados["cor_background"][k];

    }

    canvas.set_largura(dados["largura"], false);
    canvas.set_altura(dados["altura"], true);
    canvas.set_cor_padrao(cor_bg);

}

void preparar_cena(Cena& cena, std::string nome_arquivo_JSON) {

    std::vector<std::shared_ptr<Textura>> texturas;
    std::vector<Material> materiais;

    // Abrindo conexão com o arquivo.
    std::ifstream arquivo_JSON(nome_arquivo_JSON);

    // Checando se o arquivo abriu com sucesso.
    if (!arquivo_JSON.is_open()) {

        throw std::runtime_error(std::string("Erro: Não foi possível carregar as informações da cena \"") + nome_arquivo_JSON + "\"");

    }

    // Coletando informações do arquivo JSON.
    json dados = json::parse(arquivo_JSON);

    // Fechando conexão com o arquivo.
    arquivo_JSON.close();

    // --- INÍCIO DA CONFIGURAÇÃO DA CÂMERA ---

    if (!dados["camera"].empty()) {

        // Posição da câmera
        if (!dados["camera"]["posicao"].empty()) {

            cena.get_camera().set_posicao(Ponto3(dados["camera"]["posicao"][0], dados["camera"]["posicao"][1], dados["camera"]["posicao"][2]), false);

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Posição da câmera não especificada."));

        }

        // Ponto de visada (LookAt)
        if (!dados["camera"]["lookat"].empty()) {

            cena.get_camera().set_ponto_visada(Ponto3(dados["camera"]["lookat"][0], dados["camera"]["lookat"][1], dados["camera"]["lookat"][2]), false);

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": LookAt da câmera não especificado."));

        }

        // Ponto de amarra do plano sargital (ViewUp)
        if (!dados["camera"]["viewup"].empty()) {

            cena.get_camera().set_ponto_amarra(Ponto3(dados["camera"]["viewup"][0], dados["camera"]["viewup"][1], dados["camera"]["viewup"][2]));

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": ViewUp da câmera não especificado."));

        }

        // X máximo
        if (!dados["camera"]["xmax"].empty()) {

            cena.get_camera().set_x_max(dados["camera"]["xmax"]);

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": X máximo da câmera não especificado."));

        }

        // X minimo
        if (!dados["camera"]["xmin"].empty()) {

            cena.get_camera().set_x_min(dados["camera"]["xmin"]);

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": X mínimo da câmera não especificado."));

        }

        // Y máximo
        if (!dados["camera"]["ymax"].empty()) {

            cena.get_camera().set_y_max(dados["camera"]["ymax"]);

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Y máximo da câmera não especificado."));

        }

        // Y minimo
        if (!dados["camera"]["ymin"].empty()) {

            cena.get_camera().set_y_min(dados["camera"]["ymin"]);

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Y mínimo da câmera não especificado."));

        }

        // Distância focal
        if (!dados["camera"]["distancia_focal"].empty()) {

            cena.get_camera().set_distancia_focal(dados["camera"]["distancia_focal"]);

        } else {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Distância focal da câmera não especificada."));

        }

    } else {

        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Dados da câmera não especificados."));

    }

    // --- FIM DA CONFIGURAÇÃO DA CÂMERA ---

    // --- INÍCIO DO CARREGAMENTO DE TEXTURAS ---

    // Cheacando se há alguma textura especificada no arquivo.
    if (!dados["texturas"].empty()) {

        // Iterando sobre as texturas especificadas para carregá-las.
        for (std::size_t i = 0; i < dados["texturas"].size(); i++) {

            if (dados["texturas"][i]["nome"].empty()) {

                throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O nome da " + std::to_string(i+1) + "ª textura carregada não foi especificado."));

            }

            if (dados["texturas"][i]["arquivo"].empty()) {

                throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O arquivo de imagem da " + std::to_string(i+1) + "ª textura carregada não foi especificado."));

            }

            // Carregando a textura.
            texturas.push_back(std::make_shared<Textura>(dados["texturas"][i]["nome"], dados["texturas"][i]["arquivo"]));

        }

    }

    // --- FIM DO CARREGAMENTO DE TEXTURAS ---

    // --- INÍCIO DA CRIAÇÃO DOS MATERIAIS ---

    if (!dados["materiais"].empty()) {

        // Iterando sobre as texturas especificadas para carregá-las.
        for (std::size_t i = 0; i < dados["materiais"].size(); i++) {

            if (dados["materiais"][i]["nome"].empty()) {

                throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O nome do " + std::to_string(i+1) + "º material criado não foi especificado."));

            }

            // Checando se o material é uma textura.
            if (dados["materiais"][i]["textura"].empty()){

                if (dados["materiais"][i]["k_ambiente"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A componente k_ambiente do " + std::to_string(i+1) + "º material criado não foi especificado."));

                }

                if (dados["materiais"][i]["k_difusa"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A componente k_difusa do " + std::to_string(i+1) + "º material criado não foi especificado."));

                }

                if (dados["materiais"][i]["k_especular"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A componente k_especular do " + std::to_string(i+1) + "º material criado não foi especificado."));

                }

                if (dados["materiais"][i]["exp_especularidade"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O expoente de especularidade do " + std::to_string(i+1) + "º material criado não foi especificado."));

                }

                // Criando material
                materiais.push_back(Material(dados["materiais"][i]["nome"]));

                materiais[i].set_k_A(IntensidadeLuz(dados["materiais"][i]["k_ambiente"][0], dados["materiais"][i]["k_ambiente"][1], dados["materiais"][i]["k_ambiente"][2]));

                materiais[i].set_k_D(IntensidadeLuz(dados["materiais"][i]["k_difusa"][0], dados["materiais"][i]["k_difusa"][1], dados["materiais"][i]["k_difusa"][2]));

                materiais[i].set_k_E(IntensidadeLuz(dados["materiais"][i]["k_especular"][0], dados["materiais"][i]["k_especular"][1], dados["materiais"][i]["k_especular"][2]));

                materiais[i].set_espelhamento(dados["materiais"][i]["exp_especularidade"]);

            } else {

                if (texturas.size() > 0) {

                    std::size_t j = 0;
                    while (j < texturas.size() && texturas[j]->get_nome() != dados["materiais"][i]["textura"])
                        j++;

                    if (j < texturas.size()) {

                        // Criano material.
                        materiais.push_back(Material(dados["materiais"][i]["nome"], texturas[j]));

                    } else {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A textura do " + std::to_string(i+1) + "º material criado não foi carregada."));

                    }

                } else {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O " + std::to_string(i+1) + "º material especifica uma textura, porém nenhuma textura foi carregada."));

                }

            }

        }

    }

    // --- FIM DA CRIAÇÃO DOS MATERIAIS ---

    // --- INÍCIO DA INSERÇÃO DE LUZES NA CENA ---

    if (!dados["luzes"].empty()) {

        // Checando se luz ambiente está especificada.
        if (!dados["luzes"]["ambiente"].empty()) {

            // Configurando luz ambiente.
            cena.set_I_A(IntensidadeLuz(dados["luzes"]["ambiente"][0], dados["luzes"]["ambiente"][1], dados["luzes"]["ambiente"][2]));

        }

        // Checando se tem luzes pontuais especificadas.
        if (!dados["luzes"]["pontuais"].empty()) {

            // Iterando sobre as luzes pontuais especificadas.
            for (std::size_t i = 0; i < dados["luzes"]["pontuais"].size(); i++) {

                if (dados["luzes"]["pontuais"][i]["posicao"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A posição da " + std::to_string(i+1) + "ª luz pontual criada não foi especificada."));

                }

                if (dados["luzes"]["pontuais"][i]["intensidade"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A intensidade da " + std::to_string(i+1) + "ª luz pontual criada não foi especificada."));

                }

                // Criando luz pontual.
                std::unique_ptr<LuzPontual> luz = std::make_unique<LuzPontual>();

                luz->set_posicao(Ponto3(dados["luzes"]["pontuais"][i]["posicao"][0], dados["luzes"]["pontuais"][i]["posicao"][1], dados["luzes"]["pontuais"][i]["posicao"][2]));

                luz->set_intensidade(IntensidadeLuz(dados["luzes"]["pontuais"][i]["intensidade"][0], dados["luzes"]["pontuais"][i]["intensidade"][1], dados["luzes"]["pontuais"][i]["intensidade"][2]));

                if (!dados["luzes"]["pontuais"][i]["atenuacao_a"].empty()) {

                    luz->set_atenuacao_a(dados["luzes"]["pontuais"][i]["atenuacao_a"]);

                }

                if (!dados["luzes"]["pontuais"][i]["atenuacao_b"].empty()) {

                    luz->set_atenuacao_b(dados["luzes"]["pontuais"][i]["atenuacao_b"]);

                }

                if (!dados["luzes"]["pontuais"][i]["atenuacao_c"].empty()) {

                    luz->set_atenuacao_c(dados["luzes"]["pontuais"][i]["atenuacao_c"]);

                }

                // Inserindo a luz pontual.
                cena.inserir_fonte_luz(std::move(luz));

            }

        }

        // Checando se tem luzes spots especificadas.
        if (!dados["luzes"]["spots"].empty()) {

            // Iterando sobre as luzes spots especificadas.
            for (std::size_t i = 0; i < dados["luzes"]["spots"].size(); i++) {

                if (dados["luzes"]["spots"][i]["direcao"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A direção da " + std::to_string(i+1) + "ª luz spot criada não foi especificada."));

                }

                if (dados["luzes"]["spots"][i]["abertura"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O ângulo de abertura da " + std::to_string(i+1) + "ª luz spot criada não foi especificado."));

                }

                if (dados["luzes"]["spots"][i]["posicao"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A posição da " + std::to_string(i+1) + "ª luz spot criada não foi especificada."));

                }

                if (dados["luzes"]["spots"][i]["intensidade"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A intensidade da " + std::to_string(i+1) + "ª luz spot criada não foi especificada."));

                }

                // Criando luz spot.
                std::unique_ptr<LuzSpot> luz = std::make_unique<LuzSpot>();

                luz->set_direcao(Vetor3(dados["luzes"]["spots"][i]["direcao"][0], dados["luzes"]["spots"][i]["direcao"][1], dados["luzes"]["spots"][i]["direcao"][2]));

                luz->set_abertura(dados["luzes"]["spots"][i]["abertura"]);

                luz->set_posicao(Ponto3(dados["luzes"]["spots"][i]["posicao"][0], dados["luzes"]["spots"][i]["posicao"][1], dados["luzes"]["spots"][i]["posicao"][2]));

                luz->set_intensidade(IntensidadeLuz(dados["luzes"]["spots"][i]["intensidade"][0], dados["luzes"]["spots"][i]["intensidade"][1], dados["luzes"]["spots"][i]["intensidade"][2]));

                if (!dados["luzes"]["spots"][i]["atenuacao_a"].empty()) {

                    luz->set_atenuacao_a(dados["luzes"]["spots"][i]["atenuacao_a"]);

                }

                if (!dados["luzes"]["spots"][i]["atenuacao_b"].empty()) {

                    luz->set_atenuacao_b(dados["luzes"]["spots"][i]["atenuacao_b"]);

                }

                if (!dados["luzes"]["spots"][i]["atenuacao_c"].empty()) {

                    luz->set_atenuacao_c(dados["luzes"]["spots"][i]["atenuacao_c"]);

                }

                // Inserindo a luz spot.
                cena.inserir_fonte_luz(std::move(luz));

            }

        }

        // Checando se tem luzes direcionais especificadas.
        if (!dados["luzes"]["direcionais"].empty()) {

            // Iterando sobre as luzes spots especificadas.
            for (std::size_t i = 0; i < dados["luzes"]["direcionais"].size(); i++) {

                if (dados["luzes"]["direcionais"][i]["direcao"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A direção da " + std::to_string(i+1) + "ª luz direcional criada não foi especificada."));

                }

                if (dados["luzes"]["direcionais"][i]["intensidade"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A intensidade da " + std::to_string(i+1) + "ª luz direcional criada não foi especificada."));

                }

                // Criando a luz direcional.
                std::unique_ptr<LuzDirecional> luz = std::make_unique<LuzDirecional>();

                luz->set_direcao(Vetor3(dados["luzes"]["direcionais"][i]["direcao"][0], dados["luzes"]["direcionais"][i]["direcao"][1], dados["luzes"]["direcionais"][i]["direcao"][2]));
                
                luz->set_intensidade(IntensidadeLuz(dados["luzes"]["direcionais"][i]["intensidade"][0], dados["luzes"]["direcionais"][i]["intensidade"][1], dados["luzes"]["direcionais"][i]["intensidade"][2]));

                // Inserindo a luz direcional.
                cena.inserir_fonte_luz(std::move(luz));

            }

        }

    }

    // --- FIM DA INSERÇÃO DE LUZES NA CENA ---

    // --- INÍCIO DA INSERÇÃO DE SÓLIDOS NA CENA --- 

    if (!dados["solidos"].empty()) {

        if (materiais.size() == 0) {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Não foi especificado nenhum material, portanto não há como inserir sólidos."));

        }

        // Checando se tem algum plano especificado.
        if (!dados["solidos"]["planos"].empty()) {

            for (std::size_t i = 0; i < dados["solidos"]["planos"].size(); i++) {

                if (dados["solidos"]["planos"][i]["ponto_conhecido"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O ponto conhecido do " + std::to_string(i+1) + "º plano criado não foi especificado."));

                }

                if (dados["solidos"]["planos"][i]["vetor_normal"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O vetor normal do " + std::to_string(i+1) + "º plano criado não foi especificado."));

                }

                if (dados["solidos"]["planos"][i]["material"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º plano criado não foi especificado."));

                }

                // Criando o plano.
                std::shared_ptr<Plano> plano = std::make_shared<Plano>();

                plano->set_ponto(Ponto3(dados["solidos"]["planos"][i]["ponto_conhecido"][0], dados["solidos"]["planos"][i]["ponto_conhecido"][1], dados["solidos"]["planos"][i]["ponto_conhecido"][2]));

                plano->set_normal(Vetor3(dados["solidos"]["planos"][i]["vetor_normal"][0], dados["solidos"]["planos"][i]["vetor_normal"][1], dados["solidos"]["planos"][i]["vetor_normal"][2]));

                std::size_t j = 0;

                // Percorrendo o vetor de materiais para ver se o material utilizado pelo plano existe.
                while (j < materiais.size() && materiais[j].get_nome() != dados["solidos"]["planos"][i]["material"])
                    j++;

                if (j < materiais.size()) {

                    plano->set_material(materiais[j]);

                } else {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º plano criado não existe."));

                }

                // Inserindo o plano na cena.
                cena.inserir_solido(std::move(plano));

            }

        }

        // Checando se tem alguma esfera especificada.
        if (!dados["solidos"]["esferas"].empty()) {

            // Iterando sobre as esferas especificadas para inserí-las na cena.
            for (std::size_t i = 0; i < dados["solidos"]["esferas"].size(); i++) {

                if (dados["solidos"]["esferas"][i]["centro"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A posição do centro da " + std::to_string(i+1) + "ª esfera criada não foi especificada."));

                }

                if (dados["solidos"]["esferas"][i]["raio"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O raio da " + std::to_string(i+1) + "ª esfera criada não foi especificado."));

                }

                if (dados["solidos"]["esferas"][i]["material"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material da " + std::to_string(i+1) + "ª esfera criada não foi especificado."));

                }

                // Criando a esfera.
                std::shared_ptr<Esfera> esfera = std::make_shared<Esfera>();

                esfera->set_centro(Ponto3(dados["solidos"]["esferas"][i]["centro"][0], dados["solidos"]["esferas"][i]["centro"][1], dados["solidos"]["esferas"][i]["centro"][2]));

                esfera->set_raio(dados["solidos"]["esferas"][i]["raio"]);
                
                std::size_t j = 0;

                // Percorrendo o vetor de materiais para ver se o material utilizado pela esfera existe.
                while (j < materiais.size() && materiais[j].get_nome() != dados["solidos"]["esferas"][i]["material"])
                    j++;

                if (j < materiais.size()) {

                    esfera->set_material(materiais[j]);

                } else {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material da " + std::to_string(i+1) + "ª esfera criada não existe."));

                }

                // Inserindo a esfera na cena.
                cena.inserir_solido(std::move(esfera));

            }

        }

        // Checando se tem algum cilíndro especificado.
        if (!dados["solidos"]["cilindros"].empty()) {

            // Iterando sobre os cilíndros especificados para inserí-los na cena.
            for (std::size_t i = 0; i < dados["solidos"]["cilindros"].size(); i++) {

                if (dados["solidos"]["cilindros"][i]["centro_base"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A posição do centro da base do " + std::to_string(i+1) + "º cilíndro criado não foi especificada."));

                }

                if (dados["solidos"]["cilindros"][i]["direcao"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A direção do " + std::to_string(i+1) + "º cilíndro criado não foi especificada."));

                }

                if (dados["solidos"]["cilindros"][i]["raio"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O raio do " + std::to_string(i+1) + "º cilíndro criado não foi especificado."));

                }

                if (dados["solidos"]["cilindros"][i]["altura"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A altura do " + std::to_string(i+1) + "º cilíndro criado não foi especificada."));

                }

                if (dados["solidos"]["cilindros"][i]["material"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º cilíndro criado não foi especificado."));

                }

                // Criando o cilíndro.
                std::shared_ptr<Cilindro> cilindro = std::make_shared<Cilindro>();

                cilindro->set_centro_base(Ponto3(dados["solidos"]["cilindros"][i]["centro_base"][0], dados["solidos"]["cilindros"][i]["centro_base"][1], dados["solidos"]["cilindros"][i]["centro_base"][2]), false);

                cilindro->set_direcao(Vetor3(dados["solidos"]["cilindros"][i]["direcao"][0], dados["solidos"]["cilindros"][i]["direcao"][1], dados["solidos"]["cilindros"][i]["direcao"][2]), false);

                cilindro->set_raio(dados["solidos"]["cilindros"][i]["raio"]);

                cilindro->set_altura(dados["solidos"]["cilindros"][i]["altura"]);

                std::size_t j = 0;

                // Percorrendo o vetor de materiais para ver se o material utilizado pelo cilíndro existe.
                while (j < materiais.size() && materiais[j].get_nome() != dados["solidos"]["cilindros"][i]["material"])
                    j++;

                if (j < materiais.size()) {

                    cilindro->set_material(materiais[j]);

                } else {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º cilíndro criado não existe."));

                }

                // Inserindo o cilíndro na cena.
                cena.inserir_solido(std::move(cilindro));

            }

        }

        // Checando se tem algum cone especificado.
        if (!dados["solidos"]["cones"].empty()) {

            // Iterando sobre os cones especificados para inserí-los na cena.
            for (std::size_t i = 0; i < dados["solidos"]["cones"].size(); i++) {

                if (dados["solidos"]["cones"][i]["centro_base"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A posição do centro da base do " + std::to_string(i+1) + "º cone criado não foi especificada."));

                }

                if (dados["solidos"]["cones"][i]["direcao"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A direção do " + std::to_string(i+1) + "º cone criado não foi especificada."));

                }

                if (dados["solidos"]["cones"][i]["raio"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O raio do " + std::to_string(i+1) + "º cone criado não foi especificado."));

                }

                if (dados["solidos"]["cones"][i]["altura"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A altura do " + std::to_string(i+1) + "º cone criado não foi especificada."));

                }

                if (dados["solidos"]["cones"][i]["material"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º cone criado não foi especificado."));

                }

                // Criando o cone.
                std::shared_ptr<Cone> cone = std::make_shared<Cone>();

                cone->set_centro_base(Ponto3(dados["solidos"]["cones"][i]["centro_base"][0], dados["solidos"]["cones"][i]["centro_base"][1], dados["solidos"]["cones"][i]["centro_base"][2]), false);

                cone->set_direcao(Vetor3(dados["solidos"]["cones"][i]["direcao"][0], dados["solidos"]["cones"][i]["direcao"][1], dados["solidos"]["cones"][i]["direcao"][2]), false);

                cone->set_raio_base(dados["solidos"]["cones"][i]["raio"]);

                cone->set_altura(dados["solidos"]["cones"][i]["altura"]);

                std::size_t j = 0;

                // Percorrendo o vetor de materiais para ver se o material utilizado pelo cone existe.
                while (j < materiais.size() && materiais[j].get_nome() != dados["solidos"]["cones"][i]["material"])
                    j++;

                if (j < materiais.size()) {

                    cone->set_material(materiais[j]);

                } else {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º cone criado não existe."));

                }

                // Inserindo o cilíndro na cena.
                cena.inserir_solido(std::move(cone));

            }

        }

        // Checando se tem algum triangulo especificado.
        if (!dados["solidos"]["triangulos"].empty()) {

            // Iterando sobre os triângulos especificados para inserí-los na cena.
            for (std::size_t i = 0; i < dados["solidos"]["triangulos"].size(); i++) {

                if (dados["solidos"]["triangulos"][i]["vertice1"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O vértice 1 do " + std::to_string(i+1) + "º triângulo criado não foi especificado."));

                }

                if (dados["solidos"]["triangulos"][i]["vertice2"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O vértice 2 do " + std::to_string(i+1) + "º triângulo criado não foi especificado."));

                }

                if (dados["solidos"]["triangulos"][i]["vertice3"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O vértice 3 do " + std::to_string(i+1) + "º triângulo criado não foi especificado."));

                }

                if (dados["solidos"]["triangulos"][i]["material"].empty()) {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º triângulo criado não foi especificado."));

                }

                // Criando o triângulo.
                std::shared_ptr<Triangulo> triangulo = std::make_shared<Triangulo>();

                triangulo->set_vertice1(Ponto3(dados["solidos"]["triangulos"][i]["vertice1"][0], dados["solidos"]["triangulos"][i]["vertice1"][1], dados["solidos"]["triangulos"][i]["vertice1"][2]));

                triangulo->set_vertice1(Ponto3(dados["solidos"]["triangulos"][i]["vertice2"][0], dados["solidos"]["triangulos"][i]["vertice2"][1], dados["solidos"]["triangulos"][i]["vertice2"][2]));

                triangulo->set_vertice1(Ponto3(dados["solidos"]["triangulos"][i]["vertice3"][0], dados["solidos"]["triangulos"][i]["vertice3"][1], dados["solidos"]["triangulos"][i]["vertice3"][2]));

                std::size_t j = 0;

                // Percorrendo o vetor de materiais para ver se o material utilizado pelo triângulo existe.
                while (j < materiais.size() && materiais[j].get_nome() != dados["solidos"]["triangulos"][i]["material"])
                    j++;

                if (j < materiais.size()) {

                    triangulo->set_material(materiais[j]);

                } else {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material do " + std::to_string(i+1) + "º triângulo criado não existe."));

                }

                // Inserindo o cilíndro na cena.
                cena.inserir_solido(std::move(triangulo));

            }

        }

    }

    // --- FIM DA INSERÇÃO DE SÓLIDOS NA CENA --- 

    // --- INÍCIO DA INSERÇÃO DE MALHAS NA CENA ---

    if (!dados["malhas"].empty()) {

        if (materiais.size() == 0) {

            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Não foi especificado nenhum material, portanto não há como inserir malhas."));

        }

        for (std::size_t i = 0; i < dados["malhas"].size(); i++) {

            if (dados["malhas"][i]["arquivo"].empty() && dados["malhas"][i]["tipo"].empty()) {

                throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Não foi especificado um arquivo ou um tipo para a " + std::to_string(i+1) + "ª malha criada."));

            }

            if (dados["malhas"][i]["material"].empty()) {

                throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material da " + std::to_string(i+1) + "ª malha criada não foi especificado."));

            }

            std::shared_ptr<Malha> malha;

            // Checando se é uma malha pré-definida
            if (!dados["malhas"][i]["tipo"].empty()) {

                if (dados["malhas"][i]["tipo"] == "retangulo") {

                    if (dados["malhas"][i]["plano"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O plano da " + std::to_string(i+1) + "ª malha criada (retângulo) não foi especificado."));

                    }

                    if (dados["malhas"][i]["centro_base"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O centro da base da " + std::to_string(i+1) + "ª malha criada (retângulo) não foi especificado."));

                    }

                    if (dados["malhas"][i]["largura"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A largura da " + std::to_string(i+1) + "ª malha criada (retângulo) não foi especificado."));

                    }

                    if (dados["malhas"][i]["altura"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A altura da " + std::to_string(i+1) + "ª malha criada (retângulo) não foi especificado."));

                    }

                    if (dados["malhas"][i]["normal_sentido_positivo"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O booleano que indica se o vetor normal à " + std::to_string(i+1) + "ª malha criada (retângulo) aponta para o positivo não foi especificado."));

                    }

                    Ponto3 centro_base;

                    for (std::size_t k = 0; k < 3; k++)
                        centro_base[k] = dados["malhas"][i]["centro_base"][k];

                    if (dados["malhas"][i]["plano"] == "xy") {
                        
                        malha = std::make_shared<RetanguloXY>(centro_base, dados["malhas"][i]["largura"], dados["malhas"][i]["altura"], Material(), dados["malhas"][i]["normal_sentido_positivo"]);

                    } else if (dados["malhas"][i]["plano"] == "xz") {

                        malha = std::make_shared<RetanguloXZ>(centro_base, dados["malhas"][i]["largura"], dados["malhas"][i]["altura"], Material(), dados["malhas"][i]["normal_sentido_positivo"]);

                    } else if (dados["malhas"][i]["plano"] == "yz") {

                        malha = std::make_shared<RetanguloYZ>(centro_base, dados["malhas"][i]["largura"], dados["malhas"][i]["altura"], Material(), dados["malhas"][i]["normal_sentido_positivo"]);

                    } else {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O plano da " + std::to_string(i+1) + "ª malha criada (retângulo) é inválido. Valores válidos: \"xy\", \"xz\" e \"yz\""));
                        
                    }

                } else if (dados["malhas"][i]["tipo"] == "paralelepipedo") {

                    if (dados["malhas"][i]["centro_base"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O centro da base da " + std::to_string(i+1) + "ª malha criada (paralelepípedo) não foi especificado."));

                    }

                    if (dados["malhas"][i]["largura_base"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A largura da base da " + std::to_string(i+1) + "ª malha criada (paralelepípedo) não foi especificada."));

                    }

                    if (dados["malhas"][i]["comprimento_base"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O comprimento da base da " + std::to_string(i+1) + "ª malha criada (paralelepípedo) não foi especificado."));

                    }

                    if (dados["malhas"][i]["altura"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A altura da " + std::to_string(i+1) + "ª malha criada (paralelepípedo) não foi especificada."));

                    }

                    Ponto3 centro_base;

                    for (std::size_t k = 0; k < 3; k++)
                        centro_base[k] = dados["malhas"][i]["centro_base"][k];

                    malha = std::make_shared<Paralelepipedo>(centro_base, dados["malhas"][i]["largura_base"], dados["malhas"][i]["comprimento_base"], dados["malhas"][i]["altura"]);

                } else if (dados["malhas"][i]["tipo"] == "piramide") {

                    if (dados["malhas"][i]["centro_base"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O centro da base da " + std::to_string(i+1) + "ª malha criada (pirâmide) não foi especificado."));

                    }

                    if (dados["malhas"][i]["largura_base"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A largura da base da " + std::to_string(i+1) + "ª malha criada (pirâmide) não foi especificada."));

                    }

                    if (dados["malhas"][i]["comprimento_base"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O comprimento da base da " + std::to_string(i+1) + "ª malha criada (pirâmide) não foi especificado."));

                    }

                    if (dados["malhas"][i]["altura"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A altura da " + std::to_string(i+1) + "ª malha criada (pirâmide) não foi especificada."));

                    }

                    Ponto3 centro_base;

                    for (std::size_t k = 0; k < 3; k++)
                        centro_base[k] = dados["malhas"][i]["centro_base"][k];

                    malha = std::make_shared<Piramide>(centro_base, dados["malhas"][i]["largura_base"], dados["malhas"][i]["comprimento_base"], dados["malhas"][i]["altura"]);

                } else {

                    throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O tipo da " + std::to_string(i+1) + "ª malha criada é inválido. Valores válidos: \"retangulo\", \"paralelepipedo\" e \"piramide\"."));

                }

            } else {

                malha = carregar_malha(dados["malhas"][i]["arquivo"]);

            }

            // Percorrendo o vetor de materiais para ver se o material utilizado pela malha existe.
            std::size_t j = 0;
            while (j < materiais.size() && materiais[j].get_nome() != dados["malhas"][i]["material"])
                j++;

            if (j < materiais.size()) {

                malha->set_material(materiais[j]);

            } else {

                throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O material da " + std::to_string(i+1) + "ª criada não existe."));

            }

            // Checando se tem alguma transformação para ser aplicada na malha.
            if (!dados["malhas"][i]["transformacoes"].empty()) {

                // Iterando sobre as transformações para aplicá-las nas malhas.
                for (j = 0; j < dados["malhas"][i]["transformacoes"].size(); j++) {

                    if (dados["malhas"][i]["transformacoes"][j]["tipo"].empty()) {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O tipo da " + std::to_string(j+1) + "ª transformação aplicada na " + std::to_string(i+1) + "ª malha criada não foi especificado."));

                    }

                    if (dados["malhas"][i]["transformacoes"][j]["tipo"] == "translacao") {

                        if (dados["malhas"][i]["transformacoes"][j]["x"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (translação) aplicada na " + std::to_string(i+1) + "ª malha não especifica o valor x."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["y"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (translação) aplicada na " + std::to_string(i+1) + "ª malha não especifica o valor y."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["z"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (translação) aplicada na " + std::to_string(i+1) + "ª malha não especifica o valor z."));

                        }

                        // Aplicando a translação na malha.
                        malha->transladar(dados["malhas"][i]["transformacoes"][j]["x"], dados["malhas"][i]["transformacoes"][j]["y"], dados["malhas"][i]["transformacoes"][j]["z"]);

                    } else if (dados["malhas"][i]["transformacoes"][j]["tipo"] == "rotacao") {

                        if (dados["malhas"][i]["transformacoes"][j]["angulo"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (rotação) aplicada na " + std::to_string(i+1) + "ª malha não especifica o ângulo de rotação."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["eixo"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (rotação) aplicada na " + std::to_string(i+1) + "ª malha não especifica o eixo."));

                        }

                        EixoCanonico eixo;

                        if (dados["malhas"][i]["transformacoes"][j]["eixo"] == "x") {

                            eixo = EIXO_X;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo"] == "y") {

                            eixo = EIXO_Y;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo"] == "z") {

                            eixo = EIXO_Z;

                        } else {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (rotação) aplicada na " + std::to_string(i+1) + "ª malha especifica um eixo inválido. Os valores válidos são: \"x\", \"y\" e \"z\"."));

                        }

                        // Aplicando rotação na malha.
                        malha->rotacionar(dados["malhas"][i]["transformacoes"][j]["angulo"], eixo);

                    } else if (dados["malhas"][i]["transformacoes"][j]["tipo"] == "rotacao_arbitraria") {

                        if (dados["malhas"][i]["transformacoes"][j]["angulo"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (rotação) aplicada na " + std::to_string(i+1) + "ª malha não especifica o ângulo de rotação."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["ponto_eixo"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (rotação arbitrária) aplicada na " + std::to_string(i+1) + "ª malha não especifica um ponto do eixo."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["direcao_eixo"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (rotação arbitrária) aplicada na " + std::to_string(i+1) + "ª malha não especifica a direção do eixo."));

                        }

                        Ponto3 ponto_eixo(dados["malhas"][i]["transformacoes"][j]["ponto_eixo"][0], dados["malhas"][i]["transformacoes"][j]["ponto_eixo"][1], dados["malhas"][i]["transformacoes"][j]["ponto_eixo"][2]);

                        Vetor3 direcao_eixo(dados["malhas"][i]["transformacoes"][j]["direcao_eixo"][0], dados["malhas"][i]["transformacoes"][j]["direcao_eixo"][1], dados["malhas"][i]["transformacoes"][j]["direcao_eixo"][2]);

                        // Aplicando rotação arbitrária.
                        
                        malha->rotacionar(dados["malhas"][i]["transformacoes"][j]["angulo"], ponto_eixo, direcao_eixo);

                    }  else if (dados["malhas"][i]["transformacoes"][j]["tipo"] == "escala") {

                        if (dados["malhas"][i]["transformacoes"][j]["fator_x"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (escala) aplicada na " + std::to_string(i+1) + "ª malha não especifica o fator x."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["fator_y"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (escala) aplicada na " + std::to_string(i+1) + "ª malha não especifica o fator y."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["fator_z"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (escala) aplicada na " + std::to_string(i+1) + "ª malha não especifica o fator z."));

                        }

                        Ponto3 ponto_amarra(0.0);

                        if (!dados["malhas"][i]["transformacoes"][j]["ponto_amarra"].empty()) {

                            for (std::size_t k = 0; k < 3; k++) {

                                ponto_amarra[k] = dados["malhas"][i]["transformacoes"][j]["ponto_amarra"][k];

                            }

                        }

                        // Aplicando escala.
                        malha->escalar(dados["malhas"][i]["transformacoes"][j]["fator_x"], dados["malhas"][i]["transformacoes"][j]["fator_y"], dados["malhas"][i]["transformacoes"][j]["fator_z"], ponto_amarra);

                    } else if (dados["malhas"][i]["transformacoes"][j]["tipo"] == "cisalhamento") {

                        if (dados["malhas"][i]["transformacoes"][j]["angulo"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (cisalhamento) aplicada na " + std::to_string(i+1) + "ª malha não especifica o ângulo de cisalhamento."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["eixo1"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (cisalhamento) aplicada na " + std::to_string(i+1) + "ª malha não especifica o 1º eixo."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["eixo2"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (cisalhamento) aplicada na " + std::to_string(i+1) + "ª malha não especifica o 2º eixo."));

                        }

                        EixoCanonico eixo1, eixo2;

                        if (dados["malhas"][i]["transformacoes"][j]["eixo1"] == "x") {

                            eixo1 = EIXO_X;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo1"] == "y") {

                            eixo1 = EIXO_Y;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo1"] == "z") {

                            eixo1 = EIXO_Z;

                        } else {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (cisalhamento) aplicada na " + std::to_string(i+1) + "ª malha especifica um 1º eixo inválido. Os valores válidos são: \"x\", \"y\" e \"z\"."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["eixo2"] == "x") {

                            eixo2 = EIXO_X;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo2"] == "y") {

                            eixo2 = EIXO_Y;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo2"] == "z") {

                            eixo2 = EIXO_Z;

                        } else {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (cisalhamento) aplicada na " + std::to_string(i+1) + "ª malha especifica um 2º eixo inválido. Os valores válidos são: \"x\", \"y\" e \"z\"."));

                        }

                        if (eixo1 == eixo2) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (cisalhamento) aplicada na " + std::to_string(i+1) + "ª malha especifica dois eixos iguais."));

                        }

                        Ponto3 ponto_amarra(0.0);

                        if (!dados["malhas"][i]["transformacoes"][j]["ponto_amarra"].empty()) {

                            for (std::size_t k = 0; k < 3; k++) {

                                ponto_amarra[k] = dados["malhas"][i]["transformacoes"][j]["ponto_amarra"][k];

                            }

                        }

                        // Aplicando cisalhamento.
                        malha->cisalhar(dados["malhas"][i]["transformacoes"][j]["angulo"], eixo1, eixo2, ponto_amarra);

                    } else if (dados["malhas"][i]["transformacoes"][j]["tipo"] == "reflexao") {

                        if (dados["malhas"][i]["transformacoes"][j]["eixo1"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (reflexão) aplicada na " + std::to_string(i+1) + "ª malha não especifica o 1º eixo."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["eixo2"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (reflexão) aplicada na " + std::to_string(i+1) + "ª malha não especifica o 2º eixo."));

                        }

                        EixoCanonico eixo1, eixo2;

                        if (dados["malhas"][i]["transformacoes"][j]["eixo1"] == "x") {

                            eixo1 = EIXO_X;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo1"] == "y") {

                            eixo1 = EIXO_Y;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo1"] == "z") {

                            eixo1 = EIXO_Z;

                        } else {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (reflexão) aplicada na " + std::to_string(i+1) + "ª malha especifica um 1º eixo inválido. Os valores válidos são: \"x\", \"y\" e \"z\"."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["eixo2"] == "x") {

                            eixo2 = EIXO_X;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo2"] == "y") {

                            eixo2 = EIXO_Y;

                        } else if (dados["malhas"][i]["transformacoes"][j]["eixo2"] == "z") {

                            eixo2 = EIXO_Z;

                        } else {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (reflexão) aplicada na " + std::to_string(i+1) + "ª malha especifica um 2º eixo inválido. Os valores válidos são: \"x\", \"y\" e \"z\"."));

                        }

                        if (eixo1 == eixo2) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (reflexão) aplicada na " + std::to_string(i+1) + "ª malha especifica dois eixos iguais."));

                        }

                        // Aplicando a reflexão.
                        malha->refletir(eixo1, eixo2);

                    } else if (dados["malhas"][i]["transformacoes"][j]["tipo"] == "reflexao_arbitraria") {

                        if (dados["malhas"][i]["transformacoes"][j]["vetor_normal_plano"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (reflexão arbitrária) aplicada na " + std::to_string(i+1) + "ª malha não especifica o vetor normal ao plano."));

                        }

                        if (dados["malhas"][i]["transformacoes"][j]["ponto_plano"].empty()) {

                            throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": A " + std::to_string(j+1) + "ª transformação (reflexão arbitrária) aplicada na " + std::to_string(i+1) + "ª malha não especifica um ponto conhecido do plano."));

                        }

                        Vetor3 vetor_normal;
                        Ponto3 ponto_plano;

                        for (int k = 0; k < 3; k++) {

                            vetor_normal[k] = dados["malhas"][i]["transformacoes"][j]["vetor_normal_plano"][k];
                            ponto_plano[k] = dados["malhas"][i]["transformacoes"][j]["ponto_plano"][k];
                            
                        }

                        // Aplicando reflexão na malha.
                        malha->refletir(vetor_normal, ponto_plano);

                    } else {

                        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": O tipo da " + std::to_string(j+1) + "ª transformação aplicada na " + std::to_string(i+1) + "ª malha é inválido. Valores válidos: \"translacao\", \"rotacao\", \"rotacao_arbitraria\", \"escala\", \"cisalhamento\", \"reflexao\" e \"reflexao_arbitraria\"."));

                    }

                }

            }

            cena.inserir_malha(malha);

        }

    }

    // --- FIM DA INSERÇÃO DE MALHAS NA CENA ---

}

std::shared_ptr<Malha> carregar_malha(std::string nome_arquivo_JSON) {

    std::shared_ptr<Malha> malha = std::make_shared<Malha>();

    // Abrindo conexão com o arquivo.
    std::ifstream arquivo_JSON(nome_arquivo_JSON);

    // Checando se o arquivo abriu com sucesso.
    if (!arquivo_JSON.is_open()) {

        throw std::runtime_error(std::string("Erro: Não foi possível carregar a malha do arquivo \"") + nome_arquivo_JSON + "\"");

    }

    // Coletando informações do arquivo JSON.
    json dados = json::parse(arquivo_JSON);

    // Fechando conexão com o arquivo.
    arquivo_JSON.close();

    if (dados["vertices"].empty()) {

        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Nenhum vértice foi especificado!"));

    }

    if (dados["arestas"].empty()) {

        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Nenhuma aresta foi especificada!"));

    }

    if (dados["faces"].empty()) {

        throw std::runtime_error(std::string("Erro no arquivo " + nome_arquivo_JSON + ": Nenhuma face foi especificada!"));

    }

    if (!dados["nome"].empty()) {

        malha->set_nome(dados["nome"]);

    }

    for (std::size_t i = 0; i < dados["vertices"].size(); i++) {

        malha->inserir_vertice(Ponto3(dados["vertices"][i][0], dados["vertices"][i][1], dados["vertices"][i][2]));

    }

    for (std::size_t i = 0; i < dados["arestas"].size(); i++) {

        malha->inserir_aresta(Aresta(dados["arestas"][i][0], dados["arestas"][i][1]));

    }

    for (std::size_t i = 0; i < dados["faces"].size(); i++) {

        malha->inserir_face(Face(dados["faces"][i][0], dados["faces"][i][1], dados["faces"][i][2]));

    }

    return malha;

}