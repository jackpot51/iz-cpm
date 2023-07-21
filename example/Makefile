build/example.com: build/example.ihx
	objcopy -Iihex -Obinary $< $@

build/example.ihx: example.c
	mkdir -p build
	sdcc \
		-mz80 \
		--code-loc 0x100 \
		--data-loc 0x2000 \
		--stack-loc 0x4000 \
		--no-std-crt0 \
		-o $@ \
		$<

build/example.img: build/example.com
	rm -f $@.partial
	env HOME=$(PWD) dskform -type raw -format osb1sssd $@.partial
	mkfs.cpm -f osb1sssd $@.partial
	cpmcp -f osb1sssd $@.partial $< 0:example.com
	mv $@.partial $@

build/example.imd: build/example.img
	rm -f $@.partial
	env HOME=$(PWD) dsktrans -itype raw -format osb1sssd -otype imd $< $@.partial
	mv $@.partial $@

clean:
	rm -rf build

mame: build/example.imd
	mame osborne1 -floppydisk1 cpm22 -floppydisk2 $<

run: build/example.com
	cargo build --release --manifest-path ../Cargo.toml
	../target/release/iz-cpm --slow $<
