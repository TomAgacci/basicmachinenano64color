Then wherever you currently do something like:

addr = 0xd400 + voice * 7 + REG_OFFSET
emit_poke(addr, value)

you replace with:

addr = sid_voice_to_addr(voice, args) + REG_OFFSET
emit_poke(addr, value)




-------------------------------------------------------------------------


If your converter already parses some meta‑comments, you can add a pragma:

' @DUAL_SID ON

Then in Python:

def detect_dual_sid_from_bas(lines):
    for line in lines:
        if "@DUAL_SID" in line.upper():
            return True
    return False

def convert_bas(args):
    with open(args.input, "r") as f:
        lines = f.readlines()

    if detect_dual_sid_from_bas(lines):
        args.dual_sid = True

    # rest of conversion using args.dual_sid


---------------------------------------------------------------------------

If your converter outputs some intermediate JSON/struct, you can add a chip field:

python
event = {
    "time": t,
    "voice": v,
    "chip": 0 if v < 3 else 1 if args.dual_sid else 0,
    "reg": reg,
    "value": val,
}
