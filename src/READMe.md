#Deep-Dive Technical Documentation & Code Architecture:

This document provides a detailed walkthrough of the inner workings, data structures, and implementation choices   behind this C++ 3D Game 


1. Data-Oriented ECS Architecture (Registry & ComponentPool)

Instead of traditional Object-Oriented inheritance (e.g., a massive GameObject base class), the engine uses a Data-Oriented ECS to maximize cache locality and performance.Entity & Memory LayoutEntity: A simple 32-bit integer ID (typedef uint32_t Entity). It contains no data or functions.

ComponentPool<T>: Uses a Sparse-Set algorithm (Dense/Sparse vectors) to store components contiguously in 

memory:std::vector<T> dense: Stores actual component instances (TransformComponent, RigidBody) packed next to each other in memory.std::vector<uint32_t> 

denseIds: Maps the dense index back to the Entity ID.getComponent(Entity id): Performs an O(1) lookup to fetch a direct pointer to the memory location of the component.Why this matters: When iterating over components in systems (e.g., physics or rendering), the CPU reads contiguous memory arrays rather than chasing pointers across heap locations, preventing CPU cache misses.2. Physics & Collision Resolution PipelinePhysics execution is decoupled into discrete stages handled within the task scheduling pipeline:[Apply Forces / Gravity] ──> [Integrate Velocity & Position] ──> [AABB Manifold Generation] ──> [Impulse Response & Correction]

2. Key Functions & MathGravity & Integration

(applyGravity & updatePhysics):
 
 Applies constant downward acceleration to RigidBody::velocity.Integrates position
 
Broad/Narrow-phase Collision
 
Detection:Creates an AABB (Axis-Aligned Bounding Box) for entities by combining TransformComponent::position and scale.Calculates a collision Manifold (determines if colliding == true, computes penetration depth, and normal vector.Impulse & Positional Correction (collide & posCorrection):Impulse Resolution: Adjusts velocities of colliding entities based on restitution.Positional Correction: Separates overlapping bounding boxes along the collision normal to prevent entities from sinking into each other due to floating-point precision errors.


3. Raycasting & Object Selection AlgorithmMouse picking works by projecting 2D screen coordinates into 3D world space using Raylib’s matrix math:Ray Generation:GetMouseRay(mousePos, camera) converts screen coordinates $(X, Y)$ into a 3D ray with a origin vector and direction vector.Ray-Sphere Intersection:The ray is tested against temporary bounding spheres (Sphere tempSphere(pos, scale.x))

Detection & State Mutation 

If an intersection exists (hit != std::nullopt), the corresponding entity's MeshComponent::color is mutated (e.g., changed to RED).

4. Task Scheduling System (TaskScheduler)Instead of dumping all engine updates directly into a single linear loop, the engine uses a Priority-Based Task Scheduler:C++taskScheduler.insertEvent(Event(lambda_function, priority, is_repeating));

5. Prioritized Execution: Events are registered with a priority integer (e.g., Priority 1 for Physics Integration, Priority 2 for Collisions, Priority 4 for Input/Raycasting).Lambda Captures: Systems capture references/pointers to component pools ([rigidBodyPool, tfcPool]) to run isolated tasks inside taskScheduler.update(GetFrameTime()).5. Rendering Pipeline & Frame SynchronizationThe engine uses a Hybrid OpenGL Frame Workflow managed by Raylib and ImGui. Order of execution is strict to prevent framebuffer clears or state corruption

6. Live ECS Inspector Mechanics (drawECSInspector)The live editor is built using Dear ImGui coupled directly to the ECS pools via raw pointers.Entity Selection: A local state variable Entity selectedEntity tracks which entity is currently active in the UI.Hierarchy Window: Iterates over active entity IDs in tfcPool->denseIds and renders selectable UI nodes (ImGui::Selectable).Component Mutation:Component addresses are fetched: TransformComponent* tfc = tfcPool->getComponent(selectedEntity).Controls like ImGui::DragFloat3("Position", &tfc->position.x, 0.1f) pass direct pointers to struct members.Modifying sliders directly mutates memory in the dense array of the ECS, updated live in the 3D viewport without recompilation.

7. Linker Breakdown:main.cpp: Engine entry point and systems code.includes/ui/*.cpp: Compiles imgui.cpp, imgui_draw.cpp, imgui_tables.cpp, imgui_widgets.cpp, imgui_demo.cpp, and rlImGui.cpp.-I includes/ui: Header search path for ImGui includes.-lraylib -lopengl32 -lgdi32 -lwinmm: Raylib graphics core + Windows native OpenGL & Multimedia system drivers.