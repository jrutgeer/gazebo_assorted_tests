#include "DisableCollision.hh"

#include <gz/plugin/Register.hh>
#include <gz/sim/Link.hh>


using namespace disable_collision;
 

class disable_collision::DisableCollisionPrivate
{
  // Collision Entity
  public: gz::sim::Entity collisionEntity{gz::sim::kNullEntity};

  // Duration after which the collision will be deleted
  public: std::chrono::steady_clock::duration disableDuration;

  // Set to true if Configure() has succeeded.
  public: bool configured{false};
};


DisableCollision::DisableCollision()
   : dataPtr(std::make_unique<DisableCollisionPrivate>())
{
}


DisableCollision::~DisableCollision()
{
}


void DisableCollision::Configure(const gz::sim::Entity &_entity,
    const std::shared_ptr<const sdf::Element> &_sdf,
    gz::sim::EntityComponentManager &_ecm,
    gz::sim::EventManager &/*_eventMgr*/)
{
  // This plugin should be attached to a model entity in the SDF
  // Create a link based on the entity
  auto model = gz::sim::Model(_entity);

  // Check if this is a valid model
  if (!model.Valid(_ecm))
  {
    gzerr << "DisableCollision plugin should be attached to a model entity. "
          << "Failed to initialize." << std::endl;
    return;
  }

  // Read duration from SDF
  double duration;
  if (_sdf->Get("collision_duration", duration, 0.0))
  {
    // Not sure if this is the best way to do this; based on the elevator plugin (Elevator::Configure)
    std::chrono::duration<double> disable_duration{duration};
    this->dataPtr->disableDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>(disable_duration);
  }
  else
  {
    gzerr << "No <collision_duration> entry found in the SDF." << std::endl;
    return;
  }

  // Retrieve link name from SDF
  std::string link_name;
  auto sdfElem = _sdf->FindElement("link_name");
  if (sdfElem)
  {
    if (!sdfElem->Get<std::string>().empty())
    {
      link_name = sdfElem->Get<std::string>();
    }
    else
    {
      gzerr << "<link_name> entry in the SDF is empty." << std::endl;
      return;
    }
  }
  else
  {
    gzerr << "DisableCollision plugin requires an entry <link_name> in the SDF." << std::endl;
    return;
  }
  // Retrieve the corresponding link entity
  auto linkEntity = model.LinkByName(_ecm, link_name);
  auto link = gz::sim::Link(linkEntity);
  // Check if this is a valid model
  if (!link.Valid(_ecm))
  {
    gzerr << "DisableCollision plugin could not retrieve the link "
          << "corresponding to link name " << link_name << std::endl;
    return;
  }


  // Identify collision entity to be disabled based on name in SDF
  std::string collision_name;
  sdfElem = _sdf->FindElement("collision_name");
  if (sdfElem)
  {
    if (!sdfElem->Get<std::string>().empty())
    {
      collision_name = sdfElem->Get<std::string>();
    }
    else
    {
      gzerr << "<collision_name> entry in the SDF is empty." << std::endl;
      return;
    }
  }
  else
  {
    gzerr << "DisableCollision plugin requires an entry <collision_name> in the SDF." << std::endl;
    return;
  }

  // Retrieve the corresponding collision entity
  this->dataPtr->collisionEntity = link.CollisionByName(_ecm, collision_name);
  
  if (this->dataPtr->collisionEntity != gz::sim::kNullEntity)
  {
    this->dataPtr->configured = true;
  }
  else
  {
    gzerr << "DisableCollsion plugin: no collision found with name " << collision_name << std::endl;
  }
}



void DisableCollision::PreUpdate(const gz::sim::UpdateInfo &_info,
                           gz::sim::EntityComponentManager &_ecm)
{
  if(this->dataPtr->configured)
  {
    if (_info.simTime >= this->dataPtr->disableDuration)
    {
        _ecm.RequestRemoveEntity(this->dataPtr->collisionEntity);
        this->dataPtr->configured = false;
    }
  }
}




GZ_ADD_PLUGIN(
    disable_collision::DisableCollision,
    gz::sim::System,
    disable_collision::DisableCollision::ISystemConfigure,
    disable_collision::DisableCollision::ISystemPreUpdate)