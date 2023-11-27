/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 */

#include "ctrain_handler.h"

#include "locomotive.h"
#include "locomotivebehavior.h"
#include "synchrointerface.h"
#include "synchro.h"

#include <string>
#include <vector>


using JunctionList = std::vector<LocomotiveBehavior::JunctionSetting>;

struct Route {
    std::string maquetteId;
    JunctionList junctions;
    LocomotiveBehavior::Parameters paramsA;
    LocomotiveBehavior::Parameters paramsB;
};

enum class RouteName {
    ROUTE_1,
    ROUTE_2,
};

Route routeFactory(RouteName route);

// Locomotives :
// Vous pouvez changer les vitesses initiales, ou utiliser la fonction loco.fixerVitesse(vitesse);
// Laissez les numéros des locos à 0 et 1 pour ce laboratoire

// Locomotive A
static Locomotive locoA(1 /* Numéro (pour commande trains sur maquette réelle) */, 10 /* Vitesse */);

// Locomotive B
static Locomotive locoB(2 /* Numéro (pour commande trains sur maquette réelle) */, 12 /* Vitesse */);

// Arret d'urgence
void emergency_stop() {
    locoA.arreter();
    locoB.arreter();
    locoA.fixerVitesse(0);
    locoB.fixerVitesse(0);

    afficher_message("\nSTOP!");
}


//Fonction principale
int cmain() {
    /*********
     * Route *
     ********/
    Route route = routeFactory(RouteName::ROUTE_2);

    /************
     * Maquette *
     ************/

    // Choix de la maquette (A ou B)
    selection_maquette(route.maquetteId.c_str());

    /**********************************
     * Initialisation des aiguillages *
     **********************************/

     for(const auto& junction : route.junctions) {
         diriger_aiguillage(junction.junctionId, junction.direction, 0);
     }

    /********************************
     * Position de départ des locos *
     ********************************/

    // Loco A
    locoA.fixerPosition(route.paramsA.station.front, route.paramsA.station.back);

    // Loco B
    locoB.fixerPosition(route.paramsB.station.front, route.paramsB.station.back);

    /***********
     * Message *
     **********/

    // Affiche un message dans la console de l'application graphique
    afficher_message("Hit play to start the simulation...");

    /*********************
     * Threads des locos *
     ********************/

    // Création du thread pour la loco 0
    std::unique_ptr<Launchable> locoBehaveA = std::make_unique<LocomotiveBehavior>(route.paramsA);
    // Création du thread pour la loco 1
    std::unique_ptr<Launchable> locoBehaveB = std::make_unique<LocomotiveBehavior>(route.paramsB);

    // Lanchement des threads
    afficher_message(qPrintable(QString("Lancement thread loco A (numéro %1)").arg(locoA.numero())));
    locoBehaveA->startThread();
    afficher_message(qPrintable(QString("Lancement thread loco B (numéro %1)").arg(locoB.numero())));
    locoBehaveB->startThread();

    // Attente sur la fin des threads
    locoBehaveA->join();
    locoBehaveB->join();

    //Fin de la simulation
    mettre_maquette_hors_service();

    return EXIT_SUCCESS;
}

Route routeFactory(RouteName route) {
    std::shared_ptr<SynchroInterface> sharedSection = std::make_shared<Synchro>();

    switch (route) {
         default:
         case RouteName::ROUTE_1: {
             // Route junctions
             JunctionList junctions({
                 {22, DEVIE     },
                 {21, TOUT_DROIT},
                 {20, TOUT_DROIT},
                 {23, DEVIE     },
                 {16, DEVIE     },
                 {15, TOUT_DROIT},
                 {14, TOUT_DROIT},
                 {13, TOUT_DROIT},
                 {10, TOUT_DROIT},
                 {7,  TOUT_DROIT},
                 {4,  TOUT_DROIT},
                 {1,  TOUT_DROIT},
                 {21, DEVIE     },
                 {20, TOUT_DROIT},
                 {23, DEVIE     },
                 {16, DEVIE     },
                 {15, TOUT_DROIT},
                 {14, DEVIE     },
                 {9,  DEVIE     },
                 {8,  DEVIE     },
                 {11, TOUT_DROIT},
                 {5,  TOUT_DROIT},
                 {3,  DEVIE     },
                 {2,  DEVIE     },
             });


             // Paramètres de la locomotive 1
             LocomotiveBehavior::Parameters paramsA = {
                 locoA,
                 {1, 2},
                 {sharedSection,
                   {21, TOUT_DROIT},
                   {16, TOUT_DROIT},
                   1, 31,
                   21}
             };

             // Paramètres de la locomotive 2
             LocomotiveBehavior::Parameters paramsB = {
                 locoB,
                 {5, 6},
                 {sharedSection, {21, DEVIE}, {16, DEVIE}, 5, 34, 24}
             };

             return Route({MAQUETTE_A, junctions, paramsA, paramsB});
         }
         case RouteName::ROUTE_2: {
             // Route junctions
             JunctionList junctions({
                 {20, DEVIE},
                 {23, DEVIE},
                 {24, TOUT_DROIT},
                 {6, TOUT_DROIT},
                 {5, DEVIE},
             });

             LocomotiveBehavior::Parameters paramsA = {
                 locoA, {1, 2}, {sharedSection, {21, TOUT_DROIT}, {2, TOUT_DROIT}, 1, 31, 1}
             };

             LocomotiveBehavior::Parameters paramsB = {
                 locoB, {5, 6}, {sharedSection, {21, DEVIE}, {2, DEVIE}, 5, 34, 5}
             };

             return Route({MAQUETTE_A, junctions, paramsA, paramsB});
         }
    }
}

