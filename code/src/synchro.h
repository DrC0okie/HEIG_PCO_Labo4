/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 * Authors: Timothée Van Hove and Aubry Mangold
 * Date: 2023-11-27
 */


#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <QDebug>

#include <pcosynchro/pcosemaphore.h>

#include "locomotive.h"
#include "ctrain_handler.h"
#include "synchrointerface.h"


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
      isStationOccupied(false) {}

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée
     *
     * Elle doit arrêter la locomotive et mettre son thread en attente si nécessaire.
     *
     * @param loco La locomotive qui essaie accéder à la section partagée
     */
    void access(Locomotive& loco) override {
        mutexSection.acquire();

        // If the section isn't free, stop the loco and wait to acquire it.
        if (!isSectionFree) {
            loco.arreter();
            afficher_message(qPrintable(
                QString("Loco %1: S'arrête et attend").arg(loco.numero())));

            // Set the other loco to wait.
            otherIsWaiting = true;
            mutexSection.release();

            // Blockingly wait for the section to be free.
            sectionSemaphore.acquire();
            mutexSection.acquire();
            otherIsWaiting = false;

            loco.demarrer();
            afficher_message(
                qPrintable(QString("Loco %1: Redémarre").arg(loco.numero())));
        }

        // Set the section to occupied now that a loco has acquired it.
        isSectionFree = false;
        mutexSection.release();
        afficher_message(
            qPrintable(QString("Loco %1: Accès à la section partagée")
                           .arg(loco.numero())));
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

        // Give access to the other loco if it is waiting.
        if (otherIsWaiting) {
            sectionSemaphore.release();
        }

        mutexSection.release();

        afficher_message(
            qPrintable(QString("Loco %1: Sortie de la section partagée")
                           .arg(loco.numero())));
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
        afficher_message(
            qPrintable(QString("Loco %1: Arrivée en gare").arg(loco.numero())));

        // If the station is empty, stop the loco and set the station to
        // occupied.
        mutexStation.acquire();
        if (!isStationOccupied) {
            isStationOccupied = true;
            mutexStation.release();
            loco.arreter();

            // Wait to be released by the last arrived loco.
            stationSemaphore.acquire();

            // Set the priority (1 being lower priority than 0) and restart the
            // loco.
            loco.priority = 1;
            loco.demarrer();
        } else {
            // If the station is occupied, wait for 5 seconds and have the last
            // arrived loco start first by already giving it access to the
            // shared section.
            isStationOccupied = false;
            loco.arreter();

            // Wait for the 5 seconds.
            afficher_message(qPrintable(
                QString("Loco %1: Attente de 5 secondes").arg(loco.numero())));
            PcoThread::usleep(5e6);

            // Give access to the shared section before releasing other locos so
            // that we're sure it can't be acquired by another loco.
            access(loco);
            afficher_message(
                qPrintable(QString("Loco %1: Prioritaire").arg(loco.numero())));

            // Release the semaphore to signal the first loco that it may
            // continue.
            stationSemaphore.release();

            // Set the loco to the highest priority and restart it.
            // Note: this is useful in the LocoBehavior::run() while loop in
            // order to avoid the prioritized loco trying to acquire the shared
            // section again.
            loco.priority = 0;
            loco.demarrer();
            mutexStation.release();
            ;
        }

        afficher_message(qPrintable(
            QString("Loco %1: Départ de la gare").arg(loco.numero())));
    }

    private:
    PcoSemaphore mutexSection;
    PcoSemaphore mutexStation;
    PcoSemaphore sectionSemaphore;
    PcoSemaphore stationSemaphore;
    bool         isSectionFree;
    bool         otherIsWaiting;
    bool         isStationOccupied;
};

#endif // SYNCHRO_H
