{ mkShell, gnumake, xpf }:
mkShell {
  name = "xpf-dev";
  inputsFrom = [
    xpf
  ];
  packages = [
    gnumake 
  ];
}
