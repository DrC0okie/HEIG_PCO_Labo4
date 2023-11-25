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
    /**
     * @brief La structureParameters encapsule les paramètres du comportement d'une locomotive.
     *
     * @var loco La locomotive dont on représente le comportement
     * @var sharedSection Pointeur sur la section partagée
     */
    struct Parameters {
        Parameters(Locomotive& loco,
                   std::shared_ptr<SynchroInterface> sharedSection,
                   std::int32_t                      station,
                   std::int32_t                      sectionBegin,
                   std::int32_t                      sectionEnd,
                   std::int32_t                     endSectionContactNum,
                   std::int32_t                     endSectionContactDirection)
            : loco(loco),
              sharedSection(std::move(sharedSection)),
              station(station),
              sectionBegin(sectionBegin),
              sectionEnd(sectionEnd),
              endSectionContactNum(endSectionContactNum),
              endSectionContactDirection(endSectionContactDirection){}

        Locomotive&                       loco;
        std::shared_ptr<SynchroInterface> sharedSection;
        std::int32_t                      station;
        std::int32_t                      sectionBegin;
        std::int32_t                      sectionEnd;
        std::int32_t                     endSectionContactNum;
        std::int32_t                     endSectionContactDirection;
    };

    /*!
     * \brief locomotiveBehavior Constructeur de la classe
     * \param loco la locomotive dont on représente le comportement
     */
    LocomotiveBehavior(const Parameters& params)
        : loco(params.loco),
          sharedSection(params.sharedSection),
          station(params.station),
          sectionBegin(params.sectionBegin),
          sectionEnd(params.sectionEnd),
          endSectionContactNum(params.endSectionContactNum),
          endSectionContactDirection(params.endSectionContactDirection){
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
    std::int32_t station;
    std::int32_t sectionBegin;
    std::int32_t sectionEnd;
    std::int32_t endSectionContactNum;
    std::int32_t endSectionContactDirection;
};

#endif // LOCOMOTIVEBEHAVIOR_H
