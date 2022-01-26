# Flow Fields Navigation
Pathfinding with crowds
  ~  by Vincent VD
  
  Single-agent pathfinding is done with A* on a grid or navmesh.
  But while this is suitable for a small amount of actors, when you want to simulate crowds,
  you'll find it doesn't that well anymore.
  
  In this project I'll be implementing flow fields, a technique which assigns a direction to each space in a grid,
  and every actor inside will follow that direction.

## Overview

There are 3 main components in this project: a navmesh, a flock and a grid.

### The Navmesh
This is the only part I didn't write myself, I reused code from a prior project. What I did write was the FindPath algorithm that uses A* pathfinding to find a path, and smooths over the returned path with portals.

### The Flock
The flock contains an array of agents that is iterated every frame on the CellSpace (the grid). 

### The CellSpace
This is a grid that spans the same space as the navmesh. Every cell of the grid has a vector that dictates the direction of agents in the cell. When the target is changed, for every cell it changes the target of that cell (and also the direction), by calculating the path using the navmesh, and choosing the closest (first) point of that path.

## How it works

When the middle mouse button is pressed, it recalculates the grid and creates a new 'sink'. 
