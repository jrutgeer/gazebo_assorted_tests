#ifndef GZ_SIM_SYSTEMS_DISABLECOLLISION_HH_
#define GZ_SIM_SYSTEMS_DISABLECOLLISION_HH_


#include <gz/sim/System.hh>
 
namespace disable_collision
{
  // Forwared declaration 
  class DisableCollisionPrivate;


  class DisableCollision:
    // This plugin is a system.
    public gz::sim::System,
    // This plugin will be configured upon load
    public gz::sim::ISystemConfigure,
    // This class also implements the ISystemPreUpdate interface.
    public gz::sim::ISystemPreUpdate
  {
    public: DisableCollision();
    
    public: ~DisableCollision() override;

    public: void PreUpdate(const gz::sim::UpdateInfo &_info,
                           gz::sim::EntityComponentManager &_ecm) override;
    
    public: void Configure(const gz::sim::Entity &_entity,
                           const std::shared_ptr<const sdf::Element> &_sdf,
                           gz::sim::EntityComponentManager &_ecm,
                           gz::sim::EventManager &_eventMgr) override;

    // Data pointer for PIMPL programming pattern
    private: std::unique_ptr<DisableCollisionPrivate> dataPtr;
  };
}


#endif