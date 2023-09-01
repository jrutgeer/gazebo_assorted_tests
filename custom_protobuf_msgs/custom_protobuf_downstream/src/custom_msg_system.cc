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

#include "custom_protobuf_downstream/custom_msg_system.hh"

#include <custom_protobuf_interface/msgs/DetectedObject.pb.h>
#include <gz/plugin/Register.hh>
#include <gz/transport/Node.hh>
#include <gz/sim/Conversions.hh>



using namespace gz;
using namespace sim;
using namespace systems;
using custom_protobuf_interfaces::msgs::DetectedObject;

class gz::sim::systems::DownstreamPrivate
{
  /// \brief Node for publisher
  public: gz::transport::Node gz_node;

  /// \brief Publisher of DetectedObject messages
  public: gz::transport::Node::Publisher object_publisher;

  /// \brief DetectedObject message callback
  public: void OnCustomMessage(const DetectedObject& _msg);
};


//////////////////////////////////////////////////
Downstream::Downstream()
: System(), dataPtr(std::make_unique<DownstreamPrivate>())
{
}


//////////////////////////////////////////////////
void Downstream::Configure(
  const Entity & /*_entity*/,
  const std::shared_ptr<const sdf::Element> & /*_sdf*/,
  EntityComponentManager & /*_ecm*/, EventManager &/*_eventManager*/)
{
  // Publisher
  this->dataPtr->object_publisher
     = this->dataPtr->gz_node.Advertise<DetectedObject>("/detected_object");


  // Subscriber
  if (!this->dataPtr->gz_node.Subscribe( "/detected_object",
                                         &DownstreamPrivate::OnCustomMessage,
                                         this->dataPtr.get()))
  {
    gzerr << "Error subscribing to topic [" << "/detected_object" << "]. "
          << "Published messages will not be echoed." << std::endl;
  }
}


//////////////////////////////////////////////////
void Downstream::PostUpdate(
  const UpdateInfo & _info,
  const EntityComponentManager & /*_ecm*/)
{

  static uint publish_counter = 0;

  if (0 == publish_counter % 500)
  {
    DetectedObject msg;

    msg.mutable_header()->mutable_stamp()->CopyFrom(
        convert<msgs::Time>(_info.simTime));

    msg.set_object_uid(publish_counter);
    
    this->dataPtr->object_publisher.Publish(msg);
  }

  publish_counter++;
}


//////////////////////////////////////////////////
void DownstreamPrivate::OnCustomMessage(const DetectedObject& _msg)
{
  std::cout << "Received message at seconds: " << _msg.header().stamp().sec()
            << " with object uid: " << _msg.object_uid() << "." << std::endl;
}



GZ_ADD_PLUGIN(
  Downstream,
  System,
  Downstream::ISystemConfigure,
  Downstream::ISystemPostUpdate)

GZ_ADD_PLUGIN_ALIAS(Downstream, "gz::sim::systems::Downstream")

// TODO(CH3): Deprecated, remove on version 8
GZ_ADD_PLUGIN_ALIAS(Downstream, "ignition::gazebo::systems::Downstream")
