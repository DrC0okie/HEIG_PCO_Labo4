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

    while(true) {
        attendre_contact(station);
        sharedSection->stopAtStation(loco);
        // Traiter le cas spécial ou la gare n'est pas le warning du block.
        if (station != contactWarn) {
            attendre_contact(contactWarn);
        }

        // Accéder à la section partagée, sauf dans le cas de la loco prioritaire
        // qui a d'office accédé à la section partagée.
        if (loco.priority > 0) {
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
