# algorithm

All the algorithms.

## How to use this repo?

1. clone this repo: `git clone https://github.com/im-zhong/algorithm`
2. create a .etc folder to mount the ssh config and git config into the container
   1. `mkdir -p .etc`
   2. `ln -s ~/.ssh ./.etc/ssh`
   3. `ln -s ~/.gitconfig ./.etc/gitconfig`
   4. `ln -s ~/.git-credentials ./.etc/git-credentials`
   5. or you could just comment the volumes codes in the docker-compose.yml file and do not mount the .etc folder into the container, in this way you could not push your code to the repo
3. open it in the vscode
4. install the [Dev Containers](https://marketplace.visualstudio.com/items/?itemName=ms-vscode-remote.remote-containers) extension
5. use command: 'Dev Container: Reopen in container'
6. just wait for all the thing done for a few minutes, then you are good to go.

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

1. uv init . # will create several python files, espcially the pyproject.toml in the root dir
2. <https://docs.astral.sh/uv/getting-started/features/#projects>
   1. uv add ruff
   2. uv remove ruff
   3. uv run mypy .
3. 而且uv默认会在root dir下创建venv环境，nice！！！
4. uv run pre-commit install
5. uv run pre-commit run -a

## use vcpkg <https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-vscode?pivots=shell-bash>

1. vcpkg new --application
2. vcpkg add port doctest
3. cmake --preset=vcpkg
4. cmake --build build
5. ./build/HelloWorld

## 这次复习的方向

1. 这个复习的深度和广度应该如何考量？是完全解析STL，模板，还是浅尝辄止，专注于算法的实现，刷题？
2. 我认为两个方向都要考虑，这次的复习应该就是深度极深的，否则我的技术还是得不到进步，但是呢，这个事情需要细水长流的做，目前还是不急，还是以总结算法为主。为了刷算法写的链表和复刻STL的实现的链表必然是不一样的。而刷题就又是另外一种画风了。

## prompts

1. write unit test by using the given examples. do not use subcase, section, use only TEST_CASE, do not help me do the solution, only give me the test cases.
