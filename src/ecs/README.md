# entity-mini
A simple header-only entity component system. 

### What is an ECS?
An entity component system (ECS) is a data structure designed for games that manages lists of components that belong to various entities.   
Many entity systems treat entites as containers of components, where components are any user defined type that an entity could have. This saves us from the nightmarish 10,000 line entity classes of the 90's by letting us build entites out of various pieces. However, so many different systems are interested in an entity's components, that looping over entities starts to make less sense than looping over components themselves. An ECS retains this idea of entites as bags of components, but implements it with performance in mind.     

**Thus, the key idea of an entity component system is to increase game loop effeciency by minimizing CPU cache misses, however ECS's can also be useful for purely orginazational reasons.** 

### How does it work?
* Entites are actually just IDs, used to link components together
* Components are any user defined type
* The scene stores components in large contigious arrays, making looping over components effecient.
* The ECS provides a simple way to loop over every entity that has a specific set of components

Here is an example of creating a scene with some entities, giving them components, and looping over them. For an example of real use, see my [flappy bird [OUTDATED]](https://github.com/collebrusco/flappy-bird) game created with this and my graphics library.

```c++
ECS scene;
auto e1 = scene.newEntity();
auto e2 = scene.newEntity();
auto e3 = scene.newEntity(); 

scene.addComp<Transform>(e1);
scene.addComp<Transform>(e2, vec2(0, 0), 90.f); // forward component constructor args
scene.addComp<Transform>(e3);    

scene.addComp<Renderable>(e1);
scene.addComp<Renderable>(e2);

// view every entity with a transform AND a renderable...
for (auto e : scene.view<Transform, Renderable>()){   
  auto& transform = scene.getComp<Transform>(e); // get reference to component...
  // do something...
}

// give view no template args to view all entities...
for (auto e : scene.view<>()){
  // do something...
}
```
