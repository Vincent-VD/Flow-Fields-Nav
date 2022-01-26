# Flow Fields Navigation
Pathfinding with crowds
  ~  by Vincent VD
  
  Single-agent pathfinding is done with A* on a grid or navmesh.
  But while this is suitable for a small amount of actors, when you want to simulate crowds,
  you'll find it doesn't that well anymore.
  
  In this project I'll be implementing flow fields, a technique which assigns a direction to each space in a grid,
  and every actor inside will follow that direction.
