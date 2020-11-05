/**
 * @file
 * Main file concerning the simulation, holds the SpreadSimulator struct
 *
 * @author cherrysrc
 * @date 19.05.2020
 * @copyright MIT-License
 */

#ifndef SPREADSIMREVIVED_SPREADSIMULATOR_H
#define SPREADSIMREVIVED_SPREADSIMULATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Quadtree/Quadtree.h"
#include "MedicalComponent.h"
#include "SimulationSettings.h"
#include "SimulatorStats.h"

#include <time.h>
#include <stdio.h>
#include <threads.h>

/**
 * @struct SpreadSimulator
 * @brief Central structure containing all simulation related variables\n
 * Struct organizes entities in an ECS styled manner
 */
typedef struct
{
    /** Settings for the simulation*/
    SimulationSettings settings;
    /** Keeping track of numbers, e.g. count of infected, cured etc.*/
    SimulatorStats     stats;

    Vector2*           positions;
    Vector2*           velocities;
    Vector2*           accelerations;
    MedicalComponent*  medComponents;

    /** Array of central locations*/
    Vector2*           centralLocations;
    /** Specifying the number of threads used*/
    int threadCount;
} SpreadSimulator;

/**
 * @struct ThreadData
 * @brief data structure containing information used for multithreading
 */
typedef struct
{
    SpreadSimulator* simulator;
    Quadtree*        tree;
    int              startIndex;
    int              endIndex;
}ThreadData;

/**
 * Creates a new simulator
 * @param settings settings object
 * @param thread_count number of threads to be used
 * @return new simulator
 */
SpreadSimulator createSimulator(SimulationSettings* settings, int thread_count);

/**
 * Printing stats in a human-readable format
 *
 * @param simulator the simulator values to print
 */
void printStats(SpreadSimulator* simulator);

/**
 * Printing stats in a easy to parse format\n
 * Leaves out descriptions and just prints the values\n
 * Intended to be used by other programs via piping or saving to a file
 *
 * @param simulator the simulator values to print
 */
void printStatsRaw(SpreadSimulator* simulator);

/**
 * Function that simulates one time unit on a given simulator\n
 *
 * @param simulator the desired simulator object
 */
void tick(SpreadSimulator* simulator);

/**
 * Updates a given segment of entities
 * @param threaddata data
 * @return number of people infected in this iteration
 */
int updateEntityRange(void* threaddata);

/**
 * Event function of a tick\n
 * Increases tick counter on the given simulator object\n
 * Meant for internal use only, user should not call this directly
 *
 * @param simulator the affected simulator
 */
void tickEvent(SpreadSimulator* simulator);

/**
 * Event function for an entity getting infected\n
 * Adjusts the properties of the given entity and adjusts the simulator's stats\n
 * Meant for internal use only, user should not call this directly
 *
 * @param simulator the affected simulator
 * @param index the entity's index
 */
void infectionEvent(SpreadSimulator* simulator, int index);

/**
 * Event function for an entity getting cured aka surviving the infection\n
 * Adjusts the properties of the given entity and adjusts the simulator's stats\n
 * Meant for internal use only, user should not call this directly
 *
 * @param simulator the affected simulator
 * @param index the entity's index
 */
void cureEvent(SpreadSimulator* simulator, int index);

/**
 * Event function for an entity dying to the infection\n
 * Adjusts the properties of the given entity and adjusts the simulator's stats\n
 * Meant for internal use only, user should not call this directly
 *
 * @param simulator the affected simulator
 * @param index the entity's index
 */
void deathEvent(SpreadSimulator* simulator, int index);

/**
 * Event function for an entity getting send to the hospital\n
 * Adjusts the properties of the given entity and adjusts the simulator's stats\n
 * Meant for internal use only, user should not call this directly
 *
 * @param simulator the affected simulator
 * @param index the entity's index
 */
void hospitalizeEvent(SpreadSimulator* simulator, int index);

/**
 * Event function for an entity getting released from hospital\n
 * Adjusts the properties of the given entity and adjusts the simulator's stats\n
 * Meant for internal use only, user should not call this directly
 *
 * @param simulator the affected simulator
 * @param index the entity's index
 */
void dehospitalizeEvent(SpreadSimulator* simulator, int index);

/**
 * Frees all memory that was allocated for a given SpreadSimulator\n
 * @param simulator the simulator to clean up
 */
void cleanup(SpreadSimulator* simulator);

#ifdef __cplusplus
}
#endif

#endif //SPREADSIMREVIVED_SPREADSIMULATOR_H
