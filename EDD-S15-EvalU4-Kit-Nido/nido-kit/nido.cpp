/* =============================================================
   Curso      : Estructura de Datos (24UC00428)
   Evaluacion : C2 - Unidad 4 - Semana 15
   Proyecto   : Nido, la red social del campus (kit base)
   Institucion: Universidad Continental
   Docente    : Carlos del Carpio
   Semestre   : 2026-20
   Compilador : Dev-C++ (MinGW / GCC, C++11 o superior)
   NRC        :42964
   Grupo      :E
   Integrantes:
   Shany Anjhara Becerra Yari
   Dayana Quispe Sucasaca
   Marcos Abel Palomino Quispe 
   Andrea Celeste Limache Tinta
   ============================================================= */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

const int MAX = 12;
const string ARCHIVO = "nido.txt";

struct NodoUsuario {
    string usuario;
    int    id;
    NodoUsuario *izq, *der;
};

int sigue[MAX][MAX];
NodoUsuario *porId[MAX];
int nUsuarios = 0;

// ===== FICHA =====
const string COMUNIDAD = "nido-de-prueba";
const int N_U = 8;
string fUsuario[N_U] = {"@mango", "@byte", "@pixel", "@andes", "@cuy", "@nova", "@zenit", "@lofi"};
const int N_S = 10;
string fSigue[N_S][2] = {{"@byte", "@mango"}, {"@pixel", "@mango"}, {"@mango", "@byte"}, {"@cuy", "@byte"},
                         {"@andes", "@cuy"}, {"@nova", "@pixel"}, {"@zenit", "@nova"}, {"@mango", "@pixel"},
                         {"@lofi", "@zenit"}, {"@zenit", "@lofi"}};
// =================

NodoUsuario* crearUsuario(string usuario, int id) {
    NodoUsuario *n = new NodoUsuario;
    n->usuario = usuario; n->id = id;
    n->izq = n->der = nullptr;
    return n;
}

bool esUsuarioValido(string u) {
    if (u.size() < 2 || u.size() > 16 || u[0] != '@') return false;
    for (int i = 1; i < (int)u.size(); i++)
        if (!((u[i] >= 'a' && u[i] <= 'z') || (u[i] >= '0' && u[i] <= '9'))) return false;
    return true;
}

void imprimirMarcados(string titulo, bool marca[]) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) if (marca[x]) k++;
    cout << titulo << " (" << k << "):";
    if (k == 0) cout << " (nadie)";
    for (int x = 0; x < nUsuarios; x++) if (marca[x]) cout << " " << porId[x]->usuario;
    cout << endl;
}

/* ---------- Arbol binario de busqueda (todo recursivo) ---------- */

NodoUsuario* buscar(NodoUsuario *r, string u) {
    if (r == nullptr) return nullptr;
    if (u == r->usuario) return r;
    if (u < r->usuario) return buscar(r->izq, u);
    return buscar(r->der, u);
}

bool insertar(NodoUsuario *&r, NodoUsuario *nuevo) {
    if (r == nullptr) {
        r = nuevo;
        return true;
    }
    if (nuevo->usuario == r->usuario) return false;
    if (nuevo->usuario < r->usuario) return insertar(r->izq, nuevo);
    return insertar(r->der, nuevo);
}

bool registrarUsuario(NodoUsuario *&raiz, string u) {
    if (!esUsuarioValido(u)) return false;
    if (nUsuarios >= MAX) return false;
    if (buscar(raiz, u) != nullptr) return false;
    NodoUsuario *nuevo = crearUsuario(u, nUsuarios);
    if (!insertar(raiz, nuevo)) {
        delete nuevo;
        return false;
    }
    porId[nUsuarios] = nuevo;
    nUsuarios++;
    return true;
}

void enorden(NodoUsuario *r) {
    if (r == nullptr) return;
    enorden(r->izq);
    cout << " " << r->usuario;
    enorden(r->der);
}

void preorden(NodoUsuario *r) {
    if (r == nullptr) return;
    cout << " " << r->usuario;
    preorden(r->izq);
    preorden(r->der);
}

int contar(NodoUsuario *r) {
    if (r == nullptr) return 0;
    return 1 + contar(r->izq) + contar(r->der);
}

int altura(NodoUsuario *r) {
    if (r == nullptr) return 0;
    int hi = altura(r->izq);
    int hd = altura(r->der);
    return 1 + (hi > hd ? hi : hd);
}

int hojas(NodoUsuario *r) {
    if (r == nullptr) return 0;
    if (r->izq == nullptr && r->der == nullptr) return 1;
    return hojas(r->izq) + hojas(r->der);
}
/* ---------- Grafo dirigido: matriz de adyacencia sigue ---------- */

int contarSiguiendo(int id) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) if (sigue[id][x] == 1) k++;
    return k;
}

int contarSeguidores(int id) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) {
        if (sigue[x][id] == 1) k++;
    }
    return k;
}

void difundir(int v, bool visto[], bool imprimir) {
    visto[v] = true;
    if (imprimir) cout << " " << porId[v]->usuario;
    for (int x = 0; x < nUsuarios; x++) {
        if (sigue[x][v] == 1 && !visto[x]) {
            difundir(x, visto, imprimir);
        }
}
}

/* ---------- Archivos y memoria ---------- */

void guardarPreorden(NodoUsuario *r, ofstream &f) {
    if (r == nullptr) return;
    f << r->usuario << "\n";
    guardarPreorden(r->izq, f);
    guardarPreorden(r->der, f);
}

int liberar(NodoUsuario *r) {
    if (r == nullptr) return 0;
    int cant = 1 + liberar(r->izq) + liberar(r->der);
    delete r;
    return cant;
}

/* ---------- Ordenes ---------- */

void leerOrden(string linea, string &orden, string &a1, string &a2) {
    string t[6]; int n = 0;
    stringstream ss(linea);
    while (n < 6 && ss >> t[n]) n++;
    orden = a1 = a2 = "";
    if (n == 0) return;
    string c = t[0];
    if (c == "signup") c = "registrar";
    else if (c == "whois") c = "perfil";
    else if (c == "users") c = "usuarios";
    else if (c == "tree") c = "arbol";
    else if (c == "follow") c = "seguir";
    else if (c == "unfollow") c = "dejar";
    else if (c == "followers") c = "seguidores";
    else if (c == "following") c = "siguiendo";
    else if (c == "mutuals") c = "mutuos";
    else if (c == "trending") c = "popular";
    else if (c == "reach") c = "alcance";
    else if (c == "reaches") c = "llega";
    else if (c == "save") c = "guardar";
    else if (c == "load") c = "cargar";
    else if (c == "stats") c = "reporte";
    else if (c == "help" || c == "man") c = "ayuda";
    else if (c == "exit" || c == "quit") c = "salir";
    orden = c;
    if (n > 1) a1 = t[1];
    if (n > 2) a2 = t[2];
}

void ayuda() {
    cout << "  ORDEN                   EN INGLES            QUE HACE" << endl;
    cout << "  registrar @u            signup @u            crear una cuenta (ABB: insertar)" << endl;
    cout << "  perfil @u               whois @u             ver una cuenta (ABB: buscar)" << endl;
    cout << "  usuarios                users                cuentas en orden alfabetico (enorden)" << endl;
    cout << "  arbol                   tree                 preorden, nodos, altura y hojas" << endl;
    cout << "  seguir @a @b            follow @a @b         @a empieza a seguir a @b" << endl;
    cout << "  dejar @a @b             unfollow @a @b       @a deja de seguir a @b" << endl;
    cout << "  seguidores @u           followers @u         quienes siguen a @u" << endl;
    cout << "  siguiendo @u            following @u         a quienes sigue @u" << endl;
    cout << "  mutuos @u               mutuals @u           se siguen los dos" << endl;
    cout << "  popular                 trending             cuenta con mas seguidores" << endl;
    cout << "  alcance @u              reach @u             a quienes llega un post de @u (DFS)" << endl;
    cout << "  llega @a @b             reaches @a @b        un post de @a llega a @b?" << endl;
    cout << "  guardar / cargar        save / load          archivo nido.txt" << endl;
    cout << "  reporte                 stats                resumen de la comunidad" << endl;
    cout << "  salir                   exit                 terminar" << endl;
}

int main() {
    NodoUsuario *raiz = nullptr;
    for (int i = 0; i < MAX; i++) for (int j = 0; j < MAX; j++) sigue[i][j] = 0;

    for (int i = 0; i < N_U; i++) registrarUsuario(raiz, fUsuario[i]);
    for (int i = 0; i < N_S; i++) {
        NodoUsuario *a = buscar(raiz, fSigue[i][0]);
        NodoUsuario *b = buscar(raiz, fSigue[i][1]);
        if (a != nullptr && b != nullptr) sigue[a->id][b->id] = 1;
    }

    cout << "Nido - comunidad " << COMUNIDAD << " (" << nUsuarios << " cuentas). Escribe ayuda." << endl;

    string linea;
    while (true) {
        cout << "nido@" << COMUNIDAD << ":~$ ";
        if (!getline(cin, linea)) break;
        string orden, a1, a2;
        leerOrden(linea, orden, a1, a2);
        if (orden == "") continue;
        cout << "> " << linea << endl;

        if (orden == "registrar") {
            if (!esUsuarioValido(a1)) {
                cout << "E: Usuario invalido (usa @minusculas/numeros, 2 a 16 caracteres)" << endl;
            } else if (nUsuarios >= MAX) {
                cout << "E: Comunidad llena (max " << MAX << ")" << endl;
            } else if (buscar(raiz, a1) != nullptr) {
                cout << "E: Ya existe " << a1 << endl;
            } else {
                registrarUsuario(raiz, a1);
                cout << "Ok: Registrado " << a1 << " (id " << (nUsuarios - 1) << ")" << endl;
            }
        }
        else if (orden == "perfil") {
            NodoUsuario *u = buscar(raiz, a1);
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else cout << u->usuario << " | id " << u->id << " | seguidores " << contarSeguidores(u->id)
                      << " | siguiendo " << contarSiguiendo(u->id) << endl;
        }
        else if (orden == "usuarios") {
            cout << "Usuarios (enorden):";
            enorden(raiz);
            cout << endl;
        }
        else if (orden == "arbol") {
            cout << "Preorden:";
            preorden(raiz);
            cout << endl;
            cout << "Nodos: " << contar(raiz) << " | altura: " << altura(raiz) << " | hojas: " << hojas(raiz) << endl;
        }
        else if (orden == "seguir") {
        NodoUsuario *a = buscar(raiz, a1);
        NodoUsuario *b = buscar(raiz, a2);
        if (a == nullptr || b == nullptr) {
            cout << "E: Cuenta no encontrada" << endl;
        } else if (a->id == b->id) {
            cout << "E: No puedes seguirte a ti mismo" << endl;
        } else {
            sigue[a->id][b->id] = 1;
            cout << "Ok: " << a1 << " ahora sigue a " << a2 << endl;
        }
        }
        else if (orden == "dejar") {
        NodoUsuario *a = buscar(raiz, a1);
        NodoUsuario *b = buscar(raiz, a2);
        if (a == nullptr || b == nullptr) {
            cout << "E: Cuenta no encontrada" << endl;
        } else {
            sigue[a->id][b->id] = 0;
            cout << "Ok: " << a1 << " dejo de seguir a " << a2 << endl;
        }
        }
        else if (orden == "siguiendo") {
            NodoUsuario *u = buscar(raiz, a1);
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else {
                bool marca[MAX] = {false};
                for (int x = 0; x < nUsuarios; x++) if (sigue[u->id][x] == 1) marca[x] = true;
                imprimirMarcados(u->usuario + " sigue a", marca);
            }
        }
        else if (orden == "seguidores") {
            NodoUsuario *u = buscar(raiz, a1);
        if (u == nullptr) {
            cout << "E: No existe " << a1 << endl;
        } else {
            bool marca[MAX] = {false};
            for (int x = 0; x < nUsuarios; x++) {
                if (sigue[x][u->id] == 1) marca[x] = true;
            }
            imprimirMarcados("Seguidores de " + u->usuario, marca);
        }
        }
        else if (orden == "mutuos") {
            // TODO R4a
           if (a1.empty()) {
           cout << "Uso: mutuos @usuario" << endl;
     } else {
    NodoUsuario* u = buscar(raiz, a1);
    if (u == nullptr) {
        cout << "E: No existe " << a1 << endl;
    } else {
        int lista[MAX];
        int k = 0;
        for (int x = 0; x < nUsuarios; x++) {
            if (sigue[u->id][x] == 1 && sigue[x][u->id] == 1) {
                lista[k++] = x;
            }
        }
        cout << "Mutuos de " << u->usuario << " (" << k << "):";
        if (k == 0) {
            cout << " (ninguno)";
        } else {
            for (int i = 0; i < k; i++) {
                cout << " " << porId[lista[i]]->usuario;
            }
        }
        cout << endl;
            }
        }
     } else if (orden == "popular") {
            // TODO R4b
              if (nUsuarios == 0) {
           cout << "(sin cuentas)" << endl;
      } else {
       int mejorId = 0;
       int maxSeg = contarSeguidores(0);
       for (int i = 1; i < nUsuarios; i++) {
           int seg = contarSeguidores(i);
           if (seg > maxSeg) { 
               maxSeg = seg;
               mejorId = i;
            }
        }
           cout << "Popular: " << porId[mejorId]->usuario << " | seguidores: " << maxSeg << endl;
        }
        } else if (orden == "alcance") {
            NodoUsuario *u = buscar(raiz, a1);
        if (u == nullptr) {
            cout << "E: No existe " << a1 << endl;
        } else {
            bool visto[MAX] = {false};
            difundir(u->id, visto, false);
            visto[u->id] = false;
            imprimirMarcados("Alcance de " + a1, visto);
        }
        }
        else if (orden == "llega") {
        NodoUsuario *u1 = buscar(raiz, a1);
        NodoUsuario *u2 = buscar(raiz, a2);
        if (u1 == nullptr || u2 == nullptr) {
            cout << "E: Cuenta no encontrada" << endl;
        } else {
            bool visto[MAX] = {false};
            difundir(u1->id, visto, false);
            if (visto[u2->id]) {
                cout << "SI hay camino de " << a1 << " a " << a2 << endl;
            } else {
                cout << "NO hay camino de " << a1 << " a " << a2 << endl;
            }
        }
        }
      else if (orden == "guardar") {
            ofstream f(ARCHIVO);
            if (!f) {
                cout << "E: No se pudo guardar " << ARCHIVO << endl;
            } else {
                int m = 0;
                f << nUsuarios << "\n";
                guardarPreorden(raiz, f);
                for (int i = 0; i < nUsuarios; i++) {
                    for (int j = 0; j < nUsuarios; j++) {
                        f << sigue[i][j] << (j + 1 < nUsuarios ? " " : "");
                        if (sigue[i][j] == 1) m++;
                    }
                    f << "\n";
                }
                f.close();
                cout << "Guardado: " << contar(raiz) << " cuentas y " << m << " seguimientos en " << ARCHIVO << endl;
            }
        }
        else if (orden == "cargar") {
            ifstream f(ARCHIVO);
            if (!f) {
                cout << "E: No existe " << ARCHIVO << endl;
            } else {
                int m = 0;
                liberar(raiz);
                raiz = nullptr;
                nUsuarios = 0;
                for (int i = 0; i < MAX; i++) {
                    porId[i] = nullptr;
                    for (int j = 0; j < MAX; j++) {
                        sigue[i][j] = 0;
                    }
                }
                int total;
                f >> total;
                for (int i = 0; i < total; i++) {
                    string u;
                    f >> u;
                    registrarUsuario(raiz, u);
                }
                for (int i = 0; i < total; i++) {
                    for (int j = 0; j < total; j++) {
                        f >> sigue[i][j];
                        if (sigue[i][j] == 1) m++;
                    }
                }
                f.close();
                cout << "Cargado: " << contar(raiz) << " cuentas y " << m << " seguimientos | altura "
                     << altura(raiz) << endl;
            }
        }
        else if (orden == "reporte") {
           int seg = 0, mut = 0;
        for (int a = 0; a < nUsuarios; a++) {
            for (int b = 0; b < nUsuarios; b++) {
                if (sigue[a][b] == 1) seg++;
                if (a < b && sigue[a][b] == 1 && sigue[b][a] == 1) mut++;
            }
        }
        cout << "Cuentas: " << nUsuarios << " | seguimientos: " << seg << " | pares mutuos: " << mut << endl;
        cout << "Arbol: altura " << altura(raiz) << " | hojas " << hojas(raiz) << endl;
            
        }
        else if (orden == "ayuda") ayuda();
        else if (orden == "salir") break;
        else cout << orden << ": orden no encontrada (escribe ayuda)" << endl;
    }

    int liberados = liberar(raiz);
    raiz = nullptr;
    cout << "Nodos liberados: " << liberados << ". Hasta pronto." << endl;
    return 0;
}
