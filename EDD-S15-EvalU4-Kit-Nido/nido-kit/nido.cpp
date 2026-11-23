/*
   Curso      : Estructura de Datos
   Evaluacion : C2 - Unidad 4 - Semana 15
   Proyecto   : Nido, la red social del campus
   NRC        : 42964
   Grupo      : E
   Comunidad  : nido-huallata
   Compilador : Dev-C++ / GCC, C++11
*/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

const int MAX = 12;
const string ARCHIVO = "nido.txt";

struct NodoUsuario {
    string usuario;
    int id;
    NodoUsuario *izq, *der;
};

int sigue[MAX][MAX];
NodoUsuario *porId[MAX];
int nUsuarios = 0;

// ===== FICHA OFICIAL: GRUPO E =====
const string COMUNIDAD = "nido-huallata";
const int N_U = 10;
string fUsuario[N_U] = {
    "@koala", "@beat", "@cactus", "@misti", "@chasqui",
    "@delta", "@turbo", "@meme", "@pixel", "@zenit"
};
const int N_S = 16;
string fSigue[N_S][2] = {
    {"@zenit", "@misti"}, {"@delta", "@pixel"},
    {"@chasqui", "@zenit"}, {"@turbo", "@pixel"},
    {"@chasqui", "@pixel"}, {"@cactus", "@delta"},
    {"@pixel", "@koala"}, {"@cactus", "@meme"},
    {"@zenit", "@delta"}, {"@cactus", "@turbo"},
    {"@turbo", "@chasqui"}, {"@pixel", "@turbo"},
    {"@chasqui", "@turbo"}, {"@cactus", "@beat"},
    {"@misti", "@turbo"}, {"@zenit", "@turbo"}
};
// ==================================

NodoUsuario* crearUsuario(const string &usuario, int id) {
    NodoUsuario *n = new NodoUsuario;
    n->usuario = usuario;
    n->id = id;
    n->izq = n->der = nullptr;
    return n;
}

bool esUsuarioValido(const string &u) {
    if (u.size() < 2 || u.size() > 16 || u[0] != '@') return false;
    for (int i = 1; i < (int)u.size(); i++)
        if (!((u[i] >= 'a' && u[i] <= 'z') || (u[i] >= '0' && u[i] <= '9'))) return false;
    return true;
}

// ---------------- ABB: operaciones recursivas ----------------
NodoUsuario* buscar(NodoUsuario *r, const string &u) {
    if (r == nullptr) return nullptr;
    if (u == r->usuario) return r;
    if (u < r->usuario) return buscar(r->izq, u);
    return buscar(r->der, u);
}

bool insertar(NodoUsuario *&r, NodoUsuario *nuevo) {
    if (r == nullptr) { r = nuevo; return true; }
    if (nuevo->usuario == r->usuario) return false;
    if (nuevo->usuario < r->usuario) return insertar(r->izq, nuevo);
    return insertar(r->der, nuevo);
}

bool registrarUsuario(NodoUsuario *&raiz, const string &u) {
    if (!esUsuarioValido(u) || nUsuarios >= MAX || buscar(raiz, u) != nullptr) return false;
    NodoUsuario *nuevo = crearUsuario(u, nUsuarios);
    if (!insertar(raiz, nuevo)) { delete nuevo; return false; }
    porId[nUsuarios] = nuevo;
    nUsuarios++;
    return true;
}

void enorden(NodoUsuario *r) {
    if (r == nullptr) return;
    enorden(r->izq); cout << " " << r->usuario; enorden(r->der);
}
void preorden(NodoUsuario *r) {
    if (r == nullptr) return;
    cout << " " << r->usuario; preorden(r->izq); preorden(r->der);
}
int contar(NodoUsuario *r) {
    if (r == nullptr) return 0;
    return 1 + contar(r->izq) + contar(r->der);
}
int altura(NodoUsuario *r) {
    if (r == nullptr) return 0;
    int hi = altura(r->izq), hd = altura(r->der);
    return 1 + (hi > hd ? hi : hd);
}
int hojas(NodoUsuario *r) {
    if (r == nullptr) return 0;
    if (r->izq == nullptr && r->der == nullptr) return 1;
    return hojas(r->izq) + hojas(r->der);
}

// ---------------- Grafo dirigido: matriz de adyacencia ----------------
int contarSiguiendo(int id) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) if (sigue[id][x] == 1) k++;
    return k;
}
int contarSeguidores(int id) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) if (sigue[x][id] == 1) k++;
    return k;
}
void imprimirMarcados(const string &titulo, bool marca[]) {
    int k = 0;
    for (int x = 0; x < nUsuarios; x++) if (marca[x]) k++;
    cout << titulo << " (" << k << "):";
    if (k == 0) cout << " (ninguno)";
    for (int x = 0; x < nUsuarios; x++) if (marca[x]) cout << " " << porId[x]->usuario;
    cout << endl;
}

/* A sigue B: un post de B llega a quienes siguen a B.
   Se guarda el orden de visita para imprimir el DFS correctamente. */
void difundir(int v, bool visto[], int orden[], int &cantidad) {
    visto[v] = true;
    orden[cantidad++] = v;
    for (int x = 0; x < nUsuarios; x++)
        if (sigue[x][v] == 1 && !visto[x]) difundir(x, visto, orden, cantidad);
}

// ---------------- Archivos y memoria ----------------
void guardarPreorden(NodoUsuario *r, ofstream &f) {
    if (r == nullptr) return;
    f << r->id << " " << r->usuario << "\n";
    guardarPreorden(r->izq, f);
    guardarPreorden(r->der, f);
}
int liberar(NodoUsuario *r) {
    if (r == nullptr) return 0;
    int izq = liberar(r->izq), der = liberar(r->der);
    delete r;
    return 1 + izq + der;
}
void limpiarMatrizGlobal() {
    for (int i = 0; i < MAX; i++) {
        porId[i] = nullptr;
        for (int j = 0; j < MAX; j++) sigue[i][j] = 0;
    }
}

// Carga primero en estructuras temporales; solo reemplaza si todo es valido.
bool cargarComunidad(NodoUsuario *&raiz) {
    ifstream f(ARCHIVO.c_str());
    if (!f) return false;
    int total;
    if (!(f >> total) || total < 0 || total > MAX) return false;

    NodoUsuario *nuevaRaiz = nullptr;
    NodoUsuario *nuevoPorId[MAX];
    bool idUsado[MAX] = {false};
    int nuevaMatriz[MAX][MAX] = {{0}};
    for (int i = 0; i < MAX; i++) nuevoPorId[i] = nullptr;

    bool valido = true;
    for (int i = 0; i < total && valido; i++) {
        int id; string usuario;
        if (!(f >> id >> usuario)) { valido = false; break; }
        if (id < 0 || id >= total || idUsado[id] || !esUsuarioValido(usuario) ||
            buscar(nuevaRaiz, usuario) != nullptr) { valido = false; break; }
        NodoUsuario *nuevo = crearUsuario(usuario, id);
        if (!insertar(nuevaRaiz, nuevo)) { delete nuevo; valido = false; break; }
        nuevoPorId[id] = nuevo;
        idUsado[id] = true;
    }
    for (int i = 0; i < total; i++) if (!idUsado[i]) valido = false;
    for (int i = 0; i < total && valido; i++) {
        for (int j = 0; j < total; j++) {
            if (!(f >> nuevaMatriz[i][j]) ||
                (nuevaMatriz[i][j] != 0 && nuevaMatriz[i][j] != 1)) {
                valido = false; break;
            }
        }
    }
    if (!valido) { liberar(nuevaRaiz); return false; }

    liberar(raiz);
    raiz = nuevaRaiz;
    nUsuarios = total;
    limpiarMatrizGlobal();
    for (int i = 0; i < total; i++) {
        porId[i] = nuevoPorId[i];
        for (int j = 0; j < total; j++) sigue[i][j] = nuevaMatriz[i][j];
    }
    return true;
}

// ---------------- Lectura de ordenes ----------------
void leerOrden(const string &linea, string &orden, string &a1, string &a2) {
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
    cout << "  ORDEN                   QUE HACE" << endl;
    cout << "  registrar @u            crear una cuenta" << endl;
    cout << "  perfil @u               ver el perfil de una cuenta" << endl;
    cout << "  usuarios                cuentas en orden alfabetico" << endl;
    cout << "  arbol                   preorden, nodos, altura y hojas" << endl;
    cout << "  seguir @a @b            @a empieza a seguir a @b" << endl;
    cout << "  dejar @a @b             @a deja de seguir a @b" << endl;
    cout << "  seguidores @u           quienes siguen a @u" << endl;
    cout << "  siguiendo @u            a quienes sigue @u" << endl;
    cout << "  mutuos @u               cuentas con seguimiento mutuo" << endl;
    cout << "  popular                 cuenta con mas seguidores" << endl;
    cout << "  alcance @u              cuentas a las que llega un post de @u" << endl;
    cout << "  llega @a @b             comprobar si un post llega de @a a @b" << endl;
    cout << "  guardar / cargar        guardar o recuperar nido.txt" << endl;
    cout << "  reporte                 resumen de la comunidad" << endl;
    cout << "  ayuda                   mostrar esta ayuda" << endl;
    cout << "  salir                   terminar y liberar memoria" << endl;
}

int main() {
    NodoUsuario *raiz = nullptr;
    limpiarMatrizGlobal();

    for (int i = 0; i < N_U; i++) {
        if (!registrarUsuario(raiz, fUsuario[i])) {
            cout << "E: Error al cargar la ficha inicial." << endl;
            liberar(raiz);
            return 1;
        }
    }
    for (int i = 0; i < N_S; i++) {
        NodoUsuario *a = buscar(raiz, fSigue[i][0]);
        NodoUsuario *b = buscar(raiz, fSigue[i][1]);
        if (a != nullptr && b != nullptr) sigue[a->id][b->id] = 1;
    }

    cout << "Nido - comunidad " << COMUNIDAD << " (" << nUsuarios
         << " cuentas). Escribe ayuda." << endl;
    string linea;
    while (true) {
        cout << "nido@" << COMUNIDAD << ":~$ ";
        if (!getline(cin, linea)) break;
        string orden, a1, a2;
        leerOrden(linea, orden, a1, a2);
        if (orden == "") continue;
        cout << "> " << linea << endl;

        if (orden == "registrar") {
            if (a1.empty()) cout << "Uso: registrar @usuario" << endl;
            else if (!esUsuarioValido(a1)) cout << "E: Usuario invalido (usa @minusculas/numeros, 2 a 16 caracteres)" << endl;
            else if (nUsuarios >= MAX) cout << "E: Comunidad llena (max " << MAX << ")" << endl;
            else if (buscar(raiz, a1) != nullptr) cout << "E: Ya existe " << a1 << endl;
            else {
                int nuevoId = nUsuarios;
                if (registrarUsuario(raiz, a1)) cout << "Registrado: " << a1 << " (id " << nuevoId << ")" << endl;
                else cout << "E: No se pudo registrar " << a1 << endl;
            }
        }
        else if (orden == "perfil") {
            NodoUsuario *u = buscar(raiz, a1);
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else cout << u->usuario << " | id " << u->id << " | seguidores " << contarSeguidores(u->id)
                      << " | siguiendo " << contarSiguiendo(u->id) << endl;
        }
        else if (orden == "usuarios") {
            cout << "Usuarios (enorden):"; enorden(raiz); cout << endl;
        }
        else if (orden == "arbol") {
            cout << "Preorden:"; preorden(raiz); cout << endl;
            cout << "Nodos: " << contar(raiz) << " | altura: " << altura(raiz)
                 << " | hojas: " << hojas(raiz) << endl;
        }
        else if (orden == "seguir") {
            NodoUsuario *a = buscar(raiz, a1), *b = buscar(raiz, a2);
            if (a == nullptr || b == nullptr) cout << "E: Cuenta no encontrada" << endl;
            else if (a->id == b->id) cout << "E: No puedes seguirte a ti mismo" << endl;
            else if (sigue[a->id][b->id] == 1) cout << "E: " << a1 << " ya sigue a " << a2 << endl;
            else { sigue[a->id][b->id] = 1; cout << a1 << " ahora sigue a " << a2 << endl; }
        }
        else if (orden == "dejar") {
            NodoUsuario *a = buscar(raiz, a1), *b = buscar(raiz, a2);
            if (a == nullptr || b == nullptr) cout << "E: Cuenta no encontrada" << endl;
            else if (sigue[a->id][b->id] == 0) cout << "E: " << a1 << " no sigue a " << a2 << endl;
            else { sigue[a->id][b->id] = 0; cout << a1 << " dejo de seguir a " << a2 << endl; }
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
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else {
                bool marca[MAX] = {false};
                for (int x = 0; x < nUsuarios; x++) if (sigue[x][u->id] == 1) marca[x] = true;
                imprimirMarcados("Seguidores de " + u->usuario, marca);
            }
        }
        else if (orden == "mutuos") {
            NodoUsuario *u = buscar(raiz, a1);
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else {
                int lista[MAX], k = 0;
                for (int x = 0; x < nUsuarios; x++)
                    if (sigue[u->id][x] == 1 && sigue[x][u->id] == 1) lista[k++] = x;
                cout << "Mutuos de " << u->usuario << " (" << k << "):";
                if (k == 0) cout << " (ninguno)";
                for (int i = 0; i < k; i++) cout << " " << porId[lista[i]]->usuario;
                cout << endl;
            }
        }
        else if (orden == "popular") {
            if (nUsuarios == 0) cout << "(sin cuentas)" << endl;
            else {
                int mejorId = 0, maxSeg = contarSeguidores(0);
                for (int i = 1; i < nUsuarios; i++) {
                    int seg = contarSeguidores(i);
                    if (seg > maxSeg) { maxSeg = seg; mejorId = i; }
                }
                cout << "Popular: " << porId[mejorId]->usuario << " | seguidores: " << maxSeg << endl;
            }
        }
        else if (orden == "alcance") {
            NodoUsuario *u = buscar(raiz, a1);
            if (u == nullptr) cout << "E: No existe " << a1 << endl;
            else {
                bool visto[MAX] = {false}; int ordenVisita[MAX], cantidad = 0;
                difundir(u->id, visto, ordenVisita, cantidad);
                cout << "Alcance de " << a1 << " (" << cantidad - 1 << "):";
                if (cantidad <= 1) cout << " (ninguno)";
                for (int i = 1; i < cantidad; i++) cout << " " << porId[ordenVisita[i]]->usuario;
                cout << endl;
            }
        }
        else if (orden == "llega") {
            NodoUsuario *u1 = buscar(raiz, a1), *u2 = buscar(raiz, a2);
            if (u1 == nullptr || u2 == nullptr) cout << "E: Cuenta no encontrada" << endl;
            else {
                bool visto[MAX] = {false}; int ordenVisita[MAX], cantidad = 0;
                difundir(u1->id, visto, ordenVisita, cantidad);
                if (u1->id != u2->id && visto[u2->id]) cout << "SI hay camino de " << a1 << " a " << a2 << endl;
                else cout << "NO hay camino de " << a1 << " a " << a2 << endl;
            }
        }
        else if (orden == "guardar") {
            ofstream f(ARCHIVO.c_str());
            if (!f) cout << "E: No se pudo guardar " << ARCHIVO << endl;
            else {
                int m = 0;
                f << nUsuarios << "\n";
                guardarPreorden(raiz, f);
                for (int i = 0; i < nUsuarios; i++) {
                    for (int j = 0; j < nUsuarios; j++) {
                        f << sigue[i][j]; if (j + 1 < nUsuarios) f << " ";
                        if (sigue[i][j] == 1) m++;
                    }
                    f << "\n";
                }
                f.close();
                if (!f) cout << "E: Error al escribir " << ARCHIVO << endl;
                else cout << "Guardado: " << nUsuarios << " cuentas y " << m << " seguimientos en " << ARCHIVO << endl;
            }
        }
        else if (orden == "cargar") {
            if (!cargarComunidad(raiz)) cout << "E: No se pudo cargar " << ARCHIVO << " (archivo inexistente o datos invalidos)" << endl;
            else {
                int m = 0;
                for (int i = 0; i < nUsuarios; i++)
                    for (int j = 0; j < nUsuarios; j++) if (sigue[i][j] == 1) m++;
                cout << "Cargado: " << nUsuarios << " cuentas y " << m
                     << " seguimientos | altura " << altura(raiz) << endl;
            }
        }
        else if (orden == "reporte") {
            int seg = 0, mut = 0;
            for (int a = 0; a < nUsuarios; a++) for (int b = 0; b < nUsuarios; b++) {
                if (sigue[a][b] == 1) seg++;
                if (a < b && sigue[a][b] == 1 && sigue[b][a] == 1) mut++;
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
