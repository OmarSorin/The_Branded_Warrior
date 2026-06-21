# The Branded Warrior

A 2D rogue-like game about adventure and combat. It will be a turn-based game where the goal is to explore a map and find the exit. Along the way, you will encounter monsters that can either be avoided by navigating the grid-like map or fought directly. There will be different types of monsters, of course, with distinct stats and abilities( berserker, regeneration) . The player will also have access to healing potions (and hopefully other effects later on) that will consume a turn when used. I also want to implement different classes, weapons, and armor, but I will figure that out as I go.


Functionalities:

W- move upwards  
A- move to the left  
S- move downwards  
D- move to the left  
ESC- exit game  
U- heal 25hp small potion  
I- heal 50hp medium potion  
O- heal 100hp big potion  

In order to do damage you must press the corresponding direction key( W, A, S, D) whilst being on the adjacent tile to the enemy.\
Similarly, if you are on a tile and you make a move towards an obstacle/wall and the enemy is on an adjacent tile to you, you get damaged.\
You will get hit by the scary monsters after each hit you give, this will be documented in the lower-left corner.\
Exits are hardcoded green but will get textures when i find a good-looking/fitting texture, also text res needs to be worked on, arial is ok but ugly.

Background sprites from - https://craftpix.net/freebies/free-top-down-roguelike-game-kit-pixel-art/ + some drawn by me using piskelapp.com
Character sprites - will be replaced in future versions but gotten from random reddit post https://www.reddit.com/r/Berserk/comments/vflhty/necesito_sprites_de_guts_caminando_hacia/  
Enemy sprites - taken from https://aamatniekss.itch.io/bitcrawl-free-pixelart-assets?download

Random map generation.


## Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o clasă șablon cu sens; minim **2 instanțieri**
  - [ ] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
- [ ] minim 80-90% din codul propriu să fie C++
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.3` sau `v1.0`
- [ ] code review #3 2 proiecte

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).

## Resurse
<!-- renovate: datasource=github-tags depName=SFML/SFML versioning=loose -->
- [SFML](https://github.com/SFML/SFML/tree/3.0.2) (Zlib)
- adăugați trimiteri **detaliate** către resursele externe care v-au ajutat sau pe care le-ați folosit
