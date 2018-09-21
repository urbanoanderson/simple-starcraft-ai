#include "Unidade.h"

#include <fstream>

using namespace BWAPI;


Unidade::Unidade(BWAPI::Unit *unit, int index, std::map<BWAPI::Unit*, Unidade*> *m, bool fog){
        this->unit = unit;
        this->index = index;
        this->mapa = m;
        this->fow = fog;
        comando = BWAPI::UnitCommand();
}

BWAPI::UnitCommand  Unidade::getComando(){
        BWAPI::UnitCommand cm = comando;
        comando = BWAPI::UnitCommand();
        return cm;
}

/** Quando ha fog of war, retorna apenas os elementos do conjunto que estao dentro da area de visao da unidade*/
std::set<Unidade*> Unidade::filtrar(std::set<Unidade*> conjunto){
        if(!fow) return conjunto;
        std::set<Unidade*> conj2;
        for(std::set<Unidade*>::iterator it = conjunto.begin(); it != conjunto.end(); it++){
                if(unit->getDistance((*it)->unit) <= unit->getType().sightRange())
                        conj2.insert(*it);
        }
        return conj2;
}
/* instavel - cancelado
std::set<Unidade*> Unidade::getUnitsOnTile(int tileX, int tileY){
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = Broodwar->getUnitsOnTile(tileX, tileY);
        for(std::set<Unit*>::iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[(*it)]);
        }
        instavel return filtrar(conjunto);
}
/* instavel - cancelado
bool Unidade::canBuildHere(BWAPI::TilePosition position, BWAPI::UnitType type, bool checkExplored){
        instavel return Broodwar->canBuildHere(unit, position, type, checkExplored);
}*/

bool Unidade::hasPower(int tileX, int tileY, int tileWidth, int tileHeight){
        return Broodwar->hasPower(tileX, tileY, tileWidth, tileHeight);
}
bool Unidade::hasPower(BWAPI::TilePosition position, int tileWidth, int tileHeight){
        return Broodwar->hasPower(position, tileWidth, tileHeight);
}

bool Unidade::isBuildable(int tileX, int tileY){
        return Broodwar->isBuildable(tileX, tileY);
}
bool Unidade::isBuildable(BWAPI::TilePosition position){
        return Broodwar->isBuildable(position);
}

std::set<Unidade*> Unidade::getMinerals(){
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = Broodwar->getMinerals();
        for(std::set<Unit*>::iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[(*it)]);
        }
        return filtrar(conjunto);
}

std::set<Unidade*> Unidade::getGeysers(){
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = Broodwar->getGeysers();
        for(std::set<Unit*>::iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[(*it)]);
        }
        return filtrar(conjunto);
}

std::set<Unidade*> Unidade::getEnemyUnits(){
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = Broodwar->enemy()->getUnits();
        for(std::set<Unit*>::iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[(*it)]);
        }
        return filtrar(conjunto);
}

std::set<Unidade*> Unidade::getAllyUnits(){
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = Broodwar->self()->getUnits();
        for(std::set<Unit*>::iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[(*it)]);
        }
        return filtrar(conjunto);
}

bool Unidade::isEnemy(Unidade* target){
        return target->unit->getPlayer() == Broodwar->enemy();
}

bool Unidade::isAlly(Unidade* target){
        return target->unit->getPlayer() == Broodwar->self();
}

int Unidade::minerals(){
        return Broodwar->self()->minerals();
}

int Unidade::gas(){
        return Broodwar->self()->gas();
}

int Unidade::supplyTotal(){
        return Broodwar->self()->supplyTotal();
}

int Unidade::supplyUsed(){
        return Broodwar->self()->supplyUsed();
}

bool Unidade::checar(BWAPI::UnitCommand comando){
        return unit->canIssueCommand(comando);
}

bool Unidade::attack(BWAPI::Position target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::attack(unit, target);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::attack(Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::attack(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::build(BWAPI::TilePosition target, BWAPI::UnitType type){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::build(unit, target, type);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::buildAddon(BWAPI::UnitType type){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::buildAddon(unit, type);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::train(BWAPI::UnitType type){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::train(unit, type);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::morph(BWAPI::UnitType type){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::morph(unit, type);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::research(BWAPI::TechType tech){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::research(unit, tech);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::upgrade(BWAPI::UpgradeType upgrade){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::upgrade(unit, upgrade);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::setRallyPoint(BWAPI::Position target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::setRallyPoint(unit, target);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::setRallyPoint(Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::setRallyPoint(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::move(BWAPI::Position P){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::move(unit, P);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::patrol(BWAPI::Position target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::patrol(unit, target);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::holdPosition(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::holdPosition(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::stop(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::stop(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::follow(Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::follow(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::gather(Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::gather(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::returnCargo(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::returnCargo(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::repair(Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::repair(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::burrow(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::burrow(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::unburrow(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::unburrow(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::cloak(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::cloak(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::decloak(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::decloak(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::siege(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::siege(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::unsiege(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::unsiege(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::lift(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::lift(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::land(BWAPI::TilePosition target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::land(unit, target);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::load(Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::load(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::unload(Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::unload(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::unloadAll(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::unloadAll(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::unloadAll(BWAPI::Position target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::unloadAll(unit, target);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::rightClick(Unidade *target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::rightClick(unit, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::rightClick(BWAPI::Position target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::rightClick(unit, target);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::haltConstruction(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::haltConstruction(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::cancelConstruction(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::cancelConstruction(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::cancelAddon(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::cancelAddon(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::cancelTrain(int slot){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::cancelTrain(unit, slot);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::cancelMorph(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::cancelMorph(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::cancelResearch(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::cancelResearch(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::cancelUpgrade(){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::cancelUpgrade(unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::useTech(BWAPI::TechType tech){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::useTech(unit, tech);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::useTech(BWAPI::TechType tech, BWAPI::Position target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::useTech(unit, tech, target);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::useTech(BWAPI::TechType tech, Unidade* target){
        BWAPI::UnitCommand temp = BWAPI::UnitCommand::useTech(unit, tech, target->unit);
        if(!checar(temp)) return false;
        this->comando = temp;
        return true;
}

bool Unidade::isInGame(){
   return Broodwar == NULL || Broodwar->isInGame();
}

bool Unidade::checkNovoTurno(){
        return comando.getType() == BWAPI::UnitCommandTypes::None;
}

/** Returns a unique ID for this unit. It simply casts the unit's address as an integer, since each unit
  * has a unique address. 
int Unidade::getID() {
        return unit->getID();
}

/** Returns a pointer to the player that owns this unit. 
BWAPI::Player* Unidade::getPlayer() {
        return unit->getPlayer();
}

/** Returns the current type of the unit. */
BWAPI::UnitType Unidade::getType() {
        return unit->getType();
}

/** Returns the position of the unit on the map. */
BWAPI::Position Unidade::getPosition() {
        return unit->getPosition();
}

/** Returns the build tile position of the unit on the map. Useful if the unit is a building. The tile
  * position is of the top left corner of the building. */
BWAPI::TilePosition Unidade::getTilePosition() {
        return unit->getTilePosition();
}

/** Returns the direction the unit is facing, measured in radians. An angle of 0 means the unit is
  * facing east. */
double Unidade::getAngle() {
        return unit->getAngle();
}

/** Returns the x component of the unit's velocity, measured in pixels per frame. */
double Unidade::getVelocityX() {
        return unit->getVelocityX();
}

/** Returns the y component of the unit's velocity, measured in pixels per frame. */
double Unidade::getVelocityY() {
        return unit->getVelocityY();
}

/** Returns the unit's current amount of hit points. */
int Unidade::getHitPoints() {
        return unit->getHitPoints();
}

/** Returns the unit's current amount of shields. */
int Unidade::getShields() {
        return unit->getShields();
}

/** Returns the unit's current amount of energy. */
int Unidade::getEnergy() {
        return unit->getEnergy();
}

/** Returns the unit's current amount of containing resources. Useful for determining how much minerals
  * are left in a mineral patch, or how much gas is left in a geyser
  * (can also be called on a refinery/assimilator/extractor). */
int Unidade::getResources() {
        return unit->getResources();
}

/** Retrieves the group ID of a resource. Can be used to identify which resources belong to an expansion. */
int Unidade::getResourceGroup() {
        return unit->getResourceGroup();
}

/** Returns the edge-to-edge distance between the current unit and the target unit. */
int Unidade::getDistance(Unidade* target) {
        return unit->getDistance(target->unit);
}

/** Returns the distance from the edge of the current unit to the target position. */
int Unidade::getDistance(BWAPI::Position target) {
        return unit->getDistance(target);
}

/** Returns true if the unit is able to move to the target unit */
bool Unidade::hasPath(Unidade* target) {
        return unit->hasPath(target->unit);
}

/** Returns true if the unit is able to move to the target position */
bool Unidade::hasPath(BWAPI::Position target) {
        return unit->hasPath(target);
}

/** Returns the frame of the last successful command. Frame is comparable to Game::getFrameCount(). */
int Unidade::getLastCommandFrame() {
        return unit->getLastCommandFrame();
}

/** Returns the last successful command. 
BWAPI::UnitCommand Unidade::getLastCommand() {
        return unit->getLastCommand();
}

/** Returns the player's current upgrade level for the given upgrade, if the unit is affected by this
  * upgrade.*/
int Unidade::getUpgradeLevel(BWAPI::UpgradeType upgrade) {
        return unit->getUpgradeLevel(upgrade);
}

/** Returns the initial type of the unit or Unknown if it wasn't a neutral unit at the beginning of the
  * game. */
BWAPI::UnitType Unidade::getInitialType() {
        return unit->getInitialType();
}

/** Returns the initial position of the unit on the map, or Positions::Unknown if the unit wasn't a
  * neutral unit at the beginning of the game. */
BWAPI::Position Unidade::getInitialPosition() {
        return unit->getInitialPosition();
}

/** Returns the initial build tile position of the unit on the map, or TilePositions::Unknown if the
  * unit wasn't a neutral unit at the beginning of the game. The tile position is of the top left corner
  * of the building. */
BWAPI::TilePosition Unidade::getInitialTilePosition() {
        return unit->getInitialTilePosition();
}

/** Returns the unit's initial amount of hit points, or 0 if it wasn't a neutral unit at the beginning
  * of the game. */
int Unidade::getInitialHitPoints() {
        return unit->getInitialHitPoints();
}

/** Returns the unit's initial amount of containing resources, or 0 if the unit wasn't a neutral unit
  * at the beginning of the game. */
int Unidade::getInitialResources() {
        return unit->getInitialResources();
}

/** Returns the unit's current kill count. */
int Unidade::getKillCount() {
        return unit->getKillCount();
}

/** Returns the unit's acid spore count. */
int Unidade::getAcidSporeCount() {
        return unit->getAcidSporeCount();
}

/** Returns the number of interceptors the Protoss Carrier has. */
int Unidade::getInterceptorCount() {
        return unit->getInterceptorCount();
}

/** Returns the number of scarabs in the Protoss Reaver. */
int Unidade::getScarabCount() {
        return unit->getScarabCount();
}

/** Returns the number of spider mines in the Terran Vulture. */
int Unidade::getSpiderMineCount() {
        return unit->getSpiderMineCount();
}

/** Returns unit's ground weapon cooldown. It is 0 if the unit is ready to attack. */
int Unidade::getGroundWeaponCooldown() {
        return unit->getGroundWeaponCooldown();
}

/** Returns unit's air weapon cooldown. It is 0 if the unit is ready to attack. */
int Unidade::getAirWeaponCooldown() {
        return unit->getAirWeaponCooldown();
}

/** Returns unit's ground weapon cooldown. It is 0 if the unit is ready cast a spell. */
int Unidade::getSpellCooldown() {
        return unit->getSpellCooldown();
}

/** Returns the remaining hit points of the defense matrix. Initially a defense Matrix has 250 points.
  * \see Unit::getDefenseMatrixTimer, Unit::isDefenseMatrixed. */
int Unidade::getDefenseMatrixPoints() {
        return unit->getDefenseMatrixPoints();
}

/** Returns the time until the defense matrix wears off. 0 -> No defense Matrix present. */
int Unidade::getDefenseMatrixTimer() {
        return unit->getDefenseMatrixTimer();
}

/** Returns the time until the ensnare effect wears off. 0 -> No ensnare effect present. */
int Unidade::getEnsnareTimer() {
        return unit->getEnsnareTimer();
}

/** Returns the time until the radiation wears off. 0 -> No radiation present. */
int Unidade::getIrradiateTimer() {
        return unit->getIrradiateTimer();
}

/** Returns the time until the lockdown wears off. 0 -> No lockdown present. */
int Unidade::getLockdownTimer() {
        return unit->getLockdownTimer();
}

/** Returns the time until the maelstrom wears off. 0 -> No maelstrom present. */
int Unidade::getMaelstromTimer() {
        return unit->getMaelstromTimer();
}

// TODO: add doc
int Unidade::getOrderTimer() {
        return unit->getOrderTimer();
}

/** Returns the time until the plague wears off. 0 -> No plague present. */
int Unidade::getPlagueTimer() {
        return unit->getPlagueTimer();
}

/** Returns the amount of time until the unit is removed, or 0 if the unit does not have a remove timer.
  * Used to determine how much time remains before hallucinated units, dark swarm, etc have until they
  * are removed. */
int Unidade::getRemoveTimer() {
        return unit->getRemoveTimer();
}

/** Returns the time until the stasis field wears off. 0 -> No stasis field present. */
int Unidade::getStasisTimer() {
        return unit->getStasisTimer();
}

/** Returns the time until the stimpack wears off. 0 -> No stimpack boost present. */
int Unidade::getStimTimer() {
        return unit->getStimTimer();
}

/** Returns the building type a worker is about to construct. If the unit is a morphing Zerg unit or an
  * incomplete building, this returns the UnitType the unit is about to become upon completion.*/
BWAPI::UnitType Unidade::getBuildType() {
        return unit->getBuildType();
}

/** Returns the list of units queued up to be trained.
  * \see Unit::train, Unit::cancelTrain, Unit::isTraining. */
std::list<BWAPI::UnitType> Unidade::getTrainingQueue() {
        return unit->getTrainingQueue();
}

/** Returns the tech that the unit is currently researching. If the unit is not researching anything,
  * TechTypes::None is returned.
  * \see Unit::research, Unit::cancelResearch, Unit::isResearching, Unit::getRemainingResearchTime. */
BWAPI::TechType Unidade::getTech() {
        return unit->getTech();
}

/** Returns the upgrade that the unit is currently upgrading. If the unit is not upgrading anything,
  * UpgradeTypes::None is returned.
  * \see Unit::upgrade, Unit::cancelUpgrade, Unit::isUpgrading, Unit::getRemainingUpgradeTime. */
BWAPI::UpgradeType Unidade::getUpgrade() {
        return unit->getUpgrade();
}

/** Returns the remaining build time of a unit/building that is being constructed. */
int Unidade::getRemainingBuildTime() {
        return unit->getRemainingBuildTime();
}

/** Returns the remaining time of the unit that is currently being trained. If the unit is a Hatchery,
  * Lair, or Hive, this returns the amount of time until the next larva spawns, or 0 if the unit already
  * has 3 larva. */
int Unidade::getRemainingTrainTime() {
        return unit->getRemainingTrainTime();
}

/** Returns the amount of time until the unit is done researching its current tech. If the unit is not
  * researching anything, 0 is returned.
  * \see Unit::research, Unit::cancelResearch, Unit::isResearching, Unit::getTech. */
int Unidade::getRemainingResearchTime() {
        return unit->getRemainingResearchTime();
}

/** Returns the amount of time until the unit is done upgrading its current upgrade. If the unit is not
  * upgrading anything, 0 is returned.
  * \see Unit::upgrade, Unit::cancelUpgrade, Unit::isUpgrading, Unit::getUpgrade. */
int Unidade::getRemainingUpgradeTime() {
        return unit->getRemainingUpgradeTime();
}

/** If the unit is an SCV that is constructing a building, this will return the building it is
  * constructing. If the unit is a Terran building that is being constructed, this will return the SCV
  * that is constructing it. */
Unidade* Unidade::getBuildUnit() {
        return (*mapa)[unit->getBuildUnit()];
}

/** Generally returns the appropriate target unit after issuing an order that accepts a target unit
  * (i.e. attack, repair, gather, follow, etc.). To check for a target that has been acquired
  * automatically (without issuing an order) see getOrderTarget. */
Unidade* Unidade::getTarget() {
        return (*mapa)[unit->getTarget()];
}

/** Returns the target position the unit is moving to (provided a valid path to the target position
  * exists). */
BWAPI::Position Unidade::getTargetPosition() {
        return unit->getTargetPosition();
}

// TODO: add doc

BWAPI::Order Unidade::getOrder() {
        return unit->getOrder();
}

/** This is usually set when the low level unit AI acquires a new target automatically. For example if
  * an enemy probe comes in range of your marine, the marine will start attacking it, and getOrderTarget
  * will be set in this case, but not getTarget. */
Unidade* Unidade::getOrderTarget() {
        return (*mapa)[unit->getOrderTarget()];
}

BWAPI::Order Unidade::getSecondaryOrder() {
        return unit->getSecondaryOrder();
}

/** Returns the position the building is rallied to. If the building does not produce units,
  * Positions::None is returned.
  * \see Unit::setRallyPoint, Unit::getRallyUnit. */
BWAPI::Position Unidade::getRallyPosition() {
        return unit->getRallyPosition();
}

/** Returns the unit the building is rallied to. If the building is not rallied to any unit, NULL is
  * returned.
  * \see Unit::setRallyPoint, Unit::getRallyPosition. */
Unidade* Unidade::getRallyUnit() {
        return (*mapa)[unit->getRallyUnit()];
}

/** Returns the add-on of this unit, or NULL if the unit doesn't have an add-on. */
Unidade* Unidade::getAddon() {
        return (*mapa)[unit->getAddon()];
}

/** Returns the corresponding connected nydus canal of this unit, or NULL if the unit does not have a
  * connected nydus canal. */
Unidade* Unidade::getNydusExit() {
        return (*mapa)[unit->getNydusExit()];
}

/** Returns the power up the unit is holding, or NULL if the unit is not holding a power up */
Unidade* Unidade::getPowerUp() {
        return (*mapa)[unit->getPowerUp()];
}

/** Returns the dropship, shuttle, overlord, or bunker that is this unit is loaded in to. */
Unidade* Unidade::getTransport() {
        return (*mapa)[unit->getTransport()];
}

/** Returns a list of the units loaded into a Terran Bunker, Terran Dropship, Protoss Shuttle, or Zerg
  * Overlord. */
std::set<Unidade*> Unidade::getLoadedUnits() {
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = unit->getLoadedUnits();
        for(std::set<Unit*>::const_iterator it = temp.begin(); it != temp.end(); it++){
                conjunto.insert((*mapa)[*it]);
        }
        return conjunto;
}

/** For Protoss Interceptors, this returns the Carrier unit this Interceptor is controlled by. For all
  * other unit types this function returns NULL. */
Unidade* Unidade::getCarrier() {
        return (*mapa)[unit->getCarrier()];
}

/** Returns the set of interceptors controlled by this unit. If the unit has no interceptors, or is not
  * a Carrier, this function returns an empty set. */
std::set<Unidade*> Unidade::getInterceptors() {
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = unit->getInterceptors();
        for(std::set<Unit*>::const_iterator it = temp.begin(); it != temp.end(); it++){
                conjunto.insert((*mapa)[*it]);
        }
        return conjunto;
}

/** For Zerg Larva, this returns the Hatchery, Lair, or Hive unit this Larva was spawned from. For all
  * other unit types this function returns NULL. */
Unidade* Unidade::getHatchery() {
        return (*mapa)[unit->getHatchery()];
}

/** Returns the set of larva spawned by this unit. If the unit has no larva, or is not a Hatchery, Lair,
  * or Hive, this function returns an empty set. Equivalent to clicking "Select Larva" from the Starcraft
  * GUI. */
std::set<Unidade*> Unidade::getLarva() {
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = unit->getLarva();
        for(std::set<Unit*>::const_iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[*it]);
        }
        return conjunto;
}

/** Returns the set of units within the given radius of this unit */
std::set<Unidade*> Unidade::getUnitsInRadius(int radius){
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = unit->getUnitsInRadius(radius);
        for(std::set<Unit*>::const_iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[*it]);
        }
        return filtrar(conjunto);
}

/** Returns the set of units within weapon range of this unit. */
std::set<Unidade*> Unidade::getUnitsInWeaponRange(BWAPI::WeaponType weapon) {
        std::set<Unidade*> conjunto;
        std::set<Unit*> temp = unit->getUnitsInWeaponRange(weapon);
        for(std::set<Unit*>::const_iterator it = temp.begin(); it != temp.end(); it++){
                if((*mapa)[*it] != NULL)
                conjunto.insert((*mapa)[*it]);
        }
        return filtrar(conjunto);
}

/**
  * 3 cases to consider:
  *
  * - If exists() returns true, the unit exists.
  * - If exists() returns false and the unit is owned by self(), then the unit does not exist.
  * - If exists() returns false and the unit is not owned by self(), then the unit may or may not exist.
  *
  * \see Unit::isVisible.
  * */
bool Unidade::exists() {
        return unit->exists();
}

/* Returns true if the Nuclear Missile Silo has a nuke */
bool Unidade::hasNuke() {
        return unit->hasNuke();
}

/** Returns true if the unit is currently accelerating. */
bool Unidade::isAccelerating() {
        return unit->isAccelerating();
}

// TODO: add doc
bool Unidade::isAttacking() {
        return unit->isAttacking();
}

bool Unidade::isAttackFrame() {
        return unit->isAttackFrame();
}

/** Returns true if the unit is being constructed. Always true for incomplete Protoss and Zerg
  * buildings, and true for incomplete Terran buildings that have an SCV constructing them. If the SCV
  * halts construction, isBeingConstructed will return false.
  *
  * \see Unit::build, Unit::cancelConstruction, Unit::haltConstruction, Unit::isConstructing. */
bool Unidade::isBeingConstructed() {
        return unit->isBeingConstructed();
}

/** Returns true if the unit is a mineral patch or refinery that is being gathered. */
bool Unidade::isBeingGathered() {
        return unit->isBeingGathered();
}

/** Returns true if the unit is currently being healed by a Terran Medic, or repaired by a Terran SCV. */
bool Unidade::isBeingHealed() {
        return unit->isBeingHealed();
}

/** Returns true if the unit is currently blind from a Medic's Optical Flare. */
bool Unidade::isBlind() {
        return unit->isBlind();
}

/** Returns true if the unit is currently braking/slowing down. */
bool Unidade::isBraking() {
        return unit->isBraking();
}

/** Returns true if the unit is a Zerg unit that is current burrowed.
  * \see Unit::burrow, Unit::unburrow. */
bool Unidade::isBurrowed() {
        return unit->isBurrowed();
}

/** Returns true if the unit is a worker that is carrying gas.
  * \see Unit::returnCargo, Unit::isGatheringGas. */
bool Unidade::isCarryingGas() {
        return unit->isCarryingGas();
}

/** Returns true if the unit is a worker that is carrying minerals.
  * \see Unit::returnCargo, Unit::isGatheringMinerals. */
bool Unidade::isCarryingMinerals() {
        return unit->isCarryingMinerals();
}

/** Returns true if the unit is cloaked.
  * \see Unit::cloak, Unit::decloak. */
bool Unidade::isCloaked() {
        return unit->isCloaked();
}

/** Returns true if the unit has been completed. */
bool Unidade::isCompleted() {
        return unit->isCompleted();
}

/** Returns true when a unit has been issued an order to build a structure and is moving to the build
  * location. Also returns true for Terran SCVs while they construct a building.
  * \see Unit::build, Unit::cancelConstruction, Unit::haltConstruction, Unit::isBeingConstructed. */
bool Unidade::isConstructing() {
        return unit->isConstructing();
}

/** Returns true if the unit has a defense matrix from a Terran Science Vessel. */
bool Unidade::isDefenseMatrixed() {
        return unit->isDefenseMatrixed();
}

/** Returns true if the unit is detected. */
bool Unidade::isDetected() {
        return unit->isDetected();
}

/** Returns true if the unit has been ensnared by a Zerg Queen. */
bool Unidade::isEnsnared() {
        return unit->isEnsnared();
}

/** Returns true if the unit is following another unit.
  * \see Unit::follow, Unit::getTarget. */
bool Unidade::isFollowing() {
        return unit->isFollowing();
}

/** Returns true if the unit is in one of the four states for gathering gas (MoveToGas, WaitForGas,
  * HarvestGas, ReturnGas).
  * \see Unit::isCarryingGas. */
bool Unidade::isGatheringGas() {
        return unit->isGatheringGas();
}

/** Returns true if the unit is in one of the four states for gathering minerals (MoveToMinerals,
  * WaitForMinerals, MiningMinerals, ReturnMinerals).
  * \see Unit::isCarryingMinerals. */
bool Unidade::isGatheringMinerals() {
        return unit->isGatheringMinerals();
}

/** Returns true for hallucinated units, false for normal units. Returns true for hallucinated enemy
  * units only if Complete Map Information is enabled.
  * \see Unit::getRemoveTimer. */
bool Unidade::isHallucination() {
        return unit->isHallucination();
}

/** Returns true if the unit is holding position
  * \see Unit::holdPosition. */
bool Unidade::isHoldingPosition() {
        return unit->isHoldingPosition();
}

/** Returns true if the unit is not doing anything.
  * \see Unit::stop. */
bool Unidade::isIdle() {
        return unit->isIdle();
}

/** Returns true if the unit can be interrupted. */
bool Unidade::isInterruptible() {
        return unit->isInterruptible();
}

/** Returns true if the unit is invincible. */
bool Unidade::isInvincible() {
        return unit->isInvincible();
}

/** Returns true if the unit can attack a specified target from its current position. */
bool Unidade::isInWeaponRange(Unidade *target) {
        return unit->isInWeaponRange(target->unit);
}

/** Returns true if the unit is being irradiated by a Terran Science Vessel.
  * \see Unit::getIrradiateTimer. */
bool Unidade::isIrradiated() {
        return unit->isIrradiated();
}

/** Returns true if the unit is a Terran building that is currently lifted off the ground.
  * \see Unit::lift,Unit::land. */
bool Unidade::isLifted() {
        return unit->isLifted();
}

/** Return true if the unit is loaded into a Terran Bunker, Terran Dropship, Protoss Shuttle, or Zerg
  * Overlord.
  * \see Unit::load, Unit::unload, Unit::unloadAll. */
bool Unidade::isLoaded() {
        return unit->isLoaded();
}

/** Returns true if the unit is locked down by a Terran Ghost.
  *  \see Unit::getLockdownTimer. */
bool Unidade::isLockedDown() {
        return unit->isLockedDown();
}

/** Returns true if the unit is being maelstrommed.
  * \see Unit::getMaelstromTimer. */
bool Unidade::isMaelstrommed() {
        return unit->isMaelstrommed();
}

/** Returns true if the unit is a zerg unit that is morphing.
  * \see Unit::morph, Unit::cancelMorph, Unit::getBuildType, Unit::getRemainingBuildTime. */
bool Unidade::isMorphing() {
        return unit->isMorphing();
}

/** Returns true if the unit is moving.
  * \see Unit::attack, Unit::stop. */
bool Unidade::isMoving() {
        return unit->isMoving();
}

/** Returns true if the unit has been parasited by some other player. */
bool Unidade::isParasited() {
        return unit->isParasited();
}

/** Returns true if the unit is patrolling between two positions.
  * \see Unit::patrol. */
bool Unidade::isPatrolling() {
        return unit->isPatrolling();
}

/** Returns true if the unit has been plagued by a Zerg Defiler.
  * \see Unit::getPlagueTimer. */
bool Unidade::isPlagued() {
        return unit->isPlagued();
}

/** Returns true if the unit is a Terran SCV that is repairing or moving to repair another unit. */
bool Unidade::isRepairing() {
        return unit->isRepairing();
}

/** Returns true if the unit is a building that is researching tech. See TechTypes for the complete list
  * of available techs in Broodwar.
  * \see Unit::research, Unit::cancelResearch, Unit::getTech, Unit::getRemainingResearchTime. */
bool Unidade::isResearching() {
        return unit->isResearching();
}

/** Returns true if the unit has been selected by the user via the starcraft GUI. Only available if you
  * enable Flag::UserInput during AIModule::onStart.
  * \see Game::getSelectedUnits. */
bool Unidade::isSelected() {
        return unit->isSelected();
}

/** Returns true if the unit is a Terran Siege Tank that is currently in Siege mode.
  * \see Unit::siege, Unit::unsiege. */
bool Unidade::isSieged() {
        return unit->isSieged();
}

/** Returns true if the unit is starting to attack.
  * \see Unit::attackUnit, Unit::getGroundWeaponCooldown, Unit::getAirWeaponCooldown. */
bool Unidade::isStartingAttack() {
        return unit->isStartingAttack();
}

/** Returns true if the unit has been stasised by a Protoss Arbiter.
  * \see Unit::getStasisTimer. */
bool Unidade::isStasised() {
        return unit->isStasised();
}

/** Returns true if the unit is currently stimmed.
  * \see Unit::getStimTimer. */
bool Unidade::isStimmed() {
        return unit->isStimmed();
}

/** Returns true if the unit is being pushed off of another unit */
bool Unidade::isStuck() {
        return unit->isStuck();
}

/** Returns true if the unit is training units (i.e. a Barracks training Marines).
  * \see Unit::train, Unit::getTrainingQueue, Unit::cancelTrain, Unit::getRemainingTrainTime. */
bool Unidade::isTraining() {
        return unit->isTraining();
}
/** Returns true if the unit was recently attacked. */
bool Unidade::isUnderAttack(){
        return unit->isUnderAttack();
}

/** Returns true if the unit is under a Protoss Psionic Storm. */
bool Unidade::isUnderStorm() {
        return unit->isUnderStorm();
}

/** Returns true if the unit is under a Dark Swarm. */
bool Unidade::isUnderDarkSwarm() {
        return unit->isUnderDarkSwarm();
}

/** Returns true if the unit is under a Disruption Web. */
bool Unidade::isUnderDisruptionWeb() {
        return unit->isUnderDisruptionWeb();
}

/** Returns true if the unit is a Protoss building that is unpowered because no pylons are in range. */
bool Unidade::isUnpowered() {
        return unit->isUnpowered();
}

/** Returns true if the unit is a building that is upgrading. See UpgradeTypes for the complete list
  * of available upgrades in Broodwar.
  * \see Unit::upgrade, Unit::cancelUpgrade, Unit::getUpgrade, Unit::getRemainingUpgradeTime. */
bool Unidade::isUpgrading() {
        return unit->isUpgrading();
}

/** Returns true if the unit is visible. If the CompleteMapInformation?  cheat flag is enabled, existing
  * units hidden by the fog of war will be accessible, but isVisible will still return false.
  * \see Unit::exists. */
bool Unidade::isVisible() {
        return unit->isVisible();
}