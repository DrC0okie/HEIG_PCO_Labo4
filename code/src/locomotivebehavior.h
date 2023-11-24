/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 */


#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include "locomotive.h"
#include "launchable.h"
#include "synchrointerface.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une locomotive
 */
class LocomotiveBehavior : public Launchable
{
public:
    /**
     * @brief La structureParameters encapsule les paramètres du comportement d'une locomotive.
     *
     * @var loco La locomotive dont on représente le comportement
     * @var sharedSection Pointeur sur la section partagée
     */
    struct Parameters {
        Parameters(Locomotive& loco, std::shared_ptr<SynchroInterface> sharedSection) : loco(loco), sharedSection(sharedSection) {}
        Locomotive& loco;
        std::shared_ptr<SynchroInterface> sharedSection;
        // TODO passer la gare, le début et la fin de la section partagée
    };

    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(const Parameters &params) : loco(params.loco), sharedSection(params.sharedSection) {
        // Eventuel code supplémentaire du constructeur
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
};

#endif // LOCOMOTIVEBEHAVIOR_H
