;//***********************
;//******** PLAYER *******
;//***********************
Texture botonoids {
 filename="Data/Graphics/botonoids.png"
 resgroup=1
}
Texture tiles {
 filename="Data/Graphics/tiles.png"
 resgroup=1
}
Animation goldBotonoid {
 texture=botonoids
 rect=0,0,38,36
 frames=16
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=19,20
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Animation blackBotonoid {
 texture=botonoids
 rect=0,36,32,46
 frames=16
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=16,32
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Animation silverBotonoid {
 texture=botonoids
 rect=0,82,33,38
 frames=16
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=16,25
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}


;//*********************
;//******** MENU *******
;//*********************
Sprite titlescreen {
 texture="Data/Graphics/titlescreen.png"
 rect=0,0,1024,768
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1 
}
Sprite minimenu {
 texture="Data/Graphics/minimenu.png"
 rect=0,0,250,225
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1 
}
Sprite mouse {
 texture="Data/Graphics/mouse.png"
 rect=0,0,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1 
}
Sprite miniMenuButton {
 texture="Data/Graphics/menuitems.png"
 rect=0,0,248,71
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1 
}
Sprite miniMenuButtonHighlighted {
 texture="Data/Graphics/menuitems.png"
 rect=248,0,248,71
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1 
}

;//**********************
;//******** FONTS *******
;//**********************
Font debug {
 filename="Data/Fonts/debug.fnt"
 tracking=1.0
 resgroup=1
}
Font score {
 filename="Data/Fonts/score.fnt"
 tracking=1.0
 resgroup=1
}
Font timer {
 filename="Data/Fonts/timer.fnt"
 tracking=1.0
 resgroup=1
}

;//*********************
;//******** GUI ********
;//*********************
Sprite tileorder {
 texture="Data/Graphics/colororder.png"
 rect=0,0,186,26
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite gui {
 texture="Data/Graphics/GUI.png"
 rect=0,0,164,768
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}

;//Bars
Texture bars {
 filename=Data/Graphics/bars.png
 resgroup=1
}
Sprite goldBar {
 texture=bars
 rect=0,0,74,19
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite silverBar {
 texture=bars
 rect=0,19,74,19
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite blackBar {
 texture=bars
 rect=0,38,74,19
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}

;//Item windows
Texture windows {
 filename=Data/Graphics/itemwindows.png
 resgroup=1
}
Sprite goldWindow {
 texture=windows
 rect=0,0,115,98
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite blackWindow {
 texture=windows
 rect=0,98,115,98
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite silverWindow {
 texture=windows
 rect=0,196,115,98
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}

;//Botonoid face animations
Texture botofaces {
 filename=Data/Graphics/botofaces.png
 resgroup=1
}
Animation goldFace {
 texture=botofaces
 rect=0,0,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Animation blackFace {
 texture=botofaces
 rect=0,62,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Animation silverFace {
 texture=botofaces
 rect=0,124,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}

;//Health hypocycloids
Texture health {
 filename=Data/Graphics/hypocycloids.png
 resgroup=1
}
Sprite goldHealth {
 texture=health
 rect=0,0,30,31
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite blackHealth {
 texture=health
 rect=30,0,30,31
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite silverHealth {
 texture=health
 rect=60,0,30,31
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}

;**************************
;******** PARTICLE ********
;**************************
Sprite particleGraphic0 {
 texture="Data/Graphics/particles.png"
 rect=0,0,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Particle dust {
 filename="Data/particle8.psi"
 sprite=particleGraphic0
}

;***********************
;******** MUSIC ********
;***********************
Music song1 {
 filename="song1.xm"
 amplify=50
 resgroup=11
}
Music song2 {
 filename="song2.it"
 amplify=50
 resgroup=11
}
Music song3 {
 filename="song3.mod"
 amplify=50
 resgroup=11
}