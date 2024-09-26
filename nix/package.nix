{ stdenv }:
let
  pname = "xpf";
  version = "1.0.0";
in
stdenv.mkDerivation {
  inherit pname version;
  src = ../.;
  installPhase = ''
    mkdir -p $out/bin
    cp xpf $out/bin/xpf
  '';
}