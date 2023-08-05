/*
 * Copyright (C) 2023 Rock Diamond
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "remove_plugin/RemovePluginTest.hh"

#include <gz/common/Profiler.hh>
#include <gz/plugin/Register.hh>

#include <sdf/Element.hh>

#include "gz/sim/SdfEntityCreator.hh"
#include "gz/sim/components/Model.hh"
#include "gz/sim/components/Name.hh"

#include "gz/sim/Model.hh"
#include "gz/sim/Util.hh"

using namespace gz;
using namespace sim;
using namespace systems;

class gz::sim::systems::RemovePluginPrivate
{
  /// \brief SdfEntityCreator, used to _delete_ entities.
  public: std::unique_ptr<SdfEntityCreator> creator{nullptr};

  /// \brief Counts simulation cycles
  public: unsigned int counter{0};

  /// \brief Nr of simulation cycles after which the models are removed
  public: unsigned int cycles{300};

  /// \brief Configure status of the plugin
  public: bool configured{false};
};


//////////////////////////////////////////////////
RemovePlugin::RemovePlugin()
: System(), dataPtr(std::make_unique<RemovePluginPrivate>())
{
}


//////////////////////////////////////////////////
void RemovePlugin::Configure(
  const Entity & _entity,
  const std::shared_ptr<const sdf::Element> & /*_sdf*/,
  EntityComponentManager & _ecm, EventManager &_eventManager)
{
  if (!Model(_entity).Valid(_ecm))
  {
    gzerr << "Remove plugin should be attached to a model entity. "
          << "Failed to initialize." << std::endl;
    return;
  }

  this->dataPtr->creator = std::make_unique<SdfEntityCreator>(_ecm, _eventManager);
  
  this->dataPtr->configured = true;
}


////////////////////////////////////////////////////
void RemovePlugin::PreUpdate(const gz::sim::UpdateInfo &_info,
                             gz::sim::EntityComponentManager &_ecm)
{
  if (!this->dataPtr->configured) return;
  if (_info.paused) return;


  this->dataPtr->counter++;

  if (this->dataPtr->counter == this->dataPtr->cycles - 100)
  {
    auto entity = _ecm.EntityByComponents(components::Name("box_preupdate"), components::Model());
  
    if (kNullEntity == entity) gzerr << "No model named 'box_preupdate'" << std::endl;
    else{
      std::cout << "Removing model 'box_preupdate', entity " << entity << std::endl;
      this->dataPtr->creator->RequestRemoveEntity(entity);
    }
  }

  if (this->dataPtr->counter == this->dataPtr->cycles + 200) 
  {
    auto entity = _ecm.EntityByComponents(components::Name("box_preupdate"), components::Model());
    if (kNullEntity == entity) std::cout << "No model named 'box_preupdate'" << std::endl;

    entity = _ecm.EntityByComponents(components::Name("box_update"), components::Model());
    if (kNullEntity == entity) std::cout << "No model named 'box_update'" << std::endl;

    entity = _ecm.EntityByComponents(components::Name("box_postupdate"), components::Model());
    if (kNullEntity == entity) std::cout << "No model named 'box_postupdate'" << std::endl;
  }

}


//////////////////////////////////////////////////
void RemovePlugin::Update(const gz::sim::UpdateInfo &_info,
                         gz::sim::EntityComponentManager &_ecm)
{
  if (!this->dataPtr->configured) return;
  if (_info.paused) return;

 
  if (this->dataPtr->counter == this->dataPtr->cycles)
  {
    auto entity = _ecm.EntityByComponents(components::Name("box_update"), components::Model());
  
    if (kNullEntity == entity) gzerr << "No model named 'box_update'" << std::endl;
    else{
      std::cout << "Removing model 'box_update', entity " << entity << std::endl;
      this->dataPtr->creator->RequestRemoveEntity(entity);
    }
  }
}


//////////////////////////////////////////////////
void RemovePlugin::PostUpdate(
  const UpdateInfo & _info,
  const EntityComponentManager & _ecm)
{
  if (!this->dataPtr->configured) return;
  if (_info.paused) return;

 
  if (this->dataPtr->counter == this->dataPtr->cycles + 100)
  {
    auto entity = _ecm.EntityByComponents(components::Name("box_postupdate"), components::Model());
  
    if (kNullEntity == entity) gzerr << "No model named 'box_postupdate'" << std::endl;
    else{
      std::cout << "Removing model 'box_postupdate', entity " << entity << std::endl;
      this->dataPtr->creator->RequestRemoveEntity(entity);
    }
  }
}




GZ_ADD_PLUGIN(
  RemovePlugin,
  System,
  RemovePlugin::ISystemConfigure,
  RemovePlugin::ISystemPreUpdate,
  RemovePlugin::ISystemUpdate,
  RemovePlugin::ISystemPostUpdate)

GZ_ADD_PLUGIN_ALIAS(RemovePlugin, "gz::sim::systems::RemovePlugin")

// TODO(CH3): Deprecated, remove on version 8
GZ_ADD_PLUGIN_ALIAS(RemovePlugin, "ignition::gazebo::systems::RemovePlugin")
