# Makefile for creating Airstrike game graphics data
# from the PovRay v3.5 3D models. Requires _GNU_ make.
#
# Before you add animation creation in this Makefile, you
# can test animating your 'name.pov' with a command like:
#   make NAME=name WD=xx HT=yy FRAMES=kk OPT='options' BLUR=1 anim
#
# WD, HT, OPT and BLUR parameters are optional. If you don't want
# povray to wait for user click after each rendering, give
# BATCH=1 argument.
#
# (w) 2002 by Ulf Ekström & Eero Tamminen

# ---------------- default values -------------------------

ifndef WD
WD := 48
endif

ifndef HT
HT := 48
endif

# PovRay with default flags
POVRAY = povray +FN +DTC +A0.3 -J +UA -P

ifdef BATCH
POVRAYP = $(POVRAY)
else
# wait until user clicks
POVRAYP = $(POVRAY) +P
endif

# ------------------------------------------------------------

ifdef NAME

# I'm using submake to create animations because with
# shell-script you'd need to separately handle errors
# and Make does it automatically.

anim:
	$(RM) $(NAME).png
	$(POVRAY) +I$(NAME).pov +W$(WD) +H$(HT) +KFF$(FRAMES) $(OPT)
ifdef BLUR
	../utils/blur.sh $(NAME)[0-9]*.png
endif
	convert +append $(NAME)[0-9]*.png $(NAME).png
	$(RM) $(NAME)[0-9]*.png

else

# ------------------------------------------------------------

# all models
POV = $(wildcard *.pov)
PNG = $(patsubst %.pov, %.png, $(POV)) bonus.png

all: $(PNG)

# default rule for other POV files
%.png: %.pov
	$(POVRAYP) +I$< +W$(WD) +H$(HT)

# -------------------- animations -----------------------------

# explicit rules are separated into animations that use submake
# and single frames which just use povray.  They are sorted
# alphabetically so that the rules are easier to find and in
# the same order as Make will execute them with the 'all' rule.

airballoon.png: airballoon.pov
	$(MAKE) NAME=airballoon WD=64 HT=64 FRAMES=8 OPT=+KC anim

bird.png: bird.pov
	$(MAKE) NAME=bird WD=12 HT=12 FRAMES=8 anim

bomb.png: bomb.pov 
	$(MAKE) NAME=bomb WD=16 HT=16 FRAMES=64 anim

bombexplosion.png: bombexplosion.pov
	$(MAKE) NAME=bombexplosion WD=48 HT=48 FRAMES=32 anim

bonus-machine.png: bonus-machine.pov 
	$(MAKE) NAME=bonus-machine WD=48 HT=32 FRAMES=16 anim

balloon-deflate.png: balloon-deflate.pov
	$(MAKE) NAME=balloon-deflate WD=32 HT=32 FRAMES=32 OPT=+KC anim

balloon-bounce.png: balloon-bounce.pov
	$(MAKE) NAME=balloon-bounce WD=32 HT=32 FRAMES=32 OPT=+KC anim

bullet.png: bullet.pov
	$(MAKE) NAME=bullet WD=24 HT=24 FRAMES=64 anim

cannon-left.png: cannon-left.pov cannon.inc
	$(MAKE) NAME=cannon-left WD=64 HT=48 FRAMES=16 anim

cannon-right.png: cannon-right.pov cannon.inc
	$(MAKE) NAME=cannon-right WD=64 HT=48 FRAMES=16 anim

cannon-turn-lr.png: cannon-turn.pov cannon.inc
	$(MAKE) NAME=cannon-turn WD=64 HT=48 FRAMES=32 BLUR=1 anim

cannon-turn-rl.png: cannon-turn.pov cannon.inc
	$(MAKE) NAME=cannon-turn WD=64 HT=48 FRAMES=32 BLUR=1 anim

direxpl.png: direxpl.pov
	$(MAKE) NAME=direxpl WD=24 HT=24 FRAMES=256 OPT=+KF32 anim

dust.png: dust.pov
	$(MAKE) NAME=dust WD=16 HT=16 FRAMES=16 anim

energymeter.png: energymeter.pov
	$(MAKE) NAME=energymeter WD=12 HT=64 FRAMES=16 anim

fire.png: fire.pov
	$(MAKE) NAME=fire WD=24 HT=24 FRAMES=16 OPT=+KC anim

firebomb.png: firebomb.pov
	$(MAKE) NAME=firebomb WD=48 HT=48 FRAMES=32 anim

gauge.png: gauge.pov
	$(MAKE) NAME=gauge WD=48 HT=48 FRAMES=64 anim

hangar.png: hangar.pov
	$(MAKE) NAME=hangar WD=100 HT=64 FRAMES=8 anim

mark.png: mark.pov
	$(MAKE) NAME=mark WD=24 HT=24 FRAMES=16 OPT=+KC anim

missile.png: missile.pov
	$(MAKE) NAME=missile WD=24 HT=24 FRAMES=64 OPT=+KC anim

plane.png: plane.pov
	$(MAKE) NAME=plane WD=48 HT=48 FRAMES=64 OPT=+KC anim

smoke.png: smoke.pov
	$(MAKE) NAME=smoke WD=16 HT=16 FRAMES=16 anim

spikeball.png: spikeball.pov 
	$(MAKE) NAME=spikeball WD=16 HT=16 FRAMES=32 anim

ufo.png: ufo.pov
	$(MAKE) NAME=ufo WD=48 HT=48 FRAMES=64 OPT=+KC anim

zeppelin-turn-lr.png: zeppelin-turn-lr.pov zeppelin.inc
	$(MAKE) NAME=zeppelin-turn-lr WD=96 HT=64 FRAMES=32 BLUR=1 anim

zeppelin-turn-rl.png: zeppelin-turn-rl.pov zeppelin.inc
	$(MAKE) NAME=zeppelin-turn-rl WD=96 HT=64 FRAMES=32 BLUR=1 anim

zeppelin-wreck.png: zeppelin-wreck.pov
	$(MAKE) NAME=zeppelin-wreck WD=96 HT=64 FRAMES=6 anim

# --------------------- stilleben ----------------------------

balloon-float.png: balloon-float.pov
	$(POVRAYP) +I$< +W32 +H32

bonus0.png: bonus0.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32
bonus1.png: bonus1.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32
bonus2.png: bonus2.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32
bonus3.png: bonus3.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32
bonus4.png: bonus4.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32
bonus5.png: bonus5.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32
bonus6.png: bonus6.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32
bonus7.png: bonus7.pov bonus.inc
	$(POVRAY) +I$< +W32 +H32

# bonuses have to handled separately because POV-Ray bugs
# if you span switch-ranges over several includes...
bonus.png: bonus0.png bonus1.png bonus2.png bonus3.png bonus4.png bonus5.png bonus6.png bonus7.png
	convert +append bonus[0-9]*.png $@

cannonball.png: cannonball.pov
	$(POVRAYP) +I$< +W8 +H8

cloud.png: cloud.pov
	$(POVRAYP) +I$< +W240 +H120

hills.png: hills.pov
	$(POVRAYP) +I$< +W800 +H600

man.png: man.pov parachute.inc
	$(POVRAYP) +I$< +W48 +H48

tie.png: tie.pov
	$(POVRAYP) +I$< +W48 +H48

zeppelin-left.png: zeppelin-left.pov zeppelin.inc
	$(POVRAYP) +I$< +W96 +H64

zeppelin-right.png: zeppelin-right.pov zeppelin.inc
	$(POVRAYP) +I$< +W96 +H64

# ----------------------------------------------------------

endif # NAME


install: all
	$(CP) *.png ../data/

clean:
	$(RM) *~

distclean: clean
	$(RM) *.png
