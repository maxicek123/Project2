/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
   assert(c != NULL);
   assert(cap >= 0);

   if(c != NULL && cap >= 0)
    {   //alokujeme pamat o velkosti objektu * zvolena kapacita
        //nastavenie novej capacity
        c->obj = malloc(sizeof(struct obj_t) * cap);
        c->capacity = cap;
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{   //uvolnime pamat, to znamena ze odstranime vsetky objekty zhluku
    //inicialuzejeme ho na novy ale prazdny (cap 0)
    free(c->obj);
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    //ked sa objekt nezmesti ...
    if(c->size >= c->capacity)
    {
        //volame resize a zvacsujeme po jednom dokedy nebude dost miesta
        resize_cluster(c, c->capacity+1);
    }
    else 
    {
        c->obj[c->size] = obj;
        c->size = c->size + 1;
    }
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for(int i = 0; i < c2->size; i++)
    {
        append_cluster(c1, c2->obj[i]);
        i++;
    }

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
int velkost = narr -1; //pocet -1 odstanil som zhluk
clear_cluster(&carr[idx]); // na indexe idx

for(int i = idx; i < velkost; i++)// vsetky sa musia posunut o jedno
{
    carr[i] = carr[i+1];
}
    return velkost;
    
    return 0;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
   // pomocou euklida ratame vzdialenost medzi dvoma objektami
    //float obj_distance = sqrt(powf(o1->x - o2->y, 2.0) + powf(o1->y - o2->y,2.0));
    //vraciame vzdialenost
//return obj_distance;

}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
    int dist;
    int high_dist = 0;

    //porovnavame vsetky objekty z dvoch roznych zhlukov
    for(int i = 0; i < c1->size; i++)
    {
    for(int j = 0; j < c2->size; j++)
     {
            //kazdy s kazdyn
        dist = obj_distance(&c1->obj[i], &c2->obj[i]);
        }   
        if(dist > high_dist)
        {
            high_dist = dist;
        }
    }
    
    return high_dist; 
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    float closest = 0;
    //porovnavame dva zhluky pomocou cluster distancu
    for(int i = 0; i < narr ; i++)
    {   //cez dva for-y kazdy s kazdym
        for(int j = 0; i < narr; j++)
        {
            closest = cluster_distance(&carr[i],&carr[j]);
            *c1 = i;
            *c2 = j;
        }

       
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    int line_num = 0;
    int object_num;

    int object_id;
    float object_x, object_y;

    struct obj_t str_obj;

    char line[20];
    
    //nacitavame a otvarame subor na citanie
    FILE *open;
    open = fopen("soubor", "r");
    //nacitavame prvy riadok, pokila sa nacitanie podarilo
    while(fgets(line, sizeof(line), open) != NULL)
    {

        //ked sme na prvom riadku, nacitavame do object_num hodnotu z count , ktora bude reprezentovat pociatocny pocet objektov
        if(line_num == 0)
        {
            sscanf(line, "count=%d", &object_num);
            *arr = malloc(sizeof(struct cluster_t) * object_num);
            line_num++;
            //alokujeme pamat pre pole vsetkych zhlukov
            continue;
        }
        
        //volame funkciu init, ktora z kazdeho objektu(od riadku z indexom 1 spravi samostatny zhluk)
        init_cluster(&(*arr)[line_num - 1], 1);
         
        //prechadzame riadky a nacitavame cisla do ID a suradnic objektu
        sscanf(line, "%d %f %f", &object_id, &object_x, &object_y);

        //Kontrola velkosti suradnic
        if((object_x < 0 && object_x > 1000) &&  (object_y < 0 && object_y > 1000))
        {
            printf("zle format objektu");
        }
        else
        {
            //prirazdujeme nacitane hodnoty ID a suradnic do struktury obj
            str_obj.id = object_id;
            str_obj.x = object_x;
            str_obj.y = object_y;

            (*arr)[line_num - 1].obj[0] = str_obj;
            (*arr)[line_num - 1].size = 1;
        }

        //ratame riadky(postupujeme po riadkoch)
        line_num++;
    }
    
    return object_num;
}    
            

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    int wanted_args;
    char* wanted_objects;
    //kontrola argumentov
    if(argc <= 1 && argc > 4)
    {
        printf("wrong number or arguments");
        return -1;
    }
    //ak je argument 2 automaticky mu dame hodnotu 1
    if(argc ==2)
    {
         wanted_args = 1;

    }
    if(argc == 3)
    {
         wanted_objects = argv[2];
    }

 int clustersCount = load_clusters("soubor", &clusters);

    print_clusters(clusters, clustersCount);

}