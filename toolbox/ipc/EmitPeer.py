from utils import Generator, camelcase


def arguments(args):
    result = ""

    index = 0
    for arg in args:
        result += f"{args[arg]} {arg}"
        if index + 1 < len(args):
            result += ", "

        index += 1

    return result


def request_handler(gen: Generator, request_name: str, request):
    gen.emit(
        f"virtual {camelcase(request_name)+'Response'} {request_name}({arguments(request['arguments'])}) = 0;")
    gen.emit("")


def signal_handler(gen: Generator, signal_name: str, signal):
    gen.emit(
        f"virtual void {signal_name}({arguments(signal['arguments'])}) = 0;")
    gen.emit("")


def peer(gen: Generator, prot, peer):
    gen.emit("#pragma once")

    gen.emit("")
    gen.emit("// Don't edit this code !")
    gen.emit("// It was generated by ipc-compiler.py")

    gen.emit("")
    gen.emit(
        f"#include <protocols/{camelcase(prot['properties']['name'])}Protocol.h>")
    gen.emit("")

    gen.emit("namespace protocols")
    gen.emit("{")
    gen.emit("")

    gen.emit(f"class {camelcase(peer['name'])}")
    gen.emit("{")
    gen.push_ident()

    if len(peer["requests"]) > 0:
        gen.emit(
            "/* --- Requests ------------------------------------------------------------- */")
        gen.emit("")

        for req in peer["requests"]:
            request_handler(gen, req, peer["requests"][req])

    if len(peer["signals"]) > 0:
        gen.emit(
            "/* --- Signales ------------------------------------------------------------- */")
        gen.emit("")

        gen.emit("")
        for sig in peer["signals"]:
            signal_handler(gen, sig, peer["signals"][sig])

    gen.pop_ident()
    gen.emit("};")

    gen.emit("")
    gen.emit("}")