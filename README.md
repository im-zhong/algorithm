# algorithm

All the algorithms.

## 1. docker

1. 咱们还是用docker吧，因为咱们会装很多的东西，尤其是python的环境，
2. 容器同样选择fedora，比较合适

## Tips

1. <https://github.com/microsoft/vscode-pull-request-github/issues/4624> close the proxy, cause clash already use the global proxy

## Pre

1. ln -s ~/.ssh .ssh
2. ln -s ~/.gitconfig .gitconfig
3. ln -s ~/.git-credentials .git-credentials

## use python with uv

1. uv init algorithm

## use vcpkg <https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-vscode?pivots=shell-bash>

1. vcpkg new --application
2. vcpkg add port doctest
3. cmake --preset=vcpkg
4. cmake --build build
5. ./build/HelloWorld
