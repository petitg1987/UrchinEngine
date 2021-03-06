# UrchinEngine
## Features
Game engine library in C++17: 3D, Physics, Sound, Network, IA, Map Editor

Video: <https://www.youtube.com/watch?v=lSbCx7u6RtE>

* 3D Engine:
  * Graphic API: Vulkan
  * 3D models with skeleton animation & Blender exporter
  * Materials: diffuse, normal mapping, ambient color, anisotropy, mip-maps
  * Dynamic shadow: cached parallel split shadow maps, variance shadow map, blur
  * Dynamic lighting with deferred shading: omni-directional, directional
  * Terrain multi-layered and grass
  * Ambient occlusion (SSAO+)
  * Anti-aliasing (FXAA)
  * Space partitioning: octree
  * UI: text, button, window, text input, bitmap, sequence, checkbox, slider
  * Skybox
  * Fog

* Physics Engine:
  * Supported shapes: sphere, box, cylinder, capsule, cone, convex hull, compound shape, terrain
  * Body types: rigid body, ghost body
  * Algorithms:
    * Broad phase: AABBox Tree
    * Narrow phase: sphere-sphere, sphere-box, GJK/EPA, concave-convex
    * Constraint solver (iterative): external force, friction, restitution, inertia
  * Continuous collision detection (CCD)
  * Island
  * Ray cast
  * Character controller

* Sound Engine:
  * Supported formats: ogg
  * Spatial sound and global sound
  * Sound trigger based on character position

* IA Engine:
  * Automatic navigation mesh generator
  * Pathfinding A* with funnel algorithm
  * Character controller
  
* Network Engine:
  * HTTP request sender
  
* Map Editor:
  * Handle:
    * 3d objects with physics
    * Lights
    * Landscape: terrain, water, sky
    * Sound
    * AI agent
  * Debug visualization: hit box, light range, sound trigger, navigation mesh

## Build
* Environment setup: [here](./DEV.md)
* Build:
  * Debug:
    ```
    git clone https://github.com/petitg1987/urchinEngine.git && cd ./urchinEngine
    cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B .build/debug/
    cmake --build .build/debug/ -- -j 12
    ```
  * Release:
    ```
    git clone https://github.com/petitg1987/urchinEngine.git && cd ./urchinEngine
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -S ./ -B .build/release/
    cmake --build .build/release/ -- -j 12
    ```
* Execute tests:
  ```
  cd urchinEngine/test/
  ./testExecutor [unit] [integration] [monkey]
  ```
* Launch map editor:
  ```
  cd urchinEngine/mapEditor/
  ./urchinMapEditor
  ```
