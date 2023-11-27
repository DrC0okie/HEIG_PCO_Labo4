/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 */


#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include <utility>

#include "locomotive.h"
#include "launchable.h"
#include "synchrointerface.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    typedef struct {
        std::int32_t front;
        std::int32_t back;
    } Station;

    typedef struct {
        std::int32_t junctionId;
        std::int32_t direction;
    } JunctionSetting;

    /**
     * A block is a section of tracks where only one locomotive can be at a time.
     */
    typedef struct {
        std::shared_ptr<SynchroInterface> sharedSection;
        JunctionSetting                   junctionEntry;
        JunctionSetting                   junctionExit;
        std::int32_t                      contactWarn;
        std::int32_t                      contactEnter;
        std::int32_t                      contactExit;
    } SharedSection;

    /**
     * @brief La structureParameters encapsule les paramètres du comportement d'une locomotive.
     *
     * @var loco La locomotive dont on représente le comportement
     * @var sharedSection Pointeur sur la section partagée
     */
    struct Parameters {
        Locomotive&  loco;
        Station      station;
        SharedSection blockSection;
    };

    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(const Parameters& params)
        : loco(params.loco),
          sharedSection(params.blockSection.sharedSection),
          station(params.station.front),
          contactWarn(params.blockSection.contactWarn),
          contactEnter(params.blockSection.contactEnter),
          contactExit(params.blockSection.contactExit),
          junctionEntry(params.blockSection.junctionEntry),
          junctionExit(params.blockSection.junctionExit) {
        this->loco.priority = 0; // Pas initialisé par la classe.
    }

    protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de la locomotive
     */
    void run() override;

    /*!
     * \brief printStartMessage Message affiché lors du démarrage du thread
     */
    void printStartMessage() override;

    /*!
     * \brief printCompletionMessage Message affiché lorsque le thread a terminé
     */
    void printCompletionMessage() override;

    /**
     * @brief loco La locomotive dont on représente le comportement
     */
    Locomotive& loco;

    /**
     * @brief sharedSection Pointeur sur la section partagée
     */
    std::shared_ptr<SynchroInterface> sharedSection;

    /*
     * Vous êtes libres d'ajouter des méthodes ou attributs
     *
     * Par exemple la priorité ou le parcours
     */
private:
    std::int32_t    station;
    std::int32_t    contactWarn;
    std::int32_t    contactEnter;
    std::int32_t    contactExit;
    JunctionSetting junctionEntry;
    JunctionSetting junctionExit;
};

#endif // LOCOMOTIVEBEHAVIOR_H
