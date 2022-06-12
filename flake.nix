{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils, ... } @ inputs:
    utils.lib.eachDefaultSystem (system:
      let
        project-name = "main";

        pkgs = import nixpkgs { inherit system; };
        deps = [ ];
        native-deps = with pkgs; [
          llvm_13
          clang_13
          ninja
          cmake
        ];
        dev-deps = with pkgs; [
          rnix-lsp
          clang-tools
          lldb_13
        ];

        package = pkgs.pkgsStatic.stdenv.mkDerivation {
          name = project-name;
          src = ./.;
          buildInputs = deps;
          nativeBuildInputs = native-deps;
          installPhase = ''
            mkdir -p $out/bin
            cp ${project-name} $out/bin/${project-name}
          '';
        };
      in
      {
        defaultPackage = package;
        devShell = pkgs.stdenvNoCC.mkDerivation {
          name = "shell";
          nativeBuildInputs = deps ++ native-deps ++ dev-deps;
          CMAKE_EXPORT_COMPILE_COMMANDS = "yes";
        };
      });
}
