PULCMD:PALETTE_BANK=<n>

if (strncmp(cmd, "PALETTE_BANK=", 13) == 0) {
    uint8_t b = cmd[13] - '0';
    ntsc_set_palette_bank(b);
    uart_print("PULRES:PALETTE_BANK=");
    uart_tx('0' + b);
    uart_print("\n");
}

PULBIN:<hex>

7. SERIAL BOOTLOADER PROTOCOL
On reset, Nano64 waits 1 second for:

Code
PULBIN:<hex>
If received:

Loads NTSC core

Sends:

Code
PULRES:BOOTLOAD_OK
If timeout:

Boots normally

8. FULL PROTOCOL EXAMPLE
Host → Nano64:
Code
PULCMD:PALETTE_BANK=3
Nano64 → Host:

Code
PULRES:PALETTE_BANK=3
Host → Nano64:
Code
PULBIN:7F45E04E5453434F52...*A3
Nano64 → Host:

Code
PULRES:NTSC_CORE_INSTALLED
Host → Nano64:
Code
PULVRM:03A0FF0F0F0F0F0F0F
Nano64 → Host:

Code
PULRES:VRAM_OK
