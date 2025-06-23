# flgl Programmer's Guide
TODO lol sorry if you're reading this now

# flgl TODO List
I could make a board for this, but im soloing this now so simple todo list instead. These are things the library needs fixing / the addition of.
## huge things
- integrate a basic UI lib, this is already started, needs finish & test & integrate
- test suite !! for not the graphics, lots of other stuff. regression test on the main branch would be fun and useful 
## small - med things
- come back to audio -- streamers, more features, track contexts, ...
- include skybox renderer
- included basic renderer -- render basic screen space shapes, colored or textured.
- faster text renderer -- instance it
- some tool for shader syncing functions? maybe not though
- create a proper (optional) wrapper for gl / al calls that polls for & logs errors on any driver call
- add shader #include system and fix included shaders
- revisit defaultmeshes and see if that needs an update, i think so
- revamp Graphics module -- need some more things, some outdated
- if this is to ever be a properly distributed package (e.g. apt) i'd need to fix some things like the sketchy flgl path reliance and an install target
### done
- ~~**audio** (long time coming) will really complete the transform from 'fl graphics lib' to 'fl game lib'~~
- ~~fix the resource tracking system -- it works, but sucks~~
- ~~add instanced rendering support~~
- ~~add cubemap support to Texture~~
