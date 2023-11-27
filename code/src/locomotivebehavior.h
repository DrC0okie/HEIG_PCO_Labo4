/*  _____   _____ ____    ___   ___ ___  ____
 * |  __ \ / ____/ __ \  |__ \ / _ \__ \|___ \
 * | |__) | |   | |  | |    ) | | | | ) | __) |
 * |  ___/| |   | |  | |   / /| | | |/ / |__ <
 * | |    | |___| |__| |  / /_| |_| / /_ ___) |
 * |_|     \_____\____/  |____|\___/____|____/
 * Authors: Timothée Van Hove and Aubry Mangold
 * Date: 2023-11-27
 */


#ifndef LOCOMOTIVEBEHAVIOR_H
#define LOCOMOTIVEBEHAVIOR_H

#include <utility>

#include "launchable.h"
#include "locomotive.h"
#include "synchrointerface.h"

/**
 * @brief La classe LocomotiveBehavior représente le comportement d'une
 * locomotive
 */
class LocomotiveBehavior : public Launchable {
    public:
    /**
     * @brief Encapsulates the front and back contacts of the station of a
     * locomotive.
     *
     * @param front The front contact of the station.
     * @param back The back contact of the station.
     */
    typedef struct {
        std::int32_t front;
        std::int32_t back;
    } Station;

    /**
     * @brief Encapsulates the junction id and direction of a junction setting.
     *
     * @param junctionId The junction id.
     * @param direction The junction direction.
     */
    typedef struct {
        std::int32_t junctionId;
        std::int32_t direction;
    } JunctionSetting;

    /**
     * A block is a section of tracks where only one locomotive can be at a
     * time.
     *
     * @param sharedSection The shared section object.
     * @param junctionEntry The junction setting to enter the block.
     * @param junctionExit The junction setting to exit the block.
     * @param contactWarn The contact that triggers checking if the shared
     * section is free.
     * @param contactEnter The contact that triggers the junction setting to
     * enter the shared section.
     * @param contactExit The contact that triggers the junction setting to exit
     * the shared section and signal it free.
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
     * @brief Encapsulates the parameters of the locomotive behavior.
     *
     * @param loco The locomotive whose behavior is parametrized.
     * @param station The station of the locomotive.
     * @param blockSection The block section shared by the locomotives.
     */
    struct Parameters {
        Locomotive&   loco;
        Station       station;
        SharedSection blockSection;
    };

    /**
     * @brief locomotiveBehavior Class constructor.
     * @param params The parameters of the locomotive behavior.
     */
    explicit LocomotiveBehavior(const Parameters& params)
        : loco(params.loco),
          sharedSection(params.blockSection.sharedSection),
          station(params.station.front),
          contactWarn(params.blockSection.contactWarn),
          contactEnter(params.blockSection.contactEnter),
          contactExit(params.blockSection.contactExit),
          junctionEntry(params.blockSection.junctionEntry),
          junctionExit(params.blockSection.junctionExit) {
        this->loco.priority = 0;  // Not initialized by the Loco class itself.
    }

    protected:
    /*!
     * \brief run Fonction lancée par le thread, représente le comportement de
     * la locomotive
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

#endif  // LOCOMOTIVEBEHAVIOR_H
