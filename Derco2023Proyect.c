#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MaxDistribuidoras 1000
#define buffer 50
// Central
struct Derco{
    struct NodoImportadora *Importadora;
    struct NodoDistribuidora *Distribuidora;
    struct NodoCliente *abbClientes;
};
//Nodo Circular
struct NodoImportadora{
    struct Importadora *datosImportadora;
    struct NodoImportadora *sig;
};
//Importadora
struct Importadora{
    char *rutImportadora[buffer];
    char *Ubicacion[buffer];
    int idImportadora;
    int CapacidadAlmacenamiento;
    int pLibreDistribuidora;
    struct Distribuidora**Distribuidoras;
};
struct NodoDistribuidora{
    struct Distribuidora *datosDistribuidora;
    struct NodoDistribuidora *sig;
};
struct Distribuidora{
    char nombre[buffer];
    char direccion[buffer];
    int idDistribuidora;
    int pLibreDesplazamiento;
    struct Desplazamiento**Desplazamientos;
};
struct Desplazamiento{
    int TiempoViaje;
    int idImportadora;
};
struct NodoCliente{
    struct Cliente *datosCliente;
    struct NodoCliente *izq, *der;
};
struct Cliente{
    char *nombre[buffer];
    char *rut[buffer];
    int IdCliente;
    struct NodoCompra *HeadCompras;
};
struct NodoCompra{
    struct Compra *datosCompra;
    struct NodoCompra *sig;
};
struct Compra{
    int idDistribuidora;
    int idCompra;
    int fechaCompra;
    int totalCompra;
    struct Producto **productosComprados;
    int pLibreProducto;
};
struct Producto{
    char *nombre[buffer];
    char *tipo[buffer];
    int precio;
    int id;
};
/////////////////////////////////////////////////////////////////////////////////////////////////
////______________INICIALICACIONES EN NULL DE PUNTEROS___________________////////////////
struct Derco *EMP = NULL;
struct Distribuidora *nuevaDistribuidora = NULL;
struct Importadora *nuevaImportadora = NULL;
struct Producto *nuevoProducto = NULL;
struct Cliente *nuevoCliente = NULL;
struct Compra *nuevaCompra =NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
///////__________________________Funciones para Mostrar_________________////////////////
void mostrarDesplazamiento(struct Desplazamiento **desplazamientosTotales, int pLibreDesplazamiento){
    int i;

    for(i = 0; i < pLibreDesplazamiento; i++){
        printf("%d\n", desplazamientosTotales[i]->TiempoViaje);
        printf("%d\n", desplazamientosTotales[i]->idImportadora);
        printf("\n");
    }
}
//Funcion para mostrar las importadora que se poseen
void mostrarImportadoras(struct NodoImportadora *head)
{
    struct NodoImportadora *rec = head;

    if (head == NULL)
    {
        return;
    }
    rec = head->sig;

    while(rec!=head)
    {
        printf("%c\n",rec->datosImportadora->rutImportadora);
        printf("%c\n",rec->datosImportadora->Ubicacion);
        printf("%d\n",rec->datosImportadora->idImportadora);
        printf("%d\n",rec->datosImportadora->CapacidadAlmacenamiento);
        printf("\n");
        rec = rec->sig;
    }
}
//mostrar distribuidoras
void mostrarDistribuidoras(struct NodoDistribuidora *distribuidora){
    struct NodoDistribuidora *rec = distribuidora;

    while (rec!=NULL){
        printf("%s\n",rec->datosDistribuidora->nombre);
        printf("%s\n",rec->datosDistribuidora->direccion);
        printf("%d\n",rec->datosDistribuidora->idDistribuidora);
        printf("\n");

        mostrarDesplazamiento(rec->datosDistribuidora->Desplazamientos, rec->datosDistribuidora->pLibreDesplazamiento);

    }
}
//Mostrar Productos de la compra
void mostrarProductos(struct  Producto **productosDisponibles, int pLibreProducto){
    int i;

    for(i=0; i<pLibreProducto; i++){
        printf("%s\n", productosDisponibles[i]->nombre);
        printf("%d\n", productosDisponibles[i]->precio);
        printf("%d\n", productosDisponibles[i]->id);
    }
}
//Mostrar las compreas
void mostrarCompras(struct NodoCompra *compras)
{
    struct NodoCompra *rec=compras;

    while(rec!=NULL)
    {
        printf("%s\n",rec->datosCompra->fechaCompra);
        printf("%d\n",rec->datosCompra->pLibreProducto);
        printf("%d\n",rec->datosCompra->idCompra);
        printf("%d\n",rec->datosCompra->idDistribuidora);
        mostrarProductos(rec->datosCompra->productosComprados,rec->datosCompra->pLibreProducto);
        rec=rec->sig;

    }
}
//mostrar los cliente
void mostrarClientes(struct NodoCliente *abbClientes){

    if(abbClientes != NULL)
    {
        printf("%s\n",abbClientes->datosCliente->nombre);
        printf("%s\n",abbClientes->datosCliente->rut);
        printf("%d\n",abbClientes->datosCliente->IdCliente);
        mostrarCompras(abbClientes->datosCliente->HeadCompras);// llamar a funcion que muestra compras

        mostrarClientes(abbClientes->izq);
        mostrarClientes(abbClientes->der);
    }
}
//Mostrar la arr distribuidora
void mostrarArrDistribuidora(struct  Distribuidora **distribuidoras, int pLibreDistribuidora){
    int i;

    for(i=0; i<pLibreDistribuidora; i++){
        printf("%s\n", distribuidoras[i]->nombre);
        printf("%s\n", distribuidoras[i]->direccion);
        printf("%d\n", distribuidoras[i]->idDistribuidora);
        printf("\n");

        mostrarDesplazamiento(distribuidoras[i]->Desplazamientos, distribuidoras[i]->pLibreDesplazamiento);

    }
}

////////////////////////////////////////////////////////////////////////////
//////________________Funciones Agregar_______________________________///////////
//agregar a arr distribuidoras
void AgregarArrDistribuidora(struct Distribuidora **distribuidoras, struct Distribuidora *nuevo, int pLibreDistribuidora)
{
    distribuidoras[pLibreDistribuidora] = nuevo;
    pLibreDistribuidora++;
}
//agregar a las importadoras nodo circular simple
void agregarImportadora(struct NodoImportadora** head, struct Importadora *nueva) {
    struct NodoImportadora* rec = *head;

    struct NodoImportadora *NuevaImp = (struct NodoImportadora*)malloc(sizeof(struct NodoImportadora));
    NuevaImp -> datosImportadora = nueva;

    if (*head == NULL) {
        *head = NuevaImp;
        NuevaImp->sig = NuevaImp;
    } // si esta vacia, hacerla head
    else
    {
        while(rec->sig != *head){
            rec = rec->sig;
        }
        rec->sig = NuevaImp;
        NuevaImp->sig = *head; //agregarla al final de la lista
    }
}
//Agregar distruibuidora nueva lista simplemente enlazada NodoDistribuidora
void AgregarDistribuidora(struct NodoDistribuidora **distribuidoras, struct Distribuidora *nueva)
{

    if((*distribuidoras) == NULL)
    {
        (*distribuidoras) = (struct NodoDistribuidora *)malloc(sizeof (struct NodoDistribuidora));
        (*distribuidoras)->datosDistribuidora = nueva;
        (*distribuidoras)->sig = NULL;
        return;
    }

    struct NodoDistribuidora *rec = *distribuidoras;

    while(rec != NULL)
    {
        if(rec->sig == NULL)
        {
            rec->sig->datosDistribuidora = nueva;
            rec->sig->sig = NULL;
            return;
        }
        rec = rec->sig;
    }
}
//Agregar nuevo en posicion pLibre
void AgregarDesplazamiento(struct Desplazamiento **desplazamientos, struct Desplazamiento *nuevo, int pLibreDesplazamiento)
{
    desplazamientos[pLibreDesplazamiento] = nuevo;
}
//Agregar a arbol binario de clientes
void AgregarCliente(struct NodoCliente **abbclientes, struct Cliente *nuevo)
{
    if((*abbclientes) == NULL)
    {
        (*abbclientes) = (struct NodoCliente *)malloc(sizeof(struct NodoCliente));
        (*abbclientes)->datosCliente = nuevo;
        (*abbclientes)->der = (*abbclientes)->izq = NULL;
        return;
    }
    else
    {
        if((*abbclientes)->datosCliente != nuevo)  //pregunta si ya existe
        {
            if((*abbclientes)->datosCliente->IdCliente > nuevo->IdCliente)
                AgregarCliente(&((*abbclientes)->izq),nuevo);
            else
                AgregarCliente(&((*abbclientes)->der),nuevo);
        }
    }
}
//Agregar compras a nodo simple enlazado
void AgregarCompra(struct NodoCompra **compras, struct Compra *nueva)
{

    if((*compras) == NULL)
    {
        (*compras) = (struct NodoCompra *)malloc(sizeof(struct NodoCompra));
        (*compras)->datosCompra = nueva;
        (*compras)->sig = NULL;
        return;
    }
    struct NodoCompra *rec = *compras;

    while(rec != NULL)
    {
        if(rec->sig == NULL)
        {
            rec->sig->datosCompra = nueva;
            rec->sig = NULL;
            return;
        }

        rec=rec->sig;
    }
}
//Agregar producto en pLIbre
void AgregarProducto(struct Producto **productos, struct Producto *nuevo, int pLibreProducto)
{
    productos[pLibreProducto] = nuevo;
}
//////////////////////////////////////////////////////////////////////////////////
//____________FUNCIONES BUSCAR_____________________///////////////////
//Buscar Importadora
struct Importadora* BuscarImportadora(struct NodoImportadora *head, int id) {
    struct NodoImportadora *rec = head;
    if (head == NULL) {
        return NULL;
    }
    else{
        do {
            if (rec->datosImportadora->idImportadora == id) {
                return rec->datosImportadora;
            }
            rec = rec->sig;
        }while (rec != head);
        return NULL;
    }
}
//buscar cliente
struct Cliente *BuscarCliente(struct NodoCliente *AbbClientes, int id)
{
    if(AbbClientes != NULL)
    {
        if (AbbClientes->datosCliente->IdCliente > id)
            BuscarCliente(AbbClientes->izq, id);
        else
        if (AbbClientes->datosCliente->IdCliente < id)
            BuscarCliente(AbbClientes->der, id);

        if(AbbClientes->datosCliente->IdCliente == id)
            return AbbClientes->datosCliente;

    }

    return NULL; //no se encontró al cliente
}
//buscar distribuidora
struct Distribuidora *BuscarDistribuidora(struct NodoDistribuidora *distribuidoras, int id)
{
    struct NodoDistribuidora *rec = distribuidoras;

    while (rec != NULL) {
        if (rec->datosDistribuidora->idDistribuidora == id)
            return rec->datosDistribuidora;
        rec = rec->sig;
    }
    return NULL;
}
//buscar dustribuidora en array
struct Distribuidora *BuscarArrDistribuidora(struct Distribuidora **distribuidoras, int id, int pLibreDistribuidora)
{
    int i;
    for(i=0; i<pLibreDistribuidora; i++)
    {
        if(distribuidoras[i]->idDistribuidora == id)
        {
            return distribuidoras[i];
        }
    }
    return NULL; //distribuidora no encontrada
}
//buscar el desplzamiente
struct Desplazamiento *BuscarDesplazamiento(struct Desplazamiento **desplazamientos, int id, int pLibreDesplazamiento)
{
    int i;

    for(i=0; i<pLibreDesplazamiento; i++)
    {
        if(desplazamientos[i]->idImportadora == id)
        {
            return desplazamientos[i];
        }
    }
    return NULL; //id de importadora no encontrada
}
//buscar una compra en un cliente
struct Compra *BuscarCompra(struct NodoCompra *comprasCliente, int id)
{
    struct NodoCompra *rec = comprasCliente;

    while(rec != NULL)
    {
        if(rec->datosCompra->idCompra == id)
            return rec->datosCompra;
        rec=rec->sig;
    }

    return NULL;
}
//buscar un producto
struct Producto *BuscarProducto(struct Producto **productos, int id, int pLibreProducto)
{
    int i;

    for(i = 0; i < pLibreProducto; i++)
    {
        if(productos[i]->id == id)
        {
            return productos[i];
        }
    }
    return NULL; //producto no encontrado
}
////////////////////////////////////////////////////////////////////////////////////
//_______________-_________funciones borrar______________////////////

//Elimina la importadora
void EliminarImportadora(struct NodoImportadora **head, int id) {
    if (*head == NULL) {
        return; // si es el head
    }

    struct NodoImportadora* rec = *head;
    struct NodoImportadora* ant = NULL;

    while(rec->datosImportadora->idImportadora != id)
    {
        ant = rec;
        rec = rec->sig;

        if (rec == *head) {
            return; /* si no lo encontramos en la lista */
        }
    }

    if (rec->datosImportadora != NULL && rec->datosImportadora->idImportadora == id) {
        struct NodoImportadora* datosHead= rec->datosImportadora;
        struct NodoImportadora* datosAnt = datosHead;
        while (datosAnt->sig != datosHead){
            datosAnt = datosAnt->sig;
        }
        rec->datosImportadora = datosHead->sig;
        datosAnt->sig = datosHead->sig;
    }
    if (ant != NULL){
        ant->sig = rec->sig;
    }

    else{
        *head = rec->sig;
    }
}
//Elimina del array una distribuidora
void EliminarArrDistribuidora(struct Distribuidora **distribuidoras, int pLibreDistribuidora, int id)
{
    int i;
    for(i=0; i<pLibreDistribuidora; i++)
    {
        if(distribuidoras[i]->idDistribuidora == id)
        {
            distribuidoras[i] = NULL;
            distribuidoras[i]=distribuidoras[pLibreDistribuidora-1];
            return;
        }
    }
}
//elimina una distribuidora
void EliminarDistribuidora(struct NodoDistribuidora **distribuidora, int id)
{
    struct NodoDistribuidora *rec = *distribuidora;

    if ((*distribuidora)->datosDistribuidora->idDistribuidora == id)
    {
        (*distribuidora)->datosDistribuidora = NULL;
        (*distribuidora) = (*distribuidora)->sig;
        return;
    }

    while(rec != NULL)
    {
        if(rec->sig->datosDistribuidora->idDistribuidora == id && rec->sig->sig == NULL)
        {
            rec->sig = NULL;
            return;
        }

        if(rec->sig->datosDistribuidora->idDistribuidora == id)
        {
            rec->sig = rec->sig->sig;
            return;
        }

        rec=rec->sig;
    }
}
//elimina un desplzamiento
void EliminarDesplazamiento(struct Desplazamiento **desplazamientos, int pLibreDesplazamiento, int id)
{
    int i;

    for(i=0; i<pLibreDesplazamiento; i++)
    {
        if(desplazamientos[i]->idImportadora == id) //si se encuentra la id
        {
            desplazamientos[i] = NULL;
            desplazamientos[i]=desplazamientos[pLibreDesplazamiento-1];
            return;
        }
    }
}
//elimina un cliente de un abb
void EliminarCliente (struct NodoCliente **abbClientes, int idClienteEliminar)
{
    struct Cliente *aux = NULL;

    if((*abbClientes) == NULL)
        return;
    if((*abbClientes)->datosCliente->IdCliente < idClienteEliminar)
        EliminarCliente(&(*abbClientes)->der, idClienteEliminar);
    else
    {
        if ((*abbClientes)->datosCliente->IdCliente > idClienteEliminar)
            EliminarCliente(&(*abbClientes)->izq, idClienteEliminar);
        else
        {
            if ((*abbClientes)->datosCliente->IdCliente == idClienteEliminar)
            {
                aux = (*abbClientes)->datosCliente;
                if((*abbClientes)->izq == NULL)
                    (*abbClientes) = (*abbClientes)->der;
                else
                {
                    if ((*abbClientes)->der == NULL)
                        (*abbClientes) = (*abbClientes)->izq;
                }
                free(aux);
            }
        }
    }

}
//elimina una compra
void EliminarCompra(struct NodoCompra **compra, int id)
{


    if ((*compra)->datosCompra->idCompra == id)
    {
        (*compra)->datosCompra = NULL;
        (*compra) = (*compra)->sig;
        return;
    }
    struct NodoCompra *rec = *compra;
    while(rec != NULL)
    {
        if(rec->sig->datosCompra->idCompra == id && rec->sig->sig == NULL)
        {
            rec->sig = NULL;
            return;
        }

        if(rec->sig->datosCompra->idCompra == id)
        {
            rec->sig = rec->sig->sig;
            return;
        }

        rec=rec->sig;
    }
}
//eliminar producto de una compra de un cliente
void EliminarProductoDeUnaCompraDeUnCliente(struct Producto **productos, int pLibreProductos, int idProducto)
{
    int i;

    for(i=0; i<pLibreProductos; i++)
    {
        if(productos[i]->id == idProducto)
        {
            productos[i] = NULL;
            productos[i]=productos[pLibreProductos-1];
            return;
        }
    }
}
////////////////////////////////////////////////////////////////////////////
//______________________Funciones modificar__________________///////////////(Les otorga nueva ID)
//Funcion que modifica la importadora enviada pra otorgarle una nueva ID.
void ModificarImportadora(struct Importadora **importadora, int NuevaId)
{
    (*importadora)->idImportadora = NuevaId;
}
//funcion que modifica la distribuidora dentro del AArray
void ModificarArrDistribuidora(struct Distribuidora **distribuidoras, int NuevaId)
{
    (*distribuidoras)->idDistribuidora = NuevaId;
    return;
}
//funcion que modifica la distribuidora
void ModificarDistribuidora(struct NodoDistribuidora **distribuidora, int NuevaId)
{
    (*distribuidora)->datosDistribuidora->idDistribuidora=NuevaId;
}
//funcion que modifica el desplzamiento
void ModificarDesplazamiento(struct Desplazamiento **desplazamientos, int NuevaId)
{
    (*desplazamientos)->idImportadora = NuevaId;

}
//funcion que modifica un cliente
void ModificarCliente(struct Cliente **cliente, int NuevaId)
{
    (*cliente)->IdCliente = NuevaId;
    return;
}
//funcion que modifica una compra
void ModificarCompra(struct Compra **compra, int NuevaId)
{
    (*compra)->idCompra = NuevaId;
    return;
}
//funcion que modifica el producto
void ModificarProducto(struct Producto **producto, int NuevaId)
{
    (*producto)->id = NuevaId;
    return;
}
///////////////////////////////////////////////////////////////////
//________________Funciones Propias________________//////////////
//cliente con mayor gasto
int CalcularCompraCliente(struct NodoCompra *compras)
{
    int suma = 0;
    struct NodoCompra *rec = compras;
    while(rec != NULL){
        suma += rec->datosCompra->totalCompra;
        rec = rec->sig;
    }
    return suma;
}
struct Cliente *clienteMayorGasto(struct NodoCliente *abbClientes)
{
    struct Cliente *mayor = NULL;

    if(abbClientes != NULL)
    {
        if (mayor == NULL || CalcularCompraCliente(abbClientes->datosCliente->HeadCompras) > CalcularCompraCliente(mayor->HeadCompras))
            mayor = abbClientes->datosCliente;

        clienteMayorGasto(abbClientes->izq);
        clienteMayorGasto(abbClientes->der);

    }
    return mayor;
}
//
//busca la distribuidora con mas ventas
int ContarProductosVendidosACliente(struct NodoCompra *compras, int idDistribuidora)
{
    int total=0;
    struct NodoCompra *rec = compras;

    while(rec!=NULL)
    {
        if(rec->datosCompra->idDistribuidora == idDistribuidora)
            total+=rec->datosCompra->pLibreProducto-1;
        rec=rec->sig;
    }

    return total;
}
int ContarProductosVendidosAclientesTotal(struct NodoCliente *Abbclientes, int idDistribuidora)
{
    int total=0;

    if(Abbclientes != NULL)
    {
        total+= ContarProductosVendidosACliente(Abbclientes->datosCliente->HeadCompras, idDistribuidora);
        ContarProductosVendidosAclientesTotal(Abbclientes->izq, idDistribuidora);
        ContarProductosVendidosAclientesTotal(Abbclientes->der,idDistribuidora);
    }

    return total;
}
struct Distribuidora *distribuidoraConMasVentas(struct NodoDistribuidora *distribuidora, struct NodoCliente *abb)
{
    struct  NodoDistribuidora *rec = distribuidora;
    struct Distribuidora *mayor = NULL;
    while(rec != NULL)
    {
        if (mayor == NULL || ContarProductosVendidosAclientesTotal(abb, rec->datosDistribuidora->idDistribuidora) >
                             ContarProductosVendidosAclientesTotal(abb, mayor->idDistribuidora))
            mayor = rec->datosDistribuidora;
        rec=rec->sig;
    }

    return mayor;
}
//
//funcion que ordena los productos por id
void OrdenarProductosPorId(struct Producto **productos, int pLibreProducto){

    int i,j;
    struct Producto *aux = NULL;
    for(i=0; i<pLibreProducto-1; i++)
        for(j=0; j<pLibreProducto-1; j++)
        {
            if(productos[j]->id > productos[j+1]->id)
            {
                aux = productos[j];
                productos[j] = productos[j+1];
                productos[j+1] = aux;
            }
        }
    return;
}
////_______funciones propias NUEVAS_______________//////////////
struct Importadora *ImportadoraConMayorAlmacenamiento(){
    struct Derco*aux=EMP;
    int MayorAlmacenamiento=0;
    struct NodoImportadora*ImportadorasHead;
    struct Importadora*ImportadoraMayor;
    struct NodoImportadora*rec;
    ImportadorasHead=aux->Importadora;
    if(ImportadorasHead!=NULL){
        rec=ImportadorasHead;
        do{
            if(MayorAlmacenamiento==0||MayorAlmacenamiento<rec->datosImportadora->CapacidadAlmacenamiento){
                MayorAlmacenamiento=rec->datosImportadora->CapacidadAlmacenamiento;
                ImportadoraMayor=rec->datosImportadora;
            }
            rec=rec->sig;
        }while(rec->sig!=ImportadorasHead);
        if(ImportadoraMayor!=NULL){
            return ImportadoraMayor;
        }
        else return NULL;
    }
    else return NULL;

}

//////////////////////////////////////////////////////
// MENÚ DE TEXTO
///////////////////////////////////////////
void menuPrincipal();
///////////////////////////////////////////////////
void menuOtros(){
    char eleccion;
    int idCliente ,idCompra, idDistribuidora, idImportadora, idProducto;
    printf("PRESIONE 'a' PARA BUSCAR CLIENTE QUE MAS GASTA EN SUS PRODUCTOS\n");
    printf("PRESIONE 'b' PARA BUSCAR LA DISTRIBUIDORA CON MAS VENTAS\n");
    printf("PRESIONE 'c' PARA ORDENAR LOS PRODUCTOS DE UNA COMPRA DE UN CLIENTE POR ID\n");
    printf("PRESIONE 'd' PARA MOSTRAR LA IMPORTADORA CON MAYOR STOCK DE PRODUCTOS\n");
    eleccion = getchar();
    if (eleccion == 'a'){
        if(clienteMayorGasto(EMP->abbClientes) !=NULL)
        {
            printf("EL ID DEL CLIENTE QUE MAS GASTA EN SUS PRODUCTOS ES %d",clienteMayorGasto(EMP->abbClientes)->IdCliente);
        }
        else{
            printf("NO EXISTE");
        }
    }else if(eleccion== 'b'){
        if(distribuidoraConMasVentas(EMP->Distribuidora,EMP->abbClientes)!=NULL)
        {
            printf("EL ID DE LA DISTRIBUIDORA CON MAYOR VENTAS ES %d",distribuidoraConMasVentas(EMP->Distribuidora,EMP->abbClientes)->idDistribuidora);
        }
        else{
            printf("NO EXISTE");
        }

    }else if(eleccion == 'c'){
        printf("INGRESA EL ID DE CLIENTE Y EL ID DE LA COMPRA PARA ORDENAR LOS PRODUCTOS");
        scanf("%d %d",&idCliente,&idCompra);
        if(BuscarCliente(EMP->abbClientes,idCliente)!=NULL && BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) != NULL)
        {
            OrdenarProductosPorId(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->pLibreProducto);
            printf("PRODUCTOS FUERON ORDENADOS");
        }
        else{
            printf("PRODUCTOS NO SE PUDIERON ORDENAR");
        }
    }else if(eleccion == 'd'){
        struct Importadora *aux;
        aux = ImportadoraConMayorAlmacenamiento();
        if(aux == NULL){
            printf("Rut: %c \n", aux->rutImportadora);
            printf("Ubicacion: %c \n", aux->Ubicacion);
            printf("idImportadora: %d \n", aux->idImportadora);
            printf("Capacidad Almacenamiento: %d", aux->CapacidadAlmacenamiento);
        }else{
            printf("No hay Importadoras");
        }

    }else{
        return;
    }
}

void menuListarElemento(){
    char eleccion;
    printf("PRESIONE 'a' PARA LISTAR LAS IMPORTADORAS\n");
    printf("PRESIONE 'b' PARA LISTAR LAS DISTRIBUIDORAS\n");
    printf("PRESIONE 'c' PARA LISTAR LOS CLIENTES\n");
    eleccion = getchar();
    if(eleccion == 'a'){
        mostrarImportadoras(EMP->Importadora);
    }else if(eleccion == 'b'){
        mostrarDistribuidoras(EMP->Distribuidora);
    }else if(eleccion == 'c'){
        mostrarClientes(EMP->abbClientes);
    }else{
        return;
    }
}

void menuModificarElemento(){
    char eleccion;
    int idCliente ,idCompra, idDistribuidora, idImportadora, idProducto;
    int nuevaIdImportadora;
    int nuevaIdDistribuidora;
    int nuevaIdCliente;
    printf("PRESIONE 'a' PARA MODIFICAR LA ID DE UNA IMPORTADORA\n");
    printf("PRESIONE 'b' PARA MODIFICAR LA ID DE UNA DISTRIBUIDORA\n");
    printf("PRESIONE 'c' PARA MODIFICAR LA ID DE UN PRODUCTO DE LA COMPRA DE UN CLIENTE\n");
    printf("PRESIONE 'd' PARA MODIFICAR LA ID DE UN CLIENTE\n");
    printf("PRESIONE 'e' PARA MODIFICAR LA ID DE UNA DISTRIBUIDORA DE UNA IMPORTADORA\n");
    printf("PRESIONE 'f' PARA MODIFICAR LA ID DE LA COMPRA DE UN CLIENTE\n");
    eleccion = getchar();
    if(eleccion == 'a'){
        printf("INGRESA LA ID DE LA IMPORTADORA Y LA NUEVA ID QUE TENDRA");
        scanf("%d %d",&idImportadora,&nuevaIdImportadora);
        if(BuscarImportadora(EMP->Importadora,idImportadora) !=NULL && BuscarImportadora(EMP->Importadora,nuevaIdImportadora) == NULL)
        {
            ModificarImportadora(BuscarImportadora(EMP->Importadora,idImportadora),nuevaIdImportadora);
            printf("LA ID HA SIDO CAMBIADA CORRECTAMENTE");
        }
        else
        {
            printf("LA ID NO SE HA PODIDO MODIFICAR");
        }
    }else if(eleccion == 'b'){
        printf("INGRESA LA ID DE LA DISTRIBUIDORA Y LA NUEVA ID QUE TENDRA");
        scanf("%d %d",&idDistribuidora,nuevaIdDistribuidora);
        if(BuscarDistribuidora(EMP->Distribuidora,idDistribuidora) != NULL && BuscarDistribuidora(EMP->Distribuidora,nuevaIdDistribuidora) == NULL)
        {
            ModificarDistribuidora(BuscarDistribuidora(EMP->Distribuidora,idDistribuidora),nuevaIdDistribuidora);
            printf("LA ID HA SIDO CAMBIADA CON EXITO");
        }
        else{
            printf("LA ID NO SE HA PODIDO CAMBIAR");
        }
    }else if(eleccion == 'c'){
        printf("INGRESA EL ID DEL CLIENTE, DE LA COMPRA, DEL PRODUCTO Y LA NUEVA ID");
        scanf("%d %d %d %d",&idCliente,&idCompra,&idProducto,&nuevoProducto->id);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL && BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) != NULL && BuscarProducto(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,idProducto,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->pLibreProducto) != NULL && BuscarProducto(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,nuevoProducto->id,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->pLibreProducto) == NULL)
        {
            ModificarProducto(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,nuevoProducto->id);
            printf("ID DE PRODUCTO MODIFICADO CON EXITO");
        }
        else{
            printf("ID DE PRODUCTO NO SE PUDO MODIFICAR");
        }
    }else if(eleccion == 'd'){
        printf("INGRESA EL ID DEL CLIENTE Y LA NUEVA ID");
        scanf("%d %d",&idCliente,&nuevaIdCliente);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL && BuscarCliente(EMP->abbClientes,nuevaIdCliente) == NULL)
        {
            ModificarCliente(BuscarCliente(EMP->abbClientes,idCliente),nuevaIdCliente);
            printf("ID DE CLIENTE HA SIDO CAMBIADA CON EXITO");
        }else
        {
            printf("EL ID NO SE HA PODIDO CAMBIAR");
        }
    }else if(eleccion == 'e'){
        printf("INGRESA EL ID DE LA IMPORTADORA, DE LA DISTRIBUIDORA A MODIFICAR Y LA NUEVA");
        scanf("%d %d %d",&idImportadora,&idDistribuidora,&nuevaIdDistribuidora);
        if(BuscarImportadora(EMP->Importadora,idImportadora) != NULL && BuscarArrDistribuidora(BuscarImportadora(EMP->Importadora,idImportadora)->Distribuidoras,BuscarImportadora(EMP->Importadora,idImportadora)->pLibreDistribuidora,idDistribuidora) != NULL && BuscarArrDistribuidora(BuscarImportadora(EMP->Importadora,idImportadora)->Distribuidoras,BuscarImportadora(EMP->Importadora,idImportadora)->pLibreDistribuidora,nuevaIdDistribuidora )==NULL)
        {
            ModificarDistribuidora(BuscarDistribuidora(EMP->Distribuidora,idDistribuidora),nuevaIdDistribuidora);
            printf("ID DE LA DISTRIBUIDORA HA SIDO CAMBIADO");
        }
        else{
            printf("NO SE HA PODIDO MODIFICAR EL ID DE LA DISTRIBUIDORA");
        }
    }else if(eleccion == 'f'){
        printf("INGRESA EL ID DEL CLIENTE, DE LA COMPRA Y EL NUEVO ID");
        scanf("%d %d %d",&idCliente,&idCompra,&nuevaCompra->idCompra);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL && BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) !=NULL && BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,nuevaCompra->idCompra) != NULL)
        {
            ModificarCompra(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra),nuevaCompra->idCompra);
            printf("ID DE COMPRA SE HA MODIFICADO CON EXITO");
        }
        else
        {
            printf("LA COMPRA NO SE HA PODIDO MODIFICAR");
        }
    }else{
        return;
    }
}

void menuBuscarElemento(){
    char eleccion;
    int idCliente ,idCompra, idDistribuidora, idImportadora, idProducto;
    printf("PRESIONE 'a' PARA BUSCAR UNA IMPORTADORA\n");
    printf("PRESIONE 'b' PARA BUSCAR UNA DISTRIBUIDORA\n");
    printf("PRESIONE 'c' PARA BUSCAR UN PRODUCTO DE LA COMPRA DE UN CLIENTE\n");
    printf("PRESIONE 'd' PARA BUSCAR UN CLIENTE\n");
    printf("PRESIONE 'e' PARA BUSCAR UNA DISTRIBUIDORA DE UNA IMPORTADORA\n");
    printf("PRESIONE 'f' PARA BUSCAR UNA COMPRA DE UN CLIENTE\n");
    eleccion = getchar();
    if(eleccion == 'a'){
        printf("INGRESA EL ID DE LA IMPORTADORA QUE DESEAS BUSCAR");
        scanf("&d",&idImportadora);
        if(BuscarImportadora(EMP->Importadora,idImportadora) != NULL)
        {
            printf("IMPORTADORA ENCONTRADA");
        }
        else
        {
            printf("IMPORTADORA NO ENCONTRADA");
        }
    }else if(eleccion == 'b'){
        printf("INGRESA EL ID DE LA DISTRIBUIDORA");
        scanf("%d",&idDistribuidora);
        if(BuscarDistribuidora(EMP->Distribuidora,idDistribuidora) != NULL)
        {
            printf("DISTRIBUIDORA ENCONTRADA");
        }
        else
        {
            printf("DISTRIBUIDORA NO ENCONTRADA");
        }
    }else if(eleccion == 'c'){
        printf("INGRESA ID DEL CLIENTE, ID DE LA COMPRA Y EL ID DEL PRODUCTO");
        scanf("%d %d %d",&idCliente,&idCompra,&idProducto);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL && BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) != NULL && BuscarProducto(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,idProducto,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->pLibreProducto) != NULL)
        {
            EliminarProductoDeUnaCompraDeUnCliente(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->pLibreProducto,idProducto);
            printf("PRODUCTO ELIMINADO CON EXITO");
        }
        else{
            printf("NO SE PUDO ELIMINAR EL PRODUCTO");
        }
    }else if(eleccion == 'd'){
        printf("INGRESA LA ID DEL CLIENTE");
        scanf("%d",&idCliente);
        if(BuscarCliente(EMP->abbClientes,idCliente)!=NULL)
        {
            printf("CLIENTE ENCONTRADO CON EXITO");
        }
        else
        {
            printf("CLIENTE NO ENCONTRADO");
        }
    }else if(eleccion == 'e'){
        printf("INGRESA ID DE LA IMPORTADORA Y DE LA DISTRIBUIDORA");
        scanf("%d %d",&idImportadora,&idDistribuidora);

        if(BuscarImportadora(EMP->Importadora,idImportadora) != NULL &&
           BuscarArrDistribuidora(BuscarImportadora(EMP->Importadora,idImportadora)->Distribuidoras,BuscarImportadora(EMP->Importadora,idImportadora)->pLibreDistribuidora,idDistribuidora) != NULL)
        {
            printf("DISTRIBUIDORA ENCONTRADA");
        }
        else{
            printf("DISTRIBUIDORA NO ENCONTRADA");
        }
    }else if(eleccion == 'f'){
        printf("INGRESA ID DEL CLIENTE Y DE LA COMPRA");
        scanf("%d %d",&idCliente,&idCompra );
        if(BuscarCliente(EMP->abbClientes,idCliente)!=NULL && BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) != NULL)
        {
            printf("COMPRA ENCONTRADA");
        }
        else{
            printf("COMPRA NO ENCONTRADA");
        }
    }else{
        return;
    }
}

void menuEliminarElemento(){
    int eleccion  , idCliente ,idCompra, idDistribuidora, idImportadora, idProducto;
    printf("PRESIONE '0' PARA ELIMINAR UNA IMPORTADORA\n");
    printf("PRESIONE '1' PARA ELIMINAR UNA DISTRIBUIDORA\n");
    printf("PRESIONE '2' PARA ELIMINAR UN PRODUCTO A LA COMPRA DE UN CLIENTE\n");
    printf("PRESIONE '3' PARA ELIMINAR UN CLIENTE\n");
    printf("PRESIONE '4' PARA ELIMINAR UNA DISTRIBUIDORA DE UNA IMPORTADORA\n");
    printf("PRESIONE '5' PARA ELIMINAR UNA COMPRA DE UN CLIENTE\n");
    eleccion= getchar();
    if(eleccion == '0'){
        printf("INGRESE EL ID DE LA IMPORTADORA QUE DESEA ELIMINAR");
        scanf("%d",&idImportadora);

        if(BuscarImportadora(EMP->Importadora, idImportadora) != NULL)  //planta existe, se puede eliminar
        {
            EliminarImportadora(EMP->Importadora,idImportadora);
            printf("IMPORTADORA SE HA ELIMINADO CORRECTAMENTE");
        }
        else{
            printf("LA IMPORTADORA NO EXISTE, INGRESE UN ID VALIDO");
        }
    }else if(eleccion == '1') {
        printf("INGRESE EL ID DE LA DISTRIBUIDORA QUE DESEA ELIMINAR");
        scanf("%d", &idDistribuidora);

        if (BuscarDistribuidora(EMP->Distribuidora, idDistribuidora) != NULL) {
            //EliminarArrDistribuidora(EMP->Distribuidora, idDistribuidora);
            printf("DISTRIBUIDORA SE HA ELIMINADO CORRECTAMENTE");
        } else {
            printf("LA DISTRIBUIDORA NO EXISTE, INGRESE UN ID VALIDO");
        }
    }else if(eleccion == '2'){
        printf("INGRESE EL ID DEL CLIENTE");
        scanf("%d",&idCliente);

        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL)   //cliente existe
        {
            printf("INGRESE EL ID DE LA COMPRA");
            scanf("%d",&idCompra);
            if(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) != NULL) //compra existe
            {
                printf("INGRESE EL ID DEL PRODUCTO");
                scanf("%d",&idProducto);
                if(BuscarProducto(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCliente)->productosComprados,idProducto,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCliente)->pLibreProducto)!=NULL) //PRODUCTO EXISTE
                {
                    EliminarProductoDeUnaCompraDeUnCliente(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCliente)->productosComprados,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCliente)->pLibreProducto,idProducto);
                    printf("EL PRODUCTO SE HA ELIMINADO CORRECTAMENTE DE LA COMPRA DEL CLIENTE");
                }
                else
                {
                    printf("EL PRODUCTO NO EXISTE EN LA COMPRA DEL CLIENTE");
                }

            }
            else
            {
                printf("LA COMPRA NO EXISTE");
            }
        }
        else
        {
            printf("CLIENTE NO EXISTE");
        }
    }else if(eleccion == '3'){
        printf("INGRESA EL ID DEL CLIENTE");
        scanf("%d",&idCliente);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL) //CLIENTE EXISTE
        {
            EliminarCliente(EMP->abbClientes,idCliente);
            printf("CLIENTE ELIMINADO CON EXITO");
        }
        else
        {
            printf("CLIENTE NO EXISTE, INGRESA ID VALIDO");
        }
    }else if(eleccion == '4'){
        printf("INGRESA EL ID DE LA IMPORTADORA Y DE LA DISTRIBUIDORA ");
        scanf("%d %d",&idImportadora,&idDistribuidora);
        if(BuscarImportadora(EMP->Importadora,idImportadora) != NULL && BuscarArrDistribuidora(BuscarImportadora(EMP->Importadora,idImportadora)->Distribuidoras,BuscarImportadora(EMP->Importadora,idImportadora)->pLibreDistribuidora,idDistribuidora) != NULL)
        {
            EliminarArrDistribuidora(BuscarImportadora(EMP->Importadora,idImportadora)->Distribuidoras,BuscarImportadora(EMP->Importadora,idImportadora)->pLibreDistribuidora, idDistribuidora);
            printf("LA DISTRIBUIDORA SE HA ELIMINADO CORRECTAMENTE DE LA IMPORTADORA");
        } else{
            printf("NO SE HA PODIDO ELIMINAR LA DISTRIBUIDORA");
        }
    }else if(eleccion == '5'){
        printf("INGRESA EL ID DEL CLIENTE Y EL ID DE LA COMPRA A ELIMINAR");
        scanf("%d %d",&idCliente,&idCompra);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL && BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) != NULL) //ambos existen
        {
            EliminarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra);
            printf("LA COMPRA SE HA ELIMINADO CORRECTAMENTE DEL CLIENTE");
        }
        else{
            printf("LA COMPRA NO SE PUDO ELIMINAR");
        }
    }else {
        return;
    }
}

void menuAgregarElemento(){
    char eleccion;
    int idCliente ,idCompra, idDistribuidora, idImportadora, idProducto;
    printf("PRESIONE 'i' PARA AGREGAR UNA IMPORTADORA\n");
    printf("PRESIONE 'd' PARA AGREGAR UNA DISTRIBUIDORA\n");
    printf("PRESIONE 'p' PARA AGREGAR UN PRODUCTO A LA COMPRA DE UN CLIENTE\n");
    printf("PRESIONE 'c' PARA AGREGAR UN CLIENTE\n");
    printf("PRESIONE 'x' PARA AGREGAR UNA DISTRIBUIDORA A UNA IMPORTADORA\n");
    printf("PRESIONE 'n' PARA AGREGAR UNA COMPRA A UN CLIENTE\n");
    eleccion = getchar();
    if(eleccion == 'i'){
        if(BuscarImportadora(EMP->Importadora, nuevaImportadora->idImportadora) == NULL){
            AgregarDistribuidora(EMP->Importadora,nuevaImportadora);
            printf("LA IMPORTADORA SE AGREGO CON EXITO");
        }else{
            printf("LA IMPORTADORA YA EXISTE");
        }
    }else if(eleccion == 'd'){
        if(BuscarDistribuidora(EMP->Distribuidora, nuevaDistribuidora->idDistribuidora)){
            AgregarDistribuidora(EMP->Distribuidora, nuevaDistribuidora);
            printf("LA DISTRIBUIDORA SE AGREGO CORRECTAMENTE");
        }else{
            printf("LA DISTRIBUIDORA YA EXISTE");
        }
    }else if(eleccion == 'p'){
        printf("INGRESE EL ID DEL CLIENTE");
        scanf("%d",&idCliente);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL){ //cliente existe
            printf("INGRESE EL ID DE LA COMPRA");
            scanf("%d",&idCompra);
            if(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra) != NULL){
                printf("INGRESE EL ID DEL PRODUCTO");
                scanf("%d",&idProducto);
                if(BuscarProducto(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,idProducto,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->pLibreProducto) == NULL) //producto no existe, se puede agregar
                {
                    AgregarProducto(BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->productosComprados,nuevoProducto,BuscarCompra(BuscarCliente(EMP->abbClientes,idCliente)->HeadCompras,idCompra)->pLibreProducto);
                    printf("EL PRODUCTO SE AGREGO CON EXITO");
                }else{
                    printf("EL PRODUCTO YA EXISTE EN LA COMPRA DEL CLIENTE");
                }
            }else{
                printf("LA COMPRA NO EXISTE, INGRESE UN ID DE COMPRA VALIDO");
            }
        }else{
            printf("EL CLIENTE NO EXISTE, INGRESE UN ID DE CIELNTE VALIDO");
        }
    }else if(eleccion == 'c'){
        if(BuscarCliente(EMP->abbClientes,nuevoCliente->IdCliente) == NULL){
            AgregarCliente(EMP->abbClientes,nuevoCliente);
            printf("CLIENTE SE HA AGREGADO CON EXITO");
        }else{
            printf("EL CLIENTE YA EXISTE");
        }
    }else if(eleccion == 'x'){
        printf("INGRESE EL ID DE LA IMPORTADORA");
        scanf("%d",&idImportadora);
        if(BuscarImportadora(EMP->Importadora, idImportadora) != NULL){//planta productiva existe
            if (BuscarImportadora(EMP->Importadora, idImportadora)->pLibreDistribuidora < MaxDistribuidoras && BuscarArrDistribuidora(BuscarImportadora(EMP->Importadora, idImportadora)->Distribuidoras,BuscarImportadora(EMP->Importadora, idImportadora)->pLibreDistribuidora, nuevaDistribuidora->idDistribuidora) == NULL) {//hay espacio para agregar y la planta de distribucion no existe en el arreglo
                AgregarArrDistribuidora(BuscarImportadora(EMP->Importadora, idImportadora)->Distribuidoras,nuevaDistribuidora,BuscarImportadora(EMP->Importadora,idImportadora)->pLibreDistribuidora);
                printf("LA DISTRIBUIDORA SE HA AGREGADO CORRECTAMENTE A LA IMPORTADORA");
            }else{
                printf("LA DISTRIBUIDORA NO SE PUDO AGREGAR");
            }
        }else{
            printf("LA IMPORTADORA NO EXISTE, INGRESA UN ID VALIDO");
        }
    }else if(eleccion == 'n'){
        printf("INGRESA EL ID DEL CLIENTE");
        scanf("%d",&idCliente);
        if(BuscarCliente(EMP->abbClientes,idCliente) != NULL && BuscarCompra(BuscarCliente(EMP->abbClientes, idCliente)->HeadCompras,nuevaCompra->idCompra)==NULL){//cliente existe y compra no existe asi que se puede agregar
            AgregarCompra(BuscarCliente(EMP->abbClientes, idCliente)->HeadCompras,nuevaCompra);
            printf("LA COMPRA SE HA AGREGADO CORRECTAMENTE");
        }else{
            printf("LA COMPRA NO HA PODIDO SER AGREGADA, POR QUE INGRESASTE UN ID INVALIDO O LA COMPRA YA EXISTE");
        }
    }else{
        return;
    }
    return;
}

void menuPrincipal() {
    char opcion;
    bool cicl = true;
    while (cicl == true) {
        printf("PRESIONE 'a' PARA AGREGAR ELEMENTOS\n");
        printf("PRESIONE 'e' PARA ELIMINAR ELEMENTOS\n");
        printf("PRESIONE 'b' PARA BUSCAR ELEMENTOS\n");
        printf("PRESIONE 'l' PARA LISTAR ELEMENTOS\n");
        printf("PRESIONE 'm' PARA MODIFICAR ELEMENTOS\n");
        printf("PRESIONE 'o' PARA REALIZAR OTRA ACCIÓN\n");
        printf("PRESIONE 'cualquier otra letra' PARA SALIR\n");
        opcion = getchar();
        opcion = tolower(opcion);
        if(opcion == 'a'){
            getchar();
            menuAgregarElemento();
        }else if(opcion == 'e'){
            getchar();
            menuEliminarElemento();
        }else if(opcion == 'b'){
            getchar();
            menuBuscarElemento();
        }else if(opcion == 'm'){
            getchar();
            menuModificarElemento();
        }else if(opcion == 'l'){
            getchar();
            menuListarElemento();
        }else if(opcion == 'o'){
            getchar();
            menuOtros();
        }else{
            cicl = false;
        }
        getchar();
    }
}


int main() {
    menuPrincipal();
    return 0;
}