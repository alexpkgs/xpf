{ mkShell, gnumake, strlx }:
mkShell {
  name = "xpf-dev";
  inputsFrom = [
    xpf
  ];
  packages = [
    gnumake 
  ];
}