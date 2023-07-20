build/example.com: build/example.ihx
	objcopy -Iihex -Obinary $< $@

build/example.ihx: example.c
	mkdir -p build
	sdcc -mz80 --code-loc 0x100 --no-std-crt0 -o $@ $<

run: build/example.com
	cargo build --release --manifest-path ../Cargo.toml
	../target/release/iz-cpm --slow $<
