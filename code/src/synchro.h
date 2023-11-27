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
      isStationOccupied(false){
    }

    /**
     * @brief access Méthode à appeler pour accéder à la section partagée
     *
     * Elle doit arrêter la locomotive et mettre son thread en attente si nécessaire.
     *
     * @param loco La locomotive qui essaie accéder à la section partagée
     */
    void access(Locomotive& loco) override {
        mutexSection.acquire();
        if (!isSectionFree) {
            afficher_message(qPrintable(
                QString("Loco %1: S'arrête et attend").arg(loco.numero())));
            loco.arreter();
            otherIsWaiting = true;
            mutexSection.release();
            sectionSemaphore .acquire(); // Attente bloquante jusqu'à ce que la
                                         // section soit libérée.
            mutexSection.acquire();
            otherIsWaiting = false;
            afficher_message(
                qPrintable(QString("Loco %1: Redémarre").arg(loco.numero())));
            loco.demarrer();
        }

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

        mutexStation.acquire();
        if (!isStationOccupied) {
            // S'arrêter et attendre que toutes les locos soient en gare.
            isStationOccupied = true;
            mutexStation.release();
            loco.arreter();

            // Attendre d'être libéré par la dernière loco arrivée.
            stationSemaphore.acquire();
            loco.priority = 1;
            loco.demarrer();
        } else {
            // Arrêter la dernière loco et attendre 5 secondes avant de repartir
            // avec priorité.
            isStationOccupied = false;
            loco.arreter();
            afficher_message(qPrintable(
                QString("Loco %1: Attente de 5 secondes").arg(loco.numero())));
            PcoThread::usleep(5e6);

            // Donner l'accès à la section partagée à la dernière loco arrivée.
            afficher_message(
                qPrintable(QString("Loco %1: Prioritaire").arg(loco.numero())));
            access(loco);

            // Release the semaphore to signal the first loco
            stationSemaphore.release();
            loco.priority = 0;
            loco.demarrer();
            mutexStation.release();;
        }

        afficher_message(qPrintable(
            QString("Loco %1: Départ de la gare").arg(loco.numero())));
    }

private:
    static constexpr uint8_t nbToWait = 2;

    PcoSemaphore mutexSection;
    PcoSemaphore mutexStation;
    PcoSemaphore sectionSemaphore;
    PcoSemaphore stationSemaphore;
    bool isSectionFree;
    bool otherIsWaiting;
    bool isStationOccupied;
};

#endif // SYNCHRO_H
