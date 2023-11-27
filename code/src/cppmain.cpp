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

// Locomotives :
// Vous pouvez changer les vitesses initiales, ou utiliser la fonction loco.fixerVitesse(vitesse);
// Laissez les numéros des locos à 0 et 1 pour ce laboratoire

// Locomotive A
static Locomotive locoA(1 /* Numéro (pour commande trains sur maquette réelle) */, 10 /* Vitesse */);
// Locomotive B
static Locomotive locoB(2 /* Numéro (pour commande trains sur maquette réelle) */, 12 /* Vitesse */);

// Arret d'urgence
void emergency_stop() {
    // partagée, ce qui la relance.
    locoA.arreter();
    locoB.arreter();
    locoA.fixerVitesse(0);
    locoB.fixerVitesse(0);

    afficher_message("\nSTOP!");
}


//Fonction principale
int cmain() {
    /************
     * Maquette *
     ************/

    //Choix de la maquette (A ou B)
    selection_maquette(MAQUETTE_A);

    /**********************************
     * Initialisation des aiguillages *
     **********************************/

    // Initialisation des aiguillages
    // Positiion de base donnée comme exemple, vous pouvez la changer comme bon vous semble
    // Vous devrez utiliser cette fonction pour la section partagée pour aiguiller les locos
    // sur le bon parcours (par exemple à la sortie de la section partagée) vous pouvez l'
    // appeler depuis vos thread des locos par ex.

    // Parcours 1
    diriger_aiguillage(22, DEVIE, 0);
    diriger_aiguillage(21, TOUT_DROIT, 0);
    diriger_aiguillage(20, TOUT_DROIT, 0);
    diriger_aiguillage(23, DEVIE, 0);
    diriger_aiguillage(16, DEVIE, 0);
    diriger_aiguillage(15, TOUT_DROIT, 0);
    diriger_aiguillage(14, TOUT_DROIT, 0);
    diriger_aiguillage(13, TOUT_DROIT, 0);
    diriger_aiguillage(10, TOUT_DROIT, 0);
    diriger_aiguillage(7, TOUT_DROIT, 0);
    diriger_aiguillage(4, TOUT_DROIT, 0);
    diriger_aiguillage(1, TOUT_DROIT, 0);

    // Parcours 2
    diriger_aiguillage(21, DEVIE, 0);
    diriger_aiguillage(20, TOUT_DROIT, 0);
    diriger_aiguillage(23, DEVIE, 0);
    diriger_aiguillage(16, DEVIE, 0);
    diriger_aiguillage(15, TOUT_DROIT, 0);
    diriger_aiguillage(14, DEVIE, 0);
    diriger_aiguillage(9, DEVIE, 0);
    diriger_aiguillage(8, DEVIE, 0);
    diriger_aiguillage(11, TOUT_DROIT, 0);
    diriger_aiguillage(5, TOUT_DROIT, 0);
    diriger_aiguillage(3, DEVIE, 0);
    diriger_aiguillage(2, DEVIE, 0);

    /********************************
     * Position de départ des locos *
     ********************************/

    // Loco 0
    locoA.fixerPosition(1, 2);

    // Loco 1
    locoB.fixerPosition(5, 6);

    /***********
     * Message *
     **********/

    // Affiche un message dans la console de l'application graphique
    afficher_message("Hit play to start the simulation...");

    /*********************
     * Threads des locos *
     ********************/

    // Création de la section partagée
    std::shared_ptr<SynchroInterface> sharedSection = std::make_shared<Synchro>();

    // Paramètres de la locomotive 1
    LocomotiveBehavior::Parameters const locoAParams({
        locoA,
        1,
        {sharedSection, {21, TOUT_DROIT}, {16, TOUT_DROIT}, 1, 31, 21},
    });

    // Paramètres de la locomotive 2
    LocomotiveBehavior::Parameters const locoBParams({
        locoB,
        5,
        {sharedSection, {21, DEVIE}, {16, DEVIE}, 5, 34, 24},
    });

    // Création du thread pour la loco 0
    std::unique_ptr<Launchable> locoBehaveA = std::make_unique<LocomotiveBehavior>(locoAParams);
    // Création du thread pour la loco 1
    std::unique_ptr<Launchable> locoBehaveB = std::make_unique<LocomotiveBehavior>(locoBParams);

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
