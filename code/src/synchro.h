/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 */


#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "synchrointerface.h"

const uint8_t nbToWait = 2;

/**
 * @brief La classe Synchro implémente l'interface SynchroInterface qui
 * propose les méthodes liées à la section partagée.
 */
class Synchro final : public SynchroInterface
{
public:

    /**
     * @brief Synchro Constructeur de la classe qui représente la section partagée.
     * Initialisez vos éventuels attributs ici, sémaphores etc.
     */
    Synchro() :
      mutexSection(1),
      mutexStation(1),
      sectionSemaphore(0),
      stationSemaphore(0),
      isSectionFree(true),
      otherIsWaiting(false),
      nbInStation(0){
    }

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée
     *
     * Elle doit arrêter la locomotive et mettre son thread en attente si nécessaire.
     *
     * @param loco La locomotive qui essaie accéder à la section partagée
     */
    void access(Locomotive &loco) override {
        mutexSection.acquire();
        if (!isSectionFree) {
            afficher_message(qPrintable(QString("Locomotive %1 is stopping and waiting.").arg(loco.numero())));
            loco.arreter();
            otherIsWaiting = true;
            mutexSection.release();
            sectionSemaphore.acquire(); // Wait for access to the shared section
            mutexSection.acquire();
            otherIsWaiting = false;
            afficher_message(qPrintable(QString("Locomotive %1 resumes.").arg(loco.numero())));
            loco.demarrer();
        }

        isSectionFree = false;
        mutexSection.release();
        afficher_message(qPrintable(QString("Locomotive %1 has accessed the shared section.").arg(loco.numero())));
    }

    /**
     * @brief leave Méthode à appeler pour indiquer que la locomotive est sortie de la section partagée
     *
     * Réveille les threads des locomotives potentiellement en attente.
     *
     * @param loco La locomotive qui quitte la section partagée
     */
    void leave(Locomotive& loco) override {
        mutexSection.acquire();
        isSectionFree = true;
        if (otherIsWaiting) {
            sectionSemaphore.release();
        }
        mutexSection.release();

        afficher_message(qPrintable(QString("Locomotive %1 has left the shared section.").arg(loco.numero())));
    }

    /**
     * @brief stopAtStation Méthode à appeler quand la locomotive doit attendre à la gare
     *
     * Implémentez toute la logique que vous avez besoin pour que les locomotives
     * s'attendent correctement.
     *
     * @param loco La locomotive qui doit attendre à la gare
     */
    void stopAtStation(Locomotive& loco) override {
        mutexStation.acquire();
        nbInStation++;
        if (nbInStation < nbToWait) {
            afficher_message(qPrintable(QString("The engine no. %1 arrives at the station.").arg(loco.numero())));
            mutexStation.release();
            loco.arreter();
            stationSemaphore.acquire();
            afficher_message(qPrintable(QString("The engine no. %1 leaves the station.").arg(loco.numero())));
            loco.demarrer();
        } else {
            afficher_message(qPrintable(QString("The engine no. %1 arrives at the station.").arg(loco.numero())));
            nbInStation = 0;
            loco.arreter();
            afficher_message(qPrintable(QString("Waiting 5 seconds.")));
            PcoThread::usleep(5000000);
            // débloquer tous les (nbToWait - 1) threads
            for (unsigned int i = 0; i < nbToWait - 1; i++) {
                stationSemaphore.release();
            }
            afficher_message(qPrintable(QString("The engine no. %1 leaves the station.").arg(loco.numero())));
            loco.demarrer();
            mutexStation.release();
        }
    }

    /* A vous d'ajouter ce qu'il vous faut */

private:
    PcoSemaphore mutexSection;
    PcoSemaphore mutexStation;
    PcoSemaphore sectionSemaphore;
    PcoSemaphore stationSemaphore;
    bool isSectionFree;
    bool otherIsWaiting;
    uint8_t nbInStation;
};


#endif // SYNCHRO_H
