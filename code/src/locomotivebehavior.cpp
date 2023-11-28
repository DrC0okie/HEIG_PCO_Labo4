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

    while (true) {
        sections.front().synchro->stopAtStation(loco);

        // Go through each section
        for (const auto& section : sections) {
            // Wait for the warning contact to be triggered, if any.
            if (section.contactWarn != station) {
                attendre_contact(section.contactWarn);
            }

            // The first departed locomotive gets priority in the first shared
            // section and then has its priority incremented to normal.
            if (loco.priority == 0) {
                loco.priority++;
            } else {
                section.synchro->access(loco);
            }

            // Toggle the junctions once the section entry contact is hit.
            attendre_contact(section.contactEnter);
            diriger_aiguillage(section.junctionEntry.junctionId,
                               section.junctionEntry.direction, 0);
            diriger_aiguillage(section.junctionExit.junctionId,
                               section.junctionExit.direction, 0);

            // Release the shared section after passing the exit contact.
            attendre_contact(section.contactExit);
            section.synchro->leave(loco);
        }

        // Wait for the station contact if it is different from the section
        // exit contact.
        if (station != sections.back().contactExit) {
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
