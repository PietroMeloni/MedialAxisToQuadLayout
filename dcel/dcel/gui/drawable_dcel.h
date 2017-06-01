/*
 * @author    Alessandro Muntoni (muntoni.alessandro@gmail.com)
 * @author    Andreas Scalas (andreasscalas@gmail.com)
 * @copyright Alessandro Muntoni 2016.
 */

#ifndef DRAWABLE_DCEL_H
#define DRAWABLE_DCEL_H

#include <viewer/interfaces/drawable_mesh.h>
#include <common/utils.h>
#include "../dcel.h"

/**
 * \~Italian
 * @class DrawableDcel
 *
 * @brief Classe che eredita dalla classe Dcel e dall'interfaccia DrawalbleObject.
 * Rende visualizzabile in una canvas la Dcel.
 * Tuttavia, se si effettuano modifiche sulla Dcel (utilizzando quindi i metodi classici della Dcel),
 * queste non saranno immediatamente visibili sulla DrawableDcel. Per rendere visibili eventuali modifiche,
 * è necessario chiamare il metodo update().
 *
 * @warning il metodo update() non è particolarmente efficiente se chiamato ad ogni draw() (ossia ad ogni render
 * della dcel); è quindi caldamente consigliato chiamare il metodo update() solamente quando si vuole visualizzare
 * le modifiche effettuate sulla dcel.
 */
class DrawableDcel : public Dcel, public DrawableMesh {
    public:

        DrawableDcel();
        DrawableDcel(const Dcel &d);
        virtual ~DrawableDcel();

        void init();
        void clear();

        // Implementation of the
        // DrawableObject interface
        //
        void draw() const;
        Pointd sceneCenter() const;
        double sceneRadius() const;

        void update();

        void setFacesWireframe(bool b);

    protected:

        void renderPass() const;

        enum {
            DRAW_FACES_WIREFRAME = 0b10000000
        };

        std::vector<double> vertexCoordinates; /** \~Italian @brief vettore di coordinate usate per la visualizzazione: per aggiornare utilizzare metodo update() */
        std::vector<int> triangles; /** \~Italian @brief vettore di triangoli (da considerare a triple di indici) usati per la visualizzazione: per aggiornare utilizzare il metodo update() */
        std::vector<double> vertexNormals; /** \~Italian @brief vettore di normali ai vertici usate per la visualizzazione: per aggiornare utilizzare il metodo update() */
        std::vector<float> vertexColors; /** \~Italian @brief vettore di colori associati ai vertici (da considerare come triple rgb float) usati per la visualizzazione: per aggiornare utilizzare il metodo update() */
        std::vector<double> triangleNormals; /** \~Italian @brief vettore di normali ai triangoli usate per la visualizzazione: per aggiornare utilizzare il metodo update() */
        std::vector<float> triangleColors; /** \~Italian @brief vettore di colori associati ai triangoli (da considerare come triple rgb float) usati per la visualizzazione: per aggiornare utilizzare il metodo update() */

        std::vector<unsigned int> triangles_face; /** \~Italian @brief vettore di mappatura triangoli->facce (ogni entrata ha posizione corrispondente a un terzo della posizione della tripla in tris e presenta l'identificativo di una faccia */
        std::vector<std::pair<unsigned int, unsigned int> > facesWireframe; /** \~Italian @brief vettore di coppie usate per renderizzare degli edge: per aggiornare utilizzare metodo update() */

};

#endif // DRAWABLE_DCEL_H
