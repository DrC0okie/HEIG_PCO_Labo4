/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 */

#include "locomotivebehavior.h"
#include "ctrain_handler.h"

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->access(loco);
    //sharedSection->leave(loco);
    //sharedSection->stopAtStation(loco);

    while(true) {
        // Handle emergency stop
        // TODO

        // Traiter le cas spécial ou la gare est le warning du block
        if (station == contactWarn) {
            attendre_contact(station);
            sharedSection->stopAtStation(loco);
            sharedSection->access(loco);
        } else {
            attendre_contact(station);
            sharedSection->stopAtStation(loco);

            // Attendre le warning pour savoir s'il faut ralentir ou non.
            attendre_contact(contactWarn);
            sharedSection->access(loco);
        }

        // Diriger l'aiguillage d'entrée dans la section partagée.
        attendre_contact(contactEnter);
        diriger_aiguillage(junctionEntry.junctionId, junctionEntry.direction, 0);

        // Diriger l'aiguillage de sortie.
        diriger_aiguillage(junctionExit.junctionId, junctionExit.direction, 0);
        attendre_contact(contactExit);
        sharedSection->leave(loco);
    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}
