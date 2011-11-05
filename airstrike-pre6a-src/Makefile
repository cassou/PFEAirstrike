
# always regenerate
.PHONY: airstrike airstrike-sound

airstrike:
	$(MAKE) -C src airstrike
	mv src/airstrike .

# note: do 'make clean' between sound changing
airstrike-sound:
	$(MAKE) -C src OPTIONS=-DUSE_SOUND airstrike-sound
	mv src/airstrike-sound airstrike

clean:
	$(RM) $(shell find . -name '*.[od]' -o -name '*~')
	$(RM) airstrike


# package name and what to exclude from it
PKG_NAME = airstrike-$(shell date -I)
EXCLUDE = --exclude '*/pov/*.png' --exclude '*/contrib*'

package: clean
	cd ..; \
	mv airstrike $(PKG_NAME); \
	tar -zcvf $(PKG_NAME).tar.gz $(EXCLUDE) $(PKG_NAME); \
	mv $(PKG_NAME) airstrike

# packages new files listed in new-files.txt
new:
	tar -zcvf new.tgz -T new-files.txt
