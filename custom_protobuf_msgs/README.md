
# Purpose

This example repository shows:

1. How to define **custom** protobuf messages for Gazebo Sim,
   - Which **use existing messages**, and
   - Which reside in an **external repository** (i.e. not in the `gz-msgs` source tree),
   
   </br>
2. How to **use** these custom messages in an independent repository.


</br>

Hopefully this example repository can help others to avoid also needing to spend more than a day to find out what exactly to put in the `CMakeLists.txt` files.&emsp;:-(

</br>
</br>

# Overview

## The custom message definition

The `custom_protobuf_interface` folder holds an example `.proto` file, as a mockup for an 'dectected object' message holding a header and the ID and pose of the detected object:

- `gz.msgs.Header`,
- `uint32`,
- `gz.msgs.Pose`.

</br>

In the main `CMakeLists.txt` file a target `custom_msg_lib` is defined (shared library `libcustom_msg_lib.so`).

In the `proto` subfolder, another `CMakeLists.txt` generates the code for the `DetectedObject.proto` example message definition.

The `CMakeLists.txt` files contain furter documentation.

</br>

## Using a custom message definition

The `custom_protobuf_downstream` folder holds a "downstream package", i.e. an independent package that uses the custom message definition.

It contains a trivial `gz::sim::system` plugin that publishes `DetectedObject` messages to the `/detected_object` topic, as well as subscribes to that topic and echoes to `stdout` upon reception of a message.

The code itself is not really relevant; the focus is rather on how to include and link with an external package that defines custom protobuf messages.

The `CMakeLists.txt` file contains furter documentation.

</br>


## Running the example

To test, build the code and run the example world:

```
$ cd custom_protobuf_msgs
$ colcon build
$ source install/setup.bash
$ gz sim worlds/box_above_ground.sdf

```

</br>

The system plugin is both a publisher and a subscriber of the custom message.

In the terminal you will see the output of the subscriber callback. If the simulation is started, the seconds will start counting:


```
Received message at seconds: 0 with object uid: 500.
Received message at seconds: 0 with object uid: 1000.
Received message at seconds: 0 with object uid: 1500.
Received message at seconds: 0 with object uid: 2000.
Received message at seconds: 0 with object uid: 2500.
Received message at seconds: 0 with object uid: 3000.
Received message at seconds: 1 with object uid: 3500.
Received message at seconds: 1 with object uid: 4000.
Received message at seconds: 2 with object uid: 4500.
Received message at seconds: 2 with object uid: 5000.
Received message at seconds: 3 with object uid: 5500.
```

</br>


## Open issues

### gz topic

Even though `gz topic` correctly identifies the message type, it does not seem to echo any messages:

```
$ gz topic -i -t /detected_object
Publishers [Address, Message Type]:
  tcp://192.168.0.211:41469, custom_protobuf_interfaces.msgs.DetectedObject
$ gz topic -e -t /detected_object
     ** no output **
```

So I assume `gz topic` only works with the standard messages.

### CMakeLists.txt contents

The provided `CMakeLists.txt` files work and are elaborately documented. However:
- Some parts are not 100% clear to me (indicated in the comments), and
- For some parts alternative (better?) solutions are possible.

</br>


## Posible remarks

For possible remarks and comments, please open an issue in this repository.

I especially would appreciate further clarifications wrt. the open points in the `CMakeLists.txt` files.
