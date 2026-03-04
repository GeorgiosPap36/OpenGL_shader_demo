# OpenGL shader demo

This project serves as a playground to try out different shaders. I have implemented a basic engine loop that separates rendering from updating. 

The rendering is done in batches to minimize state changes using Materials. Also, the render method allows the pipelining of different post-processing shaders.


### Resources:
-  [Sponza.obj](https://github.com/jimmiebergmann/Sponza)