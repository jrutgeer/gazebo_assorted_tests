

This is a system plugin to delete a collision entity (and its components) after a certain time,
in an attempt to answer [this question](https://community.gazebosim.org/t/i-want-to-delete-a-collision-of-a-certain-link-at-a-certain-point-in-time-during-the-simulation-run-how-can-i-implement-it-through-code/1983) on community.gazebosim.org.

Things I've learned:

1) A `gz::sim::system` plugin can only be attachted to a world, model or sensor; not e.g. to a link (it is defined like that in the SDF specification).

2) The 'physics' `gz::sim::system` plugin deals with the mapping between Entity Component Manager (ECM) entities (i.e. in `gz::sim`) and the corresponding physics simulation entities (i.e. `gz::physics`).
 As it is a plugin, it has a `Configure` and `Update` method, during which entities are created or removed. However it currently [only supports removal of models](https://github.com/ejalaa12/gz-sim/blob/c03c43261094d3d78a3706df46d10dbede80a918/src/systems/physics/Physics.cc#L1880-L1885), not of sub-entities such as collision geometry.

3) So removing the collision entity from the ECM does not change the physical behavior of the links unfortunately.

4) Possible alternatives:

    1) Maybe collision bitmask functionality, see [this example](https://github.com/gazebosim/gz-sim/blob/gz-sim7/examples/worlds/shapes_bitmask.sdf). Not sure if this can be updated online.

    2) Deleting and recreating the full model (at its current pose, twist, acceleration, etc) instead of only the collision entity,

    3) Patching the physics plugin to support also removal of other entities than full models. 




To test:

- Compile with `colcon build` in the `disable_collsion` directory,
- Export the plugin directory: ``export GZ_SIM_SYSTEM_PLUGIN_PATH=`pwd`/build/disable_collision``
- Run the simulation: `gz sim src/disable_collision.sdf`


