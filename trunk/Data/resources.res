;**************************
;******** TEXTURES ********
;**************************
Texture mainTx1 {
 filename="Graphics/sprites1.png"
}
Texture menuGuiTx {
 filename="Graphics/menu2.png"
}
Texture titleScreenTx {
 filename="Graphics/menu1.png"
}
Texture botonoidSelectTx {
 filename="Graphics/menu3.png"
}

Sprite stunStar {
 texture=mainTx1
 rect=552,97,10,10
 hotspot=5,5
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
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
 filename="Fonts/debug.fnt"
 tracking=1.0
}
Font score {
 filename="Fonts/score.fnt"
 tracking=1.0
}
Font timer {
 filename="Fonts/timer.fnt"
 tracking=1.0
}
Font button {
 filename="Fonts/button.fnt"
 tracking=1.0
}
Font tooltip {
 filename="Fonts/tooltip.fnt"
 tracking=1.0
}
Font input {
 filename="Fonts/input.fnt"
 tracking=1.0
}
Font smallBattlefield {
 filename="Fonts/smallBattlefield.fnt"
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
 mode=PINGPONG,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Animation blackFace {
 texture=mainTx1
 rect=452,470,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Animation silverFace {
 texture=mainTx1
 rect=452,532,61,62
 frames=4
 fps=8.0
 mode=PINGPONG,NOLOOP
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

Sprite emptyGoldFaceBox {
 texture=mainTx1
 rect=840,408,61,62
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite emptyBlackFaceBox {
 texture=mainTx1
 rect=840,470,61,62
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite emptySilverFaceBox {
 texture=mainTx1
 rect=840,532,61,62
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
 rect=128,407,111,119
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite countdown2 {
 texture=mainTx1
 rect=239,407,111,119
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite countdown1 {
 texture=mainTx1
 rect=350,407,100,119
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}

;**************************
;******** PARTICLE ********
;**************************
Sprite dustParticleSpr {
 texture=mainTx1
 rect=700,420,32,32
 hotspot=16,16
}
Sprite particleGraphic0 {
 texture=mainTx1
 rect=700,420,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHAADD,NOZWRITE
}
Sprite particleGraphic4 {
 texture=mainTx1
 rect=700,452,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite particleGraphic5 {
 texture=mainTx1
 rect=732,452,32,32
 hotspot=16,16
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite particleGraphic11 {
 texture=mainTx1
 rect=796,484,32,32
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
Particle missileTrail {
 filename="trail.psi"
 sprite=particleGraphic0
}
Particle itemTrail {
 filename="particle9.psi"
 sprite=particleGraphic5
}

;****************************
;******** STATS PAGE ********
;****************************
Sprite statsPage {
 texture=mainTx1
 rect=328,594,630,310
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite wallIcon {
 texture=mainTx1
 rect=120,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite gardenIcon {
 texture=mainTx1
 rect=152,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite biggestComboIcon {
 texture=mainTx1
 rect=184,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite timeInFirstIcon {
 texture=mainTx1
 rect=216,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite itemsUsedIcon {
 texture=mainTx1
 rect=248,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite damageDealtIcon {
 texture=mainTx1
 rect=280,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite damageTakenIcon {
 texture=mainTx1
 rect=312,120,32,32
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
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
Sprite wallbreaker {
 texture=mainTx1
 rect=328,904,28,28
 hotspot=0,0
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite missile {
 texture=mainTx1
 rect=357,904,32,21
 hotspot=16,10
 blendmode=COLORMUL,ALPHABLEND,NOZWRITE
}
Sprite crosshair {
 texture=mainTx1
 rect=328,933,32,32
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
 filename="Sounds/menu.xm"
 amplify=50
}
Music mtalking {
 filename="Sounds/modernTalking.it"
 amplify=50
}
Music her11 {
 filename="Sounds/song3.mod"
 amplify=50
}
Music noMercy {
 filename="Sounds/noMercy.mod"
 amplify=30
}
Music trainer5 {
 filename="Sounds/trainer5.mod"
 amplify=40
}
Music heidi7 {
 filename="Sounds/heidi7.xm"
 amplify=40
}
Music aSadTouch {
 filename="Sounds/ASadTouch.xm"
 amplify=60
}
Music stStyle {
 filename="Sounds/stStyle.xm"
 amplify=50
}
Music saturn {
 filename="Sounds/Saturn.xm"
 amplify=50
}
Music moskito {
 filename="Sounds/Moskito.xm"
 amplify=50
}
Music jttimarsuEdit {
 filename="Sounds/JttimarsuEdit.xm"
 amplify=50
}
Music puzzler {
 filename="Sounds/puzzler.mid"
 amplify=20
}
Music demag {
 filename="Sounds/Demag.xm"
 amplify=40
}
Music sierain2 {
 filename="Sounds/Sierain2.xm"
 amplify=50
}

;************************
;******** SOUNDS ********
;************************
Sound snd_countdown {
 filename="Sounds/countDownSound.wav"
}
Sound snd_missile {
 filename="Sounds/missile.mp3"
}
Sound snd_explosion1 {
 filename="Sounds/explosion1.wav"
}
Sound snd_explosion2 {
 filename="Sounds/explosion2.wav"
}
Sound snd_botonoidexplode {
 filename="Sounds/botonoidexplode.mp3"
}
Sound snd_alphanoidSelect {
 filename="Sounds/Alphanoid.wav"
}
Sound snd_herbanoidSelect {
 filename="Sounds/Herbanoid.wav"
}
Sound snd_barvinoidSelect {
 filename="Sounds/Barvinoid.wav"
}