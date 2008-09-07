;**************************
;******** TEXTURES ********
;**************************
Texture particleTex {
 filename="Data/Graphics/particles.png"
}
Texture mainTx1 {
 filename="Data/Graphics/sprites1.png"
}
Texture menuGuiTx {
 filename="Data/Graphics/menu2.png"
}
Texture titleScreenTx {
 filename="Data/Graphics/menu1.png"
}
Texture botonoidSelectTx {
 filename="Data/Graphics/botonoidSelect.png"
}

;//***********************
;//******** PLAYER *******
;//***********************
Animation goldBotonoid {
 texture=mainTx1
 rect=0,0,38,36
 frames=16
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=19,20
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Animation blackBotonoid {
 texture=mainTx1
 rect=0,36,32,46
 frames=16
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=16,32
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Animation silverBotonoid {
 texture=mainTx1
 rect=0,82,33,37
 frames=16
 fps=20.0
 mode=FORWARD,LOOP
 hotspot=16,25
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//********************************
;//******** BOTONOID SELECT *******
;//********************************
Sprite selectGold {
 texture=botonoidSelectTx
 rect=0,0,281,322
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite selectBlack {
 texture=botonoidSelectTx
 rect=0,322,282,354
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite selectWhite {
 texture=botonoidSelectTx
 rect=281,0,230,356
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite check {
 texture=botonoidSelectTx
 rect=281,356,207,124
 hotspot=103,62
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

Sprite selecter1 {
 texture=titleScreenTx
 rect=752,768,75,75
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite lockedSelecter1 {
 texture=titleScreenTx
 rect=827,768,75,75
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite selecter2 {
 texture=titleScreenTx
 rect=752,843,75,75
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite lockedSelecter2 {
 texture=titleScreenTx
 rect=827,843,75,75
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite selecter3 {
 texture=titleScreenTx
 rect=752,918,75,75
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite lockedSelecter3 {
 texture=titleScreenTx
 rect=827,918,75,75
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//*********************
;//******** MENU *******
;//*********************
Sprite titlescreen {
 texture=titleScreenTx
 rect=0,0,1024,768
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite mouse {
 texture=titleScreenTx
 rect=504,910,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

Sprite buttonGraphic {
 texture=titleScreenTx
 rect=504,768,248,71
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite highlightedButtonGraphic {
 texture=titleScreenTx
 rect=504,839,248,71
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//Petals
Sprite petal1 {
 texture=titleScreenTx
 rect=0,768,201,112
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite petal2 {
 texture=titleScreenTx
 rect=201,768,151,198
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite petal3 {
 texture=titleScreenTx
 rect=352,768,152,191
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite petal4 {
 texture=titleScreenTx
 rect=0,880,200,111
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

Sprite optionsGUI {
 texture=menuGuiTx
 rect=0,0,927,511
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite volumeBlock {
 texture=menuGuiTx
 rect=927,0,59,35
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite customizeGUI {
 texture=menuGuiTx
 rect=0,511,927,511
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//**********************
;//******** FONTS *******
;//**********************
Font debug {
 filename="Data/Fonts/debug.fnt"
 tracking=1.0
}
Font score {
 filename="Data/Fonts/score.fnt"
 tracking=1.0
}
Font timer {
 filename="Data/Fonts/timer.fnt"
 tracking=1.0
}
Font button {
 filename="Data/Fonts/button.fnt"
 tracking=1.0
}
Font tooltip {
 filename="Data/Fonts/tooltip.fnt"
 tracking=1.0
}
Font input {
 filename="Data/Fonts/input.fnt"
 tracking=1.0
}
Font smallBattlefield {
 filename="Data/Fonts/smallBattlefield.fnt"
 tracking=1.0
}

;//*********************
;//******** GUI ********
;//*********************
Sprite blackbox {
 texture=mainTx1
 rect=450,650,16,16
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite guiTop {
 texture=mainTx1
 rect=0,599,164,384
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite guiBottom {
 texture=mainTx1
 rect=164,599,164,384
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//Bars
Sprite goldBar {
 texture=mainTx1
 rect=535,36,73,19
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite silverBar {
 texture=mainTx1
 rect=535,55,73,19
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite blackBar {
 texture=mainTx1
 rect=535,74,73,19
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//Item windows
Sprite goldWindow {
 texture=mainTx1
 rect=672,0,115,98
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite blackWindow {
 texture=mainTx1
 rect=787,0,115,98
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite silverWindow {
 texture=mainTx1
 rect=902,0,115,98
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//Botonoid face animations
Animation goldFace {
 texture=mainTx1
 rect=452,408,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Animation blackFace {
 texture=mainTx1
 rect=452,470,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Animation silverFace {
 texture=mainTx1
 rect=452,532,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,LOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//Health hypocycloids
Sprite goldHealth {
 texture=mainTx1
 rect=0,120,30,31
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite blackHealth {
 texture=mainTx1
 rect=30,120,30,31
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite silverHealth {
 texture=mainTx1
 rect=60,120,30,31
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

Sprite emptySlotGold {
 texture=mainTx1
 rect=608,0,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite emptySlotBlack {
 texture=mainTx1
 rect=608,32,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite emptySlotWhite {
 texture=mainTx1
 rect=608,64,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;//Count down
Sprite countdown3 {
 texture=mainTx1
 rect=333,141,111,141
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite countdown2 {
 texture=mainTx1
 rect=444,0,111,142
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite countdown1 {
 texture=mainTx1
 rect=555,0,111,141
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;**************************
;******** PARTICLE ********
;**************************
Sprite dustParticleSpr {
 texture=particleTex
 rect=0,0,32,32
 hotspot=16,16
}
Sprite particleGraphic0 {
 texture=particleTex
 rect=0,0,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHAADD,NOZWRITE
}
Sprite particleGraphic4 {
 texture=particleTex
 rect=0,32,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite particleGraphic5 {
 texture=particleTex
 rect=32,32,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite particleGraphic11 {
 texture=particleTex
 rect=96,64,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Particle dust {
 filename="particle8.psi"
 sprite=dustParticleSpr
}
Particle explosion {
 filename="explosion.psi"
 sprite=particleGraphic0
}
Particle slow {
 filename="slow.psi"
 sprite=particleGraphic11
}
Particle itemTrail {
 filename="trail.psi"
 sprite=particleGraphic0
}

;****************************
;******** STATS PAGE ********
;****************************
Sprite statsPage {
 texture=mainTx1
 rect=328,594,630,310
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite wallIcon {
 texture=mainTx1
 rect=120,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite gardenIcon {
 texture=mainTx1
 rect=152,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite biggestComboIcon {
 texture=mainTx1
 rect=184,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite timeInFirstIcon {
 texture=mainTx1
 rect=216,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite itemsUsedIcon {
 texture=mainTx1
 rect=248,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite damageDealtIcon {
 texture=mainTx1
 rect=280,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}
Sprite damageTakenIcon {
 texture=mainTx1
 rect=312,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
 resgroup=1
}

;***********************
;******** ITEMS ********
;***********************
Sprite itemcursor {
 texture=mainTx1
 rect=535,93,16,16
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite ghost {
 texture=mainTx1
 rect=344,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}


Texture items {
 filename=Data/Graphics/itemTokens.png
 resgroup=1
}
Sprite wallbreaker {
 texture="Data/Graphics/items.png"
 rect=0,0,28,28
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite missile {
 texture="Data/Graphics/items.png"
 rect=29,0,32,21
 hotspot=16,10
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite crosshair {
 texture="Data/Graphics/items.png"
 rect=0,29,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

Animation punchingglove {
 texture=mainTx1
 rect=376,121,64,30
 frames=9
 fps=45.0
 mode=FORWARD,PINGPONG,LOOP
 hotspot=0,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;***********************
;******** MUSIC ********
;***********************
Music menu {
 filename="menu.xm"
 amplify=50
 resgroup=11
}
Music mtalking {
 filename="modernTalking.it"
 amplify=50
 resgroup=11
}
Music her11 {
 filename="song3.mod"
 amplify=50
 resgroup=11
}
Music noMercy {
 filename="noMercy.mod"
 amplify=30
 resgroup=11
}
Music trainer5 {
 filename="trainer5.mod"
 amplify=40
 resgroup=11
}
Music heidi7 {
 filename="heidi7.xm"
 amplify=40
 resgroup=11
}
Music aSadTouch {
 filename="ASadTouch.xm"
 amplify=60
 resgroup=11
}
Music stStyle {
 filename="stStyle.xm"
 amplify=50
 resgroup=11
}
Music saturn {
 filename="Saturn.xm"
 amplify=50
 resgroup=11
}
Music moskito {
 filename="Moskito.xm"
 amplify=50
 resgroup=11
}
Music jttimarsuEdit {
 filename="JttimarsuEdit.xm"
 amplify=50
 resgroup=11
}
Music puzzler {
 filename="puzzler.mid"
 amplify=20
 resgroup=11
}
Music demag {
 filename="Demag.xm"
 amplify=40
 resgroup=11
}
Music sierain2 {
 filename="Sierain2.xm"
 amplify=50
 resgroup=11
}

;************************
;******** SOUNDS ********
;************************
Sound snd_countdown {
 filename="countDownSound.wav"
 resgroup=1
}
Sound snd_missile {
 filename="missile.mp3"
 resgroup=1
}
Sound snd_explosion1 {
 filename="explosion1.wav"
 resgroup=1
}
Sound snd_explosion2 {
 filename="explosion2.wav"
 resgroup=1
}
Sound snd_alphanoidSelect {
 filename="Alphanoid.wav"
 resgroup=1
}
Sound snd_herbanoidSelect {
 filename="Herbanoid.wav"
 resgroup=1
}
Sound snd_barvinoidSelect {
 filename="Barvinoid.wav"
 resgroup=1
}