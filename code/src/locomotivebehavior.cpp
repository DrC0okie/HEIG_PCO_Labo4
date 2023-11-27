/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 * Authors: Timothée Van Hove and Aubry Mangold
 * Date: 2023-11-27
 */

#include "locomotivebehavior.h"
#include "ctrain_handler.h"

void LocomotiveBehavior::run()
{
    // Locomotive initialization.
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    // Initial entry into the station.
    attendre_contact(station);

    while(true) {
        sharedSection->stopAtStation(loco);

        // Wait for the warning contact to be triggered, if any.
        if (station != contactWarn) {
            attendre_contact(contactWarn);
        }

        // Access the shared section, unless the locomotive has priority, in
        // which case it was already granted access to the shared section.
        if (loco.priority > 0) {
            sharedSection->access(loco);
        }

        // Only once we are _in_ the sections do we toggle the junctions.
        attendre_contact(contactEnter);
        diriger_aiguillage(junctionEntry.junctionId, junctionEntry.direction, 0);
        diriger_aiguillage(junctionExit.junctionId, junctionExit.direction, 0);

        // Release the shared section after passing the exit contact.
        attendre_contact(contactExit);
        sharedSection->leave(loco);

        // Wait for the station contact if it is different from the section
        // exit contact.
        if (station != contactExit) {
            attendre_contact(station);
        }
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
