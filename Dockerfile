FROM fedora:latest

# create user-specific images
ARG UID
ARG GID
ARG USER
ARG GROUP

# 还是清华源快啊
RUN sed -e 's|^metalink=|#metalink=|g' \
    -e 's|^#baseurl=http://download.example/pub/fedora/linux|baseurl=https://mirrors.tuna.tsinghua.edu.cn/fedora|g' \
    -i.bak \
    /etc/yum.repos.d/fedora.repo \
    /etc/yum.repos.d/fedora-updates.repo

RUN dnf update -y && \
    dnf install -y clear awk wget g++ clang clang-tools-extra cmake vim git zip

RUN echo "%wheel ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers.d/10-nopasswd

# 创建用户和组（确保容器内有 useradd 或 adduser 命令）
RUN groupadd -g ${GID} ${GROUP} && \
    useradd -u ${UID} -g ${GID} -m ${USER} && \
    usermod -aG wheel ${USER}

USER ${USER}

WORKDIR /home/${USER}/algorithm

# An ENTRYPOINT allows you to configure a container that will run as an executable.
# 必须启动一个进程，否则容器启动就会立刻退出
# ENTRYPOINT ["./entrypoint.sh"]
ENTRYPOINT ["/bin/bash", "-c", "tail -f /dev/null"]
# ENTRYPOINT ["/bin/bash"]