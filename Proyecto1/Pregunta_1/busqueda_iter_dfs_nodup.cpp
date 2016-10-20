/*
    Nombre: Daniel Leones 09-10977
    Descripción: Búsqueda iterativa en DFS. Habilitado para podar prefijos
        usando el automata de prefijos que está disponibles en PSVN.
    Fecha: 21/10/2016

    Ejecución: <problema>.busqueda_iter_dfs_nodup <x>
        donde:
            x Es el límite de profunidad deseado. Por defecto es 0.
*/
#include <vector>
#include <sstream>
#include <stack>

// Estructura auxiliar para guardar la distancia desde la raíz
struct Nodo
{
    state_t estado;
    int altura;
};

int main(int argc, char **argv) {
    uint64_t numAtD = 0, ant_numAtD = 0;    // Contador numero de estados
    state_t state, estado_raiz, child;      // Despacho de estados
    int cota_altura = 0, ruleid;
    int limProfundidad = 0, hist, childHist;
    ruleid_iterator_t iter;
    double factor_rami = 0.0;   // Factor de ramificación
    std::stack<Nodo> open;      // Estados por expansión
    std::stack<int> historia;   // Historia para podar los duplicados
    Nodo nodo, hijo;


    // Partir desde un estado meta.
    first_goal_state(&estado_raiz, &cota_altura);

    // Leer argumentos y convertir a entero.
    if (argc > 1) {
        if ( ! (std::istringstream(argv[1]) >> limProfundidad) ) limProfundidad = 0;
    }

    printf("----------------------------------------\n");
    printf("Depth %16s %4s%s\n", "Nro. Estados", " ", "Fac. Ram.");
    printf("----------------------------------------\n");


    cota_altura = 0;
    // Comienzo del algoritmo iterativo DFS.
    for (cota_altura = 0; cota_altura <= limProfundidad; cota_altura += 1)
    {
        numAtD = 0;
        nodo.estado = estado_raiz;
        nodo.altura = 0;
        open.push(nodo);
        historia.push(init_history);
        while( !open.empty()) {
            nodo = open.top();
            hist = historia.top();
            open.pop();
            historia.pop();
            if( nodo.altura > cota_altura) {
                continue;
            }
            else if (nodo.altura == cota_altura) {
                numAtD += 1;
            } else {
                state = nodo.estado;
                init_fwd_iter(&iter, &state);
                while( (ruleid = next_ruleid(&iter) ) >= 0 ) {
                    if (!fwd_rule_valid_for_history(hist,ruleid))
                        continue;
                    apply_fwd_rule(ruleid, &state, &child);
                    /*printf("altura: %d, estado:  ",nodo.altura + 1);
                    print_state(stdout,&child);
                    printf("\n");*/
                    hijo.estado = child;
                    hijo.altura = nodo.altura + 1;
                    open.push(hijo);
                    historia.push(next_fwd_history(hist,ruleid));
                }
            }
        }
        if (ant_numAtD == 0)
            factor_rami = numAtD / 1.0;
        else
            factor_rami = numAtD / ant_numAtD;

        ant_numAtD = numAtD;
        // Imprimir cuenta final de nodos para una profunidad determinada
        // y su factor de ramificación
        printf("%3d %3s%15"PRId64"%4s%6.2e\n", cota_altura, " "
               , numAtD, " ", factor_rami);
    }
    printf("----------------------------------------\n");

    return 0;
}
