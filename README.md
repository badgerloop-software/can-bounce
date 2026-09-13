# sc2.5-tutorial branch

Flash this to the F767 on the CAN test board. Used with the sc2.5-tutorial L432 lab.

Serial 115200, press `1` to toggle the LED command on ID `0x600`.

TODOs are in `src/candecoder.cpp` and `src/main.cpp`. See the sc2.5-tutorial README for wiring.

```bash
git submodule update --init --recursive
pio run -e nucleo_f767zi -t upload
```
