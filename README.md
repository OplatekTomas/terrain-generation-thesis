# Terrain generation and adjustment

C++ & OpenGL written app that generates 3D objects which represent small-ish area (max I tested was around 200 km^2) anywhere in the world. Buildings are 3D (they are triangulated, can be a bit broken *sometimes*) and there are multiple types of vegetation generated (using poisson sampling). The height is interpolated using NP Triangle interpolation to deal with low resolution Bing Maps heightmaps and their amazing rate limiting.

The result is renderer using deffered shading with SSAO as post processing effect.

The thesis text (in czech) is available in the `text.pdf` file.

## Compilation

Compilation has been tested on Arch Linux and all you have to do is have some dependencies installed (libpng, marble, Qt5, OpenMP, assimp), call cmake and make. It should handle pulling and compiling the rest.

## Running the app

To run the app provide config file (there is an example one) with API key for Bing Maps and other configuration. The file is provided as 1st command line argument and there is no check if the file is actually valid soooooooo just provide it and make sure it is valid 🙃

## Examples

An area around Austrian town Schladming:

![Ex1](https://i.imgur.com/Mp4n9eX.png)

Part of Brno:

![Ex2](https://i.imgur.com/BoncJi2.png)

## Citation

``` Bibtex
@bachelorsthesis{FITBT20024,
    author = "Tom\'{a}\v{s} Oplatek",
    type = "Bakal\'{a}\v{r}sk\'{a} pr\'{a}ce",
    title = "Generov\'{a}n\'{i} a \'{u}prava 3D ter\'{e}nu podle map",
    school = "Vysok\'{e} u\v{c}en\'{i} technick\'{e} v Brn\v{e}, Fakulta informa\v{c}n\'{i}ch technologi\'{i}",
    year = 2022,
    location = "Brno, CZ",
    language = "czech",
    url = "https://www.fit.vut.cz/study/thesis/20024/"
}
```

## Disclaimer

Nothing really bad should happen, HOWEVER it will use your entire GPU (vegetation rendering is heavily unoptimized since milions of polygons are rendered each frame) and your entire CPU (or most of it - not sure anymore if I limited the thread count) and your PC might crash. **I'm not responsible for any unsaved work or anything else you might lose.**
